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
	// ��߰�֎~
	Application(const Application&) = delete;
	// ����֎~
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

	// ����������
	bool Init(void);
	// ���s����
	void Run(void);
	// �I������
	void Terminate(void);
};
