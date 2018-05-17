#include "Data.h"
#include "Graphic.h"


int main() 
{
	// CREATE THE WINDOW
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(400, 600), "Minesweeper!", sf::Style::Resize);
	sf::Image icon;
	icon.loadFromFile("images/icon.png");

	//LOAD TEXT
	sf::Font font;
	font.loadFromFile("Bariol.otf");
	sf::Text text;
	text.setFont(font);
	text.setColor(sf::Color::White);
	text.setPosition(30, 210);

	//LOAD THE IMAGES
	loadTexture();
	sf::Sprite spriteBlocks(textureBlocks), spriteBackground(textureBackground), f(faces),
		spriteLogo(logo), optionbuttonsmenu(optionbuttons), spriteNewGameButton(textureNewGameButton),
		spriteGuideButton(textureGuideButton), spriteExitButton(textureExitButton),
		spriteWin(textureWin), spriteLose(textureLose), spriteTextbox(textureTextbox),
		spriteBackButton(textureBackButton), spriteNextButton(textureNextButton);

	firstDraw(&spriteLogo, &spriteTextbox, &spriteBackButton, &spriteNextButton, &spriteNewGameButton, &spriteGuideButton, &spriteExitButton);


	//THE TIME-COUNTERS
	srand(time(0));
	sf::Clock clock, clock2;
	sf::Time timer, timer2;

	// INITIALIZE THE GAME TEXT 
	sf::String playerString[4];
	sf::Text scoreText, gameTimer, playerMovesText, playerInput[4];

	initText(&scoreText, sf::Color::Red, &font);
	initText(&gameTimer, sf::Color::Red, &font);
	initText(&playerMovesText, sf::Color::Green, &font);

	// CUSTOM GAME PLAYER INPUT 
	for (int i = 1; i <= 3; i++)
	{
		initText(&playerInput[i], sf::Color::Green, &font);
	}

	// MAIN LOOP
	while (window.isOpen()) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		float x = pos.x;
		float y = pos.y;
		int mouseX = pos.x / squareSize - gameBorderLeft;
		int mouseY = pos.y / squareSize - gameBorderTop;

		switch (menuStage) {
		case 0:
			text.setString("\tThe objective in Minesweeper : \nFind and mark all the mines hidden under the \ngrey squares, in the shortest time possible by \nclicking on the squares to open them.\nEach square will have one of the following:\n\n+ 1 . A mine, click on it and you lose the game.\n+ 2 . A number, which tells you how many of\nits adjacent squares have mines in them.\n+ 3 . Nothing.In this case you know that \nnone of the adjacent squares have mines.");
			text.setCharacterSize(18); 
			sf::Event main_menu;
			if (window.isOpen())
			{
				if (displaySize) {
					width = 5;
					height = 5;
					displaySize = 0;
					window.create(sf::VideoMode(400, 600), "Minesweeper!");
					window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
				}

				// DRAW THE IMAGE
				window.draw(spriteBackground);
				window.draw(spriteNewGameButton);
				window.draw(spriteGuideButton);
				window.draw(spriteExitButton);
				timer2 = clock2.restart();
				logoAnimation += timer2.asSeconds();
				spriteLogo.setScale(lerp(0.8f, 0.81f, abs(cos(logoAnimation))), lerp(1.3f, 1.31f, abs(cos(logoAnimation))));
				window.draw(spriteLogo);
			}

			//SELECT BUTTON
			while (window.pollEvent(main_menu)) {
				switch (main_menu.type) {
				case sf::Event::Closed:
					window.close();
				case sf::Event::MouseButtonPressed:
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						mouseButton = 1;
				case sf::Event::MouseButtonReleased:
					if (mouseButton) {
						mouseButton = 0;
						if (spriteNewGameButton.getGlobalBounds().contains(x, y))
						{
							menuStage = 3;
							displaySize = true;
						}
						else if (spriteGuideButton.getGlobalBounds().contains(x, y))
						{
							menuStage = 2;
							displaySize = true;
						}
						else if (spriteExitButton.getGlobalBounds().contains(x, y))
							window.close();
					}
					break;
				default:
					break;
				}
			}
			window.display();
			break;

		case 1:
			sf::Event play;
			if (window.isOpen())
			{
				if (displaySize)
				{
					minesLeft = mineNumber;
					squareNumber = width * height;
					unrevealed = squareNumber;
					window.create(sf::VideoMode((width + gameBorderRight + gameBorderLeft) * squareSize, (height + gameBorderTop + gameBorderBottom) * squareSize, desktop.bitsPerPixel), "Minesweeper!");
					window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
					displaySize = 0;
				}

				// DRAW THE BLOCKS
				spriteBackground.setScale(5.f, 2.f);
				window.draw(spriteBackground);
				drawMainMenu(&window, &optionbuttonsmenu, &spriteBlocks, mouseX, mouseY, &scoreText, &gameTimer, &playerMovesText);
				std::ostringstream flagleft, magic_left;
				flagleft << minesLeft;
				scoreText.setString(flagleft.str());
				// GAME TIMER UPDATE 
				if (firstClick == false && gameOver != 1 && unrevealed != mineNumber) {
					timer = clock.getElapsedTime();
					if (timer.asSeconds() > gameTime)
					if ((width < 9 && gameTime < 99) || (width >= 9 && gameTime < 999))
						gameTime++;
				}

				std::ostringstream timespend, moves;
				moves << playerMoves;
				timespend << gameTime;
				gameTimer.setString(timespend.str());

				drawGamePlay(&window, &f, &spriteBlocks, mouseX, mouseY, pos);
				drawWinLose(&window, &f, &spriteWin, &spriteLose);
				window.draw(f);
				window.draw(scoreText);
				if (showMoves)
					window.draw(playerMovesText);
				else window.draw(gameTimer);;
			}
			while (window.pollEvent(play))
			{
				mainGamePlay(play, &window, mouseX, mouseY, &clock);
			}
			window.display();
			break;
			//END CASE 1 - GAME PLAY

		case 2: //GUIDE GAME
			sf::Event guide;
			if (page == 2)
				page = 0;
			if (window.isOpen())
			{
				window.draw(spriteTextbox);
				window.draw(text);
				window.draw(spriteBackButton);
				window.draw(spriteNextButton);
			}
			while (window.pollEvent(guide))
			{
				drawGuideGame(guide, &window, spriteBackButton, spriteNextButton, &text, x, y);
			}
			window.display();
			break;


		case 3: //SELECT LEVEL
			sf::Event playmode;
			if (window.isOpen())
			{
				if (displaySize) {
					width = 6;
					height = 6;
					window.create(sf::VideoMode((width + gameBorderRight + gameBorderLeft) * squareSize, (height + gameBorderTop + gameBorderBottom) * squareSize, desktop.bitsPerPixel), "Minesweeper!");
					window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
					displaySize = 0;
				}
				window.draw(spriteBackground);
				drawSelectLevel(&window, &optionbuttonsmenu, mouseX, mouseY);
			}
			while (window.pollEvent(playmode)) {
				switch (playmode.type) {
				case sf::Event::Closed:
					window.close();
				case sf::Event::MouseButtonPressed:
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						mouseButton = 1;
					break;
				case sf::Event::MouseButtonReleased:
					if (mouseButton) {
						mouseButton = 0;
						selectLevel(mouseX, mouseY, width, height, mineNumber, displaySize, menuStage);
						if (mouseY == -gameBorderTop && mouseX == width)	//EXIT
							window.close();
					}
					break;
				}
			}
			window.display();
			break;

		case 4:	//CUSTOM FIELD
			sf::Event custom;
			if (window.isOpen()) {
				if (displaySize) {
					customw = 0;
					customh = 0;
					customm = 0;
					for (int i = 1; i < 4; i++) {
						playerString[i].clear();
						playerInput[i].setString(playerString[i]);
					}
					window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
					displaySize = 0;
				}

				// Draw sprite
				window.draw(spriteBackground);
				drawCustomLevel(&window, &optionbuttonsmenu, &spriteBlocks, mouseX, mouseY);
				for (int i = 1; i < 4; i++)
					window.draw(playerInput[i]);
			}
			while (window.pollEvent(custom))
			{
				enterCustomLevel(custom, &window, mouseX, mouseY, playerString, playerInput);
			}
			window.display();
			break;


		default:
			window.close();
			break;
		}
	}
	return 0;
}

// GLOBAL VARIABLES
float logoAnimation;
int width = 5, height = 5, gameTime = 0, menuStage = 0, playerMoves = 0;
int squareSize = 32, gameBorderTop = 4, gameBorderRight = 1, gameBorderBottom = 1, gameBorderLeft = 1, mouseButton = 0, page = 0;
int squareNumber, mineNumber, unrevealed, flagNum, minesLeft, faceSize, customw, customh, customm;
int board[MAX * MAX] = { 0 };
int boardRevealed[MAX * MAX] = { 0 };
bool firstClick = true, gameOver = false, won = false, showMoves = false, markFlag = true, displaySize = true;
sf::Texture textureBlocks, textureBackground, textureWin, textureLose, faces, logo, optionbuttons,
			textureNewGameButton, textureGuideButton, textureExitButton, textureBackButton,
			textureNextButton, textureTextbox;