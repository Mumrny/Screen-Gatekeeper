#include <cassert>
#include <time.h>
#include <DxLib.h>
#include "Application.h"
#include "GameWindow.h"
#include "Stage.h"

constexpr int ChipSize = 40;

Stage::Stage(std::shared_ptr<GameWindow> gameWnd) {
	Stage::gameWnd = gameWnd;
}

Stage::~Stage() {}

Position2f
Stage::GetStartPos(void) const {
	return Position2f(
		stageData.pos.x + (stageData.startPoint.x + 0.5f) * ChipSize,
		stageData.pos.y + (stageData.startPoint.y + 1) * ChipSize
		);
}

Position2f
Stage::GetGoalPos(void) const {
	return Position2f(
		stageData.pos.x + (stageData.goalPoint.x + 0.5f) * ChipSize,
		stageData.pos.y + (stageData.goalPoint.y + 1) * ChipSize
	);
}

bool
Stage::CheckHitWall(Position2f sPos, int length) {
	int x = (sPos.x - stageData.pos.x) / ChipSize;
	int sy = (sPos.y - stageData.pos.y) / ChipSize;
	int ey = (
		static_cast<int>(sPos.y - stageData.pos.y + length) % ChipSize == 0 ?
		(sPos.y - stageData.pos.y + length) / ChipSize :
		(sPos.y - stageData.pos.y + length) / ChipSize + 1
		);

	if ((x < 0) || (x >= stageData.chipCnt.x)
		|| (sy < 0) || (ey > stageData.chipCnt.y)) {
		return false;
	}

	for (; sy < ey; sy++) {
		int chipNo = stageData.mapData[sy * stageData.chipCnt.x + x];
		if ((chipNo == 1) || (chipNo == 3) || (chipNo == 5)) {
			return true;
		}
	}

	Rect rc(Position2f(sPos.x, sPos.y + length / 2), Size(0, length));
	for (auto wndChip : wndChips) {
		if (wndChip.outWndFlag) {
			continue;
		}

		Rect chipRc(wndChip.pos, Size(ChipSize, ChipSize));
		Size overlapSize = GetOverlap(rc, chipRc).size;
		if ((overlapSize.width == 0) && (overlapSize.height > 0)) {
			return true;
		}
	}

	return false;
}

bool
Stage::CheckHitFloor(Position2f sPos, int length) {
	int y = (sPos.y - stageData.pos.y) / ChipSize;
	int sx = (sPos.x - stageData.pos.x) / ChipSize;
	int ex = (
		static_cast<int>(sPos.x - stageData.pos.x + length) % ChipSize == 0 ?
		(sPos.x - stageData.pos.x + length) / ChipSize :
		(sPos.x - stageData.pos.x + length) / ChipSize + 1
		);

	if ((y < 0) || (y >= stageData.chipCnt.y)
	 || (sx < 0) || (ex > stageData.chipCnt.x)) {
		return false;
	}

	for (; sx < ex; sx++) {
		int chipNo = stageData.mapData[y * stageData.chipCnt.x + sx];
		if ((chipNo == 1) || (chipNo == 2) || (chipNo == 5) || (chipNo == 6)) {
			return true;
		}
	}

	Rect rc(Position2f(sPos.x + length / 2, sPos.y), Size(length, 0));
	for (auto wndChip : wndChips) {
		if (wndChip.outWndFlag) {
			continue;
		}

		Rect chipRc(wndChip.pos, Size(ChipSize, ChipSize));
		Size overlapSize = GetOverlap(rc, chipRc).size;
		if ((overlapSize.width > 0) && (overlapSize.height == 0)) {
			return true;
		}
	}

	return false;
}

bool
Stage::CheckHitFloorIsRiseBlock(Position2f sPos, int length) {
	int y = (sPos.y - stageData.pos.y) / ChipSize;
	int sx = (sPos.x - stageData.pos.x) / ChipSize;
	int ex = (
		static_cast<int>(sPos.x - stageData.pos.x + length) % ChipSize == 0 ?
		(sPos.x - stageData.pos.x + length) / ChipSize :
		(sPos.x - stageData.pos.x + length) / ChipSize + 1
		);

	if ((y < 0) || (y >= stageData.chipCnt.y)
		|| (sx < 0) || (ex > stageData.chipCnt.x)) {
		return false;
	}

	for (; sx < ex; sx++) {
		int chipNo = stageData.mapData[y * stageData.chipCnt.x + sx];
		if (chipNo == 6) {
			return true;
		}
	}

	return false;
}

