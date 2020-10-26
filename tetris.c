/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * tetris.c
 * 
 * Author: Daniel Mizrahi
 * Ported by: Patrick Thomas
 */

#include "tetris.h"
#include "debug.h"

const char id_to_piece_string[] = "IOTSZLJ";

void piece_init(Piece *piece)
{
    piece->piece_num = rand() % 7;
    piece->state = 0;

    switch (piece->piece_num)
    {
    case 0:
        piece->states = PIECE_I_STATES;
        piece->num_states = 2;
        break;
    case 1:
        piece->states = &PIECE_O_STATE;
        piece->num_states = 1;
        break;
    case 2:
        piece->states = PIECE_T_STATES;
        piece->num_states = 4;
        break;
    case 3:
        piece->states = PIECE_S_STATES;
        piece->num_states = 2;
        break;
    case 4:
        piece->states = PIECE_Z_STATES;
        piece->num_states = 2;
        break;
    case 5:
        piece->states = PIECE_L_STATES;
        piece->num_states = 4;
        break;
    case 6:
        piece->states = PIECE_J_STATES;
        piece->num_states = 4;
        break;

    default:
        break;
    }

    // Assign a color.
    unsigned char color = rand() % 3;
    switch (color)
    {
    case 0:
        piece->color = RED;
        break;
    case 1:
        piece->color = GREEN;
        break;
    case 2:
        piece->color = BLUE;
        break;
    }
}

void tetris_init(Tetris *tetris)
{
    unsigned int r, c;
    for (r = 0; r < GAME_HEIGHT; r++)
    {
        for (c = 0; c < GAME_WIDTH; c++)
        {
            tetris->board[r][c] = EMPTY;
        }
    }
    tetris->score = 0;
    tetris->row = 0;
    tetris->col = 3;

    srand(0); // TODO: Seed this with MCLK's value.

    unsigned int i;
    for (i = 0; i < 6; i++)
    {
        Piece p;
        piece_init(&p);
        tetris_queue_enqueue(tetris, &p);
    }
    tetris->end_game = false;
    tetris->lines_cleared = 0;
    // pthread_mutex_init(&lock, NULL);
    // pthread_cond_init(&condition, NULL);

    tetris->changed = true;
}

bool piece_rotate(Piece *piece, Tetris *tetris)
{
    unsigned int new_state = (piece->state + 1) % piece->num_states;
    if (!piece_in_bounds(piece, tetris, new_state))
    {
        return false;
    }
    StateUnion state_union;
    state_union.state = piece->states[new_state];
    int i;
    for (i = 0; i < 4; i++)
    {
        Point *p = &state_union.points[i];
        if (tetris->board[p->x + tetris->row][p->y + tetris->col] != EMPTY)
        {
            return false;
        }
    }
    piece->state = new_state;
    return true;
}

bool piece_in_bounds(Piece *piece, Tetris *tetris, unsigned char new_state)
{
    StateUnion state_union;
    state_union.state = piece->states[new_state];
    int i;
    for (i = 0; i < 4; i++)
    {
        Point *p = &state_union.points[i];
        if (p->x + tetris->row > GAME_HEIGHT - 1)
        {
            return false;
        }
        else if ((p->y + tetris->col > GAME_WIDTH - 1) || (p->y + tetris->col < 0))
        {
            return false;
        }
    }
    return true;
}

bool tetris_shift_down(Tetris *tetris)
{
    Piece *piece = tetris_queue_get(tetris);
    StateUnion state_union;
    state_union.state = piece->states[piece->state];
    int i;
    for (i = 0; i < 4; i++)
    {
        Point *p = &state_union.points[i];
        if (tetris->board[p->x + tetris->row + 1][p->y + tetris->col] == EMPTY)
        {
            tetris_place_piece(tetris);
            return false;
        }
        if (p->x + tetris->row == GAME_HEIGHT - 1)
        {
            tetris_place_piece(tetris);
            return false;
        }
    }
    tetris->row += 1;
    return true;
}

bool tetris_move_left(Tetris *tetris)
{
    Piece *piece = tetris_queue_get(tetris);
    StateUnion state_union;
    state_union.state = piece->states[piece->state];
    int i;
    for (i = 0; i < 4; i++)
    {
        Point *p = &state_union.points[i];
        if (tetris->board[p->x + tetris->row][p->y + tetris->col - 1] != EMPTY || p->y + tetris->col <= 0)
        {
            return false;
        }
    }
    tetris->col -= 1;
    return true;
}

bool tetris_move_right(Tetris *tetris)
{
    Piece *piece = tetris_queue_get(tetris);
    StateUnion state_union;
    state_union.state = piece->states[piece->state];
    int i;
    for (i = 0; i < 4; i++)
    {
        Point *p = &state_union.points[i];
        if (tetris->board[p->x + tetris->row][p->y + tetris->col + 1] != EMPTY ||
            p->y + tetris->col >= (GAME_WIDTH - 1))
        {
            return false;
        }
    }
    tetris->col += 1;
    return true;
}

void tetris_place_piece(Tetris *tetris)
{
    Piece *piece = tetris_queue_get(tetris);
    StateUnion state_union;
    state_union.state = piece->states[piece->state];
    int i;
    for (i = 0; i < 4; i++)
    {
        Point *p = &state_union.points[i];
        tetris->board[p->x + tetris->row][p->y + tetris->col] = piece->color;
    }
    tetris_remove_lines_and_score(tetris);
    tetris_spawn_piece(tetris);
    tetris->changed = true;
    return;
}

