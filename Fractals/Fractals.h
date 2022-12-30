#pragma once

#include <complex>
#include <cmath>
#include <vector>

#include "resource.h"
#include "thread-pool.h"

#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // the title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
const int max_y = 600;
const int max_x = 600;
RECT rc;
int RenderBuffer[max_y][max_x];
std::vector<std::vector<int>> RenderBufferV(max_y, std::vector<int>(max_x));

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                Painter(HDC hdc, int[][max_x]);
void                PainterV(HDC hdc, std::vector<std::vector<int>> bufferV);

typedef std::complex<double> Complex;

Complex sqr(const Complex& v)
{
	return pow(v, 2);
}

void BufferWriter(int buff[][max_x], int y_begin, int y_end, int x_begin, int x_end)
{

    for (int y = y_begin; y != y_end; y++)
    {
        for (int x = x_begin; x != x_end; x++)
        {
            buff[y][x] = 1;
        }
    }


    /*for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < max_x; x++)
        {
            buff[y][x] = 1;
        }
    }*/

    //for (int y = 0; y < max_y; ++y)
//{
//    for (int x = 0; x < max_x; ++x)
//    {
//        Complex z(0, 0);
//        int i = 0;
//        while (i < 1000 && abs(z) < 2)
//        {
//            z = sqr(z) + Complex(1.0 * (x - max_x / 2) / 120, // Z is complex value. Equation if z = z^2 + c, there C is a variable of complex values.
//                1.0 * (y - max_y / 2) / 120);                   // x - max_x/ 2 = scren center.
//            ++i;
//        }
//        if (abs(z) >= 2)
//        {

//            // Modulus of complex value that R value is equel or higher then 2 so print a point.
//            // +40 by x axis becouse center of fractal displaysed by -1/5 of width by x
//            RenderBuffer[y][x + 40] = 1;
//        }
//    }
//}
}

void BufferWriterV(std::vector<std::vector<int>>& buff, int& y_begin, int& y_end, int& x_begin, int& x_end)
{
    std::vector<std::vector<int>>buff_p = buff;
    for (int y = y_begin; y != y_end; y++)
    {
        for (int x = x_begin; x != x_end; x++)
        {
            buff_p[y][x] = 1;
        }
    }
}



void ThreadPooler(int buff[][max_x]) {

    int dvr = 2;
    int step = max_y / dvr;
    int segments = dvr * dvr;
    int range = step - 1;
    int a = 1;

    thread_pool t(12);

    int y_start;
    int y_tail;
    int x_start;
    int x_tail;

    for (y_start = 0; y_start + range < max_y; y_start += step) {
        y_tail = y_start + range;

        for (x_start = 0; x_start + range < max_x; x_start += step) {
            x_tail = x_start + range;
            a++;
                t.add_task(BufferWriter, &buff[0], std::ref(y_start), std::ref(y_tail), std::ref(x_start), std::ref(x_tail));

        }
    }
    
    t.wait_all();

}

void ThreadPoolerV(std::vector<std::vector<int>> Buff) {

    int dvr = 2;
    int step = max_y / dvr;
    int segments = dvr * dvr;
    int range = step - 1;
    int a = 1;

    thread_pool t(12);

    int y_start;
    int y_tail;
    int x_start;
    int x_tail;

    for (y_start = 0; y_start + range < max_y; y_start += step) {
        y_tail = y_start + range;

        for (x_start = 0; x_start + range < max_x; x_start += step) {
            x_tail = x_start + range;
            a++;
            t.add_task(BufferWriterV, std::ref(Buff), std::ref(y_start), std::ref(y_tail), std::ref(x_start), std::ref(x_tail));

        }
    }

    t.wait_all();

}
