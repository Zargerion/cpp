#pragma once

#include "resource.h"


void DrawTriangle(HDC hdc, double x1, double y1, double x2, double y2, double x3, double y3) {
    POINT points[3] = { {x1, y1}, {x2, y2}, {x3, y3} };
    Polygon(hdc, points, 3);
}

void WriteStringToScreen(HDC hdc, LPCTSTR string, int x, int y) {
    TextOut(hdc, x, y, string, _tcslen(string));
}

void DrawPaintArea(HDC hdc, int x, int y, int width, int height) {
    Rectangle(hdc, x, y, x + width, y + height);
}



