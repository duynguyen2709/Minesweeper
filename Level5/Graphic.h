#include <SFML\Graphics.hpp>

//LOAD THE IMAGE USED IN GAME
void loadTexture();

//DRAW IMAGES THE FIRST TIME
void firstDraw(sf::Sprite *spriteLogo, sf::Sprite *spriteTextbox, sf::Sprite *spriteBackButton, sf::Sprite *spriteNextButton, sf::Sprite *spriteNewGameButton, sf::Sprite *spriteGuideButton, sf::Sprite *spriteExitButton);

//DRAW LEVELS TO SELECT
void drawSelectLevel(sf::RenderWindow *window, sf::Sprite *optionbuttonsmenu, int mouseX, int mouseY);

//DRAW CUSTOM FIELD TO ENTER
void drawCustomLevel(sf::RenderWindow *window, sf::Sprite *optionbuttonsmenu, sf::Sprite *spriteBlocks, int mouseX, int mouseY);

//DRAW GUIDE GAME
void drawGuideGame(sf::Event guide, sf::RenderWindow *window, sf::Sprite spriteBackButton, sf::Sprite spriteNextButton, sf::Text *text, int x, int y);

//DRAW THE GAME BOARD
void drawGameBoard(sf::RenderWindow *window, sf::Sprite *spriteBlocks);

//DRAW MAIN MENU
void drawMainMenu(sf::RenderWindow *window, sf::Sprite *optionbuttonsmenu, sf::Sprite *spriteBlocks, int mouseX, int mouseY, sf::Text *scoreText, sf::Text *gameTimer, sf::Text *player_movesText);

//DRAW WIN/LOSE IMAGE
void drawWinLose(sf::RenderWindow *window, sf::Sprite *f, sf::Sprite *spriteWin, sf::Sprite *spriteLose);

//INITIAL SOME TEXTS
void initText(sf::Text *text, sf::Color color, sf::Font *font);
void setText(sf::String *str, sf::Event custom);

//DRAW WHILE PLAYING
void drawGamePlay(sf::RenderWindow *window, sf::Sprite *f, sf::Sprite *spriteBlocks, int mouseX, int mouseY, sf::Vector2i pos);

//REVEAL TILES
bool revealed(int x, int y);
void display(int x, int y, int firstX, int firstY);
bool reveal(int x, int y);
bool showSurroundings(int x, int y);
//END REVEAL TILES

