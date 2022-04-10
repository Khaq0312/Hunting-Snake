#pragma once
#include <Windows.h>
#include <string>
struct HIGHSCORE {
	std::string name;
	int score = 0;
};
#define BG_COLOR 0
#define HEIGHT_WINDOW 30
#define WIDTH_WINDOW 110
#define HEIGH_CONSOLE 20
#define WIDTH_CONSOLE 70
#define MAX_FILE_GAME 5
#define MAX_HIGH_SCORE 5
#define MAX_SIZE_SNAKE 28
#define MAX_SIZE_FOOD 5
#define MAX_LEVEL 5
extern int SIZE_SNAKE, FOOD_INDEX, LEVEL, SCORE, nFileGame, nHighscore, OBScount, triggerCount, SEED;
extern std::string filegame[MAX_FILE_GAME];
extern HIGHSCORE highscore[MAX_HIGH_SCORE];
extern POINT OBSTACLE[100], TRIGGER[5], snake[MAX_SIZE_SNAKE], food[MAX_SIZE_FOOD], trigger;

void PauseGame(HANDLE t);
void showConsoleCursor(bool showFlag);
void fixConsoleWindow();
void setScreenBufferSize(SHORT width, SHORT height);
void setWindowSize(SHORT width, SHORT height);
void setColor(int background_color, int text_color);
void gotoXY(int x, int y);
void drawBoard(int x, int y, int width, int height);
void drawVerLine(int c, int length, int x, int y);
void drawHorLine(int c, int length, int x, int y);
void drawArtSnake(int x, int y);
int menu();
void eraseDrawing(POINT start, POINT end);
void drawGame();
void drawArtVideoGame(int x, int y);
std::string chooseFileLoadGame();
void drawArtTrophy(int x, int y);
void drawHighScore();
void drawAboutUs();
void drawVerOBS(int c, int length, int x, int y, int& count, POINT* a);
void drawHorOBS(int c, int length, int x, int y, int& count, POINT* a);
void Level2();
void Level3();
void Level4();
bool IsValidOBS(int x, int y, int mode);
void Level5();
void drawLevel();
bool IsValidGate(int x, int y);
void DrawGateIn();
void drawPointOut();
void DrawSnake();
void DrawFood();
void ClearSnake();
void ClearFood();
void drawPauseGameNoti();
int chooseYesNo(int x, int y);
int drawGameOver();
void inputHighscoreInfo();
void reDrawGamePlay();
int checkScore();
int checkFileGame(std::string filenametemp);
void changeNewFileToLast(int index);
void saveFileNamesToFile();
std::string inputFileName();
void sortListHighScore();
void WriteHighScore();
int checkPlayerName(std::string name);