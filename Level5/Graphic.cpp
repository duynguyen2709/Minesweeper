#include "Graphic.h"
#include "Data.h"

void firstDraw(sf::Sprite *spriteLogo, sf::Sprite *spriteTextbox, sf::Sprite *spriteBackButton, sf::Sprite *spriteNextButton, sf::Sprite *spriteNewGameButton, sf::Sprite *spriteGuideButton, sf::Sprite *spriteExitButton)
{
	spriteLogo->setPosition(10, 50);
	spriteLogo->setScale(0.8f, 1.3f);

	spriteTextbox->setPosition(20, 200);
	spriteTextbox->setScale(1.05f, 1.05f);

	spriteBackButton->setPosition(spriteTextbox->getGlobalBounds().left + 10, spriteTextbox->getGlobalBounds().top + spriteTextbox->getGlobalBounds().height - 65);
	spriteNextButton->setPosition(spriteTextbox->getGlobalBounds().left + spriteTextbox->getGlobalBounds().width - 65, spriteTextbox->getGlobalBounds().top + spriteTextbox->getGlobalBounds().height - 65);

	//BUTTONS
	spriteNewGameButton->setPosition(70, 210);
	spriteNewGameButton->setScale(1.1f, 1.1f);
	spriteGuideButton->setPosition(spriteNewGameButton->getGlobalBounds().left, spriteNewGameButton->getGlobalBounds().top + spriteNewGameButton->getGlobalBounds().height + 60);
	spriteGuideButton->setScale(spriteNewGameButton->getScale());
	spriteExitButton->setPosition(spriteGuideButton->getGlobalBounds().left, spriteGuideButton->getGlobalBounds().top + spriteGuideButton->getGlobalBounds().height + 60);
	spriteExitButton->setScale(spriteNewGameButton->getScale());
}

void loadTexture()
{
	textureBlocks.loadFromFile("images/tiles.png");
	faces.loadFromFile("images/faces.png");
	logo.loadFromFile("images/logo.png");
	optionbuttons.loadFromFile("images/optionbuttons.png");
	textureBackground.loadFromFile("images/background.jpg");
	textureNewGameButton.loadFromFile("images/newGameButton.png");
	textureGuideButton.loadFromFile("images/guideButton.png");
	textureExitButton.loadFromFile("images/exitButton.png");
	textureWin.loadFromFile("images/win.png");
	textureLose.loadFromFile("images/boom.png");
	textureBackButton.loadFromFile("images/backButton.png");
	textureTextbox.loadFromFile("images/textbox.png");
	textureNextButton.loadFromFile("images/nextButton.png");
}

void setText(sf::String *str, sf::Event custom)
{
	if (custom.text.unicode == '\b')
	{
		if (str->getSize()>0)
			str->erase(str->getSize() - 1, 1);
		else
			str->erase(0, 0);
	}
	else if (custom.text.unicode < 128)
		*str += custom.text.unicode;
}

void drawSelectLevel(sf::RenderWindow *window, sf::Sprite *optionbuttonsmenu, int mouseX, int mouseY)
{
	optionbuttonsmenu->setScale(1.08f, 1.08f);
	// Main Menu Back Game Button //
	if (mouseY == -gameBorderTop && mouseX == -gameBorderLeft)
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4 + 32, 0, squareSize, squareSize));
	else
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4, 0, squareSize, squareSize));
	optionbuttonsmenu->setPosition(0, 0);
	window->draw(*optionbuttonsmenu);

	// Exit Game Button //
	if (mouseY == -gameBorderTop && mouseX == width)
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4 + 32 * 3, 0, squareSize, squareSize));
	else
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4 + 32 * 2, 0, squareSize, squareSize));
	optionbuttonsmenu->setPosition(window->getDefaultView().getSize().x - squareSize - 3, 0);
	window->draw(*optionbuttonsmenu);

	// Draw the choices //
	for (unsigned int i = 0; i <= 7; i++) {
		if (i == 0) {
			// Draw Title: Options //
			optionbuttonsmenu->setTextureRect(sf::IntRect(0, squareSize, 160, squareSize));
			optionbuttonsmenu->setPosition((gameBorderLeft + 0.15f) * squareSize, 5 + squareSize);
			optionbuttonsmenu->setScale(1 + (width * 3 * 0.01f), 1.05f);
			window->draw(*optionbuttonsmenu);
		}
		else {
			// Draw Game Mode: Begginer, Intermediate, Expert, Custom //
			optionbuttonsmenu->setTextureRect(sf::IntRect(160 * i, squareSize, 160, squareSize));
			optionbuttonsmenu->setPosition((gameBorderLeft + 0.15f) * squareSize, squareSize * (i + 2));
			optionbuttonsmenu->setScale(1 + (width * 3 * 0.01f), 1.05f);
			window->draw(*optionbuttonsmenu);
			i++;
		}
	}

	// Draw Game Mode: Begginer, Intermediate, Expert, Custom  Hover //
	if ((mouseY == -1 || mouseY == 1 || mouseY == 3 || mouseY == 5) && mouseX >= 0 && mouseX <= 4) {
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * (4 + mouseY - 1), squareSize, 160, squareSize));
		optionbuttonsmenu->setPosition((gameBorderLeft + 0.1f) * squareSize, squareSize * (4 + mouseY));
		window->draw(*optionbuttonsmenu);
	}
}

