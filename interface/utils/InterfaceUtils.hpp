#ifndef INTERFACEUTILS_H
#define INTERFACEUTILS_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

sf::Vector2f gOffset(15, 15);
int gCellSize = 80;

enum class FigureType
{
	BLACK_ROOK,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_QUEEN,
	BLACK_KING,
	BLACK_PAWN,
	WHITE_ROOK,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_QUEEN,
	WHITE_KING,
	WHITE_PAWN,

	NONE
};

std::unordered_map<FigureType, std::string> gTextureFiles = {
	{FigureType::BLACK_ROOK, "images/b_rook_1x.png"},
	{FigureType::BLACK_KNIGHT, "images/b_knight_1x.png"},
	{FigureType::BLACK_BISHOP, "images/b_bishop_1x.png"},
	{FigureType::BLACK_QUEEN, "images/b_queen_1x.png"},
	{FigureType::BLACK_KING, "images/b_king_1x.png"},
	{FigureType::BLACK_PAWN, "images/b_pawn_1x.png"},
	{FigureType::WHITE_ROOK, "images/w_rook_1x.png"},
	{FigureType::WHITE_KNIGHT, "images/w_knight_1x.png"},
	{FigureType::WHITE_BISHOP, "images/w_bishop_1x.png"},
	{FigureType::WHITE_QUEEN, "images/w_queen_1x.png"},
	{FigureType::WHITE_KING, "images/w_king_1x.png"},
	{FigureType::WHITE_PAWN, "images/w_pawn_1x.png"}
};

std::unordered_map<int, FigureType> gFigureNumberToFigureType = {
	{1, FigureType::WHITE_PAWN},
	{2, FigureType::WHITE_QUEEN},
	{3, FigureType::WHITE_KNIGHT},
	{4, FigureType::WHITE_ROOK},
	{5, FigureType::WHITE_BISHOP},
	{6, FigureType::BLACK_PAWN},
	{7, FigureType::BLACK_QUEEN},
	{8, FigureType::BLACK_KNIGHT},
	{9, FigureType::BLACK_ROOK},
	{10, FigureType::BLACK_BISHOP}

};

std::vector<std::vector<FigureType>> gAllFigures = {
	{
		FigureType::BLACK_ROOK, FigureType::BLACK_KNIGHT, FigureType::BLACK_BISHOP, FigureType::BLACK_QUEEN,
		FigureType::BLACK_KING, FigureType::BLACK_BISHOP, FigureType::BLACK_KNIGHT, FigureType::BLACK_ROOK
	},
	{
		FigureType::BLACK_PAWN, FigureType::BLACK_PAWN, FigureType::BLACK_PAWN, FigureType::BLACK_PAWN,
		FigureType::BLACK_PAWN, FigureType::BLACK_PAWN, FigureType::BLACK_PAWN, FigureType::BLACK_PAWN
	},
	{
		FigureType::NONE, FigureType::NONE, FigureType::NONE, FigureType::NONE,
		FigureType::NONE, FigureType::NONE, FigureType::NONE, FigureType::NONE,
	},
	{
		FigureType::NONE, FigureType::NONE, FigureType::NONE, FigureType::NONE,
		FigureType::NONE, FigureType::NONE, FigureType::NONE, FigureType::NONE,
	},
	{
		FigureType::NONE, FigureType::NONE, FigureType::NONE, FigureType::NONE,
		FigureType::NONE, FigureType::NONE, FigureType::NONE, FigureType::NONE,
	},
	{
		FigureType::NONE, FigureType::NONE, FigureType::NONE, FigureType::NONE,
		FigureType::NONE, FigureType::NONE, FigureType::NONE, FigureType::NONE,
	},
	{
		FigureType::WHITE_PAWN, FigureType::WHITE_PAWN, FigureType::WHITE_PAWN, FigureType::WHITE_PAWN,
		FigureType::WHITE_PAWN, FigureType::WHITE_PAWN, FigureType::WHITE_PAWN, FigureType::WHITE_PAWN
	},
	{	FigureType::WHITE_ROOK, FigureType::WHITE_KNIGHT, FigureType::WHITE_BISHOP, FigureType::WHITE_QUEEN,
		FigureType::WHITE_KING, FigureType::WHITE_BISHOP, FigureType::WHITE_KNIGHT, FigureType::WHITE_ROOK
	}
};


std::string CoordinatesToChessNotation(const sf::Vector2f& coordinates)
{
	if (coordinates.x < 0 || coordinates.y < 0 ||
		coordinates.x > 640 || coordinates.y > 640)
		return "-";

	std::string chessNotation = "";
	chessNotation += static_cast<char>(coordinates.x / gCellSize + 97); //  97 - ASCII symbol 'a'
	chessNotation += static_cast<char>(7 - coordinates.y / gCellSize + 49); //  49 - ASCII symbol '1'
	return chessNotation;
}

sf::Vector2f  ChessNotationToCoordinates(char a, char b)
{
	int x = static_cast<int>(a) - 97;
	int y = 7 - static_cast<int>(b) + 49;
	return sf::Vector2f(x * gCellSize, y * gCellSize);
}

#endif