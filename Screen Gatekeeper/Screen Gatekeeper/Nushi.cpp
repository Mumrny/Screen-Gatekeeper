#include <DxLib.h>
#include "Application.h"
#include "Nushi.h"


Nushi::Nushi(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage) :
	Character(pass, divCnt, divSize, gameWnd, stage) {}

Nushi::~Nushi() {}


void
Nushi::Jump(float jumpPower) {
	speedY = -1.0f * jumpPower;
	pos.x = (Application::Instance().GetDividePosX() - divSize.width / 2);
	jumpFlag = true;
}

void
Nushi::Init(void) {
	const Size scSize = Application::Instance().GetScreenSize();
	pos = { 0, static_cast<float>(scSize.height) };

	jumpFlag = false;
	speedY = 0.0f;
}

void
Nushi::Update(void) {
	if (jumpFlag) {
		speedY += 0.7f;
		if (speedY >= 0) {
			Application::Instance().Eat();
		}

		pos.y += speedY;
		if (pos.y >= Application::Instance().GetScreenSize().height) {
			Application::Instance().Close();
			jumpFlag = false;
		}
	}
	else {
		if (Application::Instance().GetDivideLength() >= divSize.width) {
			Jump(16.0f);
		}
	}
}

void
Nushi::Draw(void) {
	if (speedY <= 0) {
		DrawGraph(pos.x, pos.y, hImg[0], true);
	}
	else {
		DrawGraph(pos.x, pos.y, hImg[1], true);
	}
}
