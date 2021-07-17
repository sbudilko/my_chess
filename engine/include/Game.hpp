#ifndef GAME_H
#define GAME_H

#include "EngineUtils.hpp"
#include "GameBoard.hpp"

#include <iostream>
#include<iomanip>


class Game
{
public:
	Game() : m_PlayerTurn(COLOR::WHITE) {}
	
	void changeTurn();

	GAME_STATUS checkCastling(std::string chess_note)
	{
		BoardCoordinate source = ChessNotationToBoardCoor(chess_note[0], chess_note[1]);
		BoardCoordinate destination = ChessNotationToBoardCoor(chess_note[2], chess_note[3]);

		// Get the cell of the selected figure
		auto SourceCell = m_GameBoard.MainGameBoard[source.row][source.col];

		// Get destination cell
		auto DestinationCell = m_GameBoard.MainGameBoard[destination.row][destination.col];

		// Ñheck that the selected figure has the correct color
		if (SourceCell->getColor() != m_PlayerTurn)
			return GAME_STATUS::TRY_TO_MOVE_ENEMY_FIGURE; //TRY_TO_MOVE_ENEMY_FIGURE

		// Check Castling
		if (SourceCell != nullptr && SourceCell->getFigureType() == FIGURE::KING &&
			SourceCell->getMoveNumber() == 0)
		{
			BoardCoordinate RookCastlingSource;
			RookCastlingSource.row = m_PlayerTurn == COLOR::WHITE ? 0 : 7;
			RookCastlingSource.col = destination.col - source.col > 0 ? 7 : 0;

			// Get Rook source cell
			auto RookSourceCell = m_GameBoard.MainGameBoard[RookCastlingSource.row][RookCastlingSource.col];

			BoardCoordinate RookCastlingDestination;
			RookCastlingDestination.row = RookCastlingSource.row;
			RookCastlingDestination.col = destination.col - source.col > 0 ? 5 : 3;
			
			// Verify Rook's ability to castling
			if (m_GameBoard.MainGameBoard[RookCastlingSource.row][RookCastlingSource.col] != nullptr &&
				m_GameBoard.MainGameBoard[RookCastlingSource.row][RookCastlingSource.col]->getColor() == m_PlayerTurn &&
				m_GameBoard.MainGameBoard[RookCastlingSource.row][RookCastlingSource.col]->getMoveNumber() == 0 &&
				m_GameBoard.MainGameBoard[RookCastlingDestination.row][RookCastlingDestination.col] == nullptr)
			{
				if (RookSourceCell->IsItPossibleToMove(RookCastlingSource, RookCastlingDestination, m_GameBoard.MainGameBoard))
				{

					// Verify King before castling
					// According the rule King must not be in the check
					if (Check(this->m_PlayerTurn))
					{
						std::cout << "Castling refuse. King is in the check\n";
						return GAME_STATUS::INVALID_MOVE;
					}

					// Increase King's move number
					SourceCell->increaseMoveNumber();

					// Move King to destination
					m_GameBoard.MainGameBoard[destination.row][destination.col] = SourceCell;

					// Delete King from source cell
					m_GameBoard.MainGameBoard[source.row][source.col] = nullptr;

					// Verify King after castling
					if (Check(this->m_PlayerTurn))
					{
						// King is in CHECK
						std::cout << m_PlayerTurn << " King is in CHECK\n";

						// Undo previous move

						// Decrease move number
						SourceCell->decreaseMoveNumber();

						// Restore destination cell
						m_GameBoard.MainGameBoard[destination.row][destination.col] = DestinationCell;

						// Restore source cell
						m_GameBoard.MainGameBoard[source.row][source.col] = SourceCell;

						std::cout << m_PlayerTurn << ": King in Check after castling\n.";
						return GAME_STATUS::KING_IN_CHECK_AFTER_CASTLING; // KING_IN_CHECK_AFTER_ Castling
					}

					// Increase Rook's move number
					RookSourceCell->increaseMoveNumber();

					// Move King to destination
					m_GameBoard.MainGameBoard[RookCastlingDestination.row][RookCastlingDestination.col] = RookSourceCell;

					// Delete King from source cell
					m_GameBoard.MainGameBoard[RookCastlingSource.row][RookCastlingSource.col] = nullptr;

					// Give turn to the next player
					this->changeTurn();

					return GAME_STATUS::CASTLING;
				}
			}

		}

		return GAME_STATUS::BAD_CASTLING;
	}

