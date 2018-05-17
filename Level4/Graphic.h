#pragma once
#include <stdint.h>

#pragma pack(1)
struct BmpSignature
{
	unsigned char data[2];
};

struct BmpHeader
{
	BmpSignature signature;
	uint32_t fileSize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t dataOffset;
};

struct BmpDib
{
	uint32_t dibSize;
	int32_t	 imageWidth;
	int32_t  imageHeight;
	uint16_t colorPlaneCount;
	uint16_t pixelSize;
	uint32_t compressMethod;
	uint32_t bitmapByteCount;
	int32_t  horizontalResolution;
	int32_t  verticalResolution;
	uint32_t colorCount;
	uint32_t importantColorCount;
};

struct Color
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

struct ColorTable
{
	Color	 *colors;
	uint32_t length;
};

struct PixelArray
{
	Color	 **pixels;
	uint32_t rowCount;
	uint32_t columnCount;
};

void readBmpHeader(FILE *f, BmpHeader &header);

void readBmpDib(FILE *f, BmpDib &dib);

void readBmpPixelArray(FILE *f, BmpHeader header, BmpDib dib, PixelArray &data);
void scanBmpPixelLine(FILE *f, Color *&line, uint32_t length);
void skipBmpPadding(FILE *f, char count);

void drawBmp(BmpDib dib, PixelArray data);
void releaseBmpPixelArray(PixelArray data);

/* go to position (X,Y) on console */
bool SetXY(int x, int y);

bool SetConsoleFontSize(COORD dwFontSize);

/*Set color for text*/
void SetColor(WORD color);

/*Draw Button*/
void drawButton(int x, int y, char *buttonText);

/*Draw rectangle */
void DrawRect(int x, int y, int width, int height, COORD **&block);

/* Set console/screen  size */
void SetConsoleWindowSize(int x, int y);
void ShowLastSystemError();
void SetWindowSize(int width, int height);
void FixedConsoleSize();

/*Draw BMP Image*/
void drawImage(char *fileName);

/*Draw the game guide*/
void drawGuideGame();