void drawGuideGame(sf::Event guide, sf::RenderWindow *window, sf::Sprite spriteBackButton, sf::Sprite spriteNextButton, sf::Text *text, int x, int y)
{
	switch (guide.type)
	{
	case sf::Event::Closed:
		window->close();
	case sf::Event::MouseButtonPressed:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			mouseButton = 1;
		break;
	case sf::Event::MouseButtonReleased:
		if (mouseButton) {
			mouseButton = 0;
			if (spriteBackButton.getGlobalBounds().contains(x, y))
			{
				displaySize = false;
				menuStage = 0;

			}
			if (spriteNextButton.getGlobalBounds().contains(x, y) && page == 1)
			{
				page = 2;
				text->setCharacterSize(18);
				text->setString("\tThe objective in Minesweeper : \nFind and mark all the mines hidden under the \ngrey squares, in the shortest time possible by \nclicking on the squares to open them.\nEach square will have one of the following:\n\n+ 1 . A mine, click on it and you lose the game.\n+ 2 . A number, which tells you how many of\nits adjacent squares have mines in them.\n+ 3 . Nothing.In this case you know that \nnone of the adjacent squares have mines.");
				window->draw(*text);
				if (spriteBackButton.getGlobalBounds().contains(x, y))
				{
					text->setString("\tThe objective in Minesweeper : \nFind and mark all the mines hidden under the \ngrey squares, in the shortest time possible by \nclicking on the squares to open them.\nEach square will have one of the following:\n\n+ 1 . A mine, click on it and you lose the game.\n+ 2 . A number, which tells you how many of\nits adjacent squares have mines in them.\n+ 3 . Nothing.In this case you know that \nnone of the adjacent squares have mines.");

				}
			}

			if (spriteNextButton.getGlobalBounds().contains(x, y) && page == 0)
			{
				page = 1;
				text->setCharacterSize(17);
				text->setString("\t\t\tGameplay\nThere are 4 actions in Minesweeper :\n+ 1. Open a square : Left clicking on a square.\n+ 2. Marking a square as a mine : Right clicking \non a square\n+ 3. Marking a square with a question mark: \nRight clicking twice on a square,or right clicking \nonce on a square already marked as a mine.\nQuestion marks are useful to mark squares \nyou're not absolutely sure are mines, but \nwant to make sure not accidentally open them.\n+ 4. Clear any marks : Again, right click on \nthe square.\nRight clicking cycle-states : \n - Bomb, Question Mark, Clear.\n ");
				if (spriteBackButton.getGlobalBounds().contains(x, y))
				{
					text->setCharacterSize(18);
					text->setString("\tThe objective in Minesweeper : \nFind and mark all the mines hidden under the \ngrey squares, in the shortest time possible by \nclicking on the squares to open them.\nEach square will have one of the following:\n\n+ 1 . A mine, click on it and you lose the game.\n+ 2 . A number, which tells you how many of\nits adjacent squares have mines in them.\n+ 3 . Nothing.In this case you know that \nnone of the adjacent squares have mines.");
					page = 0;
				}

				window->draw(*text);
			}

		}
	default:
		break;
	}
}

