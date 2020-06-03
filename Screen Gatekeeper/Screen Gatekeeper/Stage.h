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

	std::vector<int> stageData;
	Position2 pos;

	struct ChipState {
		bool outWndFlag;
		Position2f pos;
		Vector2f speed;
		float angle;
		float rotSpeed;
		bool delFlag;
	};
	std::vector<ChipState> wndChips;

	void UpdateStageImg(void);

public:
	Stage(std::shared_ptr<GameWindow> gameWnd);
	~Stage();

	void Init(void);

	void Update(void);
	void Draw(void);
};
