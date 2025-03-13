#include "NinjaMap.h"
#include "MapSymbols.h"

constexpr int LOOP = 1;
constexpr int EXPLOSION_RANGE = 2;
constexpr int UNDEFINED_SYMBOL = 2;
constexpr int NINJA_KILLED = 3;
constexpr int INITIAL_EXLPOSION_RANGE = 1;

#define CHAR2INT(charValue) charValue - '0'

NinjaMap::NinjaMap(const std::string& filePath)
{
	m_ninjaMap = std::vector<std::string>();

	FileReader fileReader(filePath);
	fileReader.ReadFile(m_ninjaMap);

	m_ninja = Ninja(SeekStartSymbol());

	m_bombPosition = {};
	m_bombTimer = 0;

	m_IsHolySymboleDestroyed = false;

	m_direction = Direction::SOUTH;
	m_mirroredDirection = Direction::WEST;

	SeekBombsPosition(m_bombPositions);
}

void NinjaMap::BreakObstacle()
{
	Destroy();
	NextMove();
}

void NinjaMap::UnbreakableObstacle()
{
	const Position& ninjaPosition = m_ninja.currentPosition();
	if (m_ninja.MirroredMove())
	{
		if (WestPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
		{
			m_direction = Direction::WEST;
		}
		else if (NorthPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
		{
			m_direction = Direction::NORTH;
		}
		else if (EastPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
		{
			m_direction = Direction::EAST;
		}
		else if (SouthPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
		{
			m_direction = Direction::SOUTH;
		}
	}
	else
	{
		if (SouthPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
		{
			m_direction = Direction::SOUTH;
		}
		else if (EastPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
		{
			m_direction = Direction::EAST;
		}
		else if (NorthPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
		{
			m_direction = Direction::NORTH;
		}
		else if (WestPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
		{
			m_direction = Direction::WEST;
		}
	}
}

void NinjaMap::HolySymbol()
{
	if (m_ninja.IsInBreakerMode() || m_ninja.NumberOfShurikens() > 0)
	{
		if (!m_ninja.IsInBreakerMode())
		{
			m_ninja.ThrowShuriken();
		}
		m_IsHolySymboleDestroyed = true;
		std::cout << "GAME OVER" << std::endl;
	}
	else
	{
		std::cout << "LOOP" << std::endl;
		exit(LOOP);
	}
}

void NinjaMap::NextMove()
{
	switch (m_direction)
	{
	case NinjaMap::SOUTH: m_ninja.moveSouth(); break;
	case NinjaMap::EAST:  m_ninja.moveEast();  break;
	case NinjaMap::NORTH: m_ninja.moveNorth(); break;
	case NinjaMap::WEST:
	default:
		m_ninja.moveWest();
		break;
	}

	CheckBombPositions();

	std::cout << DirectionToString() << std::endl;
}

void NinjaMap::Sake()
{
	m_ninja.IsInBreakerMode() ? m_ninja.SetBreakerMode(false) : m_ninja.SetBreakerMode(true);
	//std::cout << std::boolalpha << "breaker mode: " << m_ninja.IsInBreakerMode() << std::endl;
	NextMove();
}

void NinjaMap::PathModifier()
{
	NextMove();
	m_direction = GetDirection();
}

Position NinjaMap::SeekStartSymbol() const
{
	Position startSymbolPosition = {};
	for (int i = 0; i < m_ninjaMap.size(); i++)
	{
		for (int j = 0; j < m_ninjaMap[i].length(); j++)
		{
			if (m_ninjaMap[i][j] == START_SYMBOL)
			{
				startSymbolPosition.x_pos = i;
				startSymbolPosition.y_pos = j;
			}
		}
	}
	return startSymbolPosition;
}

bool NinjaMap::IsHolySymboleDestroyed() const
{
	return m_IsHolySymboleDestroyed;
}

NinjaMap::Direction NinjaMap::GetDirection() const
{
	const char direction = m_ninjaMap[m_ninja.currentPosition().x_pos][m_ninja.currentPosition().y_pos];
	switch (direction)
	{	
	case 'E': return NinjaMap::EAST;
	case 'N': return NinjaMap::NORTH;
	case 'S': return NinjaMap::SOUTH;
	case 'W': return NinjaMap::WEST;
	}
}

void NinjaMap::readNextMove()
{
	const char nextSymbol = GetNextValueByDirection();
	switch (nextSymbol)
	{
	case BREAKABLE_OBSTACLE:
		BreakObstacle();
		break;
	case UNBREAKABLE_OBSTACLE: 
		UnbreakableObstacle();
		break;
	case EMPTY_SPACE:
		NextMove();
		break;
	case HOLY_SYMBOL: 
		HolySymbol();
		break;
	case MIRRORING_SYMBOL:
		NextMove();
		Mirroring();
		break;
	case PATH_MODIFIER_SOUTH: case PATH_MODIFIER_EAST: case PATH_MODIFIER_NORTH: case PATH_MODIFIER_WEST:
		NextMove();
		m_direction = GetDirection();
		break;
	case SAKE: 
		Sake();
		break;
	case PATHWAY_F:	case PATHWAY_G:	case PATHWAY_H:	case PATHWAY_I:	case PATHWAY_J: case PATHWAY_K:	case PATHWAY_L:
		SeekPathwayPair(nextSymbol);
		break;
	case SHURIKEN:
		m_ninja.AddShuriken();
		break;
	case START_SYMBOL:
		std::cout << "LOOP" << std::endl;
		exit(LOOP);
		break;
	case '0': case '1': case '2': case '3':	case '4': case '5':	case '6': case '7':	case '8': case '9':
		BombActivation(nextSymbol);
		NextMove();
		break;
	default:
		std::cout << "Undefined symbol" << std::endl;
		exit(UNDEFINED_SYMBOL);
		break;
	}
}

void NinjaMap::CheckBombPositions()
{
	for (int i = 0; i < m_bombPositions.size(); i++)
	{
		if (m_bombPositions[i].IsActivated() && m_bombPositions[i].GetBombTimber() > 0)
		{
			m_bombPositions[i].DecreaseTimer();
		}
		else if (m_bombPositions[i].IsActivated() && m_bombPositions[i].GetBombTimber() == 0)
		{
			BombExplosion(m_bombPositions[i]);
			m_bombPositions[i].SetBombActivated(false);
		}
	}
}

void NinjaMap::AddBomb(char timeValue)
{
	Bomb newBomb(GetBombPosition(), CHAR2INT(timeValue), true);
	m_bombPositions.push_back(newBomb);
}

void NinjaMap::BombActivation(char timeValue)
{
	const Position nextBombPosition = GetBombPosition();
	for (int i = 0; i < m_bombPositions.size(); i++)
	{
		if (m_bombPositions[i].GetPosition() == nextBombPosition)
		{
			m_bombPositions[i].SetBombActivated(true);
			return;
		}
	}
}

Position NinjaMap::GetBombPosition() const
{
	Position position = {};
	switch (m_direction)
	{
		case Direction::SOUTH:
			position.x_pos = m_ninja.currentPosition().x_pos + 1;
			position.y_pos = m_ninja.currentPosition().y_pos;
		break;
		case Direction::EAST:
			position.x_pos = m_ninja.currentPosition().x_pos;
			position.y_pos = m_ninja.currentPosition().y_pos + 1;
		break;
		case Direction::NORTH:
			position.x_pos = m_ninja.currentPosition().x_pos - 1;
			position.y_pos = m_ninja.currentPosition().y_pos;
		break;
		case Direction::WEST:
			position.x_pos = m_ninja.currentPosition().x_pos;
			position.y_pos = m_ninja.currentPosition().y_pos - 1;
		break;
	}
	return position;
}

void NinjaMap::ExplosionSouthRecursion(const Position& ninjaPosition, const Bomb& activatedBomb, const Bomb& otherBomb, int explosionRange)
{
	if (m_ninjaMap[activatedBomb.GetPosition().x_pos + explosionRange][activatedBomb.GetPosition().y_pos] != UNBREAKABLE_OBSTACLE && m_ninjaMap[activatedBomb.GetPosition().x_pos + explosionRange][activatedBomb.GetPosition().y_pos] != MIRRORING_SYMBOL)
	{
		CheckNinjaKilledSouth(ninjaPosition, activatedBomb, explosionRange);
		m_ninjaMap[activatedBomb.GetPosition().x_pos][activatedBomb.GetPosition().y_pos] = EMPTY_SPACE;
		m_ninjaMap[otherBomb.GetPosition().x_pos][otherBomb.GetPosition().y_pos] = EMPTY_SPACE;
		if (explosionRange < EXPLOSION_RANGE)
		{
			explosionRange++;
			ExplosionSouthRecursion(ninjaPosition, activatedBomb, otherBomb, explosionRange);
		}
	}
}

void NinjaMap::ExplosionEastRecursion(const Position& ninjaPosition, const Bomb& activatedBomb, const Bomb& otherBomb, int explosionRange)
{
	if (m_ninjaMap[activatedBomb.GetPosition().x_pos][activatedBomb.GetPosition().y_pos + explosionRange] != UNBREAKABLE_OBSTACLE && m_ninjaMap[activatedBomb.GetPosition().x_pos][activatedBomb.GetPosition().y_pos + explosionRange] != MIRRORING_SYMBOL)
	{
		CheckNinjaKilledEast(ninjaPosition, activatedBomb, explosionRange);
		m_ninjaMap[activatedBomb.GetPosition().x_pos][activatedBomb.GetPosition().y_pos] = EMPTY_SPACE;
		m_ninjaMap[otherBomb.GetPosition().x_pos][otherBomb.GetPosition().y_pos] = EMPTY_SPACE;
		if (explosionRange < EXPLOSION_RANGE)
		{
			explosionRange++;
			ExplosionEastRecursion(ninjaPosition, activatedBomb, otherBomb, explosionRange);
		}
	}
}

void NinjaMap::ExplosionNorthRecursion(const Position& ninjaPosition, const Bomb& activatedBomb, const Bomb& otherBomb, int explosionRange)
{
	if (m_ninjaMap[activatedBomb.GetPosition().x_pos - explosionRange][activatedBomb.GetPosition().y_pos] != UNBREAKABLE_OBSTACLE && m_ninjaMap[activatedBomb.GetPosition().x_pos - explosionRange][activatedBomb.GetPosition().y_pos] != MIRRORING_SYMBOL)
	{
		CheckNinjaKilledNorth(ninjaPosition, activatedBomb, explosionRange);
		m_ninjaMap[activatedBomb.GetPosition().x_pos - explosionRange][activatedBomb.GetPosition().y_pos] = EMPTY_SPACE;
		m_ninjaMap[otherBomb.GetPosition().x_pos][otherBomb.GetPosition().y_pos] = EMPTY_SPACE;
		if (explosionRange < EXPLOSION_RANGE)
		{
			explosionRange++;
			ExplosionNorthRecursion(ninjaPosition, activatedBomb, otherBomb, explosionRange);
		}
	}
}

void NinjaMap::ExplosionWestRecursion(const Position& ninjaPosition, const Bomb& activatedBomb, const Bomb& otherBomb, int explosionRange)
{
	if (m_ninjaMap[activatedBomb.GetPosition().x_pos][activatedBomb.GetPosition().y_pos - explosionRange] != UNBREAKABLE_OBSTACLE && m_ninjaMap[activatedBomb.GetPosition().x_pos][activatedBomb.GetPosition().y_pos - explosionRange] != MIRRORING_SYMBOL)
	{
		CheckNinjaKilledWest(ninjaPosition, activatedBomb, explosionRange);
		m_ninjaMap[activatedBomb.GetPosition().x_pos][activatedBomb.GetPosition().y_pos - explosionRange] = EMPTY_SPACE;
		m_ninjaMap[otherBomb.GetPosition().x_pos][otherBomb.GetPosition().y_pos] = EMPTY_SPACE;
		if (explosionRange < EXPLOSION_RANGE)
		{
			explosionRange++;
			ExplosionWestRecursion(ninjaPosition, activatedBomb, otherBomb, explosionRange);
		}
	}
}

void NinjaMap::CheckNinjaKilledSouth(const Position& ninjaPosition, const Bomb& bomb, int explosionRange)
{
	if (ninjaPosition.x_pos == bomb.GetPosition().x_pos + explosionRange && ninjaPosition.y_pos == m_bombPosition.y_pos)
	{
		NinjaKilled();
	}
}

void NinjaMap::CheckNinjaKilledEast(const Position& ninjaPosition, const Bomb& bomb, int explosionRange)
{
	if (ninjaPosition.x_pos == bomb.GetPosition().x_pos && ninjaPosition.y_pos == bomb.GetPosition().y_pos + explosionRange)
	{
		NinjaKilled();
	}
}

void NinjaMap::CheckNinjaKilledNorth(const Position& ninjaPosition, const Bomb& bomb, int explosionRange)
{
	if (ninjaPosition.x_pos == bomb.GetPosition().x_pos - explosionRange && ninjaPosition.y_pos == bomb.GetPosition().y_pos)
	{
		NinjaKilled();
	}
}

void NinjaMap::CheckNinjaKilledWest(const Position& ninjaPosition, const Bomb& bomb, int explosionRange)
{
	if (ninjaPosition.x_pos == bomb.GetPosition().x_pos && ninjaPosition.y_pos == bomb.GetPosition().y_pos - explosionRange)
	{
		NinjaKilled();
	}
}

void NinjaMap::ExplosionSouth(const Position& ninjaPosition, const Bomb& activatedBomb)
{
	for (int i = 0; i < m_bombPositions.size(); i++)
	{
		if ((std::abs(activatedBomb.GetPosition().x_pos - m_bombPositions[i].GetPosition().x_pos) <= EXPLOSION_RANGE && activatedBomb.GetPosition().x_pos != m_bombPositions[i].GetPosition().x_pos))
		{
			ExplosionSouthRecursion(ninjaPosition, activatedBomb, m_bombPositions[i], INITIAL_EXLPOSION_RANGE);
		}
	}
}

void NinjaMap::ExplosionEast(const Position& ninjaPosition, const Bomb& activatedBomb)
{
	for (int i = 0; i < m_bombPositions.size(); i++)
	{
		if ((std::abs(activatedBomb.GetPosition().y_pos - m_bombPositions[i].GetPosition().y_pos) <= EXPLOSION_RANGE && activatedBomb.GetPosition().y_pos != m_bombPositions[i].GetPosition().y_pos))
		{
			ExplosionEastRecursion(ninjaPosition, activatedBomb, m_bombPositions[i], INITIAL_EXLPOSION_RANGE);
		}
	}
}

void NinjaMap::ExplosionNorth(const Position& ninjaPosition, const Bomb& activatedBomb)
{
	for (int i = 0; i < m_bombPositions.size(); i++)
	{
		if ((std::abs(activatedBomb.GetPosition().y_pos - m_bombPositions[i].GetPosition().y_pos) <= EXPLOSION_RANGE && activatedBomb.GetPosition().y_pos != m_bombPositions[i].GetPosition().y_pos))
		{
			ExplosionNorthRecursion(ninjaPosition, activatedBomb, m_bombPositions[i], INITIAL_EXLPOSION_RANGE);
		}
	}
}

void NinjaMap::ExplosionWest(const Position& ninjaPosition, const Bomb& activatedBomb)
{
	for (int i = 0; i < m_bombPositions.size(); i++)
	{
		if ((std::abs(activatedBomb.GetPosition().y_pos - m_bombPositions[i].GetPosition().y_pos) <= EXPLOSION_RANGE && activatedBomb.GetPosition().y_pos != m_bombPositions[i].GetPosition().y_pos))
		{
			ExplosionWestRecursion(ninjaPosition, activatedBomb, m_bombPositions[i], INITIAL_EXLPOSION_RANGE);
		}
	}
}

void NinjaMap::BombExplosion(const Bomb& activatedBomb)
{
	ExplosionSouth(m_ninja.currentPosition(), activatedBomb);
	ExplosionEast(m_ninja.currentPosition(), activatedBomb);
	ExplosionNorth(m_ninja.currentPosition(), activatedBomb);
	ExplosionWest(m_ninja.currentPosition(), activatedBomb);
}

void NinjaMap::SeekPathwayPair(char pathway)
{
	Position  positionPair = {};
	for (int i = 0; i < m_ninjaMap.size(); i++)
	{
		for (int j = 0; j < m_ninjaMap[i].length(); j++)
		{
			if (m_ninjaMap[i][j] == pathway && m_ninja.currentPosition().x_pos != i && m_ninja.currentPosition().y_pos != j)
			{
				positionPair.x_pos = i;
				positionPair.y_pos = j;
				m_ninja.currentPosition(positionPair);
				return;
			}
		}
	}
}

char NinjaMap::GetNextValueByDirection() const
{
	const Position& ninjaPosition = m_ninja.currentPosition();
	switch (m_direction)
	{
	case Direction::SOUTH: return SouthPosition(ninjaPosition);
	case Direction::EAST:  return EastPosition(ninjaPosition);
	case Direction::NORTH: return NorthPosition(ninjaPosition);
	case Direction::WEST:  return WestPosition(ninjaPosition);
	}
	return EMPTY_SPACE;
}

std::string NinjaMap::DirectionToString() const
{
	switch (m_direction)
	{
	case Direction::SOUTH: return "SOUTH";
	case Direction::EAST:  return "EAST";
	case Direction::NORTH: return "NORTH";
	case Direction::WEST:  return "WEST";
	default:			   return ""; 
	}
}

bool NinjaMap::IsSake() const
{
	const Position& ninjaPosition = m_ninja.currentPosition();
	switch (m_direction)
	{
	case Direction::SOUTH: return SouthPosition(ninjaPosition) == SAKE;
	case Direction::EAST:  return EastPosition(ninjaPosition)  == SAKE;
	case Direction::NORTH: return NorthPosition(ninjaPosition) == SAKE;
	case Direction::WEST:  return WestPosition(ninjaPosition)  == SAKE;
	}
}

void NinjaMap::Destroy()
{
	const Position& ninjaPosition = m_ninja.currentPosition();
	if (m_ninja.IsInBreakerMode())
	{
		switch (m_direction)
		{
		case NinjaMap::SOUTH:
			m_ninjaMap[m_ninja.currentPosition().x_pos + 1][m_ninja.currentPosition().y_pos] = EMPTY_SPACE;
			break;
		case NinjaMap::EAST:
			m_ninjaMap[m_ninja.currentPosition().x_pos][m_ninja.currentPosition().y_pos + 1] = EMPTY_SPACE;
			break;
		case NinjaMap::NORTH:
			m_ninjaMap[m_ninja.currentPosition().x_pos - 1][m_ninja.currentPosition().y_pos] = EMPTY_SPACE;
			break;
		case NinjaMap::WEST:
			m_ninjaMap[m_ninja.currentPosition().x_pos][m_ninja.currentPosition().y_pos - 1] = EMPTY_SPACE;
			break;
		default:
			break;
		}
		std::cout << "BREAK" << std::endl;
	}
	else if (m_ninja.NumberOfShurikens() > 0)
	{
		m_ninja.ThrowShuriken();
		switch (m_direction)
		{
		case NinjaMap::SOUTH:
			m_ninjaMap[m_ninja.currentPosition().x_pos + 1][m_ninja.currentPosition().y_pos] = EMPTY_SPACE;
			break;
		case NinjaMap::EAST:
			m_ninjaMap[m_ninja.currentPosition().x_pos][m_ninja.currentPosition().y_pos + 1] = EMPTY_SPACE;
			break;
		case NinjaMap::NORTH:
			m_ninjaMap[m_ninja.currentPosition().x_pos - 1][m_ninja.currentPosition().y_pos] = EMPTY_SPACE;
			break;
		case NinjaMap::WEST:
			m_ninjaMap[m_ninja.currentPosition().x_pos][m_ninja.currentPosition().y_pos - 1] = EMPTY_SPACE;
			break;
		default:
			break;
		}
		std::cout << "THROW" << std::endl;
	}
	else
	{
		const Position& ninjaPosition = m_ninja.currentPosition();
		if (m_ninja.MirroredMove())
		{
			if (WestPosition(ninjaPosition) != BREAKABLE_OBSTACLE && WestPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
			{
				m_direction = Direction::WEST;
			}
			else if (NorthPosition(ninjaPosition) != BREAKABLE_OBSTACLE && NorthPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
			{
				m_direction = Direction::NORTH;
			}
			else if (EastPosition(ninjaPosition) != BREAKABLE_OBSTACLE && EastPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
			{
				m_direction = Direction::EAST;
			}
			else if (SouthPosition(ninjaPosition) != BREAKABLE_OBSTACLE && SouthPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
			{
				m_direction = Direction::SOUTH;
			}
		}
		else
		{
			if (SouthPosition(ninjaPosition) != BREAKABLE_OBSTACLE && SouthPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
			{
				m_direction = Direction::SOUTH;
			}
			else if (EastPosition(ninjaPosition) != BREAKABLE_OBSTACLE && EastPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
			{
				m_direction = Direction::EAST;
			}
			else if (NorthPosition(ninjaPosition) != BREAKABLE_OBSTACLE && NorthPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
			{
				m_direction = Direction::NORTH;
			}
			else if (WestPosition(ninjaPosition) != BREAKABLE_OBSTACLE && WestPosition(ninjaPosition) != UNBREAKABLE_OBSTACLE)
			{
				m_direction = Direction::WEST;
			}
		}	
	}
}

NinjaMap::Direction NinjaMap::MirroredNextDirection() const
{
	switch (m_direction)
	{
	case NinjaMap::SOUTH: return Direction::WEST;
	case NinjaMap::EAST:  return Direction::SOUTH;
	case NinjaMap::NORTH: return Direction::EAST;
	case NinjaMap::WEST:  return Direction::NORTH;
	}
}

NinjaMap::Direction NinjaMap::UnMirroredNextDirection() const
{
	switch (m_direction)
	{
	case NinjaMap::SOUTH: return Direction::EAST;
	case NinjaMap::EAST:  return Direction::NORTH;
	case NinjaMap::NORTH: return Direction::WEST;
	case NinjaMap::WEST:  return Direction::SOUTH;
	}
}

void NinjaMap::Mirroring()
{
	if (!m_ninja.MirroredMove())
	{
		m_ninja.MirroredMove(true);
	}
	else
	{
		m_ninja.MirroredMove(false);
	}
}

void NinjaMap::StepToDirection()
{
	switch (m_direction)
	{
	case NinjaMap::SOUTH: m_ninja.moveSouth(); break;
	case NinjaMap::EAST:  m_ninja.moveEast();  break;
	case NinjaMap::NORTH: m_ninja.moveNorth(); break;
	case NinjaMap::WEST:  m_ninja.moveWest();  break;
	}
}

inline char NinjaMap::SouthPosition(const Position& pos) const
{
	return m_ninjaMap[pos.x_pos + 1][pos.y_pos];
}

inline char NinjaMap::EastPosition(const Position& pos) const
{
	return m_ninjaMap[pos.x_pos][pos.y_pos + 1];
}

inline char NinjaMap::NorthPosition(const Position& pos) const 
{
	return m_ninjaMap[pos.x_pos - 1][pos.y_pos];
}

inline char NinjaMap::WestPosition(const Position& pos) const 
{
	return m_ninjaMap[pos.x_pos][pos.y_pos - 1];
}

void NinjaMap::SeekBombsPosition(std::vector<Bomb>& bombs)
{
	for (int i = 1; i < m_ninjaMap.size() - 1; i++)
	{
		for (int j = 1; j < m_ninjaMap[i].length() - 1; j++)
		{
			if (CHAR2INT(m_ninjaMap[i][j]) >= 0 && CHAR2INT(m_ninjaMap[i][j]) <= 9)
			{
				Bomb newBomb({ i,j }, CHAR2INT(m_ninjaMap[i][j]), false);
				bombs.push_back(newBomb);
			}
		}
	}
}

inline void NinjaMap::NinjaKilled() const
{
	std::cout << "NINJA KILLED" << std::endl;
	exit(NINJA_KILLED);
}

std::ostream& operator<<(std::ostream& os, const NinjaMap& ninjaMap)
{
	for (std::size_t i = 0; i < ninjaMap.m_ninjaMap.size(); i++)
	{
		os << ninjaMap.m_ninjaMap[i] << '\n';
	}
	return os;
}