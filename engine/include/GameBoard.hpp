#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "ChessFigure.hpp"
#include <array>

class GameBoard
{
public:
	GameBoard() 
	{
		// Initialize MainGameBoard variable with nullptr
		for (int i = 0; i < 8; ++i)
			for (int j = 0; j < 8; ++j)
				MainGameBoard[i][j] = nullptr;

		// Place Whites Figures on the board
		for (int column = 0; column < 8; ++column) {
			MainGameBoard[6][column] = std::shared_ptr<ChessFigure>(new Pawn(COLOR::BLACK));
		}
		MainGameBoard[7][0] = std::shared_ptr<ChessFigure>(new Rook(COLOR::BLACK));
		MainGameBoard[7][1] = std::shared_ptr<ChessFigure>(new Knight(COLOR::BLACK));
		MainGameBoard[7][2] = std::shared_ptr<ChessFigure>(new Bishop(COLOR::BLACK));
		MainGameBoard[7][3] = std::shared_ptr<ChessFigure>(new Queen(COLOR::BLACK));
		MainGameBoard[7][4] = std::shared_ptr<ChessFigure>(new King(COLOR::BLACK));
		MainGameBoard[7][5] = std::shared_ptr<ChessFigure>(new Bishop(COLOR::BLACK));
		MainGameBoard[7][6] = std::shared_ptr<ChessFigure>(new Knight(COLOR::BLACK));
		MainGameBoard[7][7] = std::shared_ptr<ChessFigure>(new Rook(COLOR::BLACK));

		// Place Blacks figures on the board
		for (int column = 0; column < 8; ++column) {
			MainGameBoard[1][column] = std::shared_ptr<ChessFigure>(new Pawn(COLOR::WHITE));
		}
		MainGameBoard[0][0] = std::shared_ptr<ChessFigure>(new Rook(COLOR::WHITE));
		MainGameBoard[0][1] = std::shared_ptr<ChessFigure>(new Knight(COLOR::WHITE));
		MainGameBoard[0][2] = std::shared_ptr<ChessFigure>(new Bishop(COLOR::WHITE));
		MainGameBoard[0][3] = std::shared_ptr<ChessFigure>(new Queen(COLOR::WHITE));
		MainGameBoard[0][4] = std::shared_ptr<ChessFigure>(new King(COLOR::WHITE));
		MainGameBoard[0][5] = std::shared_ptr<ChessFigure>(new Bishop(COLOR::WHITE));
		MainGameBoard[0][6] = std::shared_ptr<ChessFigure>(new Knight(COLOR::WHITE));
		MainGameBoard[0][7] = std::shared_ptr<ChessFigure>(new Rook(COLOR::WHITE));
	}

	Board MainGameBoard;

};

#endif
