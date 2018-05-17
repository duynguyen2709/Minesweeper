#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include "Graphic.h"
#include "Data.h"

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
		for (i = 0; i < nRead; i++)
		{
			switch (ir[i].EventType)
			{
			case KEY_EVENT:
				if (ir[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
					blnLoop = false;
				else
				{
					key = (int)ir[i].Event.KeyEvent.uChar.AsciiChar;
				}
				break;
			case MOUSE_EVENT:
				return ir[i].Event.MouseEvent.dwMousePosition;

				break;
			}//end switch
		}//end for
	}//end do
	while (blnLoop == true);
}

//CHECK IF PLAYER CLICKED INSIDE PLAY AREA
bool checkPlayAreaClick(COORD xy, int width, int height)
{
	return (xy.X >= 4 && xy.X <= width * 2 + 2 && xy.Y >= 4 && xy.Y <= height + 3);
}

void clickEvent(int click, COORD xy, COORD **block, char **board, BlockData **blockData, int width, int height, int mines, int &noneMines, int key, int &gameLoop)
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
						printBlock(board, i, j, width, height, xy);
						checkGameConditions(board[i][j], width, height, mines, noneMines, key, gameLoop);
						(*(blockData + i) + j)->State = UNCOVERED;
						goto end;
					}
					if (click == 2)  //RIGHT CLICK ON COVERED BLOCK => CHANGE TO MARKED
					{
						SetColor(12);
						printf("%c", (char)241);
						(*(blockData + i) + j)->State = MARKED;
						if ((*(blockData + i) + j)->Data == '*')
							minesDetected++;
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
					if ((*(blockData + i) + j)->Data == '*')
						minesDetected--;
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
						printBlock(board, i, j, width, height, xy);
						checkGameConditions(board[i][j], width, height, mines, noneMines, key, gameLoop);
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

void printBlock(char **board, int i, int j, int width, int height, COORD xy)
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
	{
		printf(" ");
		bool **check = new bool *[height];
		for (int m = 0; m < height; m++)
		{
			check[m] = new bool[width];
			for (int n = 0; n < width; n++)
			{
				check[m][n] = false;
			}
		}
		revealEmpty(board, check, i, j, width, height, xy);
	}
	else if (board[i][j] == '*')
	{
		revealAll(board, width, height);
	}
	else
		printf("%c", board[i][j]);
	SetColor(15);
}
void revealEmpty(char **board, bool **&check, int i, int j, int width, int height, COORD xy)
{
	if (i < 0 || j < 0 || i == height || j == width) return;

	//TOP MID
	if (i - 1 >= 0)
	{
		if (board[i - 1][j] == '-' && check[i - 1][j] == false)
		{
			check[i - 1][j] = true;
			printEmpty(xy, 0, -1);
			revealEmpty(board, check, i - 1, j, width, height, { xy.X, xy.Y - 1 });
		}
	}

	//LEFT
	if (j - 1 >= 0)
	{
		if (board[i][j - 1] == '-' && check[i][j - 1] == false)
		{
			check[i][j - 1] = true;
			printEmpty(xy, -2, 0);
			revealEmpty(board, check, i, j - 1, width, height, { xy.X - 2, xy.Y });
		}
	}

	//RIGHT
	if (j + 1 < width)
	{
		if (board[i][j + 1] == '-' && check[i][j + 1] == false)
		{
			check[i][j + 1] = true;
			printEmpty(xy, 2, 0);
			revealEmpty(board, check, i, j + 1, width, height, { xy.X + 2, xy.Y });
		}
	}

	//BOTTOM MID
	if (i + 1 < height)
	{
		if (board[i + 1][j] == '-' && check[i + 1][j] == false)
		{
			check[i + 1][j] = true;
			printEmpty(xy, 0, 1);
			revealEmpty(board, check, i + 1, j, width, height, { xy.X, xy.Y + 1 });
		}
	}
}
void printEmpty(COORD xy, int x, int y)
{
	xy.X += x; xy.Y += y;
	SetXY(xy.X, xy.Y);
	printf(" ");
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
void checkGameConditions(char block, int width, int height, int mines, int &noneMines, int key, int &gameLoop)
{
	if (block != '*')
	{
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
				SetXY(j, i);
				printf(" ");
			}
		}

		//DRAW RESTART BUTTON
		drawButton(15 + width * 2, height - 3, "Restart");
		drawButton(15 + width * 2, height, "  Exit ");

		// WIN/LOSE
		if (block == '*')
		{
			player[playerNumber].score = (double)(minesDetected*100.0 / mines);
			player[playerNumber].time = playtime;
			drawImage("boom.bmp");
			playerNumber++;
			updateHighscore();
		}
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
				{
					exit(0);
				}
				break;
			}
		}
	} //END GAME EVENT

	return;
}
void getInput(int &width, int &height, char *fileName)
{
	FILE *f;
	f = fopen(fileName, "r+");
	fscanf(f, "%d %d%*c", &width, &height);
	while (!feof(f))
	{
		fscanf(f, "%s %f %d", player[playerNumber].username, &player[playerNumber].score, &player[playerNumber].time);
		playerNumber++;
	}
	fclose(f);
}
void mineGenerator(BlockData **&blockData, char **&board, int width, int height, int &mines, int &noneMines)
{
	int **m;
	int i, j;
	srand(time(NULL)); //start the random function
	if (width == 9 && height == 9)
		mines = 10;
	else if (width == 16 && height == 16)
		mines = 40;
	else if (width == 30 && height == 16)
		mines = 99;
	else
		mines = (int)(width*height*0.15f);
	noneMines = width*height - mines;

	//initalize matrix
	m = new int *[height];
	for (i = 0; i < height; i++)
	{
		m[i] = new int[width];
		for (j = 0; j < width; j++)
			m[i][j] = 0;
	}

	int totalMines = 0;	//count total randomized mines

	while (totalMines < mines)
	{
		i = rand() % height;
		j = rand() % width;

		if (m[i][j] != 9)
			m[i][j] = rand() % 2;	//random : 1 - mine ; 0 - no mine

		if (m[i][j] == 1)		//increase randomized mines
		{
			totalMines++;
			m[i][j] = 9;	//set mine-tiled to 9
		}

		if (totalMines == mines)	//early break loop
			goto setMatrix;
	}
setMatrix: for (i = 0; i < height; i++)
	for (j = 0; j < width; j++)
	{
		if (m[i][j] == 9)
		{
			//check 8 tiles around mine-tiled
			if (i - 1 >= 0 && j - 1 >= 0 && m[i - 1][j - 1] != 9) m[i - 1][j - 1]++;
			if (i - 1 >= 0 && m[i - 1][j] != 9) m[i - 1][j]++;
			if (i - 1 >= 0 && j + 1 < width && m[i - 1][j + 1] != 9) m[i - 1][j + 1]++;
			if (j - 1 >= 0 && m[i][j - 1] != 9) m[i][j - 1]++;
			if (j + 1 < width && m[i][j + 1] != 9) m[i][j + 1]++;
			if (i + 1 < height && j - 1 >= 0 && m[i + 1][j - 1] != 9) m[i + 1][j - 1]++;
			if (i + 1 < height && m[i + 1][j] != 9) m[i + 1][j]++;
			if (i + 1 < height && j + 1 < width && m[i + 1][j + 1] != 9) m[i + 1][j + 1]++;
		}
	}
		   board = new char *[height];
		   blockData = new BlockData*[height];

		   for (int i = 0; i < height; i++)
		   {
			   board[i] = new char[width];
			   blockData[i] = new BlockData[width];
			   for (int j = 0; j < width; j++)
			   {
				   switch (m[i][j])
				   {
				   case 9:
					   board[i][j] = (char)'*';
					   break;
				   case 0:
					   board[i][j] = (char)'-';
					   break;
				   default:
					   board[i][j] = m[i][j] + 48;
					   break;
				   }

				   (*(blockData + i) + j)->Data = board[i][j];
				   (*(blockData + i) + j)->State = COVERED;
			   }
		   }
}

