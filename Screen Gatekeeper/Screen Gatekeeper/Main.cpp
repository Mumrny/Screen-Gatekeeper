#include <windows.h>
#include <DxLib.h>
#include <cassert>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowText("Screen Gatekeeper");

	// �޽�į�߂̻��ނ��擾
	HWND hDesktop = GetDesktopWindow();
	RECT rcDesktop;
	GetWindowRect(hDesktop, &rcDesktop);
	const int widthDesktop = rcDesktop.right;
	const int heightDesktop = rcDesktop.bottom;

	// �޽�į�߂̻��ނƓ����𑜓x�̳���޳�ɂ���
	SetGraphMode(widthDesktop, heightDesktop, 16);
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return -1; // DxLib_Init�̎��s�ɂ��I��
	}

	HWND hMainWnd = GetMainWindowHandle();

	SetDrawScreen(DX_SCREEN_BACK);

	// �޽�į�߂��ޯ�ϯ�߂̏��
	BITMAPINFO bmpInfoDesktop;
	bmpInfoDesktop.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoDesktop.bmiHeader.biWidth = widthDesktop;
	bmpInfoDesktop.bmiHeader.biHeight = heightDesktop;
	bmpInfoDesktop.bmiHeader.biPlanes = 1;
	bmpInfoDesktop.bmiHeader.biBitCount = 32;
	bmpInfoDesktop.bmiHeader.biCompression = BI_RGB;
	
	// ���޲���÷�Ă��g�p�����޽�į�߂��ޯ�ϯ�߂��쐬
	HDC hdc = GetDC(hMainWnd);
	LPDWORD lpPixelDesktop; // �悭�������ĂȂ���������ޯ�ϯ�߂��폜����Ή�������炵��
	HBITMAP hBitmapDesktop = CreateDIBSection(hdc, &bmpInfoDesktop, DIB_RGB_COLORS, (void**)&lpPixelDesktop, nullptr, 0);
	HDC hMemDC = CreateCompatibleDC(hdc); // ��ذ�Ɏc���Ďg��(�Ō�ɍ폜)
	ReleaseDC(hMainWnd, hdc);

	// �޽�į�߂̕ǎ��Ʊ��݂݂̂�����
	SelectObject(hMemDC, hBitmapDesktop);
	PrintWindow(GetShellWindow(), hMemDC, 0);

	// ���ׂ����ǂ܂��悭�������ĂȂ�����
	BITMAP DDBInfo;
	GetObject(hBitmapDesktop, sizeof(BITMAP), &DDBInfo);
	BYTE *pData = new BYTE[DDBInfo.bmWidth * DDBInfo.bmHeight * 4];

	BITMAPINFO DIBInfo;
	DIBInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	DIBInfo.bmiHeader.biWidth = DDBInfo.bmWidth;
	DIBInfo.bmiHeader.biHeight = DDBInfo.bmHeight;
	DIBInfo.bmiHeader.biPlanes = 1;
	DIBInfo.bmiHeader.biBitCount = 32;
	DIBInfo.bmiHeader.biCompression = BI_RGB;

	// �悭�������ĂȂ������޽�į�߂��ޯ�ϯ�ߏ����ް���DDB����DIB����
	// �g���Ď��ް�������Ă���H
	hdc = GetDC(hMainWnd);
	GetDIBits(hdc, hBitmapDesktop, 0, DDBInfo.bmHeight, (void*)pData, &DIBInfo, DIB_RGB_COLORS);
	ReleaseDC(hMainWnd, hdc);

	// CreateGraphFromBmp���޽�į�߂̉摜����ق��擾
	// (���炭�����������߂Ɏg�����߂��ޯ�ϯ�߂̏��A�����������ް�)
	int imageDesktop = CreateGraphFromBmp(&bmpInfoDesktop, pData);

	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0)) {
		ClsDrawScreen();

		DrawGraph(0, 0, imageDesktop, false);

		ScreenFlip();
	}

	// �����ŗp�ӂ������̌㏈��
	delete pData;
	DeleteDC(hMemDC);
	DeleteObject(hBitmapDesktop);

	DxLib_End();
	return 0;
}