void drawCustomLevel(sf::RenderWindow *window, sf::Sprite *optionbuttonsmenu, sf::Sprite *spriteBlocks, int mouseX, int mouseY)
{
	// Main Menu Back Game Button //
	if (mouseY == -gameBorderTop && mouseX == -gameBorderLeft)
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4 + 32, 0, squareSize, squareSize));
	else
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4, 0, squareSize, squareSize));
	optionbuttonsmenu->setPosition(0, 0);
	optionbuttonsmenu->setScale(1.1f, 1.1f);
	window->draw(*optionbuttonsmenu);

	// Exit Game Button //
	if (mouseY == -gameBorderTop && mouseX == width)
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4 + 32 * 3, 0, squareSize, squareSize));
	else
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4 + 32 * 2, 0, squareSize, squareSize));
	optionbuttonsmenu->setPosition(window->getDefaultView().getSize().x - squareSize, 0);
	window->draw(*optionbuttonsmenu);

	// Draw the choices //
	for (unsigned int i = 0; i <= 7; i++)
	if (i == 0) {
		// Draw Title: Options //
		optionbuttonsmenu->setTextureRect(sf::IntRect(0, squareSize * 2, 160, squareSize));
		optionbuttonsmenu->setPosition(gameBorderLeft * squareSize + 2, squareSize + 10);
		optionbuttonsmenu->setScale(1.2f, 1.05f);
		window->draw(*optionbuttonsmenu);
	}
	else {
		// Draw Game Mode: Begginer, Intermediate, Expert, Custom //
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * i, squareSize * 2, 160, squareSize));
		optionbuttonsmenu->setPosition(gameBorderLeft * squareSize, squareSize * (i + 2));
		optionbuttonsmenu->setScale(1.2f, 1.f);
		window->draw(*optionbuttonsmenu);
		i++;
	}
	optionbuttonsmenu->setScale(1.f, 1.f);
	// Draw Game Mode: Beginner, Intermediate, Expert, Custom  Hover //
	if ((mouseY == 0 || mouseY == 2 || mouseY == 4 || mouseY == 5) && mouseX >= 0 && mouseX <= width - 1) {

		if (mouseY == 5) {
			optionbuttonsmenu->setTextureRect(sf::IntRect(160 * (mouseY + 3), squareSize * 2, 160, squareSize));
			optionbuttonsmenu->setPosition(gameBorderLeft * squareSize, squareSize * (mouseY + 4));
			optionbuttonsmenu->setScale(1.2f, 1.f);
		}
		else {
			optionbuttonsmenu->setTextureRect(sf::IntRect(160 * (mouseY + 2), squareSize * 2, 160, squareSize));
			optionbuttonsmenu->setPosition(gameBorderLeft * squareSize, squareSize * (mouseY + 3));
			optionbuttonsmenu->setScale(1.2f, 1.f);
		}
		window->draw(*optionbuttonsmenu);
		optionbuttonsmenu->setScale(1.f, 1.f);
	}

	// Draw the text input background //
	for (int i = 3 * width; i < 8 * width; i++) {
		spriteBlocks->setTextureRect(sf::IntRect(17 * squareSize, 0, squareSize, squareSize));
		spriteBlocks->setPosition((i % width + gameBorderLeft) * squareSize, (i / width + 1) * squareSize);
		window->draw(*spriteBlocks);
		if (i == 4 * width - 1)
			i = 5 * width - 1;
		if (i == 6 * width - 1)
			i = 7 * width - 1;
	}
}

void drawGameBoard(sf::RenderWindow *window, sf::Sprite *spriteBlocks)
{
	for (int i = 0; i < squareNumber; i++)
	{
		if (boardRevealed[i] == 1 && board[i] == -1) // reveal the death position //
			spriteBlocks->setTextureRect(sf::IntRect(10 * squareSize, 0, squareSize, squareSize));
		else if (boardRevealed[i] == 1) // reveal the i square //
			spriteBlocks->setTextureRect(sf::IntRect(board[i] * squareSize, 0, squareSize, squareSize));
		else if (boardRevealed[i] == 0) // place the hide square //
			spriteBlocks->setTextureRect(sf::IntRect(11 * squareSize, 0, squareSize, squareSize));
		else if (boardRevealed[i] == 2) // place the flag square //
			spriteBlocks->setTextureRect(sf::IntRect(12 * squareSize, 0, squareSize, squareSize));
		else if (boardRevealed[i] == 3) // place the not for sure flag //
			spriteBlocks->setTextureRect(sf::IntRect(13 * squareSize, 0, squareSize, squareSize));
		else if (boardRevealed[i] == 4)
			spriteBlocks->setTextureRect(sf::IntRect(board[i] * squareSize, squareSize, squareSize, squareSize));

		// Game End //

		if (board[i] == 9 && unrevealed == mineNumber && won && boardRevealed[i] != 2 && boardRevealed[i] != 4)
			spriteBlocks->setTextureRect(sf::IntRect(12 * squareSize, 0, squareSize, squareSize));

		else if (board[i] == 9 && gameOver && boardRevealed[i] == 2)
			spriteBlocks->setTextureRect(sf::IntRect(12 * squareSize, 0, squareSize, squareSize));

		else if (board[i] != 9 && gameOver && boardRevealed[i] == 2)
			spriteBlocks->setTextureRect(sf::IntRect(15 * squareSize, 0, squareSize, squareSize));

		else if (board[i] == 9 && gameOver && boardRevealed[i] != 4)
			spriteBlocks->setTextureRect(sf::IntRect(9 * squareSize, 0, squareSize, squareSize));

		spriteBlocks->setPosition((i % width + gameBorderLeft) * squareSize, (i / width + gameBorderTop) * squareSize);
		window->draw(*spriteBlocks);
	}
}

