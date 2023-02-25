#include "Fractals2.0.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	Application app(hInstance, nCmdShow);
	return app.Run()/* ? 0 : -1*/;
}
