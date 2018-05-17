#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MineGenerator.h"

void mineGenerator(int **&m, int &width, int &height, int &mines)
{
	int i, j;
	srand(time(NULL)); //start the random function
	printf("Nhap chieu dai va chieu rong :");
	scanf("%d %d", &width, &height);
	printf("Nhap so bom :");
	scanf("%d", &mines);
	//initalize matrix
	m = new int *[height];
	for (i = 0; i < height; i++)
	{
		m[i] = new int[width];
		for (j = 0; j < width; j++)
			m[i][j] = 0;
	}
	int totalMines = 0;	//count total randomized mines
	int random = 1 + ((height>width) ? height : width) / 10 + mines / 20;
	while (totalMines < mines) 
	{
		for (i = 0; i < height; i += 1 + rand() % random)
		for (j = 0; j < width; j += 1 + rand() % random)	//increase i,j by random number to make sure the randomly distribution
		{
			while (i >= height)
			{
				i--;
			}
			while (j >= width)		//i,j might exceed (height,width) by random 
			{
				j--;
			}

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
	}
	setMatrix: for (i = 0; i < height;i++)
		for (j = 0; j < width; j++)
		{
			if (m[i][j] == 9)
			{
				//check 8 tiles around mine-tiled

				if (i - 1 >= 0 && j - 1 >= 0 && m[i-1][j-1]!=9) m[i - 1][j - 1]++;
				if (i - 1 >= 0 && m[i - 1][j]!=9 ) m[i - 1][j]++;
				if (i - 1 >= 0 && j + 1 < width && m[i - 1][j + 1] !=9) m[i - 1][j + 1]++;
				if (j - 1 >= 0 && m[i][j - 1]!=9) m[i][j - 1]++;
				if (j + 1 < width && m[i][j + 1]!=9) m[i][j + 1]++;
				if (i + 1 < height && j - 1 >= 0 && m[i + 1][j - 1]!=9) m[i + 1][j - 1]++;
				if (i + 1 < height && m[i + 1][j]!=9) m[i + 1][j]++;
				if (i + 1 < height && j + 1<width && m[i + 1][j + 1]!=9) m[i + 1][j + 1]++;
			}
		}
		
}