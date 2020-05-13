#include <windows.h>
#include <DxLib.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowText("Screen Gatekeeper");
	SetGraphMode(800, 600, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1; // DxLib_InitÇÃé∏îsÇ…ÇÊÇËèIóπ
	}
	SetDrawScreen(DX_SCREEN_BACK);

	HWND hDesktopWnd = GetDesktopWindow();
	HWND hMainWnd = GetMainWindowHandle();

	WINDOWINFO windowInfo;
	windowInfo.cbSize = sizeof(WINDOWINFO);

	struct Pos {
		int x;
		int y;
	};

	Pos rcPos = { 700 , 300 };
	Pos rcWindowCorrection = { 8 , 31 };

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

		InvalidateRect(hDesktopWnd, nullptr, true);
		UpdateWindow(hDesktopWnd);

		GetWindowInfo(hMainWnd, &windowInfo);

		ClsDrawScreen();
		DrawBox(
			rcPos.x - windowInfo.rcWindow.left - rcWindowCorrection.x,
			rcPos.y - windowInfo.rcWindow.top - rcWindowCorrection.y,
			rcPos.x + 100 - windowInfo.rcWindow.left - rcWindowCorrection.x,
			rcPos.y + 100 - windowInfo.rcWindow.top - rcWindowCorrection.y,
			0xffffff,
			true
		);

		DrawFormatString(0, 0, 0xffffff, "%d, %d", windowInfo.rcWindow.left, windowInfo.rcWindow.top);
		ScreenFlip();

		HDC hdc;
		hdc = CreateDC("DISPLAY", nullptr, nullptr, nullptr);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		//SelectObject(hdc, CreateSolidBrush(RGB(0xFF, 0, 0)));
		Rectangle(hdc, rcPos.x, rcPos.y, rcPos.x + 100, rcPos.y + 100);
		DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
		DeleteDC(hdc);
	}

	DxLib_End();
	return 0;
}
