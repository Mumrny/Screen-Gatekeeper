#include <windows.h>
#include <DxLib.h>
#include <cassert>
#include "Geometry.h"
#include "GameWindow.h"
#include "Application.h"


Application::Application() {}

Application::~Application() {}


void
Application::CreateCaptureDataForHwnd(HWND hWnd, std::string key, bool reCreateFlag) {
	assert(hWnd != nullptr);
	for (auto capData : capDatas) {
		assert(capData.first != key);
	}

	capDatas[key].hWnd = hWnd;
	GetWindowRect(hWnd, &capDatas[key].rc);
	capDatas[key].rcSize =
		Size(
			capDatas[key].rc.right - capDatas[key].rc.left,
			capDatas[key].rc.bottom - capDatas[key].rc.top
		);
	
	capDatas[key].bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	capDatas[key].bmpInfo.bmiHeader.biWidth = capDatas[key].rcSize.width;
	capDatas[key].bmpInfo.bmiHeader.biHeight = capDatas[key].rcSize.height;
	capDatas[key].bmpInfo.bmiHeader.biPlanes = 1;
	capDatas[key].bmpInfo.bmiHeader.biBitCount = 32;
	capDatas[key].bmpInfo.bmiHeader.biCompression = BI_RGB;

	HDC hdc = GetDC(hMainWnd);
	LPDWORD lpPixel;
	capDatas[key].hBitmap = CreateDIBSection(hdc, &capDatas[key].bmpInfo, DIB_RGB_COLORS, (void**)&lpPixel, nullptr, 0);
	capDatas[key].hMemDC = CreateCompatibleDC(hdc);
	ReleaseDC(hMainWnd, hdc);

	SelectObject(capDatas[key].hMemDC, capDatas[key].hBitmap);
	HWND printHwnd = (hWnd == GetDesktopWindow() ? GetShellWindow() : hWnd);
	PrintWindow(printHwnd, capDatas[key].hMemDC, 0);
	
	GetObject(capDatas[key].hBitmap, sizeof(BITMAP), &capDatas[key].DDBInfo);
	capDatas[key].pData = new BYTE[capDatas[key].DDBInfo.bmWidth * capDatas[key].DDBInfo.bmHeight * 4];
	
	capDatas[key].DIBInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	capDatas[key].DIBInfo.bmiHeader.biWidth = capDatas[key].DDBInfo.bmWidth;
	capDatas[key].DIBInfo.bmiHeader.biHeight = capDatas[key].DDBInfo.bmHeight;
	capDatas[key].DIBInfo.bmiHeader.biPlanes = 1;
	capDatas[key].DIBInfo.bmiHeader.biBitCount = 32;
	capDatas[key].DIBInfo.bmiHeader.biCompression = BI_RGB;

	hdc = GetDC(hMainWnd);
	GetDIBits(hdc, capDatas[key].hBitmap, 0, capDatas[key].DDBInfo.bmHeight, (void*)capDatas[key].pData, &capDatas[key].DIBInfo, DIB_RGB_COLORS);
	ReleaseDC(hMainWnd, hdc);

	capDatas[key].hImg = CreateGraphFromBmp(&capDatas[key].bmpInfo, capDatas[key].pData);
	capDatas[key].reCreateFlag = reCreateFlag;
}

void
Application::ReCreateCapDataHimg(std::string key) {
	SelectObject(capDatas[key].hMemDC, capDatas[key].hBitmap);
	PrintWindow(capDatas[key].hWnd, capDatas[key].hMemDC, 0);

	HDC hdc = GetDC(hMainWnd);
	GetDIBits(hdc, capDatas[key].hBitmap, 0, capDatas[key].DDBInfo.bmHeight, (void*)capDatas[key].pData, &capDatas[key].DIBInfo, DIB_RGB_COLORS);
	ReleaseDC(hMainWnd, hdc);

	ReCreateGraphFromBmp(&capDatas[key].bmpInfo, capDatas[key].pData, capDatas[key].hImg);
}

const Size
Application::GetScreenSize(void) const {
	return screenSize;
}

bool
Application::Init(void) {
	SetWindowText("Screen Gatekeeper");
	
	RECT rcDesktop;
	GetWindowRect(GetDesktopWindow(), &rcDesktop);
	screenSize.width = rcDesktop.right;
	screenSize.height = rcDesktop.bottom;
	
	SetGraphMode(screenSize.width, screenSize.height, 16);
	ChangeWindowMode(true);

	if (DxLib_Init() == -1) {
		return false;
	}

	hMainWnd = GetMainWindowHandle();

	SetDrawScreen(DX_SCREEN_BACK);

	CreateCaptureDataForHwnd(GetDesktopWindow(), "desktop", false);
	CreateCaptureDataForHwnd(FindWindow("Shell_TrayWnd", nullptr), "taskbar", true);

	gameWnd.reset(new GameWindow());
	gameWnd->Init();

	return true;
}

void
Application::Run(void) {
	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0)) {
		ClsDrawScreen();

		DrawGraph(0, 0, capDatas["desktop"].hImg, false);

		// タスクバーの色情報の取り方が見つかるまでの仮置き背景
		SetDrawBlendMode(DX_BLENDMODE_ALPHA_X4, 256 - 4);
		SetDrawBright(68, 68, 68);
		DrawBox(
			capDatas["taskbar"].rc.left,
			capDatas["taskbar"].rc.top,
			capDatas["taskbar"].rc.right,
			capDatas["taskbar"].rc.bottom,
			0x202020,
			true
		);
		SetDrawBright(255, 255, 255);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawGraph(0, capDatas["taskbar"].rc.top, capDatas["taskbar"].hImg, true);

		gameWnd->Draw();

		ScreenFlip();

		for (auto capData : capDatas) {
			if (capData.second.reCreateFlag) {
				ReCreateCapDataHimg(capData.first);
			}
		}
	}
}

void
Application::Terminate(void) {
	for (auto capData : capDatas) {
		delete capData.second.pData;
		DeleteDC(capData.second.hMemDC);
		DeleteObject(capData.second.hBitmap);
	}

	DxLib_End();
}
