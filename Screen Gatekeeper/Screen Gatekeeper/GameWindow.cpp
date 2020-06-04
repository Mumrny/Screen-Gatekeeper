#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "GameWindow.h"


constexpr int gameWindowWidth = 400;
constexpr int gameWindowHeight = 330;

constexpr int gameClientWidth = 400;
constexpr int gameClientHeight = 300;

GameWindow::GameWindow() {}

GameWindow::~GameWindow() {}

void
GameWindow::UpdateClientPos(void) {
	clientPos.x = windowPos.x + (gameWindowWidth - gameClientWidth);
	clientPos.y = windowPos.y + (gameWindowHeight - gameClientHeight);
}

const Position2f
GameWindow::GetWindowPos(void) const {
	return windowPos;
}

const Position2f
GameWindow::GetClientPos(void) const {
	return clientPos;
}

const Size
GameWindow::GetWindowSize(void) const {
	return Size(gameWindowWidth, gameWindowHeight);
}

const Size
GameWindow::GetClientSize(void) const {
	return Size(gameClientWidth, gameClientHeight);
}

void
GameWindow::Update(void) {
	mousePosOld = { mousePos.x, mousePos.y };
	GetMousePoint(&mousePos.x, &mousePos.y);

	const RECT taskRc = Application::Instance().GetCaptureRect("taskbar");

	if (moveFlag) {
		if (taskRc.top <= mousePos.y) {
			SetMousePoint(mousePos.x, taskRc.top);
			mousePos.y = taskRc.top;
		}

		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			windowPos.x += mousePos.x - mousePosOld.x;
			windowPos.y += mousePos.y - mousePosOld.y;

			UpdateClientPos();
		}
		else {
			moveFlag = false;
		}
	}
	else {
		if ((windowPos.x <= mousePos.x) && (mousePos.x < (windowPos.x + gameWindowWidth))
		 && (windowPos.y <= mousePos.y) && (mousePos.y < clientPos.y)) {
			if ((mouseInputLeftOld == 0) && (GetMouseInput() & MOUSE_INPUT_LEFT)) {
				if (mousePos.y < taskRc.top) {
					moveFlag = true;
				}
			}
		}
	}

	mouseInputLeftOld = (GetMouseInput() & MOUSE_INPUT_LEFT);
}

void
GameWindow::Draw(void) {
	DrawGraph(windowPos.x, windowPos.y, hImg, false);
}

bool
GameWindow::Init(void) {
	hImg = LoadGraph("Image/GameWindow.png");
	assert(hImg != -1);

	const Size scSize = Application::Instance().GetScreenSize();
	windowPos.x = (scSize.width - gameWindowWidth) / 2;
	windowPos.y = (scSize.height - gameWindowHeight) / 2 - (gameWindowHeight - gameClientHeight);

	UpdateClientPos();

	moveFlag = false;
	GetMousePoint(&mousePos.x, &mousePos.y);
	mouseInputLeftOld = (GetMouseInput() & MOUSE_INPUT_LEFT);

	return true;
}
