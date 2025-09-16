#ifndef MAPPING_H
#define MAPPING_H

#include <Arduino.h>

const int MAZE_SIZE = 16;

// بت flags للجدران
const byte NORTH_WALL = 0x01;
const byte EAST_WALL = 0x02;
const byte SOUTH_WALL = 0x04;
const byte WEST_WALL = 0x08;

// مصفوفة المتاهة
extern byte maze[MAZE_SIZE][MAZE_SIZE];

// تعريف الدوال
void initMaze();
void updateWall(int x, int y, int direction, bool hasWall);
bool checkWall(int x, int y, int direction);
void printMaze();

#endif