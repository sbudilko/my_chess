#ifndef CHESSFIGURE_H
#define CHESSFIGURE_H

#include "EngineUtils.hpp"

class ChessFigure;

using Board = std::array<std::array<std::shared_ptr<ChessFigure>, 8>, 8>;

class ChessFigure
{
public:
	ChessFigure(COLOR color, FIGURE type = FIGURE::NONE) : m_FigureColor(color),
														   m_FigureType(type)
	{}

	~ChessFigure() {}

	COLOR getColor()
	{
		return m_FigureColor;
	}

	FIGURE getFigureType()
	{
		return m_FigureType;
	}

	bool IsItPossibleToMove(BoardCoordinate src, BoardCoordinate dest, Board& gameBoard, bool justCheck = false)
	{
		bool result = false;

		// Get destination cell 
		auto DestinationCell = gameBoard[dest.row][dest.col];

		// Verify that destination cell is empty
		// or contained an enemy figure
		if (DestinationCell == nullptr || DestinationCell->getColor() != m_FigureColor)
			result = checkFigureRoute(src, dest, gameBoard, justCheck);

		return result;

	}

	void increaseMoveNumber() 
	{
		m_MoveNumber++;
	}

	void decreaseMoveNumber()
	{
		m_MoveNumber--;
	}

	int getMoveNumber()
	{
		return m_MoveNumber;
	}


private:

	virtual bool checkFigureRoute(BoardCoordinate src, BoardCoordinate dest, Board& gameBoard, bool justCheck) = 0;

	int		m_MoveNumber = 0;
	COLOR	m_FigureColor;
	FIGURE	m_FigureType;
};

class Pawn : public ChessFigure
{
public:

	Pawn(COLOR color) : ChessFigure(color, FIGURE::PAWN) {}

private:


	bool checkFigureRoute(BoardCoordinate src, BoardCoordinate dest, Board& gameBoard, bool justCheck)
	{

		// Get destination cell 
		auto DestinationCell = gameBoard[dest.row][dest.col];
		// Get moving direction, +1 for Whiets, -1 for Blacks
		int MovingDirection = this->getColor() == COLOR::WHITE ? 1 : -1;

		if (DestinationCell == nullptr) // destination cell is empty
		{
			if (dest.col == src.col)
			{
				if (dest.row == src.row + 1 * MovingDirection)
					return true;

				// Go on 2 cells on first move
				if (dest.row == src.row + 2 * MovingDirection && this->getMoveNumber() == 0)
				{

					// Verify is there any figures on route, between source cell and destination
					if (gameBoard[dest.row - 1 * MovingDirection][dest.col] == nullptr)
						return true;
				}
			}
		}
		else // Attack
		{
			if (dest.col + 1 == src.col || dest.col - 1 == src.col)
			{
				if (dest.row == src.row + MovingDirection)
					return true;
			}
		}
		return false;
	}


};

class Rook : public ChessFigure
{
public:
	Rook(COLOR color) : ChessFigure(color, FIGURE::ROOK) {}
private:

	bool checkFigureRoute(BoardCoordinate src, BoardCoordinate dest, Board& gameBoard, bool justCheck)
	{
																		// X - possible move, R - Rook
		if (dest.row == src.row)										// |__|__|_X_|__|__|
		{																// |__|__|_X_|__|__|
			// Vertical moving											// |X_|X_|_R_|X_|X_|
																		// |__|__|_X_|__|__|
			// Verify where rook moves, left or right					// |__|__|_X_|__|__|
			int MovingDerection = dest.col - src.col > 0 ? 1 : -1;

			// Verify is there any figures on route, between source cell and destination
			for (int column = src.col + MovingDerection; column != dest.col; column = column + MovingDerection)
			{
				if (gameBoard[dest.row][column] != nullptr)
					return false;
			}
			return true;
		}
		else if (dest.col == src.col)
		{

			// Horisontal moving

			// Verify where rook moves, up or down
			int MovingDerection = dest.row - src.row > 0 ? 1 : -1;

			// Verify is there any figures on route, between source cell and destination
			for (int row = src.row + MovingDerection; row != dest.row; row = row + MovingDerection)
			{
				if (gameBoard[row][dest.col] != nullptr)
					return false;
			}
			return true;
		}
		else
			return false;
	}
};

class Knight : public ChessFigure
{
public:
	Knight(COLOR color) : ChessFigure(color, FIGURE::KNIGHT) {}
private:
	bool checkFigureRoute(BoardCoordinate src, BoardCoordinate dest, Board& gameBoard, bool justCheck)
	{													//  X- possible move, K -knight
		if (std::abs(dest.row - src.row) == 2)			//	|__|X_|___|_X|__|
			if (std::abs(dest.col - src.col) == 1)		//	|X_|__|___|__|X_|
				return true;							//	|__|__|_K_|__|__|
		if (std::abs(dest.row - src.row) == 1)			//	|X_|__|___|__|X_|
			if (std::abs(dest.col - src.col) == 2)		//	|__|X_|___|_X|__|
				return true;
		return false;
	}
};

class Bishop : public ChessFigure
{
public:
	Bishop(COLOR color) : ChessFigure(color, FIGURE::BISHOP) {}
private:
	bool checkFigureRoute(BoardCoordinate src, BoardCoordinate dest, Board& gameBoard, bool justCheck)
	{
		if (std::abs(dest.col - src.col) == std::abs(dest.row - src.row))
		{
			int ColumnMotionDirection = dest.col - src.col > 0 ? 1 : -1;							// X - possible move, B - Bishop
			int RowMotionDirecction = dest.row - src.row > 0 ? 1 : -1;								// |_X|__|___|__|X_|
																									// |__|_X|___|X_|__|
			// Verify is there any figures on route, between source cell and destination			// |__|__|_B_|__|__|
			for (int i = src.row + RowMotionDirecction,  j = src.col + ColumnMotionDirection;		// |__|X_|___|X_|__|
				i != dest.row; i = i + RowMotionDirecction, j = j + ColumnMotionDirection)			// |X_|__|___|__|X_|
			{
				if (gameBoard[i][j] != nullptr)
					return false;
			}
			return true;
		}
		return false;
	}
};

class Queen : public ChessFigure {
public:
	Queen(COLOR color) : ChessFigure(color, FIGURE::QUEEN) {}
private:

	bool checkFigureRoute(BoardCoordinate src, BoardCoordinate dest, Board& gameBoard, bool justCheck)
	{
		// Queen figure has behaviour of both Rook and Bishop figures
		Rook	m_Rook(this->getColor());
		Bishop	m_Bishop(this->getColor());

		// Verify, can Queen move as Rook
		if (m_Rook.IsItPossibleToMove(src, dest, gameBoard))
			return true;

		// Verify, can Queen move as Bishop
		if (m_Bishop.IsItPossibleToMove(src, dest, gameBoard))
			return true;
		return false;
	}
};

class King : public ChessFigure
{
public:
	King(COLOR color) : ChessFigure(color, FIGURE::KING) {}
private:


	bool checkFigureRoute(BoardCoordinate src, BoardCoordinate dest, Board& gameBoard, bool justCheck)
	{
		// King can move only on one cell											// X - posssible move, K - KING
		if (dest.col - src.col >= -1 && dest.col - src.col <= 1 &&					// |__|__|___|__|__|
			dest.row - src.row >= -1 && dest.row - src.row <= 1)					// |__|X_|_X_|_X|__|
		{																			// |__|X_|_K_|_X|__|
			return true;															// |__|X_|_X_|_X|__|
		}																			// |__|__|___|__|__|				
	
		return false;
	}
};

#endif // !CHESSFIGURE_H