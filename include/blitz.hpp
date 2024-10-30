#pragma once

#include <iostream>

extern int rnd_state;

void bbSeedRnd(int seed);
int getRndState();
int bbRand(int from, int to);
double bbRnd(float from, float to);
