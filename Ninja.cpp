#include "Ninja.h"

constexpr int DEFAULT_NUMBER_OF_SHURIKENS = 3;

Ninja::Ninja(const Position& position) 
{
	m_position = position;
	m_numberOfShurikens = DEFAULT_NUMBER_OF_SHURIKENS;
	m_isInBreakerMode = false;
	m_mirroredMove = false;
}

Position Ninja::currentPosition() const
{
	return m_position;
}

void Ninja::currentPosition(const Position& position)
{
	m_position.x_pos = position.x_pos;
	m_position.y_pos = position.y_pos;
}

void Ninja::moveSouth()
{
	m_position.x_pos++;
}

void Ninja::moveEast()
{
	m_position.y_pos++;
}

void Ninja::moveNorth()
{
	m_position.x_pos--;
}

void Ninja::moveWest()
{
	m_position.y_pos--;
}

void Ninja::ThrowShuriken()
{
	if (m_numberOfShurikens > 0)
	{
		m_numberOfShurikens--;
	}
}

void Ninja::AddShuriken()
{
	m_numberOfShurikens++;
}

int Ninja::NumberOfShurikens() const
{
	return m_numberOfShurikens;
}

bool Ninja::IsInBreakerMode() const
{
	return m_isInBreakerMode;
}

void Ninja::SetBreakerMode(bool breakerMode)
{
	m_isInBreakerMode = breakerMode;
}

bool Ninja::MirroredMove() const
{
	return m_mirroredMove;
}

void Ninja::MirroredMove(bool mirroredMove)
{
	m_mirroredMove = mirroredMove;
}
