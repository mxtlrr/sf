// Code for finding a pattern in a room
#ifndef __FIND_HPP_
#define __FIND_HPP_

#include <iostream>
#include <vector>
// #include <algorithm>

#include "backend/map.hpp"
#include "backend/room.hpp"

// Upper limit -- how many seeds to check.
// Obviously this will increase. The plan is to check *all*
// seeds, but 1,000 (0.0000465% of all seeds assuming 2^31-1
// seeds) is quite good for testing patterns.
// TODO: up this amount when code actually works! 2^25 is 1.5%
// of all seeds.
#define LIMIT 1000


/* Finds a seed based on a pattern of rooms. How it works:
 * 1. Generates a seed
 * 2. Checks each room in pattern. If found, increases a counter.
 * Return value is the accuracy. 1.0f means exact match found. */
float patternFind(std::vector<Room> pattern);

#endif