#ifndef BRESENHAM_H
#define BRESENHAM_H

#include "raylib.h"

void BresenhamLine(int x1, int y1, int x2, int y2, Color color);
void Bres_DashedLine(int x1, int y1, int x2, int y2, int dashLen, int gapLen, Color color);

#endif
