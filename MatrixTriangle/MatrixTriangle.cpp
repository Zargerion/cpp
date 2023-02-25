// MatrixTriangle.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "MatrixTriangle.h"

#include <Windows.h>
#include <cmath>

const float PI = 3.14159265f;

// Function to rotate a point around the origin using a matrix
POINT RotatePoint(POINT point, float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    POINT result;
    result.x = point.x * c - point.y * s;
    result.y = point.x * s + point.y * c;
    return result;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static POINT triangle[] = { { 100, 50 }, { 50, 150 }, { 150, 150 } };
    static float angle = 0.0f;

    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Rotate the triangle
        for (int i = 0; i < 3; i++)
            triangle[i] = RotatePoint(triangle[i], angle);

        // Draw the triangle
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        SelectObject(hdc, pen);
        MoveToEx(hdc, triangle[0].x, triangle[0].y, NULL);
        LineTo(hdc, triangle[1].x, triangle[1].y);
        LineTo(hdc, triangle[2].x, triangle[2].y);
        LineTo(hdc, triangle[0].x, triangle[0].y);
        DeleteObject(pen);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_UP:
            angle -= PI / 180;
            break;
        case VK_DOWN:
            angle += PI / 180;
            break;
        }
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"TriangleRenderer";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Failed to register window class!", L"Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    HWND hwnd = CreateWindowEx(0, L"TriangleRenderer", L"Triangle Renderer", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, hInstance, NULL);

    if (!hwnd)
    {
        MessageBox(NULL, L"Failed to create window!", L"Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}