#pragma once
#include <windows.h>
#include <unordered_map>
#include <memory>
#include <array>
#include "Geometry.h"


class GameWindow;
class Stage;
class Character;

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
	// ∫Àﬂ∞ã÷é~
	Application(const Application&) = delete;
	// ë„ì¸ã÷é~
	void operator=(const Application&) = delete;

	Size screenSize;

	HWND hMainWnd;
	std::unordered_map<std::string, CaptureData> capDatas;

	std::shared_ptr<GameWindow> gameWnd;
	std::shared_ptr<Stage> stage;
	std::shared_ptr<Character> alien;

	bool startFlag;

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

	const bool GetStartFlag(void) const;
	void GameStart(void);

	// èâä˙âªèàóù
	bool Init(void);
	// é¿çsèàóù
	void Run(void);
	// èIóπèàóù
	void Terminate(void);
};