void drawMainMenu(sf::RenderWindow *window, sf::Sprite *optionbuttonsmenu, sf::Sprite *spriteBlocks, int mouseX, int mouseY, sf::Text *scoreText, sf::Text *gameTimer, sf::Text *playerMovesText)
{
	// Main Menu Back Button //
	if (mouseY == -gameBorderTop && mouseX == -gameBorderLeft)
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4 + 32, 0, squareSize, squareSize));
	else
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4, 0, squareSize, squareSize));
	optionbuttonsmenu->setPosition(0, 0);
	window->draw(*optionbuttonsmenu);

	// Exit Game Button //
	if (mouseY == -gameBorderTop && mouseX == width)
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4 + 32 * 3, 0, squareSize, squareSize));
	else
		optionbuttonsmenu->setTextureRect(sf::IntRect(160 * 5 + 64 * 4 + 32 * 2, 0, squareSize, squareSize));
	optionbuttonsmenu->setPosition(window->getDefaultView().getSize().x - squareSize, 0);
	window->draw(*optionbuttonsmenu);

	// Draw the background for the score table //
	if (width >= 9)
	{
		for (int i = width; i < width * 2; i++)
		{
			spriteBlocks->setTextureRect(sf::IntRect(17 * squareSize, 0, squareSize, squareSize));
			spriteBlocks->setPosition((i % width + gameBorderLeft) * squareSize, (i / width + 1) * squareSize - 3);
			spriteBlocks->setScale(1.3f, 1.1f);
			window->draw(*spriteBlocks);
			if (i == width + 2)
				i = width * 2 - 4;
		}
	}
	else
	{
		for (int i = width; i < width * 2; i++)
		{
			spriteBlocks->setTextureRect(sf::IntRect(17 * squareSize, 0, squareSize, squareSize));
			spriteBlocks->setPosition((i % width + gameBorderLeft) * squareSize, (i / width + 1) * squareSize + 2);

			window->draw(*spriteBlocks);
			if (i == width + 1)
				i = width * 2 - 3;
		}
	}
	spriteBlocks->setScale(1.f, 1.f);
	// Update game flag number//
	float distance = 17.6f;
	if (minesLeft >= 100) {
		scoreText->setPosition((squareSize + 16), (gameBorderTop - 3) * squareSize + distance + 3);

	}
	else if (minesLeft < 100 && minesLeft >= 10) {
		scoreText->setPosition((squareSize + 16), (gameBorderTop - 3) * squareSize + distance + 3);
	}
	else {
		scoreText->setPosition((squareSize + 16), (gameBorderTop - 3) * squareSize + distance + 3);
	}
	if (gameTime >= 100 || playerMoves >= 100) {
		gameTimer->setPosition((width - gameBorderRight * 1.5) * squareSize, squareSize + distance + 3);
		playerMovesText->setPosition((width - gameBorderRight * 1.5) * squareSize, squareSize + distance);
	}
	else if (gameTime < 100 && gameTime >= 10 || playerMoves < 100 && playerMoves >= 10) {
		gameTimer->setPosition((width - gameBorderRight / 1.5) * squareSize, squareSize + distance + 3);
		playerMovesText->setPosition((width - gameBorderRight / 1.5) * squareSize, squareSize + distance);
	}
	else {
		gameTimer->setPosition((width - gameBorderRight / 1.5) * squareSize, squareSize + distance + 3);
		playerMovesText->setPosition((width - gameBorderRight / 1.5) * squareSize, squareSize + distance);
	}

}

