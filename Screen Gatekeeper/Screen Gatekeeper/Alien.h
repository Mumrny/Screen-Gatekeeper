#pragma once
#include "Character.h"


class Alien :
	public Character {
public:
	Alien(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage);
	~Alien();

	void Init(void);

	void Update(void);
	void Draw(void);
};
