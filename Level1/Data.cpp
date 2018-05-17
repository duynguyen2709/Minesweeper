#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include "Graphic.h"
#include "Data.h"

//#include "Main.cpp"

void inputData(BlockData **&blockData,char **&board,int &width,int &height,int &mines,int &noneMines,char *fileName)
{
	
	FILE *f,*f2;
	f = fopen(fileName, "r+");
	//f2 = fopen("output.txt", "w+");
	fscanf(f, "%d %d%*c", &width, &height);
	board = new char *[height];
	blockData = new BlockData*[height];

	for (int i = 0; i < height; i++)
	{
		board[i] = new char[width];
		blockData[i] = new BlockData[width];
		for (int j = 0; j < width; j++)
		{
			fscanf(f, "%s%*c", &board[i][j]);
			if (board[i][j] == '*')
				mines++;
			(*(blockData + i) + j)->Data = board[i][j];
			(*(blockData + i) + j)->State = COVERED;
		}
	}

	noneMines = width * height - mines;
	fclose(f);
	//fclose(f2);
}

COORD catchInputEvent(int &key)
{
	HANDLE hStdInput, hStdOutput;
	INPUT_RECORD ir[128];
	bool blnLoop = true;
	DWORD nRead;
	COORD xy;
	UINT i;

	hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode(hStdInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);   //<<<flags
	FlushConsoleInputBuffer(hStdInput);
	do
	{
		/*if (WaitForSingleObject(hStdInput, 3000) == WAIT_TIMEOUT)
		break;
		else
		{*/
		ReadConsoleInput(hStdInput, ir, 128, &nRead);
		for (i = 0; i<nRead; i++)
		{
			switch (ir[i].EventType)
			{
			case KEY_EVENT:
				if (ir[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
					blnLoop = false;
				else
				{
					/*xy.X = 10 + width *2; xy.Y = 4;
					SetConsoleCursorPosition(hStdOutput, xy);
					printf
						(
						"AsciiCode = %.3d: symbol = %c\n",
						ir[i].Event.KeyEvent.uChar.AsciiChar,
						ir[i].Event.KeyEvent.uChar.AsciiChar
						);*/
					key = (int)ir[i].Event.KeyEvent.uChar.AsciiChar;
				}
				break;
			case MOUSE_EVENT:
				/*xy.X = 10 + width * 2; xy.Y = 5;
				SetConsoleCursorPosition(hStdOutput, xy);
				printf
					(
					"O (%.3d;%.3d)\n",
					ir[i].Event.MouseEvent.dwMousePosition.X,
					ir[i].Event.MouseEvent.dwMousePosition.Y
					);
*/
				return ir[i].Event.MouseEvent.dwMousePosition;
				
				break;
			}//end switch
		}//end for
	}//end do
	while (blnLoop == true);
}

//CHECK IF PLAYER CLICKED INSIDE PLAY AREA
bool checkPlayAreaClick(COORD xy,int width,int height)
{
	return (xy.X >= 4 && xy.X <= width * 2 + 2 && xy.Y >= 4 && xy.Y <= height + 3);
}

void clickEvent(int click,COORD xy, COORD **block, char **board,BlockData **blockData,int width,int height,int &noneMines,int key,int &gameLoop)
{
	
		SetXY(xy.X, xy.Y);

		for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (block[i][j].X == xy.Y && block[i][j].Y == xy.X)	//FIND BLOCK COORD
			{
				if ((*(blockData + i) + j)->State == COVERED) //CHECK BLOCK STATUS COVERED
				{
					if (click == 1)
						{
						printBlock(board, i, j, width, height);
						checkGameConditions(board[i][j],width,height,noneMines,key,gameLoop);
						(*(blockData + i) + j)->State = UNCOVERED;	
						goto end;
						}
					if (click == 2)  //RIGHT CLICK ON COVERED BLOCK => CHANGE TO MARKED
						{
							SetColor(12);
							printf("%c",(char)241);
							(*(blockData + i) + j)->State = MARKED;
							SetColor(15);
							SetXY(39, 1);
							minesTemp--;
							if (minesTemp < 0)
								printf(" 0");
							else printf("%2d", minesTemp);
							goto end;
						}
				} //END IF CHECK STATUS COVERED
				
				if ((*(blockData + i) + j)->State == MARKED && click == 2)	//CHECK MARKED STATUS
				{
					SetColor(14);
					printf("?");
					(*(blockData + i) + j)->State = QUESTIONABLE;
					SetColor(15);
					SetXY(39, 1);
					minesTemp++;
					if (minesTemp < 0)
						printf(" 0");
					else printf("%2d", minesTemp);
					goto end;
				}//END IF CHECK MARKED STATUS

				if ((*(blockData + i) + j)->State == QUESTIONABLE) //CHECK QUESTIONABLE STATUS 
				{
					if (click == 1)
					{
						printBlock(board, i, j,width,height);
						checkGameConditions(board[i][j], width, height, noneMines,key, gameLoop);
						(*(blockData + i) + j)->State = UNCOVERED;
						goto end;
					}
					if (click == 2)  //RIGHT CLICK ON COVERED BLOCK => CHANGE TO MARKED
					{
						SetColor(11);
						printf("=");
						(*(blockData + i) + j)->State = COVERED;
						SetColor(15);
						goto end;
					}
				} //END IF CHECK QUESTIONABLE

			} //END IF FIND COORD
		}
	end: return;
	
}

void printBlock(char **board,int i,int j,int width,int height)
{
	switch (board[i][j])
	{
	case '1':SetColor(9);
		break;
	case '2':SetColor(10);
		break;
	case '3':SetColor(12);
		break;
	case '4':SetColor(11);
		break;
	case '5':SetColor(14);
		break;
	}

	if (board[i][j] == '-')
		printf(" ");
	else if (board[i][j] == '*')
	{
		revealAll(board, width, height);
		
		/*SetColor(12);
		printf("*");*/
	}
	else
		printf("%c", board[i][j]);
	SetColor(15);
}

void revealAll(char **board, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		SetXY(4, 4 + i);

		for (int j = 0; j < width; j++)
		{
			switch (board[i][j])
			{
			case '1':SetColor(9);
				break;
			case '2':SetColor(10);
				break;
			case '3':SetColor(12);
				break;
			case '*':SetColor(12);
				break;
			case '4':SetColor(11);
				break;
			case '5':SetColor(14);
				break;
			}
			if (board[i][j] == '-')
				(j == width - 1) ? printf(" ") : printf("  ");
			else
			{
				(j == width - 1) ? printf("%c", board[i][j]) : printf("%c ", board[i][j]);
			}
		}
	}
}
void checkGameConditions(char block,int width,int height,int &noneMines,int key,int &gameLoop)
{
	if (block != '*')
	{
		/*SetXY(25, 20);
		printf("%d\n\t%d\n\t%d", width,height,noneMines);*/
		noneMines--;
	}

	if (block == '*' || noneMines == 0)
	{
		
		//system("cls");
		//DELETE DEFAULT BUTTON
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 42; j++)
			{
				SetXY(j,i);
				printf(" ");
			}
		}
		//DRAW RESTART BUTTON
		drawButton(15 + width * 2, height - 3, "Restart");
		drawButton(15 + width * 2, height , "  Exit ");
		//DRAW IMAGE
		if (block == '*')
			drawImage("boom.bmp");
		if (noneMines == 0)
			drawImage("win.bmp");

		COORD temp;
		while (true)
		{
			temp = catchInputEvent(key);
			if ((GetKeyState(VK_LBUTTON) & 0x8000) && temp.X >= 14 + width * 2 && temp.X <= 23 + width * 2)
			{
				if (temp.Y >= height - 4 && temp.Y <= height - 2)
					gameLoop = 1;
				if (temp.Y >= height - 1 && temp.Y <= height + 1)
					exit(0);
				break;
			}
		}
	} //END GAME EVENT
	
		

	return;
}