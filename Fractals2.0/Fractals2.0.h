#pragma once
// ? = if true so a else b
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define _USE_MATH_DEFINES
#include "framework.h"



#include <cmath>
#include <vector>
#include <algorithm>
#include <complex>
#include <cassert>
#include <string>

#include "resource.h"
#include "thread-pool.h"


struct pixel_rgb { 
	unsigned char r, g, b, a; 

	pixel_rgb() {}
	pixel_rgb(unsigned char value) : r(value), g(value), b(value), a(value) {}
	pixel_rgb(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : r(_r), g(_g), b(_b), a(_a) {}

};
struct Rect { int sx, sy, ex, ey; };

//typedef std::complex<double> Complex;
//Complex sqr(const Complex& v)
//{
//	return pow(v, 2);
//}

class Geometric {
protected:
	void draw_rectangle(std::vector<pixel_rgb>& buff, const size_t buff_w, const size_t buff_h, const size_t x, const size_t y, const size_t w, const size_t h, const pixel_rgb color) {
		assert(buff.size() == buff_w * buff_h);
		for (size_t i = 0; i < w; i++) {
			for (size_t j = 0; j < h; j++) {
				size_t cx = x + i;
				size_t cy = y + j;
				assert(cx < buff_w && cy < buff_h);
				buff[cx + cy * buff_w] = color;
			}
		}
	}
};

class Canvas : private Geometric { // thread pool legacy
public:
	Canvas() /*: m_parts(0) , m_is_complete(false)*/ {
		//memset(m_BufferV, 0, sizeof(pixel_rgb));
		/*std::fill(m_BufferV.begin(), m_BufferV.end(), std::vector<int>(max_x, 0));*/
	}
	bool m_is_complete = false;

	void simpleWriter() {
		const size_t map_w = 16; // map width
		const size_t map_h = 16;
		const char map[] = "0000222222220000"\
							"1              0"\
							"1      11111   0"\
							"1     0        0"\
							"0     0  1110000"\
							"0     3        0"\
							"0   10000      0"\
							"0   0   11100  0"\
							"0   0   0      0"\
							"0   0   1  00000"\
							"0       1      0"\
							"2       1      0"\
							"0       0      0"\
							"0 0000000      0"\
							"0              0"\
							"0002222222200000";
		float player_x = 3.456;
		float player_y = 2.345;
		float player_a = 1.523; // player view direction
		const float fov = M_PI / 3.; // field of view
		assert(sizeof(map) == map_w * map_h + 1);
		for (size_t j = 0; j < win_h; j++) { // fill the screen with color gradients
			for (size_t i = 0; i < win_w; i++) {
				pixel_rgb& p = m_framebuffer[i + j * win_w];
				p.r = 255 * j / float(win_h);;
				p.g = 255 * i / float(win_w);
				p.b = 0;
				p.a = 0;

			}
		}
		pixel_rgb a;
		a.r = 0;
		a.g = 255;
		a.b = 255;
		a.a = 0;
		const size_t rect_w = win_w / map_w;
		const size_t rect_h = win_h / map_h;
		for (size_t j = 0; j < map_h; j++) { // draw the map
			for (size_t i = 0; i < map_w; i++) {
				if (map[i + j * map_w] == ' ') continue; // skip empty spaces
				size_t rect_x = i * rect_w;
				size_t rect_y = j * rect_h;

				draw_rectangle(m_framebuffer, win_w, win_h, rect_x, rect_y, rect_w, rect_h, a);
			}
		}
		// draw the player on the map
		draw_rectangle(m_framebuffer, win_w, win_h, (player_x * rect_w) - 7/2, (player_y * rect_h) - 7/2, 7, 7, pixel_rgb(255, 255, 0, 0));

		size_t pix_x;
		size_t pix_y;

		for (size_t i = 0; i < win_w; i++) { // draw the visibility cone
			float angle = player_a - fov / 2 + fov * i / float(win_w);

			for (float t = 0; t < 20; t += .05) {
				float cx = player_x + t * cos(angle); // катет x
				float cy = player_y + t * sin(angle); // rfntn y точки в которую упирается чел.
				if (map[int(cx) + int(cy) * map_w] != ' ') break;

				pix_x = cx * rect_w;
				pix_y = cy * rect_h;
				m_framebuffer[pix_x + pix_y * win_w] = pixel_rgb(255, 255, 255, 0);
			}
		}
	}

