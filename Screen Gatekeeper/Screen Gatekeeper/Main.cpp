#include <windows.h>
#include <cassert>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HRESULT result = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	assert(SUCCEEDED(result));

	WNDCLASSEX winc = {};
	winc.cbSize = sizeof(WNDCLASSEX);
	winc.lpfnWndProc = (WNDPROC)WindowProcedure;
	winc.hInstance = hInstance;
	winc.lpszClassName = "SRGK";
	assert(RegisterClassEx(&winc));

	RECT winrc = { 0,0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winrc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd;
	hwnd = CreateWindow(
		winc.lpszClassName,
		"Screen Gatekeeper",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		winrc.right - winrc.left,
		winrc.bottom - winrc.top,
		nullptr,
		nullptr,
		winc.hInstance,
		nullptr
	);
	assert(hwnd != nullptr);

	ShowWindow(hwnd, SW_SHOW);

	MSG msg;
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			break;
		}
	}

	UnregisterClass(winc.lpszClassName, winc.hInstance);

	return 0;
}

//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
//	HDC hdc;
//
//	switch (msg) {
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	case WM_PAINT:
//		hdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
//		SelectObject(hdc, CreateSolidBrush(RGB(0xFF, 0, 0)));
//		Rectangle(hdc, 100, 100, 200, 200);
//		DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
//		DeleteDC(hdc);
//	}
//	return DefWindowProc(hwnd, msg, wp, lp);
//}
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//	PSTR lpCmdLine, int nCmdShow) {
//	HWND hwnd;
//	MSG msg;
//	WNDCLASS winc;
//
//	winc.style = CS_HREDRAW | CS_VREDRAW;
//	winc.lpfnWndProc = WndProc;
//	winc.cbClsExtra = winc.cbWndExtra = 0;
//	winc.hInstance = hInstance;
//	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	winc.lpszMenuName = NULL;
//	winc.lpszClassName = TEXT("KITTY");
//
//	if (!RegisterClass(&winc)) return -1;
//
//	hwnd = CreateWindow(
//		TEXT("KITTY"), TEXT("Kitty on your lap"),
//		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
//		CW_USEDEFAULT, CW_USEDEFAULT,
//		CW_USEDEFAULT, CW_USEDEFAULT,
//		NULL, NULL,
//		hInstance, NULL
//	);
//
//	if (hwnd == NULL) return -1;
//
//	while (GetMessage(&msg, NULL, 0, 0)) {
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//	return msg.wParam;
//}


//#include <DxLib.h>
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//	SetWindowText("Screen Gatekeeper");
//	SetGraphMode(800, 600, 16);
//	ChangeWindowMode(true);
//
//	if (DxLib_Init() == -1)
//	{
//		return -1; // DxLib_InitÇÃé∏îsÇ…ÇÊÇËèIóπ
//	}
//
//	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
//	{
//	}
//
//	DxLib_End();
//	return 0;
//}
