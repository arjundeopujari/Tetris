/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * rand.h
 * 
 * Author: Patrick Thomas
 */

#pragma once

static unsigned int __seed = 0;

/*************************
 * rand
 * Get a random number, as calculated from a linear congruential generator.
 * Called in tetris.c
 *************************/
unsigned int rand();

/*************************
 * srand
 * Seed the linear congruential generator.
 * Inputs
 *      seed - Seed to start the LGC with.
 * Called in tetris.c
 *************************/
void srand(unsigned int seed);
