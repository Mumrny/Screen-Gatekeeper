#include <DxLib.h>
#include "Character.h"


Character::Character(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage) {
	hImg.resize(divCnt.x * divCnt.y);

	LoadDivGraph(
		pass.c_str(),
		hImg.size(),
		divCnt.x,
		divCnt.y,
		divSize.width,
		divSize.height,
		hImg.data()
	);

	Character::divCnt = divCnt;
	Character::divSize = divSize;

	Character::gameWnd = gameWnd;
	Character::stage = stage;
}

Character::~Character() {}
