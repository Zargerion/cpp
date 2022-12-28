#include <windows.h>
#include <gdiplus.h>
#include <ctime>
#include <string> 
#include <resource.h>

#define OnButtonClick 1

using namespace std;

//Prototipes (declaration in file).
LRESULT CALLBACK WindowMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hinst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
void draw(HDC hdc, RECT rc);
void widgets(HWND hwnd);
RECT areatopaint(HWND hWnd, HDC hdc);



int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount)
{
	// Initialize GDI+.
	Gdiplus::GdiplusStartupInput gdiplusStartupInput; // It makes an empty valiable of this stract {a, b, c ...}.
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr); // The init.

	//Init WinApi by usin' class.
	WNDCLASS WindowMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), currentInstance,
		LoadIcon(currentInstance, MAKEINTRESOURCE(IDI_ICON1)), L"MainWindowClass", WindowMainProcedure);
	if (!RegisterClassW(&WindowMainClass)) { return -1; }
	
	// Y, X, Height, Width. L"" means unicode string only.
	CreateWindow(L"MainWindowClass", L"The Magic",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style.
		CW_USEDEFAULT, CW_USEDEFAULT,     // Window initial position.
		700, 700,                         // Window size.
		NULL, NULL, NULL, NULL);

	//Main loop.
	MSG WindowMainMessage = { 0 };
	while (GetMessage(&WindowMainMessage, NULL, NULL, NULL))
	{
		TranslateMessage(&WindowMainMessage);
		DispatchMessage(&WindowMainMessage);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}


//
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hinst, HICON Icon, LPCWSTR Name, WNDPROC Procedure)
{
	WNDCLASS NWC = { 0 }; // Creating valiable of this stract (class) {a, b, c ...}.

	NWC.hCursor = Cursor;
	NWC.hIcon = Icon;
	NWC.hInstance = hinst; // Handle from treade the inst of window that contain metadata for OS.
	NWC.lpszClassName = Name; // To exit window.
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

// Logic of prosess or more then one. (Procedure).
LRESULT CALLBACK WindowMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) // Data about created window.
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_CREATE:
		widgets(hWnd);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		RECT paintingArea = areatopaint(hWnd, hdc);
		draw(hdc, paintingArea);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (wp)
		{
		case OnButtonClick:
			MessageBoxA(hWnd, "Dont click it more!", "Hi, Bro!", MB_OK | MB_ICONINFORMATION);
			return 0;
		default: break;
		}
	case WM_DESTROY: // It contain massage that window is destroyed. 
		PostQuitMessage(0); // It is killing prosess when window is destroyed.
		break;

	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void widgets(HWND hwnd)
{
	CreateWindowA("button", "Press", WS_VISIBLE | WS_CHILD | ES_CENTER, 700/2-(70/2), 620, 70, 30, hwnd, (HMENU)OnButtonClick, NULL, NULL);
}

RECT areatopaint(HWND hWnd, HDC hdc)
{
	RECT clientrc;
	GetClientRect(hWnd, &clientrc);
	RECT rc;
	int sft = 55;
	rc.left = sft;
	rc.bottom = clientrc.bottom - sft;
	rc.top = sft;
	rc.right = clientrc.bottom - sft+22; //idk why.
	return rc;
}

void draw(HDC hdc, RECT rc) 
{
	Gdiplus::Graphics gf(hdc);
	Gdiplus::Pen pen(Gdiplus::Color(255, 255, 0, 0));               // For lines, rectangles and curves.
	//Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 255, 0));      // For filled shapes.

	wstring s = to_wstring(rc.right);
	const wchar_t * w = s.c_str();

	WCHAR string[] = { (char)'|', (char)'>', w[0], w[1], w[2], (char)'<', (char)'|'};

	// Initialize arguments.
	Gdiplus::Font myFont(L"Arial", 16);
	Gdiplus::RectF layoutRect(2.0f, 15.0f, 100.0f, 20.0f);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));

	// Draw string.
	gf.DrawString(
			string,
			11,
			&myFont,
			layoutRect,
			&format,
			&blackBrush);

	// Clear WIn API.
	for (int i = 1; i < 700; i++)
	{
		SetPixel(hdc, i, i, RGB(0, 255, 255));
	}
	


	for (int i = 1; i < 700; i+=100)
	{
		gf.DrawLine(&pen, 0, i, 700, 0);
		gf.DrawLine(&pen, i, 700, 700, 0);
	}


	//gf.FillRectangle(&brush, 320, 200, 100, 100);
	gf.DrawRectangle(&pen, (int)rc.left, (int)rc.top, (int)rc.right - rc.left, (int)rc.bottom - rc.top);

	//Gdiplus::Bitmap bmp(L"ForTheBook.png");
	//gf.DrawImage(&bmp, 430, 10);

	//gf.FillEllipse(&brush, 50, 400, 200, 100);

}
