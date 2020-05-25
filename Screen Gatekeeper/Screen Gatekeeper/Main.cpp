#include <windows.h>
#include <DxLib.h>
#include <cassert>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowText("Screen Gatekeeper");

	// ﾃﾞｽｸﾄｯﾌﾟのｻｲｽﾞを取得
	HWND hDesktop = GetDesktopWindow();
	RECT rcDesktop;
	GetWindowRect(hDesktop, &rcDesktop);
	const int widthDesktop = rcDesktop.right;
	const int heightDesktop = rcDesktop.bottom;

	// ﾃﾞｽｸﾄｯﾌﾟのｻｲｽﾞと同じ解像度のｳｨﾝﾄﾞｳにする
	SetGraphMode(widthDesktop, heightDesktop, 16);
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return -1; // DxLib_Initの失敗により終了
	}

	HWND hMainWnd = GetMainWindowHandle();

	SetDrawScreen(DX_SCREEN_BACK);

	// ﾃﾞｽｸﾄｯﾌﾟのﾋﾞｯﾄﾏｯﾌﾟの情報
	BITMAPINFO bmpInfoDesktop;
	bmpInfoDesktop.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoDesktop.bmiHeader.biWidth = widthDesktop;
	bmpInfoDesktop.bmiHeader.biHeight = heightDesktop;
	bmpInfoDesktop.bmiHeader.biPlanes = 1;
	bmpInfoDesktop.bmiHeader.biBitCount = 32;
	bmpInfoDesktop.bmiHeader.biCompression = BI_RGB;
	
	// ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄを使用してﾃﾞｽｸﾄｯﾌﾟのﾋﾞｯﾄﾏｯﾌﾟを作成
	HDC hdc = GetDC(hMainWnd);
	LPDWORD lpPixelDesktop; // よく分かってないやつだけどﾋﾞｯﾄﾏｯﾌﾟを削除すれば解放されるらしい
	HBITMAP hBitmapDesktop = CreateDIBSection(hdc, &bmpInfoDesktop, DIB_RGB_COLORS, (void**)&lpPixelDesktop, nullptr, 0);
	HDC hMemDC = CreateCompatibleDC(hdc); // ﾓﾒﾘｰに残して使う(最後に削除)
	ReleaseDC(hMainWnd, hdc);

	// ﾃﾞｽｸﾄｯﾌﾟの壁紙とｱｲｺﾝのみをｷｬﾌﾟﾁｬ
	SelectObject(hMemDC, hBitmapDesktop);
	PrintWindow(GetShellWindow(), hMemDC, 0);

	// 調べたけどまだよく分かってない部分
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

	// よく分かってないけどﾃﾞｽｸﾄｯﾌﾟのﾋﾞｯﾄﾏｯﾌﾟ情報をﾍﾞｰｽにDDB情報とDIB情報を
	// 使って実ﾃﾞｰﾀを作っている？
	hdc = GetDC(hMainWnd);
	GetDIBits(hdc, hBitmapDesktop, 0, DDBInfo.bmHeight, (void*)pData, &DIBInfo, DIB_RGB_COLORS);
	ReleaseDC(hMainWnd, hdc);

	// CreateGraphFromBmpでﾃﾞｽｸﾄｯﾌﾟの画像ﾊﾝﾄﾞﾙを取得
	// (恐らく第一引数が解釈に使うためのﾋﾞｯﾄﾏｯﾌﾟの情報、第二引数が実ﾃﾞｰﾀ)
	int imageDesktop = CreateGraphFromBmp(&bmpInfoDesktop, pData);

	// ﾀｽｸﾊﾞｰの表示
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

		// タスクバーの情報を更新
		SelectObject(hMemDcTaskbar, hBitmapTaskbar);
		PrintWindow(hTaskbarWnd, hMemDcTaskbar, 0);

		hdc = GetDC(hMainWnd);
		GetDIBits(hdc, hBitmapTaskbar, 0, DDBInfoTaskbar.bmHeight, (void*)pDataTaskbar, &DIBInfoTaskbar, DIB_RGB_COLORS);
		ReleaseDC(hMainWnd, hdc);

		ReCreateGraphFromBmp(&bmpInfoTaskbar, pDataTaskbar, imageTaskbar);
	}

	// 自分で用意した分の後処理
	delete pData;
	DeleteDC(hMemDC);
	DeleteObject(hBitmapDesktop);

	delete pDataTaskbar;
	DeleteDC(hMemDcTaskbar);
	DeleteObject(hBitmapTaskbar);

	DxLib_End();
	return 0;
}
