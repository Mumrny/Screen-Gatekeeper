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

	// ����ް�̕\��
	HWND hTaskbarWnd = FindWindow("Shell_TrayWnd", nullptr);
	
	RECT rcTaskbar;
	GetWindowRect(hTaskbarWnd, &rcTaskbar);
	const int widthTaskbar = rcTaskbar.right;
	const int heightTaskbar = rcTaskbar.bottom - rcTaskbar.top;

	BITMAPINFO bmpInfoTaskbar;
	bmpInfoTaskbar.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoTaskbar.bmiHeader.biWidth = widthTaskbar;
	bmpInfoTaskbar.bmiHeader.biHeight = heightTaskbar;
	bmpInfoTaskbar.bmiHeader.biPlanes = 1;
	bmpInfoTaskbar.bmiHeader.biBitCount = 32;
	bmpInfoTaskbar.bmiHeader.biCompression = BI_RGB;

	hdc = GetDC(hMainWnd);
	LPDWORD lpPixelTaskbar;
	HBITMAP hBitmapTaskbar = CreateDIBSection(hdc, &bmpInfoTaskbar, DIB_RGB_COLORS, (void**)&lpPixelTaskbar, nullptr, 0);
	HDC hMemDcTaskbar = CreateCompatibleDC(hdc);
	ReleaseDC(hMainWnd, hdc);

	SelectObject(hMemDcTaskbar, hBitmapTaskbar);
	PrintWindow(hTaskbarWnd, hMemDcTaskbar, 0);

	BITMAP DDBInfoTaskbar;
	GetObject(hBitmapTaskbar, sizeof(BITMAP), &DDBInfoTaskbar);
	BYTE *pDataTaskbar = new BYTE[DDBInfoTaskbar.bmWidth * DDBInfoTaskbar.bmHeight * 4];

	BITMAPINFO DIBInfoTaskbar;
	DIBInfoTaskbar.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	DIBInfoTaskbar.bmiHeader.biWidth = DDBInfoTaskbar.bmWidth;
	DIBInfoTaskbar.bmiHeader.biHeight = DDBInfoTaskbar.bmHeight;
	DIBInfoTaskbar.bmiHeader.biPlanes = 1;
	DIBInfoTaskbar.bmiHeader.biBitCount = 32;
	DIBInfoTaskbar.bmiHeader.biCompression = BI_RGB;

	hdc = GetDC(hMainWnd);
	GetDIBits(hdc, hBitmapTaskbar, 0, DDBInfoTaskbar.bmHeight, (void*)pDataTaskbar, &DIBInfoTaskbar, DIB_RGB_COLORS);
	ReleaseDC(hMainWnd, hdc);

	int imageTaskbar = CreateGraphFromBmp(&bmpInfoTaskbar, pDataTaskbar);

	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0)) {
		ClsDrawScreen();

		DrawGraph(0, 0, imageDesktop, false);
		DrawGraph(0, rcTaskbar.top, imageTaskbar, false);

		ScreenFlip();

		// �^�X�N�o�[�̏����X�V
		SelectObject(hMemDcTaskbar, hBitmapTaskbar);
		PrintWindow(hTaskbarWnd, hMemDcTaskbar, 0);

		hdc = GetDC(hMainWnd);
		GetDIBits(hdc, hBitmapTaskbar, 0, DDBInfoTaskbar.bmHeight, (void*)pDataTaskbar, &DIBInfoTaskbar, DIB_RGB_COLORS);
		ReleaseDC(hMainWnd, hdc);

		ReCreateGraphFromBmp(&bmpInfoTaskbar, pDataTaskbar, imageTaskbar);
	}

	// �����ŗp�ӂ������̌㏈��
	delete pData;
	DeleteDC(hMemDC);
	DeleteObject(hBitmapDesktop);

	delete pDataTaskbar;
	DeleteDC(hMemDcTaskbar);
	DeleteObject(hBitmapTaskbar);

	DxLib_End();
	return 0;
}