	void GenerateAsync(thread_pool* pTP) {

		struct _S { static void GenerateDelta(Canvas* Fractal, Rect rect) { Fractal->GenerateDelta(rect); } }; //Вызывает функцию заполнения буфера.

		const int Delta = 60;
		m_parts = (int)std::ceil(600 / float(Delta)) * (int)std::ceil( 600 / float(Delta) );
		
		for (int y = 0; y < 600; y += Delta) {
			for (int x = 0; x < 600; x += Delta) {
				Rect rect;
				rect.sx = x;
				rect.ex = MIN(x + Delta, 600);
				rect.sy = y;
				rect.ey = MIN(y + Delta, 600);


				pTP->add_task(_S::GenerateDelta, this, rect);
			}
		}
		
		pTP->wait_all();

		if (pTP->get_completed_idx() == pTP->get_completed_task_ids())
		{
			m_is_complete = true;
		}
	}

	void GenerateDelta(Rect rect) {

		for (int y = rect.sy; y < rect.ey; ++y) {
			for (int x = rect.sx; x < rect.ex; ++x) {

				//Complex zcomplex(0, 0);
				//int i = 0;
				//while (i < 10 && abs(zcomplex) < 2)
				//{
				//	zcomplex = sqr(zcomplex) + Complex(1.0 * (x - 600 / 2) / 120, // Z is complex value. Equation if z = z^2 + c, there C is a variable of complex values.
				//		1.0 * (y - 600 / 2) / 120);                   // x - max_x/ 2 = scren center.
				//	++i;
				//}
				//if (abs(zcomplex) >= 2)
				//{

				//	// Modulus of complex value that R value is equel or higher then 2 so print a point.
				//	// +40 by x axis becouse center of fractal displaysed by -1/5 of width by x
				//	pixel_rgb& p = m_BufferV[y][x]; // p is link to element of m_BufferV
				//	p.r = 0;
				//	p.g = 0;
				//	p.b = 1;
				//	p.a = 0; // +40 TO X.
				//}

				//pixel_rgb& p = m_BufferV[y][x]; // p is link to element of m_BufferV
				//p.r = 1;
				//p.g = 1;
				//p.b = 1;
				//p.a = 1;

				// cercle
				//const int w = 600;
				//const int reducedw = w;
				//if (((x - w / 2) * (x - w / 2) + (y - w / 2) * (y - w / 2)) <= reducedw / 2 ||
				//	((((x - w / 2) * (x - w / 2) + (y - w / 2) * (y - w / 2) >= reducedw * 2)) &&
				//		(((x - w / 2) * (x - w / 2) + (y - w / 2) * (y - w / 2) <= reducedw * 4))))
				//{
				//	pixel_rgb& p = m_BufferV[y][x]; // p is link to element of m_BufferV
				//	p.r = 0;
				//	p.g = 1;
				//	p.b = 0;
				//	p.a = 0;
				//}



				int y = (sin(1.0 * x * 2 * M_PI / 600) + 1) / 2 * 600;
				if ((y >= 0) && (y < 600))
				{
					pixel_rgb& p = m_BufferV[y][x]; // p is link to element of m_BufferV
					p.r = 1;
					p.g = 0;
					p.b = 0;
					p.a = 0;
				}

			}
		}
		
	}
	bool IsComplete() const { /*return m_parts == 0;*/ return m_is_complete; }
	const std::vector<std::vector<pixel_rgb>> GetBuffer() const
	{
		return m_BufferV;
	}
	const std::vector<pixel_rgb> GetFrameBuffer() const
	{
		return m_framebuffer;
	}
private:


	const int max_y = 600;
	const int max_x = 600;

	const size_t win_w = 600; // image width
	const size_t win_h = 600; // image height

	std::vector<pixel_rgb> m_framebuffer = std::vector<pixel_rgb>(win_w * win_h, {255, 255, 255, 255});
	//pixel_rgb a = { a.r = 0, a.g = 0, a.b = 0, a.a = 0, };

	//std::vector<pixel_rgb>m_vec1d = std::vector<pixel_rgb>(max_x * max_x * max_x, a);

	std::vector<std::vector<pixel_rgb>>m_BufferV = std::vector<std::vector<pixel_rgb>>(max_y, std::vector<pixel_rgb>(max_x));
	
	//std::vector<std::vector<std::vector<pixel_rgb>>>m_Space3d = std::vector<std::vector<std::vector<pixel_rgb>>>(max_x, std::vector<std::vector<pixel_rgb>>(max_x, std::vector<pixel_rgb>(max_x)));

