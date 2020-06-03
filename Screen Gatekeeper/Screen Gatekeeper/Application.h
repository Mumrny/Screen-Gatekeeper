#pragma once
#include <windows.h>
#include <unordered_map>
#include <memory>
#include <array>
#include "Geometry.h"


class GameWindow;
class Stage;

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
	// ΊΛί°Φ~
	Application(const Application&) = delete;
	// γόΦ~
	void operator=(const Application&) = delete;

	Size screenSize;

	HWND hMainWnd;
	std::unordered_map<std::string, CaptureData> capDatas;

	std::shared_ptr<GameWindow> gameWnd;
	std::shared_ptr<Stage> stage;

	void CreateCaptureDataForHwnd(HWND hWnd, std::string key, bool reCreateFlag);
	void ReCreateCapDataHimg(std::string key);

	void Update(void);
	void Draw(void);

public:
	~Application();
	static Application& Instance(void) {
		static Application _instance;
		return _instance;
	}

	const Size GetScreenSize(void) const;
	const RECT GetCaptureRect(std::string key) const;

	// ϊ»
	bool Init(void);
	// ΐs
	void Run(void);
	// IΉ
	void Terminate(void);
};
