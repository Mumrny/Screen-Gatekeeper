#include <time.h>
#include <DxLib.h>
#include "Application.h"
#include "GameWindow.h"
#include "Stage.h"


constexpr int StageSizeWidth = 960;
constexpr int StageSizeHeight = 720;

constexpr int ChipSize = 40;

constexpr int ChipCntX = StageSizeWidth / ChipSize; // 24
constexpr int ChipCntY = StageSizeHeight / ChipSize; // 18

Stage::Stage(std::shared_ptr<GameWindow> gameWnd) {
	Stage::gameWnd = gameWnd;
}

Stage::~Stage() {}

void
Stage::UpdateStageImg(void) {
	SetDrawScreen(hStageImg);
	for (int i = 0; i < ChipCntY; i++) {
		for (int j = 0; j < ChipCntX; j++) {
			DrawGraph(j * ChipSize, i * ChipSize, hChipsImg[stageData[i * ChipCntX + j]], true);
		}
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

void
Stage::Init(void) {
	stageData.resize(ChipCntX * ChipCntY);

	// ˆê’U’¼’l(ŽžŠÔ‚ª‚ ‚ê‚Î“Ç‚Ýž‚Ý‚Æ‚©‚à‚µ‚½‚¢)
	stageData = {
	 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 3,
	 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1
	};

	LoadDivGraph("Image/Chips.png", 8, 4, 2, ChipSize, ChipSize, hChipsImg.data());

	hStageImg = MakeScreen(StageSizeWidth, StageSizeHeight);

	UpdateStageImg();

	wndChips.resize(0);

	const Size scSize = Application::Instance().GetScreenSize();
	const RECT taskRc = Application::Instance().GetCaptureRect("taskbar");
	int taskHeight = taskRc.bottom - taskRc.top;
	pos = {
		(scSize.width - StageSizeWidth) / 2,
		(scSize.height - StageSizeHeight) / 2 - taskHeight
	};

	SRand(static_cast<int>(time(nullptr)));
}

void
Stage::Update(void) {
	const Position2f clientPos = gameWnd->GetClientPos();
	const Size clientSize = gameWnd->GetClientSize();

	const Size scSize = Application::Instance().GetScreenSize();

	for (auto& wndChip : wndChips) {
		if (wndChip.outWndFlag) {
			wndChip.speed += Vector2f(0.0f, 0.1f);
			wndChip.pos += wndChip.speed;
			wndChip.angle += wndChip.rotSpeed;

			if (wndChip.pos.x < -1.5f || scSize.width + 1.5f < wndChip.pos.x
			 || wndChip.pos.y < -1.5f || scSize.height + 1.5f < wndChip.pos.y) {
				wndChip.delFlag = true;
			}
		}
		else {
			if (wndChip.pos.x < clientPos.x || clientPos.x + clientSize.width < wndChip.pos.x
			 || wndChip.pos.y < clientPos.y || clientPos.y + clientSize.height < wndChip.pos.y) {
				wndChip.outWndFlag = true;

				if (GetRand(1)) {
					wndChip.speed.x = 0.5f * (GetRand(5) + 1);
					wndChip.rotSpeed = 0.1f * (GetRand(5) + 1);
				}
				else {
					wndChip.speed.x = -0.5f * (GetRand(5) + 1);
					wndChip.rotSpeed = -0.1f * (GetRand(5) + 1);
				}

				wndChip.speed.y = -0.5f * (GetRand(3) + 5);
			}
		}
	}

	auto itr = std::remove_if(wndChips.begin(), wndChips.end(), [](ChipState & state) {return state.delFlag; });
	itr = wndChips.erase(itr, wndChips.end());

	int sx = max(clientPos.x - pos.x, 0);
	sx = (sx % ChipSize == 0 ? sx / ChipSize : sx / ChipSize + 1);
	int sy = max(clientPos.y - pos.y, 0);
	sy = (sy % ChipSize == 0 ? sy / ChipSize : sy / ChipSize + 1);

	int ex = clientPos.x + clientSize.width - pos.x;
	ex = min(ex / ChipSize, ChipCntX);
	int ey = clientPos.y + clientSize.height - pos.y;
	ey = min(ey / ChipSize, ChipCntY);

	bool dataChangeFlag = false;

	for (int i = sy; i < ey; i++) {
		for (int j = sx; j < ex; j++) {
			if (stageData[i * ChipCntX + j] == 5) {
				ChipState wndChip;
				wndChip.pos = {
					static_cast<float>(pos.x + (ChipSize * j) + ChipSize / 2),
					static_cast<float>(pos.y + (ChipSize * i) + ChipSize / 2)
				};
				wndChip.speed = { 0, 0 };
				wndChip.angle = 0;
				wndChip.rotSpeed = 0;
				wndChip.outWndFlag = false;
				wndChip.delFlag = false;

				wndChips.emplace_back(wndChip);

				stageData[i * ChipCntX + j] = 0;
				dataChangeFlag = true;
			}
		}
	}
	if (dataChangeFlag) {
		UpdateStageImg();
	}
}

void
Stage::Draw(void) {
	const Position2f clientPos = gameWnd->GetClientPos();
	const Size clientSize = gameWnd->GetClientSize();

	Position2f drawStartPos;
	drawStartPos.x = (pos.x < clientPos.x ? clientPos.x : pos.x);
	drawStartPos.y = (pos.y < clientPos.y ? clientPos.y : pos.y);

	Position2f drawEndPos;
	drawEndPos.x = (
		pos.x + StageSizeWidth < clientPos.x + clientSize.width ?
		pos.x + StageSizeWidth : clientPos.x + clientSize.width
		);
	drawEndPos.y = (
		pos.y + StageSizeHeight < clientPos.y + clientSize.height ?
		pos.y + StageSizeHeight : clientPos.y + clientSize.height
		);

	Position2f drawSrcPos;
	drawSrcPos.x = (drawStartPos.x - pos.x > 0 ? drawStartPos.x - pos.x : 0);
	drawSrcPos.y = (drawStartPos.y - pos.y > 0 ? drawStartPos.y - pos.y : 0);

	DrawRectGraph(
		drawStartPos.x,
		drawStartPos.y,
		drawSrcPos.x,
		drawSrcPos.y,
		drawEndPos.x - drawStartPos.x,
		drawEndPos.y - drawStartPos.y,
		hStageImg,
		true
	);

	for (auto wndChip : wndChips) {
		DrawRotaGraph(wndChip.pos.x, wndChip.pos.y, 1.0f, wndChip.angle, hChipsImg[5], true);
	}
}