void enterUsername()
{
	system("cls");
	SetWindowSize(15, 15);
	SetConsoleFontSize({ 18, 18 });
userEnter: SetXY(8, 10);
	SetColor(11);
	printf("Enter username :\n");
	SetXY(8, 11);
	printf("(No space include . Max 12 characters)\n");
	SetXY(25, 10);
	char temp[255];
	SetColor(15);
	gets_s(temp);
	if (strlen(temp) > 12 || strlen(temp) <= 0)
	{
		SetXY(8, 12);
		SetColor(12);
		printf("Error in username lenght. Re-enter\n");
		SetXY(25, 10);
		for (int i = 0; i < strlen(temp); i++)
			printf(" ");
		goto userEnter;
	}
	for (int i = 0; i < strlen(temp); i++)
	{
		if (temp[i] == ' ')
		{
			SetXY(8, 12);
			SetColor(12);
			printf("Error : Username contains space. Re-enter\n");
			SetXY(25, 10);
			for (int j = 0; j < strlen(temp); j++)
				printf(" ");
			goto userEnter;
		}
	}
	strcpy(player[playerNumber].username, temp);
	SetColor(15);
	system("cls");
}

void updateHighscore()
{
	FILE *f;
	f = fopen(output, "w+");
	for (int i = 0; i < playerNumber - 1; i++)
	{
		for (int j = i + 1; j < playerNumber; j++)
		{
			if ((player[i].score < player[j].score) || (player[i].score == player[j].score && player[i].time > player[j].time))
			{
				PlayerScore temp = player[i];
				player[i] = player[j];
				player[j] = temp;
			}
		}
	}
	fprintf(f, "HIGHSCORE RANKING\n");
	fprintf(f, "  Username  | Score | Time |\n");
	for (int i = 0; i < playerNumber; i++)
	{
		fprintf(f, "%12s| %5.1f | %4d |\n", player[i].username, player[i].score, player[i].time);
	}
	fclose(f);
}