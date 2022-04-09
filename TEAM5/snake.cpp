#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <string>
#include "Header.h"
using namespace std;
struct HIGHSCORE {
	string name;
	int score = 0;
};
#define MAX_SIZE_SNAKE 28
#define MAX_SIZE_FOOD 5
#define MAX_LEVEL 5
#define MAX_HIGH_SCORE 5
#define MAX_FILE_GAME 5
#define BG_COLOR 0
static int HEIGHT_WINDOW = 30, WIDTH_WINDOW = 110;
static int HEIGH_CONSOLE = 20, WIDTH_CONSOLE = 70;
int LEVEL;
int OBScount = 0;
int SCORE;
POINT snake[MAX_SIZE_SNAKE];
POINT food[5];
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
bool NOTI = 0;
atomic_bool stop_thread = 0;
//Function exit game
void ExitGame(HANDLE t) {
	system("cls");
	TerminateThread(t, 0);
}
//Function pause game
void PauseGame(HANDLE t) {
	SuspendThread(t);
}
// Chuc nang: an/hien con tro chuot
void showConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}
void fixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void setScreenBufferSize(SHORT width, SHORT height) {
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size;
	size.X = width;
	size.Y = height;
	SetConsoleScreenBufferSize(output, size);
}
void setWindowSize(SHORT width, SHORT height) {
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT size;
	size.Top = 0;
	size.Left = 0;
	size.Right = width - 1;
	size.Bottom = height - 1;
	SetConsoleWindowInfo(output, 1, &size);
}
void gotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void drawBoard(int x, int y, int width, int height) {
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
	gotoXY(0, 0);
}
void setColor(int background_color, int text_color) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	int color_code = background_color * 16 + text_color;
	SetConsoleTextAttribute(hStdout, color_code);
}
// Chuc nang: ve duong thang dung
// Tham so truyen vao: ki tu, chieu dai cua doan thang, toa do bat dau
void drawVerLine(int c, int length, int x, int y) {
	for (int i = 0; i < length; i++) {
		gotoXY(x, y + i);
		cout << (char)c;
	}
}
void drawHorLine(int c, int length, int x, int y) {
	for (int i = 0; i < length; i++) {
		gotoXY(x + i, y);
		cout << (char)c;
	}
}
void drawArtSnake(int x, int y) {
	for (int i = 0; i < 10; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0: cout << "   __"; break;
		case 1: cout << "  {0O}"; break;
		case 2: cout << "  \\__/"; break;
		case 3: cout << "  /^/"; break;
		case 4: cout << " ( ("; break;
		case 5: cout << " \\_\\_____"; break;
		case 6: cout << " (_______)"; break;
		case 7: cout << "(_________()Oo"; break;
		}
		y++;
	}
}
void drawMenu() {
	system("cls");
	setColor(BG_COLOR, 14);
	// draw title
	int x = (WIDTH_WINDOW - 100) / 2, y = 4;
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < 6; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0:
			wcout << L"██╗░░██╗██╗░░░██╗███╗░░██╗████████╗██╗███╗░░██╗░██████╗░   ░██████╗███╗░░██╗░█████╗░██╗░░██╗███████╗"; break;
		case 1:
			wcout << L"██║░░██║██║░░░██║████╗░██║╚══██╔══╝██║████╗░██║██╔════╝░   ██╔════╝████╗░██║██╔══██╗██║░██╔╝██╔════╝"; break;
		case 2:
			wcout << L"███████║██║░░░██║██╔██╗██║░░░██║░░░██║██╔██╗██║██║░░██╗░   ╚█████╗░██╔██╗██║███████║█████═╝░█████╗░░"; break;
		case 3:
			wcout << L"██╔══██║██║░░░██║██║╚████║░░░██║░░░██║██║╚████║██║░░╚██╗   ░╚═══██╗██║╚████║██╔══██║██╔═██╗░██╔══╝░░"; break;
		case 4:
			wcout << L"██║░░██║╚██████╔╝██║░╚███║░░░██║░░░██║██║░╚███║╚██████╔╝   ██████╔╝██║░╚███║██║░░██║██║░╚██╗███████╗"; break;
		case 5:
			wcout << L"╚═╝░░╚═╝░╚═════╝░╚═╝░░╚══╝░░░╚═╝░░░╚═╝╚═╝░░╚══╝░╚═════╝░   ╚═════╝░╚═╝░░╚══╝╚═╝░░╚═╝╚═╝░░╚═╝╚══════╝"; break;
		}
		y++;
		this_thread::sleep_for(chrono::milliseconds(80));
	}
	mode = _setmode(_fileno(stdout), mode);
	// draw art snake
	x = (WIDTH_WINDOW - 24) / 2, y = (HEIGHT_WINDOW - 10) / 2 + 5;
	setColor(BG_COLOR, 10);
	drawArtSnake(x - 25, y);
	drawArtSnake(x + 34, y);
	setColor(BG_COLOR, 7);
	// draw main menu
	x = (WIDTH_WINDOW - 24) / 2 - 1, y = (HEIGHT_WINDOW - 10) / 2 + 3;
	drawBoard(x, y, 24, 12);
	x += 8;
	gotoXY(x, y + 2);
	setColor(BG_COLOR, 12);
	cout << "NEW GAME";
	setColor(BG_COLOR, 7);
	gotoXY(x, y + 4);
	cout << "LOAD GAME";
	gotoXY(x, y + 6);
	cout << "HIGH SCORE";
	gotoXY(x, y + 8);
	cout << "ABOUT US";
	gotoXY(x, y + 10);
	cout << "EXIT";
}
// Chuc nang: chon menu bang cac phim len, xuong
int chooseMenu() {
	int choice = 1;
	int temp;
	int x = (WIDTH_WINDOW - 24) / 2 + 7, y = (HEIGHT_WINDOW - 10) / 2 + 5;
	while (true) {
		temp = _getch();
		switch (temp) {
		case 72:
			choice--;
			if (choice < 1) choice = 1; break;
		case 80:
			choice++;
			if (choice > 5) choice = 5; break;
		case 13: return choice;
		default: break;
		}
		for (int i = 1; i <= 5; i++) {
			if (i == choice) setColor(BG_COLOR, 12);
			switch (i) {
			case 1:
				gotoXY(x, y);
				cout << "NEW GAME"; break;
			case 2:
				gotoXY(x, y + 2);
				cout << "LOAD GAME"; break;
			case 3:
				gotoXY(x, y + 4);
				cout << "HIGH SCORE"; break;
			case 4:
				gotoXY(x, y + 6);
				cout << "ABOUT US"; break;
			case 5:
				gotoXY(x, y + 8);
				cout << "EXIT"; break;
			}
			if (i == choice) setColor(BG_COLOR, 7);
		}
	}
}
void eraseDrawing(POINT start, POINT end) {
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = start.x; i <= end.x; i++) {
		for (int j = start.y; j < end.y; j++) {
			gotoXY(i, j);
			wcout << L" ";
		}
	}
	mode = _setmode(_fileno(stdout), mode);
}
// Chuc nang: ve giao dien luc choi game
void drawGame() {
	// draw board
	setColor(BG_COLOR, 14);
	drawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	// draw "3 2 1 Go!"
	setColor(BG_COLOR, 8);
	int x = (WIDTH_CONSOLE - 7) / 2, y = (HEIGH_CONSOLE - 5) / 2;
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int n = 0; n < 4; n++) {
		for (int i = 0; i < 5; i++) {
			gotoXY(x, y + i);
			switch (n) {
			case 0:
				switch (i) {
				case 0: case 4: wcout << L"██████"; break;
				case 1: case 3: wcout << L"     ██"; break;
				case 2: wcout << L" █████"; break;
				}
				break;
			case 1:
				switch (i) {
				case 0: wcout << L"██████"; break;
				case 1: wcout << L"     ██"; break;
				case 2: wcout << L" █████"; break;
				case 3: wcout << L"██"; break;
				case 4: wcout << L"███████"; x = (WIDTH_CONSOLE - 3) / 2; break;
				}
				break;
			case 2:
				switch (i) {
				case 0: case 2: case 3: wcout << L" ██"; break;
				case 1: wcout << L"███"; break;
				case 4: wcout << L" ██"; x = (WIDTH_CONSOLE - 20) / 2; break;
				}
				break;
			case 3:
				switch (i) {
				case 0: case 4: wcout << L" ██████   ██████  ██"; break;
				case 1: wcout << L"██       ██    ██ ██"; break;
				case 2: wcout << L"██   ███ ██    ██ ██"; break;
				case 3: wcout << L"██    ██ ██    ██"; break;
				}
				break;
			}
		}
		this_thread::sleep_for(chrono::seconds(1));
		eraseDrawing({ 1,1 }, { WIDTH_CONSOLE - 1, HEIGH_CONSOLE - 1 });
	}
	mode = _setmode(_fileno(stdout), mode);
	// Instruction
	setColor(BG_COLOR, 8);
	int width = WIDTH_WINDOW - WIDTH_CONSOLE - 5;
	int xBeg = WIDTH_CONSOLE + 4;
	drawBoard(xBeg, 0, width, HEIGH_CONSOLE);
	y = 4;
	gotoXY((width - 15) / 2 + xBeg, y);
	setColor(BG_COLOR, 12);
	cout << "--INSTRUCTION--";
	y += 3;
	gotoXY((width - 5) / 2 + xBeg, y);
	setColor(BG_COLOR, 14);
	cout << "MOVE";
	x = (width - 20) / 2 + xBeg, y++;
	gotoXY(x, y);
	setColor(BG_COLOR, 7);
	cout << "W: Up";
	y++;
	gotoXY(x, y);
	cout << "S: Down";
	gotoXY(x + 13, y - 1);
	cout << "D: Right";
	gotoXY(x + 13, y);
	cout << "A: Left";

	y += 3;
	gotoXY((width - 9) / 2 + xBeg, y);
	setColor(BG_COLOR, 14);
	cout << "FEATURES";
	y++;
	gotoXY(x, y);
	setColor(BG_COLOR, 7);
	cout << "P: Pause";
	gotoXY(x + 13, y);
	cout << "L: Save";

	setColor(BG_COLOR, 8);
	x = (WIDTH_WINDOW - 40) / 2, y = HEIGH_CONSOLE + 4;
	drawVerLine(219, 3, x, y);
	y++;
	gotoXY(x + 3, y);
	setColor(BG_COLOR, 14);
	cout << "LEVEL: ";
	setColor(BG_COLOR, 7);
	cout << LEVEL;
	gotoXY(x + 30, y);
	setColor(BG_COLOR, 14);
	cout << "SCORE: ";
	setColor(BG_COLOR, 7);
	cout << SCORE;
}
// Chuc nang: doc danh sach ten cac file luu data game cua nguoi choi
void readFileName(string filePath) {
	ifstream fIn;
	fIn.open(filePath, ios::beg);
	while (!fIn.eof()) {
		fIn >> filegame[nFileGame++];
	}
	fIn.close();
}
void drawArtVideoGame(int x, int y) {
	for (int i = 0; i < 10; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0: cout << " _._______"; break;
		case 1: cout << "| _______ |"; break;
		case 2: cout << "||,-----.||"; break;
		case 3: cout << "|||     |||"; break;
		case 4: cout << "|||_____|||"; break;
		case 5: cout << "|`-------'|"; break;
		case 6: cout << "| +     O |"; break;
		case 7: cout << "|      O  |"; break;
		case 8: cout << "| / /  ##,\""; break;
		case 9: cout << " `------\""; break;
		}
		y++;
	}
}
// Chuc nang: ve danh sach file load game
void drawLoadGame() {
	system("cls");
	// draw title
	setColor(BG_COLOR, 14);
	int x = (WIDTH_WINDOW - 51) / 2;
	int y = 4;
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < 3; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0:
			wcout << L"▒█░░░ ▒█▀▀▀█ ░█▀▀█ ▒█▀▀▄   ▒█▀▀█ ░█▀▀█ ▒█▀▄▀█ ▒█▀▀▀";
			break;
		case 1:
			wcout << L"▒█░░░ ▒█░░▒█ ▒█▄▄█ ▒█░▒█   ▒█░▄▄ ▒█▄▄█ ▒█▒█▒█ ▒█▀▀▀";
			break;
		case 2:
			wcout << L"▒█▄▄█ ▒█▄▄▄█ ▒█░▒█ ▒█▄▄▀   ▒█▄▄█ ▒█░▒█ ▒█░░▒█ ▒█▄▄▄";
			break;
		}
		y++;
		this_thread::sleep_for(chrono::milliseconds(80));
	}
	mode = _setmode(_fileno(stdout), mode);
	// draw art video game
	setColor(BG_COLOR, 6);
	drawArtVideoGame(x - 15, 1);
	drawArtVideoGame(x + 55, 1);
	// draw board
	setColor(BG_COLOR, 7);
	y += 4;
	drawBoard(x, y, 50, 12);
	// show list
	x = (WIDTH_WINDOW - 24) / 2, y += 2;
	for (int i = 0; i < nFileGame; i++) {
		if (i == 0) setColor(BG_COLOR, 12);
		gotoXY(x, y + i);
		cout << filegame[i];
		y++;
		if (i == 0) setColor(BG_COLOR, 7);
	}
}
// Chuc nang: chon file de load game bang cac phim len xuong
string chooseFileLoadGame() {
	int choice = 0;
	int temp;
	int x = (WIDTH_WINDOW - 24) / 2, y = 13;
	while (true) {
		do {
			temp = _getch();
		} while (temp == 224);
		switch (temp) {
		case 72:
			choice--;
			if (choice < 0) choice = 0; break;
		case 80:
			choice++;
			if (choice >= nFileGame) choice = nFileGame - 1; break;
		case 13: return filegame[choice];
		default: break;
		}
		for (int i = 0; i < nFileGame; i++) {
			if (i == choice) setColor(BG_COLOR, 12);
			gotoXY(x, y + i * 2);
			cout << filegame[i];
			if (i == choice) setColor(BG_COLOR, 7);
		}
	}
}
void drawArtTrophy(int x, int y) {
	for (int i = 0; i < 10; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0: cout << "  ___________"; break;
		case 1: cout << " '._==_==_=_.'"; break;
		case 2: cout << " .-\\:      /-."; break;
		case 3: cout << "| (|:.     |) |"; break;
		case 4: cout << " '-|:.     |-'"; break;
		case 5: cout << "   \\::.    /"; break;
		case 6: cout << "    '::. .'"; break;
		case 7: cout << "      ) ("; break;
		case 8: cout << "    _.' '._"; break;
		case 9: cout << "   `\"\"\"\"\"\"\"`"; break;
		}
		y++;
	}
}
void readHighScore(const char* filePath) {
	ifstream fIn;
	string s;
	fIn.open(filePath, ios::beg);
	while (!fIn.eof() && nHighscore < MAX_HIGH_SCORE) {
		getline(fIn, highscore[nHighscore].name);
		fIn >> highscore[nHighscore].score;
		getline(fIn, s);
		nHighscore++;
	}
	fIn.close();
}
// Chuc nang: ve danh sach 5 nguoi choi dat diem cao
void drawHighScore() {
	system("cls");
	//draw title
	setColor(BG_COLOR, 14);
	int x = (WIDTH_WINDOW - 55) / 2, y = 4;
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < 3; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0:
			wcout << L"▒█░▒█ ▀█▀ ▒█▀▀█ ▒█░▒█   ▒█▀▀▀█ ▒█▀▀█ ▒█▀▀▀█ ▒█▀▀█ ▒█▀▀▀";
			break;
		case 1:
			wcout << L"▒█▀▀█ ▒█░ ▒█░▄▄ ▒█▀▀█   ░▀▀▀▄▄ ▒█░░░ ▒█░░▒█ ▒█▄▄▀ ▒█▀▀▀";
			break;
		case 2:
			wcout << L"▒█░▒█ ▄█▄ ▒█▄▄█ ▒█░▒█   ▒█▄▄▄█ ▒█▄▄█ ▒█▄▄▄█ ▒█░▒█ ▒█▄▄▄";
			break;
		}
		y++;
		this_thread::sleep_for(chrono::milliseconds(80));
	}
	x -= 2, y += 2;
	setColor(BG_COLOR, 4);
	for (int i = 0; i < 4; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0:
			wcout << L" __________  ___    ____   ___  __   _____  _________  ____";
			break;
		case 1:
			wcout << L"/_  __/ __ \\/ _ \\  / __/  / _ \\/ /  / _ \\ \\/ / __/ _ \\/ __/";
			break;
		case 2:
			wcout << L" / / / /_/ / ___/ /__ \\  / ___/ /__/ __ |\\  / _// , _/\\ \\  ";
			break;
		case 3:
			wcout << L"/_/  \\____/_/    /____/ /_/  /____/_/ |_|/_/___/_/|_/___/  ";
			break;
		}
		y++;
	}
	mode = _setmode(_fileno(stdout), mode);
	// draw trophy art
	setColor(BG_COLOR, 6);
	drawArtTrophy(8, 1);
	drawArtTrophy(WIDTH_WINDOW - 24, 1);
	// draw board
	setColor(BG_COLOR, 7);
	x += 2, y += 2;
	drawBoard(x, y, 54, 12);
	drawVerLine(219, 11, x + 4, y + 1);
	drawVerLine(219, 11, x + 40, y + 1);
	// show list of high scores
	setColor(BG_COLOR, 7);
	y += 2;
	for (int i = 0; i < nHighscore; i++) {
		if (i == 0)setColor(0, 10);
		if (i == 1 || i == 2)setColor(0, 3);
		gotoXY(x + 2, y + i);
		cout << i + 1;
		gotoXY(x + 7, y + i);
		cout << highscore[i].name;
		gotoXY(x + 43, y + i);
		cout << highscore[i].score;
		if (i == 0 || i == 1 || i == 2)setColor(0, 7);
		y++;
	}
}
void drawAboutUs() {
	system("cls");
	// draw title
	int x = (WIDTH_WINDOW - 45) / 2, y = 4;
	setColor(BG_COLOR, 14);
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < 3; i++) {
		gotoXY(x, y);
		switch (i)
		{
		case 0:
			wcout << L"░█▀▀█ ▒█▀▀█ ▒█▀▀▀█ ▒█░▒█ ▀▀█▀▀   ▒█░▒█ ▒█▀▀▀█";
			break;
		case 1:
			wcout << L"▒█▄▄█ ▒█▀▀▄ ▒█░░▒█ ▒█░▒█ ░▒█░░   ▒█░▒█ ░▀▀▀▄▄";
			break;
		case 2:
			wcout << L"▒█░▒█ ▒█▄▄█ ▒█▄▄▄█ ░▀▄▄▀ ░▒█░░   ░▀▄▄▀ ▒█▄▄▄█";
			break;
		}
		y++;
		this_thread::sleep_for(chrono::milliseconds(80));
	}
	mode = _setmode(_fileno(stdout), mode);
	// draw board
	setColor(BG_COLOR, 7);
	x = (WIDTH_WINDOW - 55) / 2, y += 6;
	drawBoard(x, y, 54, 9);
	// art
	setColor(BG_COLOR, 2);
	x = (WIDTH_WINDOW - 23) / 2, y -= 4;
	mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < 5; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0: wcout << L"▄▄▀█▄"; break;
		case 1: wcout << L"▀▀▀██"; break;
		case 2: wcout << L" ▄██▀  ███     ███"; break;
		case 3: wcout << L"███▀▄███ ███ ███ ███ ▄"; break;
		case 4: wcout << L"▀█████▀   ▀███▀   ▀██▀"; break;
		}
		y++;
	}
	mode = _setmode(_fileno(stdout), mode);
	// info
	setColor(BG_COLOR, 7);
	x = (WIDTH_WINDOW - 55) / 2 + 3, y += 1;
	for (int i = 0; i < 7; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0:
			cout << "School: Ho Chi Minh University of Science (HCMUS)"; break;
		case 1:
			cout << "Class: 21CLC7";
			x += 20; y--;
			break;
		case 2:
			cout << "Team: 5";
			x -= 20;
			break;
		case 3:
			cout << "Members:";
			x += 3;
			break;
		case 4:
			cout << "Tran Thi Thanh Van - 21127478"; break;
		case 5:
			cout << "Ha Minh Khang - 21127623"; break;
		case 6:
			cout << "Nguyen Chau Vi Cuong - 21127237"; break;
		}
		y++;
	}
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
	//GenerateFood();
	OBScount = 0;
}
void loadGame(string filePath) {
	system("cls");
	readData(filePath);
	drawGame();
	drawLevel();
	if (triggerCount == 5) {
		triggerCount = 0;
		DrawGateIn();
	}
	else {
		trigger = { 0,0 };
		triggerCount = 0;
	}
	GenerateFood();
	STATE = 1;//Start running Thread
}
void ProcessDead() {
	STATE = 0;
	//stop_thread = 1;
}
void Eat() {
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1) {
		FOOD_INDEX = 0;
		SIZE_SNAKE++;
		DrawGateIn();
	}
	else {
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
	SCORE++;
}
void ResetData() {
	SEED = (unsigned)time(NULL);
	srand(SEED);
	//Initialize the global values
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, WIDTH_CONSOLE = 70,
		HEIGH_CONSOLE = 20, SIZE_SNAKE = 3, SCORE = 0, LEVEL = 1;
	OBScount = 0; triggerCount = 0; trigger = { 0,0 };
	// Initialize default values for snake
	snake[0] = { 10, 1 }; snake[1] = { 11, 1 };
	snake[2] = { 12, 1 };
	GenerateFood();//Create food array
}
/*
void drawVerOBS(int c, int length, int x, int y, int& count) {
	for (int i = 0; i < length; i++) {
		gotoXY(x, y + i);
		cout << (char)c;
		OBSTACLE[count] = { x,y + i };
		count++;
	}
}
void drawHorOBS(int c, int length, int x, int y, int& count) {
	for (int i = 0; i < length; i++) {
		gotoXY(x + i, y);
		cout << (char)c;
		OBSTACLE[count] = { x + i,y };
		count++;
	}
}

void drawVerTRIG(int c, int length, int x, int y, int& count) {
	for (int i = 0; i < length; i++) {
		gotoXY(x, y + i);
		cout << (char)c;
		TRIGGER[count] = { x,y + i };
		count++;
	}
}
void drawHorTRIG(int c, int length, int x, int y, int& count) {
	for (int i = 0; i < length; i++) {
		gotoXY(x + i, y);
		cout << (char)c;
		TRIGGER[count] = { x + i,y };
		count++;
	}
}
*/
void drawVerOBS(int c, int length, int x, int y, int& count, POINT* a) {
	for (int i = 0; i < length; i++) {
		gotoXY(x, y + i);
		cout << (char)c;
		a[count] = { x,y + i };
		count++;
	}
}
void drawHorOBS(int c, int length, int x, int y, int& count, POINT* a) {
	for (int i = 0; i < length; i++) {
		gotoXY(x + i, y);
		cout << (char)c;
		a[count] = { x + i,y };
		count++;
	}
}
/*
void Level2(int& count)
{
	drawHorOBS(219, 5, WIDTH_CONSOLE / 2 - 2, HEIGH_CONSOLE / 2, count);
	drawVerOBS(219, 5, WIDTH_CONSOLE / 2, HEIGH_CONSOLE / 2 - 2, count);
}
void Level3(int& count)
{
	if (OBScount == 0) Level2(OBScount);
	drawVerOBS(219, 5, 3, 3, count);
	drawHorOBS(219, 5, 3, 3, count);
	drawVerOBS(219, 5, WIDTH_CONSOLE - 3, 3, count);
	drawHorOBS(219, 5, WIDTH_CONSOLE - 7, 3, count);
	drawHorOBS(219, 5, 3, HEIGH_CONSOLE - 3, count);
	drawVerOBS(219, 5, 3, HEIGH_CONSOLE - 7, count);
	drawHorOBS(219, 5, WIDTH_CONSOLE - 7, HEIGH_CONSOLE - 3, count);
	drawVerOBS(219, 5, WIDTH_CONSOLE - 3, HEIGH_CONSOLE - 7, count);
}
void Level4(int& count)
{
	if (OBScount == 0) Level3(OBScount);
	drawVerOBS(219, 3, WIDTH_CONSOLE / 2, 1, count);
	drawHorOBS(219, 5, 1, HEIGH_CONSOLE / 2, count);
	drawVerOBS(219, 3, WIDTH_CONSOLE / 2, HEIGH_CONSOLE - 3, count);
	drawHorOBS(219, 5, WIDTH_CONSOLE - 5, HEIGH_CONSOLE / 2, count);
}
void Level5(int& count)
{
	srand(SEED);
	for (int i = 0; i < (LEVEL - 1) * 5 + LEVEL; i++) {
		rand(); rand();
	}
	if (OBScount == 0) Level4(OBScount);
	int x, y;
	for (int i = 0; i < 2; i++)
	{
		do {
			x = rand() % (WIDTH_CONSOLE - 17) + 8;
			y = rand() % (HEIGH_CONSOLE - 9) + 3;
		} while (!IsValid(x, y));
		drawVerOBS(219, 3, x, y, count);
		do {
			x = rand() % (WIDTH_CONSOLE - 17) + 8;
			y = rand() % (HEIGH_CONSOLE - 7) + 3;
		} while (!IsValid(x, y));
		drawHorOBS(219, 5, x, y, count);
		//drawVerOBS(219, 3, rand() % (WIDTH_CONSOLE - 5), rand() % (HEIGH_CONSOLE - 5), count);
		//drawHorOBS(220, 5, rand() % (WIDTH_CONSOLE - 5), rand() % (HEIGH_CONSOLE - 5), count);
	}
}
*/
void Level2()
{
	drawHorOBS(219, 5, WIDTH_CONSOLE / 2 - 2, HEIGH_CONSOLE / 2, OBScount, OBSTACLE);
	drawVerOBS(219, 5, WIDTH_CONSOLE / 2, HEIGH_CONSOLE / 2 - 2, OBScount, OBSTACLE);
}
void Level3()
{
	if (OBScount == 0) Level2();
	drawVerOBS(219, 5, 3, 3, OBScount, OBSTACLE);
	drawHorOBS(219, 5, 3, 3, OBScount, OBSTACLE);
	drawVerOBS(219, 5, WIDTH_CONSOLE - 3, 3, OBScount, OBSTACLE);
	drawHorOBS(219, 5, WIDTH_CONSOLE - 7, 3, OBScount, OBSTACLE);
	drawHorOBS(219, 5, 3, HEIGH_CONSOLE - 3, OBScount, OBSTACLE);
	drawVerOBS(219, 5, 3, HEIGH_CONSOLE - 7, OBScount, OBSTACLE);
	drawHorOBS(219, 5, WIDTH_CONSOLE - 7, HEIGH_CONSOLE - 3, OBScount, OBSTACLE);
	drawVerOBS(219, 5, WIDTH_CONSOLE - 3, HEIGH_CONSOLE - 7, OBScount, OBSTACLE);
}
void Level4()
{
	if (OBScount == 0) Level3();
	drawVerOBS(219, 3, WIDTH_CONSOLE / 2, 1, OBScount, OBSTACLE);
	drawHorOBS(219, 5, 1, HEIGH_CONSOLE / 2, OBScount, OBSTACLE);
	drawVerOBS(219, 3, WIDTH_CONSOLE / 2, HEIGH_CONSOLE - 3, OBScount, OBSTACLE);
	drawHorOBS(219, 5, WIDTH_CONSOLE - 5, HEIGH_CONSOLE / 2, OBScount, OBSTACLE);
}
bool IsValidOBS(int x, int y, int mode) {
	for (int j = 0; j < 5 - 2 * mode; j++)
	{
		int count = OBScount;
		if (SIZE_SNAKE > OBScount)
			int count = SIZE_SNAKE;
		for (int i = 0; i < count; i++) {
			if (i < triggerCount)
				if (TRIGGER[i].x == x + (1 - mode) * j && TRIGGER[i].y == y + mode * j)
					return false;
			if (i < SIZE_SNAKE)
				if (snake[i].x == x + (1 - mode) * j && snake[i].y == y + mode * j)
					return false;
			if (i < OBScount)
				if (OBSTACLE[i].x == x + (1 - mode) * j && OBSTACLE[i].y == y + mode * j)
					return false;
		}
	}
	return true;
}
void Level5()
{
	srand(SEED);
	for (int i = 0; i < (LEVEL - 1) * 5 + LEVEL; i++) {
		rand(); rand();
	}
	if (OBScount == 0) Level4();
	int x, y;
	for (int i = 0; i < 2; i++)
	{
		do {
			x = rand() % (WIDTH_CONSOLE - 17) + 8;
			y = rand() % (HEIGH_CONSOLE - 9) + 3;
		} while (!IsValidOBS(x, y, 1));
		drawVerOBS(219, 3, x, y, OBScount, OBSTACLE);
		do {
			x = rand() % (WIDTH_CONSOLE - 17) + 8;
			y = rand() % (HEIGH_CONSOLE - 7) + 3;
		} while (!IsValidOBS(x, y, 0));
		drawHorOBS(219, 5, x, y, OBScount, OBSTACLE);
	}
}
bool TouchOBS(int x, int y)
{
	for (int i = 0; i < OBScount; ++i)
	{
		if (x == OBSTACLE[i].x && y == OBSTACLE[i].y)
			return false;
	}
	for (int i = 0; i < triggerCount; ++i)
	{
		if (x == TRIGGER[i].x && y == TRIGGER[i].y)
			return false;
	}
	return true;
}
bool TouchBody(int x, int y)
{
	for (int i = SIZE_SNAKE - 3; i >= 0; --i)
	{
		if (x == snake[i].x && y == snake[i].y)
			return false;
	}
	return true;
}
void MoveRight()
{
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE || TouchBody(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == 0
		|| TouchOBS(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == 0)
	{
		ProcessDead();
	}
	else if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y)
	{
		Eat();
	}
	else if (triggerCount == 1 && snake[0].x == WIDTH_CONSOLE - 8 && snake[0].y == 1) {
		clearGate();
	}
	else {
		for (int i = 0; i < SIZE_SNAKE - 1; ++i)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x++;
	}
}
void MoveLeft()
{
	if (snake[SIZE_SNAKE - 1].x - 1 == 0 || TouchBody(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == 0
		|| TouchOBS(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == 0)
	{
		ProcessDead();
	}
	else if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y)
	{
		Eat();
	}
	else if (triggerCount == 1 && snake[0].x == WIDTH_CONSOLE - 8 && snake[0].y == 1) {
		clearGate();
	}
	else {
		for (int i = 0; i < SIZE_SNAKE - 1; ++i)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x--;
	}
}
void MoveUp()
{
	if (snake[SIZE_SNAKE - 1].y - 1 == 0 || TouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1) == 0
		|| TouchOBS(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1) == 0)
	{
		ProcessDead();
	}
	else if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y)
	{
		Eat();
	}
	else if (triggerCount == 1 && snake[0].x == WIDTH_CONSOLE - 8 && snake[0].y == 1) {
		clearGate();
	}
	else {
		for (int i = 0; i < SIZE_SNAKE - 1; ++i)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y--;
	}
}
void MoveDown()
{
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE || TouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1) == 0
		|| TouchOBS(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1) == 0)
	{
		ProcessDead();
	}
	else if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y)
	{
		Eat();
	}
	else if (trigger.x != 0 && trigger.y != 0 && snake[SIZE_SNAKE - 1].x == trigger.x && snake[SIZE_SNAKE - 1].y + 1 == trigger.y) {
		moveGate();
	}
	else if (triggerCount == 1 && snake[0].x == WIDTH_CONSOLE - 8 && snake[0].y == 1) {
		clearGate();
	}
	else {
		for (int i = 0; i < SIZE_SNAKE - 1; ++i)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y++;
	}
}
void moveGate() {
	for (int i = 0; i < SIZE_SNAKE - 1; ++i)
	{
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
		//---------------//
		OBScount = 0; triggerCount = 0; trigger = { 0,0 };
		eraseDrawing({ 1,1 }, { WIDTH_CONSOLE - 1,HEIGH_CONSOLE });
		GenerateFood();
		SCORE += 10;
		return;
		//---------------//
	}
	else {
		snake[SIZE_SNAKE - 1] = { WIDTH_CONSOLE - 8,1 };
		LEVEL++; SPEED++;
		SCORE += 5;
		drawLevel();
	}
	triggerCount = 1; // Dieu kien gia dinh de xac nhan ran da di qua cong
}
void drawLevel() {
	switch (LEVEL)
	{
	case 2:
		Level2();
		break;
	case 3:
		Level3();
		break;
	case 4:
		Level4();
		break;
	case 5:
		Level5();
		break;
	default:
		break;
	}
}
void clearGate() {
	triggerCount = 0;
	setColor(BG_COLOR, 0);
	drawHorLine(219, 3, trigger.x - 1, trigger.y + 1);
	drawHorLine(219, 3, trigger.x - 1, trigger.y);
	setColor(BG_COLOR, 7);
	trigger = { 0,0 };
	if (LEVEL != 5)
		drawLevel();
	GenerateFood();
}
bool IsValidGate(int x, int y) {
	int a[] = { -1,0,1,-1,1,-1,0,1 };
	int b[] = { -1,-1,-1,0,0,1,1,1 };
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < SIZE_SNAKE; i++) {
			if (snake[i].x == x + a[j] && snake[i].y == y + b[j])
				return false;
		}
		for (int i = 0; i < OBScount; i++) {
			if (OBSTACLE[i].x == x + a[j] && OBSTACLE[i].y == y + b[j])
				return false;
		}
	}
	return true;
}
void DrawGateIn()
{
	food[FOOD_INDEX] = { 0 , HEIGH_CONSOLE + 1 };
	setColor(BG_COLOR, 9);

	srand(SEED);
	for (int i = 1; i < LEVEL; i++) {
		rand(); rand();
	}

	do
	{
		trigger.x = rand() % (WIDTH_CONSOLE - 5) + 2;
		trigger.y = rand() % (HEIGH_CONSOLE - 7) + 4;
	} while (!IsValidGate(trigger.x, trigger.y));
	gotoXY(trigger.x, trigger.y);
	cout << 'x';
	/*
	drawHorTRIG(219, 3, trigger.x - 1, trigger.y + 1, triggerCount);
	drawVerTRIG(219, 1, trigger.x - 1, trigger.y, triggerCount);
	drawVerTRIG(219, 1, trigger.x + 1, trigger.y, triggerCount);
	*/
	drawHorOBS(219, 3, trigger.x - 1, trigger.y + 1, triggerCount, TRIGGER);
	drawVerOBS(219, 1, trigger.x - 1, trigger.y, triggerCount, TRIGGER);
	drawVerOBS(219, 1, trigger.x + 1, trigger.y, triggerCount, TRIGGER);
	setColor(BG_COLOR, 7);
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

void DrawSnake() {
	setColor(BG_COLOR, 10);
	char ms[25] = "211274782112762321127237";
	int j = 0;
	for (int i = SIZE_SNAKE - 1; i >= 0; i--) {
		gotoXY(snake[i].x, snake[i].y);
		cout << ms[j];
		j++;
		if (j == 24) {
			j = 0;
		}
	}
	setColor(BG_COLOR, 7);
}
void DrawFood() {
	gotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	setColor(BG_COLOR, 4);
	cout << (char)3;
	setColor(BG_COLOR, 7);
}
void ClearSnake() {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		gotoXY(snake[i].x, snake[i].y);
		cout << " ";
	}
}
void ClearFood() {
	gotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	cout << " ";
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
			DrawSnake();
			gotoXY(x + 10, y);
			cout << LEVEL;
			gotoXY(x + 37, y);
			cout << SCORE;
			Sleep(300 / SPEED);//Sleep function with SPEEED variable
		}
	}
}
void drawPauseGameNoti() {
	int w = 30, h = 5;
	int x = (WIDTH_CONSOLE - w) / 2, y = (HEIGH_CONSOLE - h) / 2;
	setColor(BG_COLOR, 14);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, x, y + i);
		this_thread::sleep_for(chrono::milliseconds(80));
	}
	setColor(14, 0);
	gotoXY(x + 2, y + 2);
	cout << "Press any key to continue!";
	setColor(BG_COLOR, 7);
}
int checkFileGame(string filenametemp)//Kiem tra file game co bi trung ten hay khong
{
	for (int i = 0; i < nFileGame; ++i)
	{
		if (filenametemp.compare(filegame[i]) == 0) return i;
	}
	return -1;
}
int chooseYesNo(int x, int y) { // yes->1, no->0
	int temp, choice = 1;
	const int bgColor = 14;
	while (true) {
		do {
			temp = _getch();
		} while (temp == 224);
		switch (temp) {
		case 75: choice = 1; break;
		case 77: choice = 0; break;
		case 13: return choice;
		default: break;
		}
		gotoXY(x, y);
		setColor(bgColor, 0);
		if (choice == 1) setColor(bgColor, 4);
		cout << "YES";
		gotoXY(x + 10, y);
		setColor(bgColor, 0);
		if (choice == 0) setColor(bgColor, 4);
		cout << "NO";
	}
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
string inputFileName() {
	int w = 34, h = 6, x, y, choice, index, xS, yS;
	const int boardColor = 14;
	string name;
	xS = (WIDTH_CONSOLE - w) / 2, yS = (HEIGH_CONSOLE - h) / 2;
	do {
		x = xS, y = yS;
		setColor(BG_COLOR, boardColor);
		for (int i = 0; i < h; i++) {
			drawHorLine(219, w, x, y + i);
			this_thread::sleep_for(chrono::milliseconds(80));
		}
		setColor(boardColor, 0);
		x += 2, y += 1;
		gotoXY(x, y);
		showConsoleCursor(true);
		cout << "Enter file name: ";
		gotoXY(x, y + 1);
		cin >> name;
		showConsoleCursor(false);
		name = name + ".txt";
		index = checkFileGame(name);
		if (index != -1) {
			gotoXY(x, y);
			cout << "File name has already existed.";
			gotoXY(x, y + 1);
			cout << "Do you want to replace it?";
			x = x - 2 + (w - 15) / 2, y += 3;
			gotoXY(x, y);
			setColor(boardColor, 4);
			cout << "YES";
			gotoXY(x + 10, y);
			setColor(boardColor, 0);
			cout << "NO";
			choice = chooseYesNo(x, y);
		}
		else {
			if (nFileGame < MAX_FILE_GAME) {
				filegame[nFileGame++] = name;
			}
			else
				filegame[MAX_FILE_GAME - 1] = name;
			saveFileNamesToFile();
			choice = 1;
		}
	} while (choice == 0);
	setColor(BG_COLOR, boardColor);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, xS, yS + i);
	}
	gotoXY(xS + (w - 23) / 2, yS + 2); setColor(boardColor, 4);
	cout << "SAVE GAME SUCCESSFULLY!";
	setColor(BG_COLOR, 7);
	return name;
}

