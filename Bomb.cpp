#include "Bomb.h"

Bomb::Bomb()
{
	m_bombPosition = {};
	m_bombTimer = 0;
	m_isActivated = false;
}

Bomb::Bomb(const Position& position, int bombTimer, bool isActivated)
{
	m_bombPosition = position;
	m_bombTimer = bombTimer;
	m_isActivated = isActivated;
}

Position Bomb::GetPosition() const
{
	return m_bombPosition;
}

int Bomb::GetBombTimber() const
{
	return m_bombTimer;
}

bool Bomb::IsActivated() const
{
	return m_isActivated;
}

void Bomb::DecreaseTimer()
{
	m_bombTimer--;
}

void Bomb::SetBombActivated(bool isActivated)
{
	m_isActivated = isActivated;
}

void Bomb::SetBombTimer(int bombTimer)
{
	m_bombTimer = bombTimer;
}

void Bomb::SetPosition(const Position& position)
{
	m_bombPosition = position;
}

bool Bomb::operator==(const Bomb& bomb) const
{
	return m_bombPosition == bomb.m_bombPosition && m_bombTimer == bomb.m_bombTimer && m_isActivated == bomb.m_isActivated;
}
