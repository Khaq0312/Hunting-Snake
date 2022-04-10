#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <conio.h>
#include "Header.h"
#include "graphic.h"
using namespace std;
int LEVEL;
int OBScount = 0;
int SCORE;
POINT snake[MAX_SIZE_SNAKE];
POINT food[MAX_SIZE_FOOD];
POINT OBSTACLE[100];
int CHAR_LOCK;
int MOVING;
int SPEED;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE = 1;
HIGHSCORE highscore[MAX_HIGH_SCORE];
int nHighscore = 0;
string filegame[MAX_FILE_GAME];
int nFileGame = 0;
POINT trigger = { 0,0 };
int triggerCount;
POINT TRIGGER[5];
int SEED;
bool stop_thread;
//Function pause game
void PauseGame(HANDLE t) {
	SuspendThread(t);
}
// doc danh sach ten cac file luu data game cua nguoi choi
void readFileName() {
	ifstream fIn;
	fIn.open("FileGame.txt", ios::beg);
	while (!fIn.eof() && nFileGame < MAX_FILE_GAME) {
		getline(fIn, filegame[nFileGame++]);
	}
	fIn.close();
}
void readHighScore() {
	ifstream fIn;
	string sTemp;
	fIn.open("HighScore.txt", ios::beg);
	while (!fIn.eof() && nHighscore < MAX_HIGH_SCORE) {
		getline(fIn, highscore[nHighscore].name);
		fIn >> highscore[nHighscore].score;
		getline(fIn, sTemp);
		nHighscore++;
	}
	fIn.close();
}
bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		if (snake[i].x == x && snake[i].y == y)
			return false;
	}
	for (int i = 0; i < OBScount; i++) {
		if (OBSTACLE[i].x == x && OBSTACLE[i].y == y)
			return false;
	}
	for (int i = 0; i < triggerCount; i++) {
		if (TRIGGER[i].x == x && TRIGGER[i].y == y)
			return false;
	}
	return true;
}
void GenerateFood() {
	int x, y;
	srand(SEED);
	for (int i = 0; i < (LEVEL - 1) * 5 + LEVEL; i++) {
		rand(); rand();
	}
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do {
			x = rand() % (WIDTH_CONSOLE - 1) + 1;
			y = rand() % (HEIGH_CONSOLE - 1) + 1;
		} while (IsValid(x, y) == false);
		food[i] = { x,y };
	}
}
void readData(string filePath) {
	ifstream fIn;
	fIn.open(filePath, ios::beg);
	fIn >> SEED;
	while (!fIn.eof()) {
		fIn >> CHAR_LOCK;
		fIn >> MOVING;
		fIn >> SPEED >> LEVEL >> FOOD_INDEX >> SIZE_SNAKE >> SCORE;
		for (int i = 0; i < SIZE_SNAKE; i++) {
			fIn >> snake[i].x >> snake[i].y;
		}
		fIn >> triggerCount;
	}
	fIn.close();
	srand(SEED);
	OBScount = 0;
}
void loadGame(string filePath) {
	PlaySound(NULL, NULL, SND_ASYNC);
	system("cls");
	readData(filePath);
	drawGame();
	drawLevel();
	if (triggerCount == 5) {
		triggerCount = 0;
		DrawGateIn();
		if (LEVEL != 5) {
			setColor(BG_COLOR, 9);
			drawPointOut();
			setColor(BG_COLOR, 7);
		}
	}
	else {
		trigger = { 0,0 };
		triggerCount = 0;
		GenerateFood();
	}	
	STATE = 1;
}
void Eat() {
	PlaySound(TEXT("eat_food.wav"), NULL, SND_ASYNC);
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1) {
		FOOD_INDEX = 0;
		SIZE_SNAKE++;
		DrawGateIn();
		// diem ra
		if (LEVEL != 5) {
			setColor(BG_COLOR, 9);
			drawPointOut();
			setColor(BG_COLOR, 7);
		}
	}
	else {
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
	SCORE++;
}
void ResetData() {
	PlaySound(NULL, NULL, SND_ASYNC);
	SEED = (unsigned)time(NULL);
	srand(SEED);
	//Initialize the global values
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, SIZE_SNAKE = 3, SCORE = 0, LEVEL = 1,
		OBScount = 0, triggerCount = 0, trigger = { 0,0 };
	// Initialize default values for snake
	snake[0] = { 10, 1 }; snake[1] = { 11, 1 };
	snake[2] = { 12, 1 };
	GenerateFood();//Create food array
}
bool TouchOBS(int x, int y) {
	for (int i = 0; i < OBScount; ++i) {
		if (x == OBSTACLE[i].x && y == OBSTACLE[i].y)
			return false;
	}
	for (int i = 0; i < triggerCount; ++i) {
		if (x == TRIGGER[i].x && y == TRIGGER[i].y)
			return false;
	}
	return true;
}
bool TouchBody(int x, int y) {
	for (int i = SIZE_SNAKE - 3; i >= 0; --i) {
		if (x == snake[i].x && y == snake[i].y)
			return false;
	}
	return true;
}
void MoveRight() {
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE || TouchBody(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == 0
		|| TouchOBS(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == 0) {
		ProcessDead();
	}
	else if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
		Eat();
	}
	else if (triggerCount == 1 && snake[0].x == WIDTH_CONSOLE - 10 && snake[0].y == 1) {
		clearGate();
	}
	else {
		for (int i = 0; i < SIZE_SNAKE - 1; ++i) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x++;
	}
}
void MoveLeft() {
	if (snake[SIZE_SNAKE - 1].x - 1 == 0 || TouchBody(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == 0
		|| TouchOBS(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == 0) {
		ProcessDead();
	}
	else if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
		Eat();
	}
	else if (triggerCount == 1 && snake[0].x == WIDTH_CONSOLE - 10 && snake[0].y == 1) {
		clearGate();
	}
	else {
		for (int i = 0; i < SIZE_SNAKE - 1; ++i) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x--;
	}
}
void MoveUp() {
	if (snake[SIZE_SNAKE - 1].y - 1 == 0 || TouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1) == 0
		|| TouchOBS(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1) == 0) {
		ProcessDead();
	}
	else if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y) {
		Eat();
	}
	else if (triggerCount == 1 && snake[0].x == WIDTH_CONSOLE - 10 && snake[0].y == 1) {
		clearGate();
	}
	else {
		for (int i = 0; i < SIZE_SNAKE - 1; ++i) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y--;
	}
}
void MoveDown() {
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE || TouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1) == 0
		|| TouchOBS(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1) == 0) {
		ProcessDead();
	}
	else if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y) {
		Eat();
	}
	else if (trigger.x != 0 && trigger.y != 0 && snake[SIZE_SNAKE - 1].x == trigger.x && snake[SIZE_SNAKE - 1].y + 1 == trigger.y) {
		moveGate();
	}
	else if (triggerCount == 1 && snake[0].x == WIDTH_CONSOLE - 10 && snake[0].y == 1) {
		clearGate();
	}
	else {
		for (int i = 0; i < SIZE_SNAKE - 1; ++i) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y++;
	}
}
void moveGate() {
	PlaySound(TEXT("level_completed.wav"), NULL, SND_ASYNC);
	for (int i = 0; i < SIZE_SNAKE - 1; ++i) {
		snake[i].x = snake[i + 1].x;
		snake[i].y = snake[i + 1].y;
	}
	if (LEVEL == MAX_LEVEL) {
		LEVEL = 1;
		SPEED = SPEED - 3; // tang 1 so voi speed ban dau cua vong truoc
		snake[2] = { trigger.x, trigger.y };
		snake[1] = { trigger.x, trigger.y - 1 };
		snake[0] = snake[SIZE_SNAKE - 3];
		SIZE_SNAKE = 3;
		OBScount = 0; triggerCount = 0; trigger = { 0,0 };
		eraseDrawing({ 1,1 }, { WIDTH_CONSOLE - 1,HEIGH_CONSOLE - 1 });
		GenerateFood();
		SCORE += 10;
		return;
	}
	else {
		snake[SIZE_SNAKE - 1] = { WIDTH_CONSOLE - 10,1 };
		LEVEL++; SPEED++;
		SCORE += 5;
		drawLevel();
	}
	triggerCount = 1; // Dieu kien gia dinh de xac nhan ran da di qua cong
}
void clearGate() {
	triggerCount = 0;
	setColor(BG_COLOR, 0);
	drawHorLine(219, 3, trigger.x - 1, trigger.y + 1);
	drawHorLine(219, 3, trigger.x - 1, trigger.y);
	setColor(BG_COLOR, 14);
	drawPointOut();
	setColor(BG_COLOR, 7);
	trigger = { 0,0 };
	if (LEVEL != 5) {
		OBScount = 0;
		drawLevel();
	}
	GenerateFood();
}
void ThreadFunc() {
	int x = (WIDTH_WINDOW - 40) / 2, y = HEIGH_CONSOLE + 5;
	while (!stop_thread) {
		if (STATE) {
			ClearSnake();
			ClearFood();
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
			if (trigger.x == 0 && trigger.y == 0) DrawFood();
			setColor(BG_COLOR, 10);
			DrawSnake();
			setColor(BG_COLOR, 7);
			gotoXY(x + 10, y);
			cout << LEVEL;
			gotoXY(x + 37, y);
			cout << SCORE;
			Sleep(600 / (SPEED + 1));
		}
	}
}
int checkFileGame(string filenametemp) {//Kiem tra file game co bi trung ten hay khong
	for (int i = 0; i < nFileGame; ++i) {
		if (filenametemp.compare(filegame[i]) == 0) return i;
	}
	return -1;
}
void changeNewFileToLast(int index) {
	string temp = filegame[index];
	for (int i = index; i < nFileGame - 1; i++) {
		filegame[i] = filegame[i + 1];
	}
	filegame[nFileGame - 1] = temp;
}
void saveFileNamesToFile() {
	ofstream fOut;
	fOut.open("FileGame.txt", ios::ate);
	for (int i = 0; i < nFileGame - 1; i++) {
		fOut << filegame[i] << endl;
	}
	fOut << filegame[nFileGame - 1];
	fOut.close();
}
int checkPlayerName(string name) {
	for (int i = 0; i < nHighscore; ++i) {
		if (name.compare(highscore[i].name) == 0) return 1;
	}
	return 0;
}
//Doi vi tri thu tu diem
void swapScore(int& x, int& y) {
	x = x + y;
	y = x - y;
	x = x - y;
}
//Doi vi tri thu tu nguoi choi
void swapName(string& x, string& y) {
	string temp = x;
	x = y;
	y = temp;
}
//Kiem tra diem co nam trong top 5 khong
int checkScore() {
	if (SCORE == 0) return 0;
	if (nHighscore < MAX_HIGH_SCORE) return 1;
	for (int i = 0; i < nHighscore; ++i) {
		if (SCORE > highscore[i].score) return -1;
	}
	return 0;
}
//sap xep thu tu diem cao den thap
void sortListHighScore() {
	for (int i = 0; i < nHighscore - 1; ++i) {
		for (int j = i + 1; j < nHighscore; ++j) {
			if (highscore[i].score < highscore[j].score) {
				swapScore(highscore[i].score, highscore[j].score);
				swapName(highscore[i].name, highscore[j].name);
			}
		}
	}
}
//Luu list high score vao file
void WriteHighScore() {
	ofstream fOut;
	fOut.open("HighScore.txt", ios::ate);
	for (int i = 0; i < nHighscore - 1; ++i) {
		fOut << highscore[i].name << endl << highscore[i].score << endl;
	}
	fOut << highscore[nHighscore - 1].name << endl << highscore[nHighscore - 1].score;
	fOut.close();
}
void SaveGame(string filePath) {
	ofstream fOut;
	fOut.open(filePath);
	fOut << SEED << endl;
	fOut << CHAR_LOCK << endl;
	fOut << MOVING << endl;
	fOut << SPEED << endl;
	fOut << LEVEL << endl;
	fOut << FOOD_INDEX << endl;
	fOut << SIZE_SNAKE << endl;
	fOut << SCORE << endl;
	for (int i = 0; i < SIZE_SNAKE; ++i) {
		fOut << snake[i].x << " " << snake[i].y << endl;
	}
	fOut << triggerCount;
	fOut.close();
}
void StartGame(string f) {
	system("cls");
	ResetData();
	drawGame();
	STATE = 1;
}
void ProcessDead() {
	STATE = 0;
	PlaySound(TEXT("game_over.wav"), NULL, SND_ASYNC);
	for (int i = 1; i <= 15; i++) {
		setColor(BG_COLOR, i);
		DrawSnake();
		Sleep(40);
	}
	gotoXY((WIDTH_CONSOLE - 26) / 2, HEIGH_CONSOLE + 1);
	setColor(BG_COLOR, 8);
	cout << "Press 'Enter' to continue";
}
int processGame(string file, void (*mode)(string)) {
	thread t1(ThreadFunc);
	HANDLE handle_t1 = t1.native_handle();
	stop_thread = 0;
	bool NOTI = 0;
	int temp;
	while (1) {
		temp = toupper(_getch());
		if (STATE == 1) {
			if (temp == 'P') {
				PauseGame(handle_t1);
				drawPauseGameNoti();
				NOTI = 1;
			}
			else if (temp == 27) {
				system("cls");
				STATE = 0;
				stop_thread = 1;
				t1.join();
				return 0;
			}
			else if (temp == 'L') {
				PauseGame(handle_t1);
				SaveGame(inputFileName());
				NOTI = 1;
			}
			else {
				if (NOTI) {
					reDrawGamePlay();
					NOTI = 0;
				}
				ResumeThread(handle_t1);
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp ==
					'W' || temp == 'S')) {
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else {
			if (temp == 13) {
				if (drawGameOver()) {
					mode(file);
				}
				else {
					system("cls");
					stop_thread = 1;
					t1.join();
					return 0;
				}
			}
		}
	}
}
int main() {
	readFileName();
	readHighScore();
	setWindowSize(WIDTH_WINDOW, HEIGHT_WINDOW);
	setScreenBufferSize(WIDTH_WINDOW, HEIGHT_WINDOW);
	fixConsoleWindow();
	showConsoleCursor(false);
	while (1) {
		PlaySound(TEXT("snake_music.wav"), NULL, SND_ASYNC | SND_LOOP);
		int choice = menu();
		int temp;
		while (1) {
			if (choice == 0) {
				StartGame(" ");
				if (!processGame(" ", StartGame)) break;
			}
			else if (choice == 1) {
				string file = chooseFileLoadGame();
				if (file == " ") break;
				loadGame(file);
				if (!processGame(file, loadGame)) break;
			}
			else if (choice == 2) {
				drawHighScore();
				temp = _getch();
				if (temp == 27 || temp == 13) break;
			}
			else if (choice == 3) {
				drawAboutUs();
				temp = _getch();
				if (temp == 27 || temp == 13) break;
			}
			else if (choice == 4) {
				system("cls");
				return 0;
			}
		}
	}
}