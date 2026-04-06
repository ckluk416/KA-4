#include "bresenham.h"
#include <stdlib.h>

void BresenhamLine(int x1, int y1, int x2, int y2, Color color) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        DrawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void Bres_DashedLine(int x1, int y1, int x2, int y2, int dashLen, int gapLen, Color color) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    int counter = 0;
    int drawing = 1;
    int current_limit = dashLen;

    while (1) {
        if (drawing) {
            DrawPixel(x1, y1, color);
        }
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
        
        counter++;
        if (counter >= current_limit) {
            counter = 0;
            drawing = !drawing;
            current_limit = drawing ? dashLen : gapLen;
        }
    }
}

static void DrawCirclePoints(int xc, int yc, int x, int y, Color color) {
    DrawPixel(xc + x, yc + y, color);
    DrawPixel(xc - x, yc + y, color);
    DrawPixel(xc + x, yc - y, color);
    DrawPixel(xc - x, yc - y, color);
    DrawPixel(xc + y, yc + x, color);
    DrawPixel(xc - y, yc + x, color);
    DrawPixel(xc + y, yc - x, color);
    DrawPixel(xc - y, yc - x, color);
}

void MidpointCircle(int xc, int yc, int r, Color color) {
    if (r < 0) return;
    int x = 0, y = r;
    int p = 1 - r;
    DrawCirclePoints(xc, yc, x, y, color);
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        DrawCirclePoints(xc, yc, x, y, color);
    }
}

void MidpointCircleFill(int xc, int yc, int r, Color color) {
    if (r < 0) return;
    int x = 0, y = r;
    int p = 1 - r;
    
    BresenhamLine(xc - y, yc, xc + y, yc, color);
    BresenhamLine(xc - x, yc + y, xc + x, yc + y, color);
    BresenhamLine(xc - x, yc - y, xc + x, yc - y, color);
    
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        
        BresenhamLine(xc - x, yc + y, xc + x, yc + y, color);
        BresenhamLine(xc - x, yc - y, xc + x, yc - y, color);
        BresenhamLine(xc - y, yc + x, xc + y, yc + x, color);
        BresenhamLine(xc - y, yc - x, xc + y, yc - x, color);
    }
}

void DrawRectBresenham(int x, int y, int w, int h, Color color) {
    BresenhamLine(x, y, x + w, y, color);
    BresenhamLine(x + w, y, x + w, y + h, color);
    BresenhamLine(x + w, y + h, x, y + h, color);
    BresenhamLine(x, y + h, x, y, color);
}
