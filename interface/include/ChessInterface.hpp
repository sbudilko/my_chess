#ifndef CHESINTERFACE_H
#define CHESINTERFACE_H

#include <SFML/Graphics.hpp>

#include <string>
#include <array>
#include <iostream>

#include "InterfaceUtils.hpp"
#include "Game.hpp"


class FiguresData
{
public:
	void setTexture(std::string PathToFile)
	{
		bool flag = FigureTexture.loadFromFile(PathToFile);
	}
	void setSprite()
	{
		FigureSprite.setTexture(FigureTexture);
	}
	sf::Texture FigureTexture;
	sf::Sprite FigureSprite;
};


class ChessInterface
{
public:
	void startMainGameLoop();
private:

	void loadMainGameWindow();
	void loadAllFigures();
	void EatEnemyFigure(std::string chess_notation);
	void moveRook(std::string chess_notation);
	FigureType chooseFigureForPromotion(COLOR color);

	sf::RenderWindow m_MainGameWindow;
	sf::Texture m_MainGameWindowTexture;
	sf::Sprite m_MainGameWindowSprite;

	std::array<FiguresData, 32> m_AllFiguresSprite;

	// Game engine
	Game m_GameEngine;

};

void ChessInterface::startMainGameLoop()
{
	// Create main window (game board)
	this->loadMainGameWindow();

	// Load figures on game board
	this->loadAllFigures();

	// Additional variables
	bool clickOnFigure = false;
	bool isMove = false;	// Accessing for moving a figure
	int figureNumber = 0;   // Selected figure number
	int dx = 0, dy = 0;
	sf::Vector2f OldFigurePosition;

	// Start Main Game's Loop
	// The main loop ends as soon as the window is closed
	while (m_MainGameWindow.isOpen())
	{

		// Get the current position of the mouse in desktop coordinates
		sf::Vector2i currenMousePosition = sf::Mouse::getPosition(m_MainGameWindow);

		// Event processing
		sf::Event event;
		while (m_MainGameWindow.pollEvent(event))
		{
			// Request for closing window
			if (event.type == sf::Event::Closed)
				m_MainGameWindow.close();

			// Request for moving figure
			if (event.type == sf::Event::MouseButtonPressed)
				if (event.key.code == sf::Mouse::Left)
				{
					for (int i = 0; i < 32; ++i)
						if (m_AllFiguresSprite[i].FigureSprite.getGlobalBounds().contains(currenMousePosition.x, currenMousePosition.y))
						{
							clickOnFigure = true;
							isMove = true;
							figureNumber = i;
							dx = m_AllFiguresSprite[figureNumber].FigureSprite.getPosition().x - currenMousePosition.x;
							dy = m_AllFiguresSprite[figureNumber].FigureSprite.getPosition().y - currenMousePosition.y;
							OldFigurePosition = m_AllFiguresSprite[figureNumber].FigureSprite.getPosition();
						}
				}


			// Request for apply moving or cancellattion if move is invalid
			if (event.type == sf::Event::MouseButtonReleased)
				if (event.key.code == sf::Mouse::Left && clickOnFigure)
				{
					clickOnFigure = false;
					isMove = false;
					sf::Vector2f CurrentFigurePosition = m_AllFiguresSprite[figureNumber].FigureSprite.getPosition() + sf::Vector2f(gCellSize / 2, gCellSize / 2);
					sf::Vector2f newPos = sf::Vector2f(gCellSize * static_cast<int>(CurrentFigurePosition.x / gCellSize),
						gCellSize* static_cast<int>(CurrentFigurePosition.y / gCellSize));

					// Convert window coordinates to chess notation
					std::string chess_notation = CoordinatesToChessNotation(OldFigurePosition) + CoordinatesToChessNotation(newPos);

					GAME_STATUS gameStatus;

					if (chess_notation == "e1g1" || chess_notation == "e8g8" ||
						chess_notation == "e1c1" || chess_notation == "e8c8")
					{
						gameStatus = m_GameEngine.checkCastling(chess_notation);
					}
					else
					{
						// Using game engine to verify correctness of the moving
						gameStatus = m_GameEngine.nextMove(chess_notation);
					}
					if (gameStatus == GAME_STATUS::GOOD_MOVE || gameStatus == GAME_STATUS::WHITE_PROMOTION ||
						gameStatus == GAME_STATUS::BLACK_PROMOTION)
					{
						// Moving is correct
						std::cout << "Move: " << chess_notation << std::endl;
						this->EatEnemyFigure(chess_notation);
						m_AllFiguresSprite[figureNumber].FigureSprite.setPosition(newPos);

						if (gameStatus == GAME_STATUS::WHITE_PROMOTION)
						{
							FigureType figure = chooseFigureForPromotion(COLOR::WHITE);
							gameStatus = m_GameEngine.promotion(chess_notation, figure);
							m_AllFiguresSprite[figureNumber].setTexture(gTextureFiles[figure]);
						}
						else if (gameStatus == GAME_STATUS::BLACK_PROMOTION)
						{
							FigureType figure = chooseFigureForPromotion(COLOR::BLACK);
							gameStatus = m_GameEngine.promotion(chess_notation, figure);
							m_AllFiguresSprite[figureNumber].setTexture(gTextureFiles[figure]);
						}
						if (gameStatus == GAME_STATUS::GAME_OVER)
							m_MainGameWindow.close();
					}
					else if (gameStatus == GAME_STATUS::CASTLING)
					{
						// Castling is correct
						std::cout << "Castling: " << chess_notation << std::endl;
						// Move Rook
						moveRook(chess_notation);
						// Move King
						m_AllFiguresSprite[figureNumber].FigureSprite.setPosition(newPos);
					}
					else if (gameStatus == GAME_STATUS::GAME_OVER)
						m_MainGameWindow.close();
					else
					{
						// Moving is not correct
						m_AllFiguresSprite[figureNumber].FigureSprite.setPosition(OldFigurePosition);
						if (gameStatus == GAME_STATUS::TRY_TO_MOVE_ENEMY_FIGURE)
							std::cout << "You are trying to move enemy figure\n";
						else if (gameStatus == GAME_STATUS::INVALID_MOVE)
							std::cout << "Invalid move!\n";
						else if (gameStatus == GAME_STATUS::BAD_CASTLING)
							std::cout << "Invalid castling\n";
						else if (gameStatus == GAME_STATUS::KING_IN_CHECK_AFTER_CASTLING)
							std::cout << ": King in Check after castling\n.";
					}
				}
		}

		if (isMove)
			m_AllFiguresSprite[figureNumber].FigureSprite.setPosition(currenMousePosition.x + dx, currenMousePosition.y + dy);

		// Clear the whole window before rendering a new frame
		m_MainGameWindow.clear();

		// Draw some graphical entities
		m_MainGameWindow.draw(m_MainGameWindowSprite);
		for (int i = 0; i < 32; ++i)
		{
			sf::Vector2f coor;
			coor = m_AllFiguresSprite[i].FigureSprite.getPosition();
			m_AllFiguresSprite[i].FigureSprite.setPosition(coor + gOffset);
		}
		for (int i = 0; i < 32; ++i) m_MainGameWindow.draw(m_AllFiguresSprite[i].FigureSprite);
		for (int i = 0; i < 32; ++i)
		{
			sf::Vector2f coor;
			coor = m_AllFiguresSprite[i].FigureSprite.getPosition();
			m_AllFiguresSprite[i].FigureSprite.setPosition(coor - gOffset);
		}

		// End the current frame and display its contents on screen
		m_MainGameWindow.display();
	}
}