	std::atomic<int> m_parts;
};
Canvas static canvas;
class Application {
	
public:
	Application(HINSTANCE hInstance, int nCmdShow) /*:m_hInst(hInstance),*/: m_TP(12) {
		MyRegisterClass(hInstance);
		m_hWnd = CreateWindowW(L"MyWindowsClass", L"SUUUUUUUUUUUUUKA!", WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, this);

		canvas.GenerateAsync(&m_TP);
		canvas.simpleWriter();

		ShowWindow(m_hWnd, nCmdShow);
	}
	bool Run() {
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}

private:
	//HINSTANCE m_hInst;
	HWND m_hWnd;
	thread_pool m_TP;
private:

	void MyRegisterClass(HINSTANCE hInstance)
	{
		/*struct _S {
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
				Application* pApp = (Application*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
				if (pApp == nullptr) {
					if (message == WM_CREATE) {
						CREATESTRUCT* str_cr = (CREATESTRUCT*)lParam;
						Application* pApp = (Application*)str_cr->lpCreateParams;
						SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pApp);
					}
					else return DefWindowProc(hWnd, message, wParam, lParam);
				}
				else pApp->WndProc(hWnd, message, wParam, lParam);
			}
		};*/

		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = /*_S::WndProc;*/ (WNDPROC) WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FRACTALS20));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = L"Menu";
		wcex.lpszClassName = L"MyWindowsClass";
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		RegisterClassExW(&wcex);
	}

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		
		switch (message)
		{
		case WM_NCCREATE:
		{
			Application* pApp = (Application*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (pApp == nullptr) {
				if (message == WM_CREATE) {
					CREATESTRUCT* str_cr = (CREATESTRUCT*)lParam;
					Application* pApp = (Application*)str_cr->lpCreateParams;
					SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pApp);
				}
				else return DefWindowProc(hWnd, message, wParam, lParam);
			}
			else pApp->WndProc(hWnd, message, wParam, lParam);
		}
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
			{
			std::vector<std::vector<pixel_rgb>> bufferV;
			bufferV = canvas.GetBuffer();
			std::vector<pixel_rgb>framebuffer;
			framebuffer = canvas.GetFrameBuffer();
			const size_t win_w = 600; // image width
			const size_t win_h = 600; // image height
			char text[80];
			static int count = 0;
			HDC hDC;
			hDC = GetDC(hWnd);
			RECT rect;
			HFONT hfontArial;
			HGDIOBJ oldfont;
			PAINTSTRUCT ps;
			GetClientRect(hWnd, &rect);
			hfontArial = CreateFont(16, 0, FW_DONTCARE, FW_DONTCARE, FW_DONTCARE,
				FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Times New Roman Bold");
			oldfont = SelectObject(hDC, hfontArial);
			SetTextColor(hDC, RGB(255, 0, 0));
			DrawText(hDC, L"SSUka", 8, &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			DeleteObject(hfontArial);
			ReleaseDC(hWnd, hDC);
			HDC hdc;
			hdc	= BeginPaint(hWnd, &ps);
			//render buffer
			snprintf(text, count++, "Count = %d");
			for (size_t j = 0; j < win_h; j++) { // fill the screen with color gradients
				for (size_t i = 0; i < win_w; i++) {
					pixel_rgb& p = framebuffer[i + j * win_w];
					SetPixel(hdc, i, j, RGB(p.r, p.g, p.b));
				}
			}
			if (canvas.IsComplete()) {
				for (int y = 0; y < 600; y++)
				{
					for (int x = 0; x < 600; x++)
					{
						if (bufferV[y][x].r == 1)
						{
							SetPixel(hdc, x, y, RGB(255, 255, 255));
						}
						if (bufferV[y][x].g == 1)
						{
							SetPixel(hdc, x, y, RGB(0, 255, 0));
						}
						if (bufferV[y][x].b == 1)
						{
							SetPixel(hdc, x, y, RGB(0, 0, 255));
						}

						/*if (bufferV[y][x].a == 1)
						{
							int r = bufferV[y][x].r;
							int g = bufferV[y][x].g;
							int b = bufferV[y][x].b;
							SetPixel(hdc, x, y, RGB(r, g, b));
						}*/
						//________________________________________
					}
				}
			}
			else 
			{
				for (size_t i = 0; i < 100; i++)
				{
					for (size_t j = 0; j < 100; j++)
					{
						SetPixel(hdc, i, j, RGB(0, 0, 0));
					}
				}
			}
			EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
};