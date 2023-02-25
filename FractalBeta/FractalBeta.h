#pragma once

#include "resource.h"

#include <Windows.h>
#include <cmath>

class Mandelbrot {
public:
    Mandelbrot(int width, int height, double zoom, int maxIter)
        : m_width(width), m_height(height), m_zoom(zoom), m_maxIter(maxIter) {}

    int calculate(double x, double y) {
        double real = x;
        double imag = y;

        int iterations = 0;
        while (iterations < m_maxIter && (real * real + imag * imag) < 4.0) {
            double r2 = real * real;
            double i2 = imag * imag;

            imag = 2 * real * imag + y;
            real = r2 - i2 + x;

            iterations++;
        }

        return iterations;
    }

    void render(HDC hdc) {
        for (int x = 0; x < m_width; x++) {
            for (int y = 0; y < m_height; y++) {
                double xFractal = (x - m_width / 2) * m_zoom / m_width;
                double yFractal = (y - m_height / 2) * m_zoom / m_height;

                int iterations = calculate(xFractal, yFractal);
                int color = 0xFF0000 * iterations / m_maxIter;

                SetPixel(hdc, x, y, color);
            }
        }
    }

private:
    int m_width;
    int m_height;
    double m_zoom;
    int m_maxIter;
};

class Window {
public:
    Window(HINSTANCE hInstance, int nCmdShow)
        : m_hInstance(hInstance), m_nCmdShow(nCmdShow), m_fractal(new Mandelbrot(800, 600, 1, 100)) {}

    ~Window() {
        delete m_fractal;
    }

    int create() {
        WNDCLASSEX wc;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = reinterpret_cast<WNDPROC>(&Window::WndProc);
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = L"Mandelbrot";
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(&wc)) {
            MessageBox(NULL, L"Error registering windowclass", L"Error", MB_ICONERROR | MB_OK);
            return 1;
        }

        m_hwnd = CreateWindowEx(
            0,
            L"Mandelbrot",
            L"Mandelbrot",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            600,
            NULL,
            NULL,
            m_hInstance,
            this
        );

        if (!m_hwnd) {
            MessageBox(NULL, L"Error creating window", L"Error", MB_ICONERROR | MB_OK);
            return 1;
        }

        ShowWindow(m_hwnd, m_nCmdShow);
        UpdateWindow(m_hwnd);

        return messageLoop();
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            m_fractal->render(hdc);

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
    int messageLoop() {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return (int)msg.wParam;
    }
private:
    HINSTANCE m_hInstance;
    int m_nCmdShow;
    HWND m_hwnd;
    Mandelbrot* m_fractal;
};