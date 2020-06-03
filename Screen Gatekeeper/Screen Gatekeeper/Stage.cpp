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
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1
	};

	LoadDivGraph("Image/Chips.png", 8, 4, 2, ChipSize, ChipSize, hChipsImg.data());

	hStageImg = MakeScreen(StageSizeWidth, StageSizeHeight);

	UpdateStageImg();

	wndChips.resize(0);
}

void
Stage::Update(void) {
}

void
Stage::Draw(void) {
	const Size scSize = Application::Instance().GetScreenSize();
	const RECT taskRc = Application::Instance().GetCaptureRect("taskbar");
	int taskHeight = taskRc.bottom - taskRc.top;
	Position2 stagePos = {
		(scSize.width - StageSizeWidth) / 2,
		(scSize.height - StageSizeHeight) / 2 - taskHeight
	};

	const Position2f clientPos = gameWnd->GetClientPos();
	const Size clientSize = gameWnd->GetClientSize();

	Position2f drawStartPos;
	drawStartPos.x = (stagePos.x < clientPos.x ? clientPos.x : stagePos.x);
	drawStartPos.y = (stagePos.y < clientPos.y ? clientPos.y : stagePos.y);

	Position2f drawEndPos;
	drawEndPos.x = (
		stagePos.x + StageSizeWidth < clientPos.x + clientSize.width ?
		stagePos.x + StageSizeWidth : clientPos.x + clientSize.width
		);
	drawEndPos.y = (
		stagePos.y + StageSizeHeight < clientPos.y + clientSize.height ?
		stagePos.y + StageSizeHeight : clientPos.y + clientSize.height
		);

	Position2f drawSrcPos;
	drawSrcPos.x = (drawStartPos.x - stagePos.x > 0 ? drawStartPos.x - stagePos.x : 0);
	drawSrcPos.y = (drawStartPos.y - stagePos.y > 0 ? drawStartPos.y - stagePos.y : 0);

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
}