int drawGameOver() { //yes(1) -> play again, no(0) -> turn back menu
	const int boardColor = 14;
	int w = 60, h = 9, x = (WIDTH_CONSOLE - w) / 2 + 1, y = (HEIGH_CONSOLE - h) / 2;
	setColor(boardColor, boardColor);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, x, y + i);
		this_thread::sleep_for(chrono::milliseconds(80));
	}
	setColor(boardColor, 4);
	x += (w - 55) / 2, y += 2;
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < 5; i++) {
		gotoXY(x, y);
		switch (i) {
		case 0:
			wcout << L" █████  ███  ██   ██ █████    ████  █    █ █████ █████";
			break;
		case 1:
			wcout << L"█      █   █ █ █ █ █ █       █    █ █    █ █     █    █";
			break;
		case 2:
			wcout << L"█   ██ █████ █  █  █ ███     █    █ █    █ ███   █████";
			break;
		case 3:
			wcout << L"█    █ █   █ █     █ █       █    █  █  █  █     █    █";
			break;
		case 4:
			wcout << L" █████ █   █ █     █ █████    ████    ██   █████ █    █";
			break;
		}
		y++;
	}
	mode = _setmode(_fileno(stdout), mode);

	if (checkScore()) {
		inputHighscoreInfo();
	}
	this_thread::sleep_for(chrono::milliseconds(200));
	x = (WIDTH_CONSOLE - w) / 2 + 1, y = (HEIGH_CONSOLE - h) / 2;
	setColor(boardColor, boardColor);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, x, y + i);
		this_thread::sleep_for(chrono::milliseconds(80));
	}
	setColor(boardColor, 0);
	y += (h - 3) / 2;
	gotoXY(x + (w - 26) / 2, y);
	cout << "Do you want to play again?";
	y += 2, x += (w - 15) / 2;
	gotoXY(x, y);
	setColor(boardColor, 4);
	cout << "YES";
	gotoXY(x + 10, y);
	setColor(boardColor, 0);
	cout << "NO";
	int ch = chooseYesNo(x, y);
	setColor(BG_COLOR, 7);
	return ch;
}
int checkPlayerName(string name) {
	for (int i = 0; i < nHighscore; ++i) {
		if (name.compare(highscore[i].name) == 0) return 1;
	}
	return 0;
}
void inputHighscoreInfo() {
	int w = 60, h = 9, x, y, exist, xS, yS;
	const int boardColor = 14;
	string name;
	xS = (WIDTH_CONSOLE - w) / 2 + 1, yS = (HEIGH_CONSOLE - h) / 2;
	do {
		x = xS, y = yS;
		setColor(BG_COLOR, boardColor);
		for (int i = 0; i < h; i++) {
			drawHorLine(219, w, xS, y + i);
			this_thread::sleep_for(chrono::milliseconds(80));
		}
		setColor(boardColor, 0);
		x = xS + (w - 40) / 2, y = yS + (h - 3) / 2;
		gotoXY(x, y);
		cout << "CONGRATULATIONS! YOUR SCORE IS IN TOP 5!";
		x = xS + (w - 17) / 2;
		gotoXY(x, ++y);
		showConsoleCursor(true);
		cout << "Enter your name: ";
		gotoXY(x, ++y);
		getline(cin, name);
		showConsoleCursor(false);
		exist = checkPlayerName(name);
		if (exist) {
			gotoXY(xS + (w - 30) / 2, ++y);
			cout << "Your name has already existed.";
			this_thread::sleep_for(chrono::milliseconds(200));
		}
		else {
			highscore[nHighscore++].score = SCORE;
			highscore[nHighscore - 1].name = name;
			sortListHighScore();
			WriteHighScore("HighScore.txt");
		}
	} while (exist);
	setColor(BG_COLOR, boardColor);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, xS, yS + i);
	}
	gotoXY(xS + (w - 48) / 2, yS + (h - 1) / 2);
	setColor(boardColor, 4);
	this_thread::sleep_for(chrono::milliseconds(200));
	cout << "GO TO 'HIGH SCORE' IN THE MENU TO SEE YOUR NAME!";
	setColor(BG_COLOR, 7);
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
bool checkScore() {
	if (SCORE == 0) return 0;
	if (nHighscore < MAX_HIGH_SCORE) return 1;
	for (int i = 0; i < nHighscore; ++i) {
		if (SCORE > highscore[i].score) return 1;
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
void WriteHighScore(const char* filePath) {
	ofstream fOut;
	fOut.open(filePath, ios::ate);
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
void reDrawGamePlay() {
	eraseDrawing({ 1,1 }, { WIDTH_CONSOLE - 1,HEIGH_CONSOLE });
	for (int i = 0; i < OBScount; i++) {
		gotoXY(OBSTACLE[i].x, OBSTACLE[i].y);
		cout << (char)219;
	}
	if (triggerCount != 0) {
		setColor(BG_COLOR, 9);
		for (int i = 0; i < 5; i++) {
			gotoXY(TRIGGER[i].x, TRIGGER[i].y);
			cout << (char)219;
		}
		gotoXY(trigger.x, trigger.y);
		cout << "x";
		setColor(BG_COLOR, 7);
	}
}
int processGame(string file = " ", void (*mode)(string) = StartGame) {
	thread t1(ThreadFunc);
	HANDLE handle_t1 = t1.native_handle();
	stop_thread = 0;
	while (1) {
		int temp;
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
int main() {
	readFileName("FileGame.txt");
	readHighScore("HighScore.txt");
	setWindowSize(WIDTH_WINDOW, HEIGHT_WINDOW);
	setScreenBufferSize(WIDTH_WINDOW, HEIGHT_WINDOW);
	fixConsoleWindow();
	showConsoleCursor(false);
	while (1) {
		drawMenu();
		int temp;
		while (1) {
			int choice = chooseMenu();
			if (choice == 1) {
				StartGame(" ");
				if (!processGame()) break;
			}
			else if (choice == 2) {
				drawLoadGame();
				string file = chooseFileLoadGame();
				loadGame(file);
				if (!processGame(file, loadGame)) break;
			}
			else if (choice == 3) {
				drawHighScore();
				temp = _getch();
				if (temp == 27 || temp == 13) break;
			}
			else if (choice == 4) {
				drawAboutUs();
				temp = _getch();
				if (temp == 27 || temp == 13) break;
			}
			else if (choice == 5) {
				system("cls");
				return 0;
			}
		}
	}
}