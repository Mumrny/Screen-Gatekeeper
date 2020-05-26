#include <DxLib.h>
#include <cassert>
#include "Geometry.h"
#include "Application.h"
#include "GameWindow.h"


constexpr int gameWindowWidth = 800;
constexpr int gameWindowHeight = 631;

constexpr int gameClientWidth = 800;
constexpr int gameClientHeight = 600;

GameWindow::GameWindow() {}

GameWindow::~GameWindow() {}

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
GameWindow::Draw(void) {
	DrawGraph(windowPos.x, windowPos.y, hImg, false);
}

bool
GameWindow::Init(void) {
	hImg = LoadGraph("Image/GameWindow.png");
	assert(hImg != -1);

	const Size scSize = Application::Instance().GetScreenSize();
	windowPos.x = (scSize.width - gameWindowWidth) / 2;
	windowPos.y = (scSize.height - gameWindowHeight) / 2;

	clientPos.x = windowPos.x + (gameWindowWidth - gameClientWidth);
	clientPos.y = windowPos.y + (gameWindowHeight - gameClientHeight);

	return true;
}
