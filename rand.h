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

unsigned int rand();
void srand(unsigned int seed);
