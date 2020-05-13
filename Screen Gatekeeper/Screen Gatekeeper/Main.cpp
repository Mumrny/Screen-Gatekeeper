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

	SetDrawScreen(DX_SCREEN_OTHER);

	int pos_x = 700;
	int pos_y = 300;

	char key[256];

	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
	{
		GetHitKeyStateAll(key);

		if (key[KEY_INPUT_RIGHT]) {
			pos_x += 3;
		}
		if (key[KEY_INPUT_LEFT]) {
			pos_x -= 3;
		}
		if (key[KEY_INPUT_DOWN]) {
			pos_y += 3;
		}
		if (key[KEY_INPUT_UP]) {
			pos_y -= 3;
		}
		
		ClsDrawScreen();

		DrawGraph(0, 0, 0, true);

		ScreenFlip();

		/*UpdateWindow(GetMainWindowHandle());*/

		HDC hdc;
		hdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		//SelectObject(hdc, CreateSolidBrush(RGB(0xFF, 0, 0)));
		Rectangle(hdc, pos_x, pos_y, pos_x + 100, pos_y + 100);
		DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
		DeleteDC(hdc);
	}

	DxLib_End();
	return 0;
}
