#ifndef POSITION_H
#define POSITION_H

struct Position
{
	int x_pos;
	int y_pos;

	bool operator==(const Position& position) const 
	{
		return x_pos == position.x_pos && y_pos == position.y_pos;
	}
};

#endif // POSITION_H

