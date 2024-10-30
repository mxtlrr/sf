#include "blitz.hpp"

const int RND_A = 48271;
const int RND_M = 2147483647;
const int RND_Q = 44488;
const int RND_R = 3399;

int rnd_state = 0;

double rnd() {
  rnd_state = RND_A * (rnd_state % RND_Q) - RND_R * (rnd_state / RND_Q);
  if (rnd_state < 0) rnd_state += RND_M;
  return (rnd_state & 65535) / 65536.0f + (.5f / 65536.0f);
}

void bbSeedRnd(int seed) {
  seed &= 0x7fffffff;
  rnd_state = (seed != 0) ? seed : 1;
}

int getRndState() {
  return rnd_state;
}

int bbRand(int from, int to) {
  if (to < from) {
      int a = from;
      from = to;
      to = a;
  }
  return (int) (rnd() * (to - from + 1)) + from;
}

double bbRnd(float from, float to) {
  return rnd() * (to - from) + from;
}