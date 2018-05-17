#ifndef _DATA_H_
#define _DATA_H_
//DATA STRUCTURES
#pragma once		 
enum Status
{
	UNCOVERED,
	COVERED,
	MARKED,
	QUESTIONABLE
};
struct BlockData
{
	char Data;
	Status State;
};
//GET INPUT DATA
void inputData(BlockData **&blockData,char **&board, int &width, int &height,int &mines,int &noneMines, char *fileName);

//DELETE MATRIX OF ANY TYPE
template<class T>
void deleteData(T **&m, int height)
{
	for (int i = 0; i < height; i++)
	{
		delete[] m[i];
	}
	delete[]m;
}

//HOOK MOUSE/KEYBOARD EVENT
COORD catchInputEvent(int &key);
//CHECK THE MOUSE CLICK POSITION
bool checkPlayAreaClick(COORD xy, int width, int height);
//PROCESS THE CLICK EVENT
void clickEvent(int click, COORD xy, COORD **block, char **board, BlockData **blockData, int width, int height,int &noneMines, int key, int &gameLoop);
//PRINT THE BLOCK USER CLICKED
void printBlock(char **board, int i, int j, int width, int height);
//REVEAL ALL TILES IF USER LOSE
void revealAll(char **board, int width, int height);
//CHECK GAME WIN/LOSE
void checkGameConditions(char block, int width,int height,int &noneMines,int key, int &gameLoop);
//RANDOM PLACE MINES
void mineGenerator(BlockData **&blockData, char **&board, int &width, int &height, int &mines, int &noneMines, char *fileName);
extern int minesTemp;
#endif