void tetris_visualize(Tetris *tetris)
{
    Piece *piece = tetris_queue_get(tetris);
    StateUnion state_union;
    state_union.state = piece->states[piece->state];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        Point *p = &state_union.points[i];
        tetris->board[p->x + tetris->row][p->y + tetris->col] = piece->color;
    }

#ifdef DEBUG
    for (i = 0; i < GAME_HEIGHT; i++)
    {
        for (j = 0; j < GAME_WIDTH; j++)
        {
            switch (tetris->board[i][j])
            {
            case RED:
                debug_print("R");
                break;
            case GREEN:
                debug_print("G");
                break;
            case BLUE:
                debug_print("B");
                break;
            default:
                break;
            }
            debug_print(" ");
        }
        debug_print("\n");
    }
    debug_print("\n");
#endif

    for (i = 0; i < 4; i++)
    {
        Point *p = &state_union.points[i];
        tetris->board[p->x + tetris->row][p->y + tetris->col] = EMPTY;
    }
    //    printf("Score: %d\n", score);
    //    printf("Next Pieces: ");
    //    for (list<Piece>::iterator it = piece_queue.begin(); it != piece_queue.end(); it++)
    //    {
    //        if (!(it == piece_queue.begin()))
    //        {
    //            int num = ((*it).piece_num);
    //            printf(id_to_piece_string[num]);
    //            printf(" ");
    //        }
    //    }
    //    printf("\n");
    tetris->changed = true;
    return;
}

void tetris_spawn_piece(Tetris *tetris)
{
    tetris->row = 0;
    tetris->col = 3;
    tetris_queue_pop(tetris);

    Piece p;
    piece_init(&p);
    tetris_queue_enqueue(tetris, &p);

    Piece *piece = tetris_queue_get(tetris);
    StateUnion state_union;
    state_union.state = piece->states[piece->state];
    int i;
    for (i = 0; i < 4; i++)
    {
        Point *p = &state_union.points[i];
        if (tetris->board[p->x + tetris->row][p->y + tetris->col] != EMPTY)
        {
            tetris->end_game = true;
            return;
        }
    }
    return;
}

void tetris_shift_rows_down(Tetris *tetris, unsigned char row_index)
{
    int r, c;
    for (r = row_index; r > 0; r--)
    {
        for (c = 0; c < GAME_WIDTH; c++)
        {
            tetris->board[r][c] = tetris->board[r - 1][c];
        }
    }
    for (c = 0; c < GAME_WIDTH; c++)
    {
        tetris->board[0][c] = EMPTY;
    }
    tetris->changed = true;
    return;
}

int tetris_remove_lines_and_score(Tetris *tetris)
{
    int r, c;
    int lines = 0;
    for (r = 0; r < GAME_HEIGHT; r++)
    {
        bool remove = true;
        for (c = 0; c < GAME_WIDTH; c++)
        {
            if (tetris->board[r][c] == EMPTY)
            {
                remove = false;
                break;
            }
        }
        if (remove)
        {
            tetris_shift_rows_down(tetris, r);
            lines++;
            r--;
        }
    }
    tetris->score += lines * lines;
    return lines;
}

//static void *async_function(void *arg)
//{
//    Game *tetris = (Game *)arg;
//    int ch;
//    while (((ch = getch()) != '\n') && !(tetris->end_game))
//    {
//        pthread_mutex_lock(&(tetris->lock));
//        switch (ch)
//        {
//        case KEY_UP:
//            tetris->piece_queue.front().rotate(tetris);
//            break;
//        case ' ': // Space Bar
//            while (tetris->shift_down())
//            {
//                continue;
//            }
//            break;
//        case KEY_LEFT:
//            tetris->move_left();
//            break;
//        case KEY_RIGHT:
//            tetris->move_right();
//            break;
//            //default: tetris->end_game = true;
//        }
//        clear();
//        tetris->visualize();
//        refresh();
//        pthread_mutex_unlock(&(tetris->lock));
//    }
//    return tetris;
//}
//
//static void *sync_function(void *arg)
//{
//    Game *tetris = (Game *)arg;
//    while (!(tetris->end_game))
//    {
//        pthread_mutex_lock(&(tetris->lock));
//        clear();
//        tetris->visualize();
//        refresh();
//        pthread_mutex_unlock(&(tetris->lock));
//        usleep(500000 * pow(0.95, int(tetris->lines_cleared / 5)));
//        pthread_mutex_lock(&(tetris->lock));
//        tetris->shift_down();
//        clear();
//        tetris->visualize();
//        refresh();
//        pthread_mutex_unlock(&(tetris->lock));
//    }
//    return tetris;
//}

void tetris_queue_enqueue(Tetris *tetris, Piece *piece)
{
    tetris->piece_queue[tetris->piece_queue_back] = *piece;
    tetris->piece_queue_back = (tetris->piece_queue_back + 1) % PIECE_QUEUE_SIZE;
}

void tetris_queue_pop(Tetris *tetris)
{
    tetris->piece_queue_front = (tetris->piece_queue_front + 1) % PIECE_QUEUE_SIZE;
}

Piece *tetris_queue_get(Tetris *tetris)
{
    Piece *p = &tetris->piece_queue[tetris->piece_queue_front];
    return p;
}

// int main()
// {
//     Game tetris = Game();
//     tetris.spawn_piece();
//     pthread_t async;
//     pthread_t sync;
//     initscr();
//     raw();
//     keypad(stdscr, TRUE);
//     noecho();
//     pthread_create(&async, NULL, async_function, (void *)&tetris);
//     pthread_create(&sync, NULL, sync_function, (void *)&tetris);
//     pthread_join(async, NULL);
//     pthread_join(sync, NULL);
//     refresh();
//     getch();
//     endwin();
//     return 0;
// }