	GAME_STATUS nextMove(std::string chess_note)
	{

		GAME_STATUS gameStatus = GAME_STATUS::GOOD_MOVE;

		if (chess_note[0] == '-' || chess_note[2] == '-')
			return GAME_STATUS::INVALID_MOVE;

		BoardCoordinate source = ChessNotationToBoardCoor(chess_note[0], chess_note[1]);
		BoardCoordinate destination = ChessNotationToBoardCoor(chess_note[2], chess_note[3]);

		// Get the cell of the selected figure
		auto SourceCell = m_GameBoard.MainGameBoard[source.row][source.col];

		// Get destination cell
		auto DestinationCell = m_GameBoard.MainGameBoard[destination.row][destination.col];

		// Ñheck that the selected figure has the correct color
		if (SourceCell->getColor() != m_PlayerTurn)
			return GAME_STATUS::TRY_TO_MOVE_ENEMY_FIGURE; //TRY_TO_MOVE_ENEMY_FIGURE

		if (SourceCell->IsItPossibleToMove(source, destination, m_GameBoard.MainGameBoard))
		{

			// Increase move number
			SourceCell->increaseMoveNumber();

			// Replace destination cell with source
			m_GameBoard.MainGameBoard[destination.row][destination.col] = SourceCell;

			// Delete figure from source cell
			m_GameBoard.MainGameBoard[source.row][source.col] = nullptr;

			// Verify King after move
			if (Check(this->m_PlayerTurn))
			{
				// King is in CHECK
				std::cout << m_PlayerTurn << " King is in CHECK\n";

				// Undo previous move

				// Decrease move number
				SourceCell->decreaseMoveNumber();

				// Restore destination cell
				m_GameBoard.MainGameBoard[destination.row][destination.col] = DestinationCell;

				// Restore source cell
				m_GameBoard.MainGameBoard[source.row][source.col] = SourceCell;

				return GAME_STATUS::KING_IN_CHECK_AFTER_MOVE; // KING_IN_CHECK_AFTER_MOVE
			}

		}
		else
			return GAME_STATUS::INVALID_MOVE; // INVALID_MOVE

		// Verify Pawn Promotion
		{
			int PromotionRow = m_PlayerTurn == COLOR::WHITE ? 7 : 0;

			if (m_GameBoard.MainGameBoard[destination.row][destination.col]->getFigureType() == FIGURE::PAWN
				&& destination.row == PromotionRow)
			{
				return m_PlayerTurn == COLOR::WHITE ? GAME_STATUS::WHITE_PROMOTION : GAME_STATUS::BLACK_PROMOTION;
				//m_GameBoard.MainGameBoard[destination.row][destination.col] = std::shared_ptr<ChessFigure>(new Queen(m_PlayerTurn));
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																				// Additional info
																	      // Print board to the console
		//for (int i = 0; i < 8; ++i)
		//{
		//	for (int j = 0; j < 8; ++j)
		//	{
		//		if (m_GameBoard.MainGameBoard[i][j] == nullptr)
		//			std::cout << FIGURE::NONE << "|";
		//		else
		//			std::cout << m_GameBoard.MainGameBoard[i][j]->getColor() << m_GameBoard.MainGameBoard[i][j]->getFigureType() <<
		//			"-" << m_GameBoard.MainGameBoard[i][j]->getMoveNumber() << "|";
		//	}
		//	std::cout << "\n";
		//}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Give turn to the next player
		this->changeTurn();

		// Verify that game is not over
		if (gameIsOver())
			return GAME_STATUS::GAME_OVER; // GAME_OVER

		return gameStatus;
	}

	GAME_STATUS promotion(std::string chess_note, FigureType figure)
	{
		BoardCoordinate destination = ChessNotationToBoardCoor(chess_note[2], chess_note[3]);

		if (figure == FigureType::WHITE_PAWN || figure == FigureType::BLACK_PAWN)
			m_GameBoard.MainGameBoard[destination.row][destination.col] = std::shared_ptr<ChessFigure>(new Pawn(m_PlayerTurn));
		else if (figure == FigureType::WHITE_QUEEN || figure == FigureType::BLACK_QUEEN)
			m_GameBoard.MainGameBoard[destination.row][destination.col] = std::shared_ptr<ChessFigure>(new Queen(m_PlayerTurn));
		else if (figure == FigureType::WHITE_KNIGHT || figure == FigureType::BLACK_KNIGHT)
			m_GameBoard.MainGameBoard[destination.row][destination.col] = std::shared_ptr<ChessFigure>(new Knight(m_PlayerTurn));
		else if (figure == FigureType::WHITE_ROOK || figure == FigureType::BLACK_ROOK)
			m_GameBoard.MainGameBoard[destination.row][destination.col] = std::shared_ptr<ChessFigure>(new Rook(m_PlayerTurn));
		else if (figure == FigureType::WHITE_BISHOP || figure == FigureType::BLACK_BISHOP)
			m_GameBoard.MainGameBoard[destination.row][destination.col] = std::shared_ptr<ChessFigure>(new Bishop(m_PlayerTurn));

		// Give turn to the next player
		this->changeTurn();

		// Verify that game is not over
		if (gameIsOver())
			return GAME_STATUS::GAME_OVER; // GAME_OVER

		return GAME_STATUS::GOOD_MOVE;
	}

private:
	COLOR		m_PlayerTurn;
	GameBoard	m_GameBoard;


	bool CanMove(COLOR player)
	{
		for (int source_row = 0; source_row < 8; ++source_row)
		{
			for (int source_col = 0; source_col < 8; ++source_col)
			{
				if (m_GameBoard.MainGameBoard[source_row][source_col] != nullptr)
				{
					if (m_GameBoard.MainGameBoard[source_row][source_col]->getColor() == player)
					{
						for (int destination_row = 0; destination_row < 8; ++destination_row)
						{
							for (int destination_col = 0; destination_col < 8; ++destination_col)
							{
								if (m_GameBoard.MainGameBoard[source_row][source_col]->IsItPossibleToMove({ source_row, source_col }, { destination_row, destination_col }, m_GameBoard.MainGameBoard, true))
								{
									auto SourceCell = m_GameBoard.MainGameBoard[source_row][source_col];
									auto DestinationCell = m_GameBoard.MainGameBoard[destination_row][destination_col];

									// Make Move
									m_GameBoard.MainGameBoard[destination_row][destination_col] = SourceCell;
									m_GameBoard.MainGameBoard[source_row][source_col] = nullptr;

									// Verify that King is not in Check after move
									bool IsCheck = this->Check(player);

									// Undo previous move
									m_GameBoard.MainGameBoard[destination_row][destination_col] = DestinationCell;
									m_GameBoard.MainGameBoard[source_row][source_col] = SourceCell;

									if (!IsCheck)
										return true;
								}
							}
						}
					}
				}
			}
		}
		return false;
	}

	bool Check(COLOR player)
	{
		// Firstly, find player's King
		BoardCoordinate KingCoordinate;
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if (m_GameBoard.MainGameBoard[i][j] != nullptr)

					if (m_GameBoard.MainGameBoard[i][j]->getColor() == player)
						if (m_GameBoard.MainGameBoard[i][j]->getFigureType() == FIGURE::KING)
						{
							KingCoordinate.row = i; KingCoordinate.col = j;
						}
			}
		}

		COLOR enemy = player == COLOR::BLACK ? COLOR::WHITE : COLOR::BLACK;

		// Verify that any enemy's figures can not take the player's king
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if (m_GameBoard.MainGameBoard[i][j] != nullptr)
					if (m_GameBoard.MainGameBoard[i][j]->getColor() == enemy)
					{
						if (m_GameBoard.MainGameBoard[i][j]->IsItPossibleToMove({ i, j }, KingCoordinate, m_GameBoard.MainGameBoard))
							return true;
					}
			}
		}

		return false;
	}

	bool gameIsOver()
	{
		// Verify that the current player can move
		bool canMove = this->CanMove(m_PlayerTurn);

		if (!canMove)
		{
			if (this->Check(m_PlayerTurn))
			{
				COLOR enemy = m_PlayerTurn == COLOR::BLACK ? COLOR::WHITE : COLOR::BLACK;
				std::cout << "Checkmate! " << enemy << " player wins\n";
			}
			else
				std::cout << "Stalemate for " << m_PlayerTurn << " player\n";

			return true;
		}

		return false;
	}
};

void Game::changeTurn()
{
	m_PlayerTurn = m_PlayerTurn == COLOR::WHITE ? COLOR::BLACK : COLOR::WHITE;
}

#endif // !GAME_H
