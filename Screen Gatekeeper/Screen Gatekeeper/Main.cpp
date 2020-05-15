#include <windows.h>
#include <DxLib.h>
#include <cassert>

WNDPROC dxWindowProcedure;
HWND hMainWnd;

struct Pos {
	int x;
	int y;
};

Pos rcPos = { 700 , 300 };

void Draw(void) {
	WINDOWINFO windowInfo;
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hMainWnd, &windowInfo);

	ClsDrawScreen();
	DrawBox(
		rcPos.x - windowInfo.rcClient.left,
		rcPos.y - windowInfo.rcClient.top,
		rcPos.x + 100 - windowInfo.rcClient.left,
		rcPos.y + 100 - windowInfo.rcClient.top,
		0xffffff,
		true
	);

	DrawFormatString(0, 0, 0xffffff, "%d, %d", windowInfo.rcClient.left, windowInfo.rcClient.top);
	ScreenFlip();
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOVE:
		//Draw(); 使うと止まる(処理が重い？)
		break;
	}
	
	return CallWindowProc(dxWindowProcedure, hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowText("Screen Gatekeeper");
	SetGraphMode(800, 600, 16);
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return -1; // DxLib_Initの失敗により終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// DxLibのウィンドウ+自前のコールバック関数にするための設定
	hMainWnd = GetMainWindowHandle();
	dxWindowProcedure = (WNDPROC)GetWindowLongPtr(hMainWnd, GWLP_WNDPROC); // GWLP_WNDPROC(ウィンドウプロシージャのアドレス)
	SetWindowLongPtr(hMainWnd, GWLP_WNDPROC, (LONG_PTR)WindowProcedure);

	/*HWND hDesktopWnd = GetDesktopWindow();*/

	char key[256];

	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
	{
		GetHitKeyStateAll(key);

		if (key[KEY_INPUT_RIGHT]) {
			rcPos.x += 3;
		}
		if (key[KEY_INPUT_LEFT]) {
			rcPos.x -= 3;
		}
		if (key[KEY_INPUT_DOWN]) {
			rcPos.y += 3;
		}
		if (key[KEY_INPUT_UP]) {
			rcPos.y -= 3;
		}

		/*InvalidateRect(hDesktopWnd, nullptr, true);
		UpdateWindow(hDesktopWnd);*/

		WINDOWINFO windowInfo;
		GetWindowInfo(hMainWnd, &windowInfo);

		Draw();

		//HDC hdc;
		//hdc = CreateDC("DISPLAY", nullptr, nullptr, nullptr);
		//SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		////SelectObject(hdc, CreateSolidBrush(RGB(0xFF, 0, 0)));
		//Rectangle(hdc, rcPos.x, rcPos.y, rcPos.x + 100, rcPos.y + 100);
		//DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
		//DeleteDC(hdc);

		/*InvalidateRect(hDesktopWnd, nullptr, true);
		UpdateWindow(hDesktopWnd);*/
	}

	DxLib_End();
	return 0;
}
