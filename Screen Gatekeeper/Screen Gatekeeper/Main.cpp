#include <windows.h>
#include <DxLib.h>
#include <cassert>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowText("Screen Gatekeeper");
	SetGraphMode(1280, 720, 16);
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return -1; // DxLib_Initの失敗により終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// デスクトップのビットマップハンドルを作成
	HWND hDesktop = GetDesktopWindow();
	RECT rcDesktop;
	GetWindowRect(hDesktop, &rcDesktop);

	BITMAPINFO bmpInfoDesktop;
	bmpInfoDesktop.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoDesktop.bmiHeader.biWidth = rcDesktop.right;
	bmpInfoDesktop.bmiHeader.biHeight = rcDesktop.bottom;
	bmpInfoDesktop.bmiHeader.biPlanes = 1;
	bmpInfoDesktop.bmiHeader.biBitCount = 32;
	bmpInfoDesktop.bmiHeader.biCompression = BI_RGB;
	
	HDC hdc = GetDC(GetMainWindowHandle());
	LPDWORD lpPixelDesktop;
	HBITMAP hBitmapDesktop = CreateDIBSection(hdc, &bmpInfoDesktop, DIB_RGB_COLORS, (void**)&lpPixelDesktop, nullptr, 0);
	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, hBitmapDesktop);
	ReleaseDC(GetMainWindowHandle(), hdc);

	hdc = GetDC(hDesktop);
	BitBlt(hMemDC, 0, 0, bmpInfoDesktop.bmiHeader.biWidth, bmpInfoDesktop.bmiHeader.biHeight, hdc, 0, 0, SRCCOPY);
	ReleaseDC(hDesktop, hdc);

	BITMAP DDBInfo;
	BITMAPINFO DIBInfo;

	GetObject(hBitmapDesktop, sizeof(BITMAP), &DDBInfo);
	DIBInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	DIBInfo.bmiHeader.biWidth = DDBInfo.bmWidth;
	DIBInfo.bmiHeader.biHeight = DDBInfo.bmHeight;
	DIBInfo.bmiHeader.biPlanes = 1;
	DIBInfo.bmiHeader.biBitCount = 32;
	DIBInfo.bmiHeader.biCompression = BI_RGB;

	BYTE *pData = new BYTE[DDBInfo.bmWidth * DDBInfo.bmHeight * 4];
	hdc = GetDC(GetMainWindowHandle());
	GetDIBits(hdc, hBitmapDesktop, 0, DDBInfo.bmHeight, (void*)pData, &DIBInfo, DIB_RGB_COLORS);
	ReleaseDC(GetMainWindowHandle(), hdc);

	DeleteObject(hBitmapDesktop);

	int imageDesktop = CreateGraphFromBmp(&bmpInfoDesktop, pData);
	delete pData;

	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
	{
		ClsDrawScreen();

		DrawGraph(0, 0, imageDesktop, false);

		ScreenFlip();
	}

	DeleteDC(hMemDC);
	DxLib_End();
	return 0;
}
