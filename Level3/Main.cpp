#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include "Graphic.h"
#include "Data.h"
#include <time.h>

using namespace std;
int minesTemp;
void main(int argc, char *argv[])
{

	if (strcmp(argv[1], "3") == 0)
	{
		drawGuideGame();
		start: char **board = NULL;
		int width, height, mines = 0, noneMines;
		int gameLoop = 0, key = 0;
		COORD xy;
		COORD **block;
		BlockData **blockData;
		char fileName[255];

		strcpy(fileName, argv[2]);
		mineGenerator(blockData, board, width, height, mines, noneMines, argv[2]);
		minesTemp = mines;
		//DRAW THE FIELD
		DrawRect(3, 3, width, height, block);
		SetXY(25, 1);
		printf("TOTAL MINES : %d", mines);

		//CHECK EVENT
		while (true)
		{

			xy = catchInputEvent(key);
			if (gameLoop == 1)
			{
				gameLoop = 0;
				goto start;
			}
			int click = 0;

			if (GetKeyState(VK_LBUTTON) & 0x8000)
				click = 1;	//CLICK 1 <=> LEFT CLICK
			if (GetKeyState(VK_RBUTTON) & 0x8000)
				click = 2; //CLICK 2 <=> RIGHT CLICK

			if (click != 0)
			{
				if (checkPlayAreaClick(xy, width, height) && (xy.X % 2 == 0))
					clickEvent(click, xy, block, board, blockData, width, height, noneMines, key, gameLoop);
				if (xy.Y >= 0 && xy.Y <= 2)
				{
					if (xy.X >= 1 && xy.X <= 10)		//NEW GAME BUTTON
						goto start;
					if (xy.X >= 14 && xy.X <= 23)		//EXIT BUTTON
						exit(0);
				}
			}//END CLICK EVENT
		}
		deleteData(board, height);
		deleteData(block, height);
		deleteData(blockData, height);
	}
	else
	{
		printf("Wrong Command Line Arguments\n");
		exit(0);
	}

	
}