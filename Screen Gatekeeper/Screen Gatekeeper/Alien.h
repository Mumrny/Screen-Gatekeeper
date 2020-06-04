#pragma once
#include "Character.h"


class Alien :
	public Character {
public:
	Alien(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage);
	~Alien();

	bool walkDirIsLeft;
	bool outWndFlag;
	bool outWndStandbyFlag;

	Position2 mousePos;
	int mouseInputLeftOld;

	void(Alien::*state)(void);

	void WaitState(void);
	void FallState(void);
	void WalkState(void);
	void RiseState(void);

	void CheckLack(void);

	void Init(void);

	void Update(void);
	void Draw(void);
};
