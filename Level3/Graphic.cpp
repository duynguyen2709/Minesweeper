#include <iostream>
#include <stdexcept>
#include <Windows.h>
#include <conio.h>
#include "Graphic.h"

using namespace std;

bool SetXY(int x, int y)
{
	COORD c = { x, y };
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

bool SetConsoleFontSize(COORD dwFontSize)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info{ sizeof(CONSOLE_FONT_INFOEX) };
	if (!GetCurrentConsoleFontEx(output, false, &info))
		return false;
	info.dwFontSize = dwFontSize;
	return SetCurrentConsoleFontEx(output, false, &info);
}

void drawButton(int x, int y, char *buttonText)
{
	int i;
	SetColor(14);
	SetXY(x - 1, y - 1);
	cout << char(201);
	for (i = 0; i < strlen(buttonText); i++)
		cout << char(205);
	cout << char(187);
	SetXY(x - 1, y + 1);
	cout << char(200);
	for (i = 0; i < strlen(buttonText); i++)
		cout << char(205);
	cout << char(188);
	SetXY(x - 1, y);
	cout << char(186);

	SetXY(x, y);
	SetColor(15);
	cout << buttonText;

	SetColor(14);
	SetXY(x + strlen(buttonText), y);
	cout << char(186) << " ";
	SetColor(15);
}
void DrawRect(int x, int y, int width, int height, COORD **&block)
{
	SetWindowSize(width, height);
	int i, j;
	system("cls");

	//SAVE BLOCKS POSITIONS TO COORD
	block = new COORD *[height];
	for (i = 0; i < height; i++)
	{
		block[i] = new COORD[width];
	}

	COORD temp;
	temp.X = x + 1;
	temp.Y = y + 1;
	block[0][0] = temp;
	i = 0;

	while (i < height)
	{
		for (j = 0; j < width; j++)
		{
			if (i != 0 && j == 0)
			{
				temp.X = x + i + 1;
				temp.Y = block[i - 1][j].Y;
				block[i][j] = temp;
			}
			if (j != 0)
			{
				block[i][j] = { block[i][j - 1].X, block[i][j - 1].Y + 2 };
			}
		}
		i++;
	} //END WHILE

	//SET WINDOW FONT SIZE

	if ((width > height ? width : height) >= 30)
		SetConsoleFontSize({ 17, 17 });
	else SetConsoleFontSize({ 20, 20 });

	//DRAW BUTTON
	drawButton(2, 1, "NEW GAME");
	drawButton(15, 1, "  EXIT  ");

	//PRINT RECTANGLE
	SetXY(x, y);
	cout << char(201);
	for (i = 1; i < width * 2; i++)
		cout << char(205);
	cout << char(187);
	SetXY(x, height + y + 1);
	cout << char(200);
	for (i = 1; i < width * 2; i++)
		cout << char(205);
	cout << char(188);
	j = 0;
	for (i = y + 1; i < height + y + 1; i++)
	{
		SetXY(x, i);
		SetColor(15);
		cout << char(186);

		//PRINT BOARD
		for (j = 0; j < width; j++)
		{
			SetColor(11);
			cout << "= ";
		}	//end for

		cout << endl;

		SetXY(x + width * 2, i);
		SetColor(15);
		cout << char(186);
	}  //end for

	SetColor(15);
	SetXY(0, 0);
}

//SET WINDOW SIZE
void SetConsoleWindowSize(int x, int y)
{
	FixedConsoleSize();
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	if (h == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Unable to get stdout handle.");

	{
		COORD largestSize = GetLargestConsoleWindowSize(h);
		if (x > largestSize.X)
			throw std::invalid_argument("The x dimension is too large.");
		if (y > largestSize.Y)
			throw std::invalid_argument("The y dimension is too large.");
	}

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	if (!GetConsoleScreenBufferInfo(h, &bufferInfo))
		throw std::runtime_error("Unable to retrieve screen buffer info.");

	SMALL_RECT& winInfo = bufferInfo.srWindow;
	COORD windowSize = { winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1 };

	if (windowSize.X > x || windowSize.Y > y)
	{
		// window size needs to be adjusted before the buffer size can be reduced.
		SMALL_RECT info =
		{
			0,
			0,
			x < windowSize.X ? x - 1 : windowSize.X - 1,
			y < windowSize.Y ? y - 1 : windowSize.Y - 1
		};

		if (!SetConsoleWindowInfo(h, TRUE, &info))
			throw std::runtime_error("Unable to resize window before resizing buffer.");
	}

	COORD size = { x, y };
	if (!SetConsoleScreenBufferSize(h, size))
		throw std::runtime_error("Unable to resize screen buffer.");

	SMALL_RECT info = { 0, 0, x - 1, y - 1 };
	if (!SetConsoleWindowInfo(h, TRUE, &info))
		throw std::runtime_error("Unable to resize window after resizing buffer.");
}
void ShowLastSystemError()
{
	LPSTR messageBuffer;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		0,          // source
		GetLastError(),
		0,          // lang
		(LPSTR)&messageBuffer,
		0,
		NULL);

	std::cerr << messageBuffer << '\n';
	LocalFree(messageBuffer);
}
void SetWindowSize(int width, int height)
{
	SetConsoleTitle(TEXT("Minesweeper"));
	COORD consoleSize;
	consoleSize.X = width * 2 + 30;
	consoleSize.Y = height + 10;
	if (consoleSize.X != 0 && consoleSize.Y != 0)
	{
		try
		{
			SetConsoleWindowSize(consoleSize.X, consoleSize.Y);
		}

		catch (std::logic_error& ex)
		{
			std::cerr << ex.what() << '\n';
		}

		catch (std::exception& ex)
		{
			std::cerr << ex.what() << "\nSystem error: ";
			ShowLastSystemError();
		}
	}
}

