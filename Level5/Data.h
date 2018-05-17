#include <SFML/Graphics.hpp>
#include <windows.h>
#include <cmath>
#include <sstream>
#include <ctime>
#include <iostream>
#define  MAX 50


// SELECT THE LEVEL
void selectLevel(int mouseX, int mouseY, int &width, int &height, int &mineNumber, bool &displaySize, int &menuStage);

//ENTER NUMBERS IN CUSTOM LEVEL
void enterCustomLevel(sf::Event custom, sf::RenderWindow *window, int mouseX, int mouseY, sf::String playerString[], sf::Text playerInput[]);

//CONTROL THE GAMEPLAY
void mainGamePlay(sf::Event play, sf::RenderWindow *window, int mouseX, int mouseY, sf::Clock *clock);

//GAME LOGIC
int nearMines(int x, int y);
void countSurrounding(int x, int y);
void placeMines(int firstClick_poz);
void reset();
float lerp(float a, float b, float c);

// GLOBAL VARIABLES
extern float logoAnimation;
extern int width, height, gameTime, menuStage, playerMoves;
extern int squareNumber, mineNumber, unrevealed, flagNum, minesLeft, faceSize, customw, customh, customm, mouseButton, page, squareSize;
extern int gameBorderTop, gameBorderRight, gameBorderBottom, gameBorderLeft;
extern int board[MAX * MAX];
extern int boardRevealed[MAX * MAX];
extern bool firstClick, gameOver, won, showMoves;
extern bool markFlag, displaySize;

extern sf::Texture textureBlocks, textureBackground, textureWin, textureLose, faces, logo, optionbuttons,
textureNewGameButton, textureGuideButton, textureExitButton, textureBackButton,
textureNextButton, textureTextbox;