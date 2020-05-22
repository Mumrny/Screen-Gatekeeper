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

	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0)) {
		ClsDrawScreen();

		DrawGraph(0, 0, imageDesktop, false);

		ScreenFlip();
	}

	// 自分で用意した分の後処理
	delete pData;
	DeleteDC(hMemDC);
	DeleteObject(hBitmapDesktop);

	DxLib_End();
	return 0;
}
