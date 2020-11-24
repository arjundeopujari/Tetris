/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * tetris.h
 * 
 * Author: Daniel Mizrahi
 * Ported by: Patrick Thomas
 */

#pragma once

#include "msp.h"
#include "rand.h"
#include "types.h"

//#ifdef DEBUG
//#include "ITM.h"
//#endif

/*
 * Mapping of button pins:
 *
 * P6.0 Rotate
 * P6.1 Down
 * -
 * -
 * P6.4 Right
 * P6.5 Left
 * P6.6 Pause
 * P6.7 NewGame
 */
#define ROTATE_MASK 0b00000001  // P6.0
#define DOWN_MASK 0b00000010    // P6.1
#define RIGHT_MASK 0b00010000   // P6.4
#define LEFT_MASK 0b00100000    // P6.5
#define PAUSE_MASK 0b01000000   // P6.6
#define NEWGAME_MASK 0b10000000 // P6.7

#define GAME_WIDTH 10
#define GAME_HEIGHT 20
#define PIECE_QUEUE_SIZE 6

typedef enum
{
    EMPTY,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    TEAL,
    PURPLE,
    WHITE
} BoardValue;

/**
 * A Point represents a pair of integers that represent a coordinate on the Tetris board (not the display).
 */
typedef struct
{
    unsigned char x, y;
} Point;

/**
 * A State it a set of Points that represent a single tetronimo.
 */
typedef struct
{
    Point a, b, c, d;
} State;

/**
 * Union of a State and array of Points for easier manipulation and access of the underlying points.
 */
typedef union
{
    State state;
    Point points[4];
} StateUnion;

/**
 * Represent a Piece (Tetronimo) in memory.
 */
typedef struct
{
    unsigned char piece_num;
    unsigned char state;
    State *states;
    unsigned char num_states;
    BoardValue color;
} Piece;

/**
 * Represents the entire game in memory, keeping track of all needed current states and the piece queue.
 */
typedef struct
{
    BoardValue board[GAME_HEIGHT][GAME_WIDTH];
    unsigned int score;
    int row;
    int col;
    bool end_game;
    unsigned char lines_cleared;
    Piece piece_queue[PIECE_QUEUE_SIZE];
    unsigned char piece_queue_front;
    unsigned char piece_queue_back;

    bool changed;
} Tetris;

/**
 * Encoded states for all rotations of the I piece.
 */
static State PIECE_I_STATES[2] = {
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}}};

/**
 * Encoded state of the O piece.
 */
static State PIECE_O_STATE =
    {{0, 1}, {0, 2}, {1, 1}, {1, 2}};

/**
 * Encoded states for all rotations of the T piece.
 */
static State PIECE_T_STATES[4] = {
    {{0, 0}, {0, 1}, {0, 2}, {1, 1}},
    {{0, 1}, {1, 0}, {1, 1}, {2, 1}},
    {{1, 1}, {2, 0}, {2, 1}, {2, 2}},
    {{0, 1}, {1, 1}, {2, 1}, {1, 2}}};

/**
 * Encoded states for all rotations of the S piece.
 */
static State PIECE_S_STATES[2] = {
    {{0, 1}, {0, 2}, {1, 0}, {1, 1}},
    {{0, 1}, {1, 1}, {1, 2}, {2, 2}}};

/**
 * Encoded states for all rotations of the Z piece.
 */
static State PIECE_Z_STATES[2] = {
    {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
    {{0, 1}, {1, 1}, {1, 0}, {2, 0}}};

/**
 * Encoded states for all rotations of the L piece.
 */
static State PIECE_L_STATES[4] = {
    {{0, 0}, {0, 1}, {0, 2}, {1, 0}},
    {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
    {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
    {{0, 1}, {1, 1}, {2, 1}, {2, 2}}};

/**
 * Encoded states for all rotations of the J piece.
 */
static State PIECE_J_STATES[4] = {
    {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
    {{0, 1}, {1, 1}, {2, 1}, {2, 0}},
    {{1, 0}, {2, 0}, {2, 1}, {2, 2}},
    {{0, 1}, {0, 2}, {1, 1}, {2, 1}}};

void piece_init(Piece *piece);
bool piece_rotate(Piece *piece, Tetris *tetris);
bool piece_in_bounds(Piece *piece, Tetris *tetris, unsigned char new_state);
void piece_visualize();

void tetris_init(Tetris *tetris);
bool tetris_shift_down(Tetris *tetris);
bool tetris_move_left(Tetris *tetris);
bool tetris_move_right(Tetris *tetris);
void tetris_place_piece(Tetris *tetris);
void tetris_visualize(Tetris *tetris);
void tetris_spawn_piece(Tetris *tetris);
int tetris_remove_lines_and_score(Tetris *tetris);
void tetris_shift_rows_down(Tetris *tetris, unsigned char row_index);
void tetris_queue_enqueue(Tetris *tetris, Piece *piece);
void tetris_queue_pop(Tetris *tetris);
Piece *tetris_queue_get(Tetris *tetris);