bool
Stage::CheckHitFloorIsFenceBlock(Position2f sPos, int length) {
	if (static_cast<int>(sPos.y - stageData.pos.y) % ChipSize < ChipSize - 1) {
		return false;
	}

	Rect rc(Position2f(sPos.x + length / 2, sPos.y), Size(length, 0));
	for (auto fenceChip : fenceChips) {
		Rect chipRc(fenceChip.pos, Size(ChipSize, ChipSize));
		Size overlapSize = GetOverlap(rc, chipRc).size;
		if ((overlapSize.width > 0) && (overlapSize.height == 0)) {
			return true;
		}
	}
	return false;
}

bool
Stage::CheckOverlap(Rect rc) {
	int sx = max(rc.Left() - stageData.pos.x, 0);
	sx /= ChipSize;
	int sy = max(rc.Top() - stageData.pos.y, 0);
	sy /= ChipSize;

	int ex = min(rc.Right() - stageData.pos.x, stageData.size.width);
	ex = (ex % ChipSize == 0 ? ex / ChipSize : ex / ChipSize + 1);
	int ey = min(rc.Bottom() - stageData.pos.y, stageData.size.height);
	ey = (ey % ChipSize == 0 ? ey / ChipSize : ey / ChipSize + 1);

	for (int i = sy; i < ey; i++) {
		for (int j = sx; j < ex; j++) {
			int md = stageData.mapData[i * stageData.chipCnt.x + j];
			if (md != 0) {
				return true;
			}
		}
	}

	return false;
}

