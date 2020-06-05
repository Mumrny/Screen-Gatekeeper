#pragma once
#include <array>
#include <vector>
#include <memory>
#include "Geometry.h"


class GameWindow;

class Stage {
private:
	int hStageImg;
	std::array<int, 8> hChipsImg;

	std::shared_ptr<GameWindow> gameWnd;

	struct StageData {
		Vector2 chipCnt;
		Size size;
		Position2 pos;

		std::vector<int> mapData;
		Position2 startPoint;
		Position2 goalPoint;
	};
	StageData stageData;

	struct ChipState {
		bool outWndFlag;
		Position2f pos;
		Vector2f speed;
		float angle;
		float rotSpeed;
		bool delFlag;
	};
	std::vector<ChipState> wndChips;
	std::vector<ChipState> fenceChips;

	void UpdateStageImg(void);

public:
	Stage(std::shared_ptr<GameWindow> gameWnd);
	~Stage();

	Position2f GetStartPos(void) const;
	Position2f GetGoalPos(void) const;

	bool CheckHitWall(Position2f sPos, int length);
	bool CheckHitFloor(Position2f sPos, int length);
	bool CheckHitFloorIsRiseBlock(Position2f sPos, int length);
	bool CheckHitFloorIsFenceBlock(Position2f sPos, int length);

	bool CheckOverlap(Rect rc);

	void Init(void);

	void Update(void);
	void DrawMap(void);
	void DrawFenceChips(void);
	void DrawWndChips(void);
};