void drawWinLose(sf::RenderWindow *window, sf::Sprite *f, sf::Sprite *spriteWin, sf::Sprite *spriteLose)
{
	// If game win //
	if (unrevealed == mineNumber && !gameOver)
	{
		if (width % 2 == 0)
			f->setTextureRect(sf::IntRect(faceSize * 4 + squareSize * 7, 0, faceSize, squareSize));
		else
			f->setTextureRect(sf::IntRect(faceSize * 4, 0, faceSize, squareSize));
		window->draw(*f);
		if (width >= 9 && width <= 16)
		{
			if (height >= 10)
				spriteWin->setPosition((width - 6) * 32 / 2, 160);
			else spriteWin->setPosition((width - 6) * 32 / 2, 120);
			spriteWin->setScale(0.8f, 0.85f);
		}
		else if (width > 16)
		{
			if (height >= 16)
			{
				spriteWin->setPosition((width - 7) * 32 / 2, 180);
				spriteWin->setScale(1 + 0.1f, 1 + 0.1f);
			}
			else
			{
				spriteWin->setPosition((width - 7) * 32 / 2, 120);
				spriteWin->setScale(0.8f, 0.7f);
			}
		}
		else
		{
			spriteWin->setPosition((width - 4) * 32 / 2, 120);
			spriteWin->setScale(0.55f, 0.55f);
		}
		window->draw(*spriteWin);
		if (won == false) {
			won = true;
			minesLeft = 0;
		}
	}

	// If game lose //
	if (gameOver == 1) {

		if (width >= 9 && width <= 16)
		{
			spriteLose->setPosition((width - 7) * 32 / 2, 180);
			spriteLose->setScale(0.2f, 0.2f);
		}
		else if (width > 16 && width <= 36)
		{
			spriteLose->setPosition((width - 9) * 32 / 2, 180);
			spriteLose->setScale(0.3f, 0.3f);
		}
		else
		{
			spriteLose->setPosition((width - 5) * 32 / 2, 120);
			spriteLose->setScale(0.12f, 0.12f);
		}
		window->draw(*spriteLose);
		if (width % 2 == 0)
			f->setTextureRect(sf::IntRect(faceSize * 5 + squareSize * 7, 0, faceSize, squareSize));
		else
			f->setTextureRect(sf::IntRect(faceSize * 5, 0, faceSize, squareSize));
		window->draw(*f);
	}
}

void initText(sf::Text *text, sf::Color color, sf::Font *font)
{
	text->setFont(*font);
	text->setColor(color);
	text->setCharacterSize(20);
	text->setStyle(sf::Text::Bold);
	text->setScale(2.f, 2.f);
}

void drawGamePlay(sf::RenderWindow *window, sf::Sprite *f, sf::Sprite *spriteBlocks, int mouseX, int mouseY, sf::Vector2i pos)
{

	if (width % 2 == 0)
		faceSize = squareSize * 2;
	else
		faceSize = squareSize;

	f->setPosition(window->getDefaultView().getSize().x / 2 - faceSize / 2, squareSize);
	if (width >= 9 && height >= 9)
		f->setScale(1.2f, 1.2f);
	else
		f->setScale(1.1f, 0.8f);
	unsigned int on_face = window->getDefaultView().getSize().x / 2 - faceSize / 2;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
		sf::Mouse::isButtonPressed(sf::Mouse::Middle) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		if (pos.x >= on_face && pos.x <= on_face + faceSize && mouseY == -3) {
			if (width % 2 == 0)
				f->setTextureRect(sf::IntRect(faceSize * 1 + squareSize * 7, 0, faceSize, squareSize));
			else
				f->setTextureRect(sf::IntRect(faceSize * 1, 0, faceSize, squareSize));
			reset();
		}
		else if (mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
			if (width % 2 == 0)
				f->setTextureRect(sf::IntRect(faceSize * 3 + squareSize * 7, 0, faceSize, squareSize));
			else
				f->setTextureRect(sf::IntRect(faceSize * 3, 0, faceSize, squareSize));
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (pos.x >= on_face && pos.x <= on_face + faceSize && mouseY == -3) {
			if (width % 2 == 0)
				f->setTextureRect(sf::IntRect(faceSize * 6 + squareSize * 7, 0, faceSize, squareSize));
			else
				f->setTextureRect(sf::IntRect(faceSize * 6, 0, faceSize, squareSize));
		}
	}
	else {
		if (width % 2 == 0)
			f->setTextureRect(sf::IntRect(faceSize * 0 + squareSize * 7, 0, faceSize, squareSize));
		else
			f->setTextureRect(sf::IntRect(faceSize * 0, 0, faceSize, squareSize));
	}
	drawGameBoard(window, spriteBlocks);


	//LEFT CLICK
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (boardRevealed[mouseX + width * mouseY] == 0 || boardRevealed[mouseX + width * mouseY] == 3)) {
		if ((mouseX >= 0) && (mouseX < width) && (mouseY >= 0) && (mouseY < height)) {
			if (boardRevealed[mouseX + width * mouseY] == 3)
				spriteBlocks->setTextureRect(sf::IntRect(14 * squareSize, 0, squareSize, squareSize));
			else
				spriteBlocks->setTextureRect(sf::IntRect(0 * squareSize, 0, squareSize, squareSize));
			spriteBlocks->setPosition((gameBorderLeft + mouseX) * squareSize, (gameBorderTop + mouseY) * squareSize);
			if (gameOver != 1 && unrevealed != mineNumber)
				window->draw(*spriteBlocks);
		}
	}

}

