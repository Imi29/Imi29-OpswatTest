#ifndef BOMB_H
#define BOMB_H

#include <iostream>
#include "Position.h"

class Bomb
{
public:
	Bomb();
	Bomb(const Position& position, int bombTimer, bool isActivated);

	Position GetPosition() const;
	int GetBombTimber() const;
	bool IsActivated() const;

	void DecreaseTimer();
	
	void SetBombActivated(bool isActivated);
	void SetBombTimer(int bombTimer);
	void SetPosition(const Position& position);

	bool operator==(const Bomb& bomb) const;

private:
	Position m_bombPosition;
	int m_bombTimer;
	bool m_isActivated;
};

#endif // BOMB_H