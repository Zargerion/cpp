#include <Windows.h>
#include <iostream>

// Function to handle keyboard input
void HandleKeyboardInput()
{
    if (GetAsyncKeyState('Q') & 0x8000)
    {
        std::cout << "Q Key pressed" << std::endl;
    }
}

// Function to draw a line
void DrawLine(HDC hdc, POINT start, POINT end)
{
    MoveToEx(hdc, start.x, start.y, NULL);
    LineTo(hdc, end.x, end.y);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        POINT start = { 100, 100 };
        POINT end = { 200, 200 };

        HandleKeyboardInput();

        // Change the end point if the 'Q' key is pressed
        if (GetAsyncKeyState('Q') & 0x8000)
        {
            end.x += 50;
            end.y += 50;
        }

        DrawLine(hdc, start, end);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndClass = { 0 };
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hInstance = hInstance;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.lpszClassName = L"MyWindowClass";

    if (!RegisterClassEx(&wndClass))
    {
        return 1;
    }

    HWND hwnd = CreateWindowEx(0, L"MyWindowClass", L"My Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    if (!hwnd)
    {
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    MSG msg;
    BOOL gResult;
    while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (gResult == -1)
    {
        return 1;
    }
    else
    {
        return (int)msg.wParam;
    }
}