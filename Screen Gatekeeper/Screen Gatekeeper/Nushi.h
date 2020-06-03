#pragma once
#include "Character.h"


class Nushi :
	public Character {
public:
	Nushi(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage);
	~Nushi();

	void Init(void);

	void Update(void);
	void Draw(void);
};
