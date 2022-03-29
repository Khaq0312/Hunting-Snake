#pragma once
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

void StartGame();
bool IsValid(int x, int y);
void GenerateFood();
void ResetData();
void StartGame();
void DrawBoard(int x, int y, int width, int height);
void PauseGame(HANDLE t);
void ExitGame(HANDLE t);
void ThreadFunc();
void gotoXY(int x, int y);
void Level2(int& count);
void Level3(int& count);
void Level4(int& count);
void Level5(int& count);
void DrawGate(int& count);