void ChessInterface::loadMainGameWindow()
{
	// create a new render-window
	m_MainGameWindow.create(sf::VideoMode(640, 640), "My Chess");

	// Load main window's texture
	m_MainGameWindowTexture.loadFromFile("images/board_640x640.png");

	// Create main window's sprite
	m_MainGameWindowSprite.setTexture(m_MainGameWindowTexture);
}

void ChessInterface::loadAllFigures()
{
	int FigureNumber = 0;
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (gAllFigures[i][j] == FigureType::NONE)
				continue;
			m_AllFiguresSprite[FigureNumber].setTexture(gTextureFiles[gAllFigures[i][j]]);
			m_AllFiguresSprite[FigureNumber].setSprite();
			m_AllFiguresSprite[FigureNumber].FigureSprite.setPosition(gCellSize * j, gCellSize * i);
			++FigureNumber;
		}
	}
}

void ChessInterface::EatEnemyFigure(std::string chess_notation)
{
	sf::Vector2f oldPosition = ChessNotationToCoordinates(chess_notation[0], chess_notation[1]);
	sf::Vector2f newPosition = ChessNotationToCoordinates(chess_notation[2], chess_notation[3]);


	// Move eaten figure outside the render window
	for (int i = 0; i < 32; ++i)
		if (m_AllFiguresSprite[i].FigureSprite.getPosition() == newPosition) m_AllFiguresSprite[i].FigureSprite.setPosition(-100, 100);

	// Put new figure instead eaten one
	for (int i = 0; i < 32; ++i)
		if (m_AllFiguresSprite[i].FigureSprite.getPosition() == oldPosition) m_AllFiguresSprite[i].FigureSprite.setPosition(newPosition);
}

