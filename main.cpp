#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100];
int tailY[100];
int nTail;
int speed = 10;
enum eDirection
{
	STOP = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

eDirection dir;

void getFruitX ()
{
	fruitX = 1 + rand() % (width - 2);
}

void getFruitY ()
{
	fruitY = 1 + rand() % (height - 2);
}

void Setup()
{
	gameOver = false;
	dir = STOP;
	x = width / 2 - 1;
	y = height / 2 - 1;

	getFruitX();
	getFruitY();
		
	for (int i = 0; i < 100; i++)
	{
		nTail = 0;
		tailX[i] = 0;
		tailY[i] = 0;
	}
	score = 0;
}

void Draw()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, { 0, 0 });
	
	SetConsoleTextAttribute(hConsole, 15);
	for (int i = 0; i < width; i++)
	{
		printf("#");
	}
	cout << endl;
	
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 0; j < width; j++)
		{
			bool empty = true;
			if (j == 0 || j == width - 1 && empty == true)
			{
				SetConsoleTextAttribute(hConsole, 15);
				printf ("#");
				empty = false;
			}
			
			
			if( i == y && j == x && empty == true)
			{
				SetConsoleTextAttribute(hConsole, 10);
				printf ("O");
				empty = false;
			}
			
			if (i == fruitY && j == fruitX && empty == true)
			{
				if( fruitY != y || fruitX != x)
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "F";
					empty = false;
				}
			}	
			
			for (int k = 0; k < nTail; k++)
			{
				if (tailY[k] == i && tailX[k] == j && empty == true)
				{
					SetConsoleTextAttribute(hConsole, 10);
					
					if(k == nTail - 1)
						SetConsoleTextAttribute(hConsole, 12);
						
					printf("o");
					empty = false;
				}
					
			}
			
			if  (empty == true)
				printf(" ");
		}
		printf("\n");
		
	}
	for (int i = 0; i < width; i++)
	{
		printf("#");
	}
	printf("\nScore %i\n", score);
	printf("Speed %i\n", speed);
	printf("\nWASD - control\n");
	printf("[ ] - speed");
	printf("\nWASD - control\n");
}

void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
			case 'a':
				if (dir != RIGHT)
					dir = LEFT;
				break;
			case 'd':
				if (dir != LEFT)
					dir = RIGHT;
				break;
			case 'w':
				if (dir != DOWN)
					dir = UP;
				break;
			case 's':
				if (dir != UP)
					dir = DOWN;
				break;
			case 'x':
				gameOver = true;
				break;
			case ']':
				if (speed <= 80)
					speed += 10;
				break;
			case '[':
				if (speed >= 20)
					speed -= 10;
				break;
		}
	}
}

void Logic()
{
	if(x == fruitX && y == fruitY)
	{
		score = score + 10 + ((speed - 10)  /10);

		getFruitX();
		getFruitY();
		
		int n = 0;
		for (int k = 0; k < nTail; k++)
		{
			if (tailY[k] == fruitY && tailX[k] == fruitX)
			while (tailY[k] == fruitY && tailX[k] == fruitX)
			{
				getFruitX();
				getFruitY();
				k = 0;
				
				n++;
				if (n > 15)
				{
					gameOver = true;
					break;
				}
			}
		}		
		nTail++;
	}
	
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
		
		switch(dir)
	{
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case DOWN:
			y++;
			break;
		case UP:
			y--;
			break;	
	}
	
	if (x >= width - 1 || x <= 0 || y >= height - 1 || y <= 0)
		gameOver = true;
		
		
	for (int i = 1; i < nTail; i++)
	{
		if (tailX[i] == x && tailY[i] == y)
		{
			gameOver = true;
		}
	}
	
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

int main() 
{
	Setup();
	ShowConsoleCursor(false);
	system("cls");
	while (!gameOver)
	{
		Sleep(100 - speed);
		Input();
		Logic();
		Draw();
	
	}
	
	if (gameOver)
	{
		system("cls");
		cout<<"Your score "<< score << endl;
		gameOver = false;	
	}
	
	printf("\nPress 'r' to restart.\n");
	printf("Press any button to exit.\n");
	if (_getch() == 'r')
	{
		main();
	}
	return 0;
}
