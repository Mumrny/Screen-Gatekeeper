#pragma once
#include <windows.h>
#include <unordered_map>


struct Size {
	int width;
	int height;
	Size() {}
	Size(int w, int h) : width(w), height(h) {}
};

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

	HWND hMainWnd;
	std::unordered_map<std::string, CaptureData> capDatas;

	void CreateCaptureDataForHwnd(HWND hWnd, std::string key, bool reCreateFlag);
	void ReCreateCapDataHimg(std::string key);

public:
	~Application();
	static Application& Instance(void) {
		static Application _instance;
		return _instance;
	}

	// ϊ»
	bool Init(void);
	// ΐs
	void Run(void);
	// IΉ
	void Terminate(void);
};
