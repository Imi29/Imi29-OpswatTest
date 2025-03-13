#ifndef NINJAMAP_H
#define NINJAMAP_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Ninja.h"
#include "Bomb.h"
#include "FileReader.h"

class NinjaMap
{
public:
	enum Direction { SOUTH, EAST, NORTH, WEST };

	NinjaMap(const std::string& filePath);

	void BreakObstacle();
	void HolySymbol();
	void NextMove();
	void Sake();
	void Mirroring();
	void PathModifier();
	void SeekPathwayPair(char pathway);
	void Destroy();
	void readNextMove();
	void AddBomb(char timeValue);
	void BombActivation(char timeValue);
	void BombExplosion(const Bomb& activatedBomb);
	void CheckBombPositions();

	void ExplosionSouth(const Position& ninjaPosition, const Bomb& activatedBomb);
	void ExplosionEast(const Position& ninjaPosition, const Bomb& activatedBomb);
	void ExplosionNorth(const Position& ninjaPosition, const Bomb& activatedBomb);
	void ExplosionWest(const Position& ninjaPosition, const Bomb& activatedBomb);

	void CheckNinjaKilledSouth(const Position& ninjaPosition, const Bomb& bomb, int explosionRange);
	void CheckNinjaKilledEast(const Position& ninjaPosition, const Bomb& bomb, int explosionRange);
	void CheckNinjaKilledNorth(const Position& ninjaPosition, const Bomb& bomb, int explosionRange);
	void CheckNinjaKilledWest(const Position& ninjaPosition, const Bomb& bomb, int explosionRange);

	void ExplosionSouthRecursion(const Position& ninjaPosition, const Bomb& activatedBomb, const Bomb& otherBomb, int explosionRange);
	void ExplosionEastRecursion(const Position& ninjaPosition, const Bomb& activatedBomb, const Bomb& otherBomb, int explosionRange);
	void ExplosionNorthRecursion(const Position& ninjaPosition, const Bomb& activatedBomb, const Bomb& otherBomb, int explosionRange);
	void ExplosionWestRecursion(const Position& ninjaPosition, const Bomb& activatedBomb, const Bomb& otherBomb, int explosionRange);

	Position GetBombPosition() const;
	Position SeekStartSymbol() const;

	void StepToDirection();
	void UnbreakableObstacle();

	bool IsHolySymboleDestroyed() const;
	bool IsSake() const;

	Direction GetDirection() const;
	Direction MirroredNextDirection() const;
	Direction UnMirroredNextDirection() const;

	char GetNextValueByDirection()  const;
	std::string DirectionToString() const;

	inline char SouthPosition(const Position& pos) const;
	inline char EastPosition(const Position& pos)  const;
	inline char NorthPosition(const Position& pos) const;
	inline char WestPosition(const Position& pos)  const;

	inline void NinjaKilled() const;

	void SeekBombsPosition(std::vector<Bomb>& bombs);

	friend std::ostream& operator<<(std::ostream& out, const NinjaMap& ninjaMap);

private:
	std::vector<std::string> m_ninjaMap;
	std::vector<Bomb> m_bombPositions;
	Ninja m_ninja;
	Position m_bombPosition;
	unsigned int m_bombTimer;
	bool m_IsHolySymboleDestroyed;
	bool m_bombActivated;
	Direction m_direction;
	Direction m_mirroredDirection;
};

#endif // NINJAMAP_H