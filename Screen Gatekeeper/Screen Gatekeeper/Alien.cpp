#include <DxLib.h>
#include "Application.h"
#include "GameWindow.h"
#include "Stage.h"
#include "Alien.h"


Alien::Alien(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage) :
	Character(pass, divCnt, divSize, gameWnd, stage) {}

Alien::~Alien() {}

void
Alien::WaitState(void) {
	if ((pos.x < mousePos.x) && (mousePos.x < (pos.x + divSize.width))
	 && (pos.y < mousePos.y) && (mousePos.y < (pos.y + divSize.height))) {
		if ((mouseInputLeftOld == 0) && (GetMouseInput() & MOUSE_INPUT_LEFT)) {
			state = &Alien::WalkState;
			Application::Instance().GameStart();
		}
	}
}

void
Alien::FallState(void) {
	pos.y++;
	if (stage->CheckHitFloor(pos + Position2f(0, divSize.height), divSize.width)) {
		state = &Alien::WaitState;
	}
}

void
Alien::WalkState(void) {
	if (walkDirIsLeft) {
		if (stage->CheckHitWall(pos - Position2f(1, 0), divSize.height)) {
			walkDirIsLeft = false;
		}
		else {
			pos.x--;
		}
	}
	else {
		if (stage->CheckHitWall(pos + Position2f(divSize.width, 0), divSize.height)) {
			walkDirIsLeft = true;
		}
		else {
			pos.x++;
		}
	}

	if ((pos.x < mousePos.x) && (mousePos.x < (pos.x + divSize.width))
		&& (pos.y < mousePos.y) && (mousePos.y < (pos.y + divSize.height))) {
		if ((mouseInputLeftOld == 0) && (GetMouseInput() & MOUSE_INPUT_LEFT)) {
			state = &Alien::WaitState;
		}
	}

	if (!stage->CheckHitFloor(pos + Position2f(0, divSize.height), divSize.width)) {
		state = &Alien::FallState;
	}
}

void
Alien::RiseState(void) {
}

void
Alien::CheckLack(void) {
	if (!Application::Instance().GetStartFlag()) {
		return;
	}

	const Position2f clientPos = gameWnd->GetClientPos();
	const Size clientSize = gameWnd->GetClientSize();
	if (pos.x < clientPos.x) {
		return;
	}
	if (clientPos.x + clientSize.width < pos.x + divSize.width) {
		return;
	}
	if (pos.y < clientPos.y) {
		return;
	}
	if (clientPos.y + clientSize.height < pos.y + divSize.height) {
		return;
	}

	outWndStandbyFlag = true;
}

void
Alien::Init(void) {
	Position2f startPos = stage->GetStartPos();
	pos = { startPos.x - divSize.width / 2, startPos.y - divSize.height };

	state = &Alien::WaitState;
	animCnt = 0;

	GetMousePoint(&mousePos.x, &mousePos.y);
	mouseInputLeftOld = (GetMouseInput() & MOUSE_INPUT_LEFT);

	walkDirIsLeft = false;
	outWndFlag = false;
	outWndStandbyFlag = false;
}

void
Alien::Update(void) {
	GetMousePoint(&mousePos.x, &mousePos.y);
	(this->*state)();
	mouseInputLeftOld = (GetMouseInput() & MOUSE_INPUT_LEFT);

	if (!outWndStandbyFlag) {
		CheckLack();
	}

	animCnt++;
}

void
Alien::Draw(void) {
	int animState = 0;
	if (state == &Alien::FallState) {
		animState = 2;
	}
	if (state == &Alien::WalkState) {
		animState = 4;
	}
	if (state == &Alien::RiseState) {
		animState = 6;
	}

	if (outWndStandbyFlag) {
		DrawRotaGraph(
			pos.x + divSize.width / 2,
			pos.y + divSize.height / 2,
			1.0f,
			0.0f,
			hImg[animCnt / 4 % 2 + animState],
			true,
			walkDirIsLeft
			);
	}
	else {
		const Position2f clientPos = gameWnd->GetClientPos();
		const Size clientSize = gameWnd->GetClientSize();

		Position2f clientCenterPos = {
			static_cast<float>(clientPos.x + clientSize.width / 2),
			static_cast<float>(clientPos.y + clientSize.height / 2)
		};
		Rect clientRc(clientCenterPos, clientSize);

		Position2f alienCenterPos = {
			static_cast<float>(pos.x + divSize.width / 2),
			static_cast<float>(pos.y + divSize.height / 2)
		};
		Rect alienRc(alienCenterPos, divSize);

		Rect overlap = GetOverlap(clientRc, alienRc);

		Position2 sRcPos = { 0, 0 };
		if (alienRc.Left() <= clientRc.Left()) {
			sRcPos.x = divSize.width - overlap.size.width;
		}
		if (alienRc.Top() <= clientRc.Top()) {
			sRcPos.y = divSize.height - overlap.size.height;
		}

		DrawRectGraph(
			overlap.Left(),
			overlap.Top(),
			sRcPos.x,
			sRcPos.y,
			overlap.size.width,
			overlap.size.height,
			hImg[animCnt / 4 % 2 + animState],
			true,
			walkDirIsLeft
		);
	}
}
