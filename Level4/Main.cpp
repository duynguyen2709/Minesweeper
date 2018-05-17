#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include "Graphic.h"
#include "Data.h"
#include <time.h>

using namespace std;
int minesTemp,minesDetected;
int playerNumber = 0, playtime;
PlayerScore player[50];
char output[255];
void main(int argc, char *argv[])
{
	if (strcmp(argv[1], "4") == 0)
	{
		int width, height;
		drawGuideGame();
		getInput(width, height, argv[2]);

		//INITIALIZE 
start: char **board = NULL;
		int mines = 0, noneMines;
		int gameLoop = 0, key = 0;
		COORD xy;
		COORD **block;
		BlockData **blockData;
		time_t start;
		strcpy(output, argv[3]);
		int firstClick = 0;
		int sec = 0;

		mineGenerator(blockData, board, width, height, mines, noneMines);
		minesTemp = mines;
		minesDetected = 0;
		playtime = 0;

		enterUsername();
		//DRAW THE FIELD
		DrawRect(3, 3, width, height, block);
		SetXY(25, 1);
		printf("TOTAL MINES : %d", mines);
		SetXY(25, 2);
		printf("TIME :  0s");

		//CHECK EVENT
		start = clock();
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
			{
				if (firstClick == 0 && checkPlayAreaClick(xy, width, height))
				{
					firstClick = 1;
					start = clock();
				}
				click = 1;	//CLICK 1 <=> LEFT CLICK
			}
			if (GetKeyState(VK_RBUTTON) & 0x8000)
				click = 2; //CLICK 2 <=> RIGHT CLICK

			if (click != 0)
			{
				if (checkPlayAreaClick(xy, width, height) && (xy.X % 2 == 0))
					clickEvent(click, xy, block, board, blockData, width, height,mines, noneMines, key, gameLoop);
				if (xy.Y >= 0 && xy.Y <= 2)
				{
					if (xy.X >= 1 && xy.X <= 10)		//NEW GAME BUTTON
						goto start;
					if (xy.X >= 14 && xy.X <= 23)		//EXIT BUTTON
						exit(0);
				}
			}//END CLICK EVENT
			if ((int)(clock() - start) > sec * 1000 && firstClick == 1)
			{
				sec++;
				SetXY(32, 2);
				playtime = (int)((clock() - start) / CLOCKS_PER_SEC);
				printf("%2ds", playtime);
			}
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