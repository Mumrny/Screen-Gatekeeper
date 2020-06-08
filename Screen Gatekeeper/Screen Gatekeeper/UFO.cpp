#include <DxLib.h>
#include "Application.h"
#include "GameWindow.h"
#include "Stage.h"
#include "UFO.h"


UFO::UFO(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage) :
	Character(pass, divCnt, divSize, gameWnd, stage) {}

UFO::~UFO() {}

void
UFO::Init(void) {
	Position2f goalPos = stage->GetGoalPos();
	pos = { goalPos.x - divSize.width / 2, goalPos.y - divSize.height };

	animCnt = 0;
	frame = 0;
	exRate = 1.0f;
}

void
UFO::Update(void) {
	if (Application::Instance().GetGoalFlag()) {
		int animSpeed = frame / 5;

		if (frame % 5 == 0) {
			if (animSpeed < 10) {
				pos.x -= (10 - animSpeed);
				pos.y += animSpeed;
			}
			else if (animSpeed < 20) {
				pos.x += ((animSpeed - 10) * 2);
				pos.y += ((30 - animSpeed) / 2);
			}
			else {
				pos.x += ((80 - animSpeed) / 3);
				pos.y -= ((animSpeed - 20) * 3);
			}

			if (exRate < 4.0f) {
				exRate += 0.1f;
			}
		}

		frame++;
		if (frame >= 180) {
			//ƒNƒŠƒAˆ—
		}
	}
	animCnt++;
}

void
UFO::Draw(void) {
	if (Application::Instance().GetGoalFlag()) {
		DrawRotaGraph(
			pos.x + divSize.width / 2,
			pos.y + divSize.height / 2,
			exRate,
			0.0f,
			hImg[animCnt / 10 % 2 + 2],
			true
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
			hImg[animCnt / 10 % 2],
			true
		);
	}
}
