#include "GameWindow.h"
#include "Alien.h"


Alien::Alien(std::string pass, Vector2f divCnt, Size divSize, std::shared_ptr<GameWindow> gameWnd, std::shared_ptr<Stage> stage) :
	Character(pass, divCnt, divSize, gameWnd, stage) {}

Alien::~Alien() {}

void
Alien::Init(void) {
	pos = gameWnd->GetClientPos();
	animCnt = 0;
}

void
Alien::Update(void) {
}

void
Alien::Draw(void) {
}
