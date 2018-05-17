#include "Data.h"
#include "Graphic.h"
void enterCustomLevel(sf::Event custom, sf::RenderWindow *window, int mouseX, int mouseY, sf::String playerString[], sf::Text playerInput[])
{
	switch (custom.type) {
	case sf::Event::Closed:
		window->close();
	case sf::Event::TextEntered:

		if (mouseY == 0 && mouseX >= 0 && mouseX <= width - 1)
		{
			setText(&playerString[1], custom);
			playerInput[1].setString(playerString[1]);
			playerInput[1].setPosition(window->getDefaultView().getSize().x / 2 - playerString[1].getSize() * 10, gameBorderTop + squareSize * 4 - 16);
			std::stringstream convert(playerString[1]);
			convert >> customw;

		}
		if (mouseY == 2 && mouseX >= 0 && mouseX <= width - 1) {
			setText(&playerString[2], custom);
			playerInput[2].setString(playerString[2]);
			playerInput[2].setPosition(window->getDefaultView().getSize().x / 2 - playerString[2].getSize() * 10, gameBorderTop + squareSize * 6 - 16);
			std::stringstream convert(playerString[2]);
			convert >> customh;

		}
		if (mouseY == 4 && mouseX >= 0 && mouseX <= width - 1) {
			setText(&playerString[3], custom);
			playerInput[3].setString(playerString[3]);
			playerInput[3].setPosition(window->getDefaultView().getSize().x / 2 - playerString[3].getSize() * 10, gameBorderTop + squareSize * 8 - 16);
			std::stringstream convert(playerString[3]);
			convert >> customm;

		}
		if (customw > 36 || customw < 5) playerInput[1].setColor(sf::Color::Red);
		else playerInput[1].setColor(sf::Color::Blue);
		if (customh > 18 || customh < 5) playerInput[2].setColor(sf::Color::Red);
		else playerInput[2].setColor(sf::Color::Blue);
		if (customm > 320 || customm < 5) playerInput[3].setColor(sf::Color::Red);
		else playerInput[3].setColor(sf::Color::Blue);
		break;
	case sf::Event::MouseButtonPressed:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			mouseButton = 1;
		break;
	case sf::Event::MouseButtonReleased:
		if (mouseButton) {
			mouseButton = 0;
			if (mouseY == 5 && mouseX >= 0 && mouseX <= 4) {
				if (customw != 0 && customh != 0 && customm != 0) {
					if (customw < 5 || customw > 36)
						width = 5;
					else
						width = customw;
					if (customh < 5 || customh > 18)
						height = 5;
					else
						height = customh;
					if (customm < 5)
						mineNumber = 5;
					else if (customm >= width * height || customm > 320)
						mineNumber = width * height - 1;
					else
						mineNumber = customm;
					menuStage = 1;
					displaySize = true;

				}

			}
			if (mouseY == -gameBorderTop && mouseX == -gameBorderLeft) {
				displaySize = 1;
				menuStage = 0;
			}
			if (mouseY == -gameBorderTop && mouseX == width)
				window->close();
		}
		break;
	}
}
void mainGamePlay(sf::Event play, sf::RenderWindow *window, int mouseX, int mouseY, sf::Clock *clock)
{
	if (play.type == sf::Event::Closed)
		window->close();
	if ((gameOver != 1) && (unrevealed != mineNumber) && (mouseX >= 0) && (mouseX < width) && (mouseY >= 0) && (mouseY < height)) {
		switch (play.type) {
		case sf::Event::MouseButtonPressed:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				mouseButton = 1;
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				mouseButton = 2;
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
				mouseButton = 3;
			break;
		case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				mouseButton = 2;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				mouseButton = 3;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
				mouseButton = 4;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
				if (gameOver == 1 || unrevealed == mineNumber) {
				}
				reset();
			}
			break;
		case sf::Event::KeyReleased:
			if (mouseButton == 2) {
				if (boardRevealed[mouseY * width + mouseX] == 2) {
					minesLeft++;

					if (markFlag)
						boardRevealed[mouseY * width + mouseX] = 3;
					else
						boardRevealed[mouseY * width + mouseX] = 0;
				}
				else if (boardRevealed[mouseY * width + mouseX] == 3) {

					boardRevealed[mouseY * width + mouseX] = 0;
				}
				else if (boardRevealed[mouseY * width + mouseX] == 0 && minesLeft > -9) {
					minesLeft--;
					boardRevealed[mouseY * width + mouseX] = 2;

				}
				mouseButton = 0;
			}
			else if (mouseButton == 3) {
				if (boardRevealed[mouseY * width + mouseX] == 1) {
					if (showSurroundings(mouseX, mouseY)) {

					}
				}
				mouseButton = 0;
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (mouseButton == 1)
			{
				if (firstClick)
				{
					// generate the map after the first click every time //
					firstClick = false;
					clock->restart();

					// mark the position as a safe point //
					board[mouseY * width + mouseX] = -2;

					placeMines(mouseY * width + mouseX);

					// place the numbers //
					for (int i = 0; i < squareNumber; i++)
						countSurrounding(i % width, i / width);

				}
				if (boardRevealed[mouseY * width + mouseX] == 0 || boardRevealed[mouseY * width + mouseX] == 3) {


					if (reveal(mouseX, mouseY))
						board[mouseY * width + mouseX] = -1;
				}
				mouseButton = 0;
			}
			else if (mouseButton == 2) {
				if (boardRevealed[mouseY * width + mouseX] == 2) {
					minesLeft++;

					if (markFlag)
						boardRevealed[mouseY * width + mouseX] = 3;
					else
						boardRevealed[mouseY * width + mouseX] = 0;
				}
				else if (boardRevealed[mouseY * width + mouseX] == 3) {

					boardRevealed[mouseY * width + mouseX] = 0;
				}
				else if (boardRevealed[mouseY * width + mouseX] == 0 && minesLeft > -9) {
					minesLeft--;
					boardRevealed[mouseY * width + mouseX] = 2;

				}
				mouseButton = 0;
			}
			else if (mouseButton == 3) {
				if (boardRevealed[mouseY * width + mouseX] == 1)
				if (showSurroundings(mouseX, mouseY)) {

				}
				mouseButton = 0;
			}
			break;
		default:
			break;
		}
	}
	else {
		switch (play.type) {
		case sf::Event::Closed:
			window->close();
		case sf::Event::MouseButtonPressed:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				mouseButton = 1;
		case sf::Event::MouseButtonReleased:
			if (mouseButton == 1) {
				if (mouseY == -(gameBorderTop - 2) && mouseX <= width - gameBorderRight && mouseX >= 0) {
					if (width < 9) {
						if (mouseX >= width - gameBorderRight - 1) {
							mouseButton = 0;
							if (showMoves)
								showMoves = false;
							else
								showMoves = true;
						}
						if (mouseX <= 1) {
							mouseButton = 0;

						}
					}
					else if (mouseX <= 2) {
						mouseButton = 0;

					}
					else if (mouseX >= width - gameBorderRight - 2) {
						mouseButton = 0;
						if (showMoves)
							showMoves = false;
						else
							showMoves = true;
					}
				}
				if (mouseY == -gameBorderTop && mouseX == -gameBorderLeft) {
					mouseButton = 0;
					menuStage = 0;
					displaySize = 1;

					reset();
				}
				if (mouseY == -gameBorderTop && mouseX == width) {
					mouseButton = 0;
					window->close();
				}
			}
			break;
		case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
				if (gameOver == 1 || unrevealed == mineNumber) {

				}
				reset();
			}
			break;
		default:
			break;
		}
	}
}
int nearMines(int x, int y) {
	if (x >= 0 && x < width && y >= 0 && y <= height) {
		if (board[y * width + x] == 9) return 1;
		else return 0;
	}
	else return 0;
}

