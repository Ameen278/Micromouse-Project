#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <Arduino.h>

// مصفوفة المسافات (Flood Fill)
extern int floodArray[16][16];

// تعريف الدوال
void initFloodFill();
void floodFill(int targetX, int targetY);
int getNextDirection(int x, int y);
bool isAtCenter(int x, int y);

#endif