bool revealed(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height) return true;
	if (boardRevealed[y * width + x] == 1 || boardRevealed[y * width + x] == 2) return true;
	return false;
}

void display(int x, int y, int firstX, int firstY) {
	if (x < 0 || x >= width || y < 0 || y >= height) return;
	int i = y * width + x;
	if (boardRevealed[i] == 1 || boardRevealed[i] == 2 || board[i] == 9) return;
	boardRevealed[i] = 1;
	--unrevealed;
	if (board[i] == 0) {
		if (!revealed(x + 1, y)) {
			display(x + 1, y, x, y);
		}
		if (!revealed(x - 1, y)) {
			display(x - 1, y, x, y);
		}
		if (!revealed(x, y + 1)) {
			display(x, y + 1, x, y);
		}
		if (!revealed(x, y - 1)) {
			display(x, y - 1, x, y);
		}
		if (!revealed(x + 1, y + 1)) {
			display(x + 1, y + 1, x, y);
		}
		if (!revealed(x + 1, y - 1)) {
			display(x + 1, y - 1, x, y);
		}
		if (!revealed(x - 1, y + 1)) {
			display(x - 1, y + 1, x, y);
		}
		if (!revealed(x - 1, y - 1)) {
			display(x - 1, y - 1, x, y);
		}
	}
}

bool reveal(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height) return false;
	int i = y * width + x;
	if (boardRevealed[i] == 0 || boardRevealed[i] == 3) {
		boardRevealed[i] = 1;
		--unrevealed;
		playerMoves++;

		if (board[i] == 0) {
			display(x + 1, y, x, y);
			display(x - 1, y, x, y);
			display(x, y + 1, x, y);
			display(x, y - 1, x, y);
			display(x + 1, y - 1, x, y);
			display(x + 1, y + 1, x, y);
			display(x - 1, y - 1, x, y);
			display(x - 1, y + 1, x, y);
		}
		if (board[i] == 9) {
			gameOver = true;
			return true;
		}
	}
	return false;
}

bool showSurroundings(int mouseX, int mouseY) {
	int flag = 0;
	for (int i = -1; i <= 1; i++)
	for (int j = -1; j <= 1; j++) {
		if (mouseX + i >= 0 && mouseX + i < width && mouseY + j >= 0 && mouseY + j < height) {
			if (boardRevealed[(mouseX + i) + width * (mouseY + j)] == 2)
				flag++;
			if (board[(mouseX + i) + width * (mouseY + j)] == 9 && boardRevealed[(mouseX + i) + width * (mouseY + j)] == 4)
				flag++;
		}
	}
	if (flag == board[mouseX + width * mouseY]) {
		if (reveal(mouseX, mouseY - 1) ||
			reveal(mouseX - 1, mouseY - 1) ||
			reveal(mouseX + 1, mouseY - 1) ||
			reveal(mouseX - 1, mouseY) ||
			reveal(mouseX + 1, mouseY) ||
			reveal(mouseX, mouseY + 1) ||
			reveal(mouseX - 1, mouseY + 1) ||
			reveal(mouseX + 1, mouseY + 1))
			return true;
	}
	return false;
}