void countSurrounding(int x, int y) {
	if (board[y * width + x] == 9) return;
	unsigned int result = 0;
	if (nearMines(x + 1, y)) result++;
	if (nearMines(x - 1, y)) result++;
	if (nearMines(x, y - 1)) result++;
	if (nearMines(x, y + 1)) result++;
	if (nearMines(x + 1, y + 1)) result++;
	if (nearMines(x + 1, y - 1)) result++;
	if (nearMines(x - 1, y + 1)) result++;
	if (nearMines(x - 1, y - 1)) result++;
	board[y * width + x] = result;
}

void placeMines(int firstClick_poz) {
	int count = 0;
	while (count < mineNumber) {
		int randPosition = rand() % squareNumber;
		if (board[randPosition] != 9 && board[randPosition] != -2) {
			int temp = rand() % 2;
			if (temp == 1)
			{
				board[randPosition] = 9;
				++count;
			}
		}
	}
	if (board[firstClick_poz] == -2)
		board[firstClick_poz] = 0;
}

void reset() {
	for (int i = 0; i < squareNumber; i++) {
		board[i] = 0;
		boardRevealed[i] = 0;
	}
	gameOver = false;
	firstClick = true;
	won = false;
	minesLeft = mineNumber;
	unrevealed = squareNumber;
	gameTime = 0;
	playerMoves = 0;
	showMoves = 0;
}

float lerp(float a, float b, float c) {
	return a + c * (b - a);
}
void selectLevel(int mouseX, int mouseY, int &width, int &height, int &mineNumber, bool &displaySize, int &menuStage)
{
	if (mouseY == -1 && mouseX >= 0 && mouseX <= width - 1) {	//BEGINNER MODE
		width = 9;
		height = 9;
		mineNumber = 10;
		displaySize = true;
		menuStage = 1;

	}
	if (mouseY == 1 && mouseX >= 0 && mouseX <= width - 1) {	//INTERMEDIATE MODE
		width = 16;
		height = 16;
		mineNumber = 40;
		displaySize = true;
		menuStage = 1;

	}
	if (mouseY == 3 && mouseX >= 0 && mouseX <= width - 1) {	//EXPERT MODE
		width = 30;
		height = 16;
		mineNumber = 99;
		displaySize = true;
		menuStage = 1;

	}
	if (mouseY == 5 && mouseX >= 0 && mouseX <= width - 1) {	//CUSTOM MODE
		menuStage = 4;
		displaySize = true;

	}
	if (mouseY == -gameBorderTop && mouseX == -gameBorderLeft) { //MAIN MENU 
		menuStage = 0;
		displaySize = 1;
	}
}
