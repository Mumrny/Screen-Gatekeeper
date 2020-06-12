#pragma once
#include "Character.h"


class Nushi :
	public Character {
	bool jumpFlag;
	float speedY;

	void Jump(float jumpPower);

public:
	Nushi(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage);
	~Nushi();

	void Init(void);

	void Update(void);
	void Draw(void);
};