void ChessInterface::moveRook(std::string chess_notation)
{
	if (chess_notation == "e1g1")	moveRook("h1f1");
	if (chess_notation == "e8g8")	moveRook("h8f8");
	if (chess_notation == "e1c1")	moveRook("a1d1");
	if (chess_notation == "e8c8")	moveRook("a8d8");

	sf::Vector2f oldPosition = ChessNotationToCoordinates(chess_notation[0], chess_notation[1]);
	sf::Vector2f newPosition = ChessNotationToCoordinates(chess_notation[2], chess_notation[3]);


	// Put new figure instead eaten one
	for (int i = 0; i < 32; ++i)
		if (m_AllFiguresSprite[i].FigureSprite.getPosition() == oldPosition) m_AllFiguresSprite[i].FigureSprite.setPosition(newPosition);
}

FigureType ChessInterface::chooseFigureForPromotion(COLOR color)
{
	int figure_number;

	sf::RenderWindow WindowWithFigures;
	sf::Texture WindowWithFiguresTexture;
	sf::Sprite WindowWithFiguresSprite;

	// create new window for choose figures
	WindowWithFigures.create(sf::VideoMode(400, 80), "Choose your figure!");

	// Load texture
	std::string TextureFile = color == COLOR::WHITE ? "images/white_fgiures.png" :
		"images/black_fgiures.png";
	WindowWithFiguresTexture.loadFromFile(TextureFile);

	// Create Sprite
	WindowWithFiguresSprite.setTexture(WindowWithFiguresTexture);

	while (WindowWithFigures.isOpen())
	{
		// Get the current position of the mouse in desktop coordinates
		sf::Vector2i currenMousePosition = sf::Mouse::getPosition(WindowWithFigures);

		// Event processing
		sf::Event event;
		while (WindowWithFigures.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed)
				if (event.key.code == sf::Mouse::Left)
				{
					figure_number = currenMousePosition.x / gCellSize + 1;
					WindowWithFigures.close();
				}
		}

		// Draw some graphical entities
		WindowWithFigures.clear();
		WindowWithFigures.draw(WindowWithFiguresSprite);
		WindowWithFigures.display();
	}

	int i = color == COLOR::WHITE ? 0 : 5;
	return gFigureNumberToFigureType[figure_number + i];
}


#endif // !CHESINTERFACE_H