#pragma once
#include <windows.h>
#include <unordered_map>
#include <memory>
#include "Geometry.h"


class GameWindow;

struct CaptureData {
	HWND hWnd;
	RECT rc;
	Size rcSize;
	BITMAPINFO bmpInfo;
	HBITMAP hBitmap;
	HDC hMemDC;
	BITMAP DDBInfo;
	BITMAPINFO DIBInfo;
	BYTE* pData;
	int hImg;
	bool reCreateFlag;
};

class Application {
private:
	Application();
	// ºËß°‹Ö~
	Application(const Application&) = delete;
	// ‘ã“ü‹Ö~
	void operator=(const Application&) = delete;

	Size screenSize;

	HWND hMainWnd;
	std::unordered_map<std::string, CaptureData> capDatas;

	std::shared_ptr<GameWindow> gameWnd;

	void CreateCaptureDataForHwnd(HWND hWnd, std::string key, bool reCreateFlag);
	void ReCreateCapDataHimg(std::string key);

public:
	~Application();
	static Application& Instance(void) {
		static Application _instance;
		return _instance;
	}

	const Size GetScreenSize(void) const;

	// ‰Šú‰»ˆ—
	bool Init(void);
	// Àsˆ—
	void Run(void);
	// I—¹ˆ—
	void Terminate(void);
};
