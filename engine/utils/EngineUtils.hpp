#ifndef ENGINEUTILS_H
#define ENGINEUTILS_H

#include <string>
#include <iostream>
#include <memory>

enum class GAME_STATUS
{
	GAME_OVER,
	INVALID_MOVE,
	TRY_TO_MOVE_ENEMY_FIGURE,
	KING_IN_CHECK_AFTER_MOVE,
	KING_IN_CHECK_AFTER_CASTLING,
	CASTLING,
	BAD_CASTLING,
	WHITE_PROMOTION,
	BLACK_PROMOTION,

	GOOD_MOVE

};

enum class COLOR
{
	BLACK,
	WHITE
};

std::ostream& operator<<(std::ostream& os, COLOR color)
{
	if (color == COLOR::WHITE)
		os << "WHITE";
	else
		os << "BLACK";
	return os;
}

enum class FIGURE
{
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING,
	PAWN,

	NONE
};

std::ostream& operator<<(std::ostream& os, FIGURE figure)
{
	if (figure == FIGURE::ROOK)
		os << "ROOK";
	else if (figure == FIGURE::KNIGHT)
		os << "KNIGHT";
	else if (figure == FIGURE::BISHOP)
		os << "BISHOP";
	else if (figure == FIGURE::QUEEN)
		os << "QUEEN";
	else if (figure == FIGURE::KING)
		os << "KING";
	else if (figure == FIGURE::PAWN)
		os << "PAWN";
	else if (figure == FIGURE::NONE)
		os << "NONE";
	return os;
}


class BoardCoordinate
{
public:
	int row;
	int col;
};

std::ostream& operator<<(std::ostream& os, BoardCoordinate coor)
{
	os << "{" << coor.row << "," << coor.col << "}";
	return os;
}

BoardCoordinate ChessNotationToBoardCoor(char a, char b)
{
	BoardCoordinate result;

	result.col = static_cast<int>(a - 97);
	result.row = static_cast<int>(b - 49);

	return result;
}

#endif // !CHESSFIGURE_H
