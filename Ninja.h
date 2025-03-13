#ifndef NINJA_H
#define NINJA_H

#include <iostream>
#include "Position.h"

class Ninja
{
public:
	Ninja() = default;
	Ninja(const Position& position);

	Position currentPosition() const;
	void currentPosition(const Position& position);

	void moveSouth();
	void moveEast();
	void moveNorth();
	void moveWest();

	void ThrowShuriken();
	void AddShuriken();
	int NumberOfShurikens() const;

	bool IsInBreakerMode() const;
	void SetBreakerMode(bool breakerMode);

	bool MirroredMove() const;
	void MirroredMove(bool mirroredMove);

private:
	Position m_position;
	int m_numberOfShurikens;
	bool m_isInBreakerMode;
	bool m_mirroredMove;
};

#endif // NINJA_H