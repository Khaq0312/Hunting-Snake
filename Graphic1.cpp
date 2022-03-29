#include <Windows.h>
#include "Header.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <fstream>
using namespace std;
#define MAX_SIZE_SNAKE 28
#define MAX_SIZE_FOOD 5
#define MAX_LEVEL 5
#define MAX_HIGH_SCORE 5
#define MAX_FILE_GAME 5
static int HEIGHT_WINDOW = 100, WIDTH_WINDOW = 200;
extern int LEVEL;
extern int SCORE;
#define MAX_SPEED 3
POINT snake[10];
POINT food[5];
POINT OBSTACLE[100];
int CHAR_LOCK;
int MOVING;
int SPEED;
static int HEIGH_CONSOLE = 1000, WIDTH_CONSOLE = 2000;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE;
static char Snake[] = { '0' , '1' , '2', '3', '4', '5', '6', '7', '8', '9' };
//Ham co san
void ProcessDead() {
	STATE = 0;
	gotoXY(0, HEIGH_CONSOLE + 2);
	printf("Dead, type y to continue or anykey to exit");
}
void Eat() {
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
	{
		FOOD_INDEX = 0;
		SIZE_SNAKE = 6;
		if (SPEED == MAX_SPEED) SPEED = 1;
		else SPEED++;
		GenerateFood();
	}
	else {
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
}
void ResetData() {
	//Initialize the global values
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, WIDTH_CONSOLE = 70,
		HEIGH_CONSOLE = 20, SIZE_SNAKE = 6;
	// Initialize default values for snake
	snake[0] = { 10, 1 }; snake[1] = { 11, 1 };
	snake[2] = { 12, 1 }; snake[3] = { 13, 1 };
	snake[4] = { 14, 1 }; snake[5] = { 15, 1 };
	GenerateFood();//Create food array
}
void GenerateFood() {
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do {
			x = rand() % (WIDTH_CONSOLE - 1) + 1;
			y = rand() % (HEIGH_CONSOLE - 1) + 1;
		} while (IsValid(x, y) == false);
		food[i] = { x,y };
	}
}
//Ham moi/chinh sua
void gotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void drawVerOBS(char c, int length, int x, int y, int& count) {
	for (int i = 0; i < length; i++) {
		gotoXY(x, y + i);
		cout << c;
		OBSTACLE[count] = { x,y + i };
		count++;
	}
}
void drawHorOBS(char c, int length, int x, int y, int& count) {
	for (int i = 0; i < length; i++) {
		gotoXY(x + i, y);
		cout << c;
		OBSTACLE[count] = { x,y + i };
		count++;
	}
}
void Level2(int& count)
{
	drawHorOBS(220, 5, WIDTH_CONSOLE / 2 - 2, HEIGH_CONSOLE / 2, count);
	drawVerOBS(219, 5, WIDTH_CONSOLE / 2, HEIGH_CONSOLE / 2 - 2, count);
}
void Level3(int& count)
{
	drawVerOBS(219, 5, 3, 3, count);
	drawHorOBS(220, 5, 3, 3, count);
	drawVerOBS(219, 5, WIDTH_CONSOLE - 3, 3, count);
	drawHorOBS(220, 5, WIDTH_CONSOLE - 7, 3, count);
	drawHorOBS(220, 5, 3, HEIGH_CONSOLE - 3, count);
	drawVerOBS(219, 5, 3, HEIGH_CONSOLE - 7, count);
	drawHorOBS(220, 5, WIDTH_CONSOLE - 7, HEIGH_CONSOLE - 3, count);
	drawVerOBS(219, 5, WIDTH_CONSOLE - 3, HEIGH_CONSOLE - 7, count);
}
void Level4(int& count)
{
	int temp = rand() % 8;
	drawVerOBS(219, temp, WIDTH_CONSOLE / 2, 1, count);
	drawHorOBS(220, temp, 1, HEIGH_CONSOLE / 2, count);
	drawVerOBS(219, temp, WIDTH_CONSOLE / 2, HEIGH_CONSOLE - temp, count);
	drawHorOBS(220, temp, WIDTH_CONSOLE - temp, HEIGH_CONSOLE / 2, count);
}
void Level5(int& count)
{
	for (int i = 0; i < 2; i++)
	{
		drawVerOBS(219, 5, rand() % (WIDTH_CONSOLE - 5), rand() % (HEIGH_CONSOLE - 5), count);
		drawHorOBS(220, 5, rand() % (WIDTH_CONSOLE - 5), rand() % (HEIGH_CONSOLE - 5), count);
	}
}
void MoveRight() {
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE
			- 1].y == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			gotoXY(snake[i].x, snake[i].y);
			cout << ' ';
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x++;
	}
}
void MoveLeft() {
	if (snake[SIZE_SNAKE - 1].x - 1 == 0) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE
			- 1].y == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			gotoXY(snake[i].x, snake[i].y);
			cout << ' ';
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x--;
	}
}
void MoveDown() {
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE -
			1].y + 1 == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			gotoXY(snake[i].x, snake[i].y);
			cout << ' ';
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y++;
	}
}
void MoveUp() {
	if (snake[SIZE_SNAKE - 1].y - 1 == 0) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE -
			1].y - 1 == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			gotoXY(snake[i].x, snake[i].y);
			cout << ' ';
			snake[i].x = snake[i + 1].x; snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y--;
	}
}
void DrawBoard(int x, int y, int width, int height) {
	int count = 0;
	gotoXY(x, y);
	cout << (char)220;
	for (int i = 1; i < width; i++)cout << (char)220;
	cout << (char)220;
	gotoXY(x, height + y); cout << (char)223;
	for (int i = 1; i < width; i++)cout << (char)223;
	cout << (char)223;
	for (int i = y + 1; i < height + y; i++) {
		gotoXY(x, i); cout << (char)219;
		gotoXY(x + width, i); cout << (char)219;
	}
	Level2(count);
	Level3(count);
	Level4(count);
	Level5(count);
	DrawGate(count);
	gotoXY(0, 0);
}
bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		if ((snake[i].x == x && snake[i].y == y) || (OBSTACLE[i].x == x && OBSTACLE[i].y == y))
			return false;
		return true;
	}
}
void DrawGate(int& count)
{
	POINT trigger;
	do
	{
		trigger.x = rand() % WIDTH_CONSOLE;
		trigger.y = rand() % HEIGH_CONSOLE;
	} while (!IsValid(trigger.x, trigger.y));
	gotoXY(trigger.x, trigger.y);
	cout << 'x';
	drawHorOBS(219, 3, trigger.x - 1, trigger.y + 1, count);
	drawVerOBS(220, 1, trigger.x - 1, trigger.y, count);
	drawVerOBS(220, 1, trigger.x + 1, trigger.y, count);
}
void DrawSnake(char str[]) {
	cout << ' ';
	for (int i = 0; i < SIZE_SNAKE; i++) {
		gotoXY(snake[i].x, snake[i].y);
		cout << str[i];
	}
}
void DrawFood(char f)
{
	gotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	cout << f;
}
void ThreadFunc() {
	while (1) {
		if (STATE == 1) {//If my snake is alive
			DrawFood('F');
			switch (MOVING) {
			case 'A':
				MoveLeft();
				break;
			case 'D':
				MoveRight();
				break;
			case 'W':
				MoveUp();
				break;
			case 'S':
				MoveDown();
				break;
			}
			DrawSnake(Snake);
			Sleep(1000 / SPEED);//Sleep function with SPEEED variable
		}
	}
}
