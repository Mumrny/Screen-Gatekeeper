#pragma once
#include "Geometry.h"


class GameWindow {
private:
	int hImg;
	Position2f windowPos;
	Position2f clientPos;
public:
	GameWindow();
	~GameWindow();

	const Position2f GetWindowPos(void) const;
	const Position2f GetClientPos(void) const;

	const Size GetWindowSize(void) const;
	const Size GetClientSize(void) const;

	void Draw(void);

	bool Init(void);
};