void
Stage::UpdateStageImg(void) {
	fenceChips.resize(0);

	SetDrawScreen(hStageImg);
	for (int i = 0; i < stageData.chipCnt.y; i++) {
		for (int j = 0; j < stageData.chipCnt.x; j++) {
			if (stageData.mapData[i * stageData.chipCnt.x + j] == 7) {
				ChipState fenceChip;
				fenceChip.pos = {
					static_cast<float>(stageData.pos.x + (ChipSize * j) + ChipSize / 2),
					static_cast<float>(stageData.pos.y + (ChipSize * i) + ChipSize / 2)
				};
				fenceChips.emplace_back(fenceChip);
				DrawGraph(j * ChipSize, i * ChipSize, hChipsImg[0], true);
			}
			else {
				DrawGraph(j * ChipSize, i * ChipSize, hChipsImg[stageData.mapData[i * stageData.chipCnt.x + j]], true);
			}
		}
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

void
Stage::Init(void) {
	// àÍíUíºíl(éûä‘Ç™Ç†ÇÍÇŒì«Ç›çûÇ›Ç∆Ç©Ç‡ÇµÇΩÇ¢)
	StageData stage1;
	stage1.chipCnt = { 24, 18 };
	stage1.size = { stage1.chipCnt.x * ChipSize, stage1.chipCnt.y * ChipSize };

	const Size scSize = Application::Instance().GetScreenSize();
	const RECT taskRc = Application::Instance().GetCaptureRect("taskbar");
	int taskHeight = taskRc.bottom - taskRc.top;

	assert((stage1.size.width <= scSize.width)
		&& (stage1.size.height <= (scSize.height - taskHeight)));

	stage1.pos = {
		(scSize.width - stage1.size.width) / 2,
		(scSize.height - stage1.size.height) / 2 - taskHeight
	};

	stage1.mapData.resize(stage1.chipCnt.x * stage1.chipCnt.y);

	stage1.mapData = {
	 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	 3, 0, 0, 0, 5, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 5, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 0, 0, 0, 5, 0, 0, 0, 3, 4, 4, 4, 3, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	 1, 2, 2, 2, 1, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 3, 7, 7, 7, 3, 0, 0, 0, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 3, 0, 0, 0, 1, 2, 2, 2, 1,
	 3, 4, 4, 4, 1, 2, 2, 2, 1, 4, 4, 4, 3, 5, 5, 3, 0, 0, 0, 3, 4, 4, 4, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 3, 0, 0, 0, 3, 4, 4, 4, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 3, 0, 0, 0, 3, 4, 4, 4, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0, 3, 0, 0, 0, 1, 2, 2, 2, 1,
	 3, 4, 4, 4, 3, 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 3, 5, 5, 5, 1, 0, 0, 0, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3,
	 3, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3,
	 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 6, 6, 1, 6, 6, 6, 1, 2, 2, 2, 1
	};

	stage1.startPoint = { 2, 3 };
	stage1.goalPoint = { 17, 7 };

	stageData = stage1;

	wndChips.resize(0);

	LoadDivGraph("Image/Chips.png", 8, 4, 2, ChipSize, ChipSize, hChipsImg.data());

	hStageImg = MakeScreen(stageData.size.width, stageData.size.height);

	UpdateStageImg();

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

	int sx = max(clientPos.x - stageData.pos.x, 0);
	sx = (sx % ChipSize == 0 ? sx / ChipSize : sx / ChipSize + 1);
	int sy = max(clientPos.y - stageData.pos.y, 0);
	sy = (sy % ChipSize == 0 ? sy / ChipSize : sy / ChipSize + 1);

	int ex = clientPos.x + clientSize.width - stageData.pos.x;
	ex = min(ex / ChipSize, stageData.chipCnt.x);
	int ey = clientPos.y + clientSize.height - stageData.pos.y;
	ey = min(ey / ChipSize, stageData.chipCnt.y);

	bool dataChangeFlag = false;

	for (int i = sy; i < ey; i++) {
		for (int j = sx; j < ex; j++) {
			if (stageData.mapData[i * stageData.chipCnt.x + j] == 5) {
				ChipState wndChip;
				wndChip.pos = {
					static_cast<float>(stageData.pos.x + (ChipSize * j) + ChipSize / 2),
					static_cast<float>(stageData.pos.y + (ChipSize * i) + ChipSize / 2)
				};
				wndChip.speed = { 0, 0 };
				wndChip.angle = 0;
				wndChip.rotSpeed = 0;
				wndChip.outWndFlag = false;
				wndChip.delFlag = false;

				wndChips.emplace_back(wndChip);

				stageData.mapData[i * stageData.chipCnt.x + j] = 0;
				dataChangeFlag = true;
			}
		}
	}
	if (dataChangeFlag) {
		UpdateStageImg();
	}
}

void
Stage::DrawMap(void) {
	const Position2f clientPos = gameWnd->GetClientPos();
	const Size clientSize = gameWnd->GetClientSize();

	Position2f clientCenterPos = {
		static_cast<float>(clientPos.x + clientSize.width / 2),
		static_cast<float>(clientPos.y + clientSize.height / 2)
	};
	Rect clientRc(clientCenterPos, clientSize);

	Position2f mapCenterPos = {
		static_cast<float>(stageData.pos.x + stageData.size.width / 2),
		static_cast<float>(stageData.pos.y + stageData.size.height / 2)
	};
	Rect stageRc(mapCenterPos, stageData.size);

	Rect overlap = GetOverlap(clientRc, stageRc);

	DrawRectGraph(
		overlap.Left(),
		overlap.Top(),
		overlap.Left() - stageData.pos.x,
		overlap.Top() - stageData.pos.y,
		overlap.size.width,
		overlap.size.height,
		hStageImg,
		true
	);
}

void
Stage::DrawFenceChips(void) {
	const Position2f clientPos = gameWnd->GetClientPos();
	const Size clientSize = gameWnd->GetClientSize();

	Position2f clientCenterPos = {
		static_cast<float>(clientPos.x + clientSize.width / 2),
		static_cast<float>(clientPos.y + clientSize.height / 2)
	};
	Rect clientRc(clientCenterPos, clientSize);

	for (auto fenceChip : fenceChips) {
		Rect fenceChipRc(fenceChip.pos, Size(ChipSize, ChipSize));

		Rect overlap = GetOverlap(clientRc, fenceChipRc);

		Position2 sRcPos = { 0, 0 };
		if (fenceChipRc.Left() <= clientRc.Left()) {
			sRcPos.x = ChipSize - overlap.size.width;
		}
		if (fenceChipRc.Top() <= clientRc.Top()) {
			sRcPos.y = ChipSize - overlap.size.height;
		}

		DrawRectGraph(
			overlap.Left(),
			overlap.Top(),
			sRcPos.x,
			sRcPos.y,
			overlap.size.width,
			overlap.size.height,
			hChipsImg[7],
			true
		);
	}
}

void
Stage::DrawWndChips(void) {
	for (auto wndChip : wndChips) {
		DrawRotaGraph(wndChip.pos.x, wndChip.pos.y, 1.0f, wndChip.angle, hChipsImg[5], true);
	}
}