//END SET WINDOW SIZE FUNCTIONS

void SetColor(WORD color)
{
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void FixedConsoleSize()
{
	HWND hConsole;
	MENUITEMINFO mii;
	SetConsoleTitle(TEXT("Minesweeper"));
	Sleep(10); //Let the window to update the Title!
	hConsole = FindWindow(NULL, TEXT("Minesweeper"));
	SetWindowLong(hConsole, GWL_STYLE, GetWindowLong(hConsole, GWL_STYLE) & ~WS_MAXIMIZEBOX);
	SetWindowLong(hConsole, GWL_STYLE, GetWindowLong(hConsole, GWL_STYLE)&~WS_SIZEBOX);
}

//DRAW BMP FILE
void readBmpHeader(FILE *f, BmpHeader &header)
{
	if (f == NULL)
		return;

	fseek(f, 0, 0L);
	fread(&header, sizeof(BmpHeader), 1, f);
}
void readBmpDib(FILE *f, BmpDib &dib)
{
	if (f == NULL)
		return;

	fseek(f, sizeof(BmpHeader), 0L);
	fread(&dib, sizeof(BmpDib), 1, f);
}
void readBmpPixelArray(FILE *f, BmpHeader header, BmpDib dib, PixelArray &data)
{
	if (f == NULL)
		return;

	data.rowCount = dib.imageHeight;
	data.columnCount = dib.imageWidth;
	data.pixels = new Color*[data.rowCount];

	char paddingCount = (4 - (dib.imageWidth * (dib.pixelSize / 8) % 4)) % 4;

	fseek(f, header.dataOffset, 0L);

	for (int i = 0; i < data.rowCount; i++)
	{
		scanBmpPixelLine(f, data.pixels[data.rowCount - i - 1], dib.imageWidth);
		skipBmpPadding(f, paddingCount);
	}
}
void scanBmpPixelLine(FILE *f, Color *&line, uint32_t length)
{
	if (f == NULL)
		return;

	line = new Color[length];
	fread(line, sizeof(Color), length, f);
}
void skipBmpPadding(FILE *f, char count)
{
	if (f == NULL)
		return;

	if (count == 0)
		return;

	char padding[3];
	fread(padding, count, 1, f);
}
void drawBmp(BmpDib dib, PixelArray data)
{
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);

	for (int i = 0; i < dib.imageHeight; i++)
		for (int j = 0; j < dib.imageWidth; j++)
		{
			if (data.pixels[i][j].blue != 0 && data.pixels[i][j].red != 0 && data.pixels[i][j].green != 0)
			{
				Color pixel = data.pixels[i][j];
				SetPixelV(hdc, j + 75, i + 100, RGB(pixel.red, pixel.green, pixel.blue));
			}
		}
}
void releaseBmpPixelArray(PixelArray data)
{
	for (int i = 0; i < data.rowCount; i++)
		delete[]data.pixels[i];

	delete[]data.pixels;
}

//END DRAW BMP FUNCTIONS

void drawImage(char *fileName)
{
	FILE *fbmp1;
	BmpHeader Header;
	BmpDib Dib;
	PixelArray Data;

	//open file BMP
	fbmp1 = fopen(fileName, "rb+");

	//read bmp info
	readBmpHeader(fbmp1, Header);
	readBmpDib(fbmp1, Dib);
	readBmpPixelArray(fbmp1, Header, Dib, Data);
	drawBmp(Dib, Data);
	releaseBmpPixelArray(Data);

	fclose(fbmp1);
}
void drawGuideGame()
{
	SetWindowSize(25, 15);
	SetColor(15);
	SetConsoleFontSize({ 16, 16 });
	char *guide = "\t\tGameplay\nThere are essentially 4 actions you can take in Minesweeper :\n 1 . Open a square : Left clicking on a square.\n 2 . Marking a square as a mine : Right clicking on a square\n 3 . Marking a square with a question mark : Right clicking twice on a square, \nor right clicking once on a square that's already marked as a mine. \n 4 . Clear any marks.Again, right click on the square.\nRight clicking cycles through the following states : \n + Bomb, Question Mark, Clear.\n";
	printf("%s", guide);
	SetColor(14);
	printf("\nClick on tiles to play game\n");
	SetColor(11);
	printf("\nPress any key to start game\n");
	SetColor(15);
	getch();
}