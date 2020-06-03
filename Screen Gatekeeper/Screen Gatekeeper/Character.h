#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Geometry.h"


class GameWindow;
class Stage;

class Character {
protected:
	std::vector<int> hImg;

	std::shared_ptr<GameWindow> gameWnd;
	std::shared_ptr<Stage> stage;

	Position2f pos;
	int animCnt;

public:
	Character(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage);
	virtual ~Character();

	virtual void Init(void) = 0;

	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
};
