#pragma once
#include "Character.h"


class UFO :
	public Character {
private:
	int frame;
	float exRate;

public:
	UFO(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage);
	~UFO();

	void Init(void);

	void Update(void);
	void Draw(void);
};
