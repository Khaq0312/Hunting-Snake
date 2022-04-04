#include <Windows.h>
//#include "graphic2.h"
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
	std::string name;
	int score = 0;
};
#define MAX_SIZE_SNAKE 28
#define MAX_SIZE_FOOD 1
#define MAX_LEVEL 5
#define MAX_HIGH_SCORE 5
#define MAX_FILE_GAME 5
static int HEIGHT_WINDOW = 30, WIDTH_WINDOW = 110;
int LEVEL;
int OBScount = 0;
int SCORE;
#define MAX_SPEED 3
POINT snake[MAX_SIZE_SNAKE];
POINT food[5];
POINT OBSTACLE[100];
int CHAR_LOCK;
int MOVING;
int SPEED;
static int HEIGH_CONSOLE = 20, WIDTH_CONSOLE = 70;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE;
HIGHSCORE highscore[MAX_HIGH_SCORE]; int nHighscore = 0;
string filegame[MAX_FILE_GAME];
int nFileGame = 0;
POINT trigger = { 0,0 };
int triggerCount;
POINT TRIGGER[10];
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
	setColor(0, 14);
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
	setColor(0, 10);
	drawArtSnake(x - 25, y);
	drawArtSnake(x + 34, y);
	setColor(0, 7);
	// draw main menu
	x = (WIDTH_WINDOW - 24) / 2 - 1, y = (HEIGHT_WINDOW - 10) / 2 + 3;
	drawBoard(x, y, 24, 12);
	x += 8;
	gotoXY(x, y + 2);
	setColor(0, 12);
	cout << "NEW GAME";
	setColor(0, 7);
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
			if (choice < 1) choice = 1;
			break;
		case 80:
			choice++;
			if (choice > 5) choice = 5;
			break;
		case 13:
			return choice;
		}
		for (int i = 1; i <= 5; i++) {
			if (i == choice) setColor(0, 12);
			switch (i) {
			case 1:
				gotoXY(x, y);
				cout << "NEW GAME";
				break;
			case 2:
				gotoXY(x, y + 2);
				cout << "LOAD GAME";
				break;
			case 3:
				gotoXY(x, y + 4);
				cout << "HIGH SCORE";
				break;
			case 4:
				gotoXY(x, y + 6);
				cout << "ABOUT US";
				break;
			case 5:
				gotoXY(x, y + 8);
				cout << "EXIT";
				break;
			}
			if (i == choice) setColor(0, 7);
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
	setColor(0, 14);
	drawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	// draw "3 2 1 Go!"
	setColor(0, 8);
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
	setColor(0, 8);
	int width = WIDTH_WINDOW - WIDTH_CONSOLE - 5;
	int xBeg = WIDTH_CONSOLE + 4;
	drawBoard(xBeg, 0, width, HEIGH_CONSOLE);
	y = 4;
	gotoXY((width - 15) / 2 + xBeg, y);
	setColor(0, 12);
	cout << "--INSTRUCTION--";
	y += 3;
	gotoXY((width - 5) / 2 + xBeg, y);
	setColor(0, 14);
	cout << "MOVE";
	x = (width - 20) / 2 + xBeg, y++;
	gotoXY(x, y);
	setColor(0, 7);
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
	setColor(0, 14);
	cout << "FEATURES";
	y++;
	gotoXY(x, y);
	setColor(0, 7);
	cout << "P: Pause";
	gotoXY(x + 13, y);
	cout << "L: Save";

	setColor(0, 8);
	x = (WIDTH_WINDOW - 40) / 2, y = HEIGH_CONSOLE + 4;
	drawVerLine(219, 3, x, y);
	y++;
	gotoXY(x + 3, y);
	setColor(0, 14);
	cout << "LEVEL: ";
	setColor(0, 7);
	cout << LEVEL;
	gotoXY(x + 30, y);
	setColor(0, 14);
	cout << "SCORE: ";
	setColor(0, 7);
	cout << SCORE;
}
// Chuc nang: doc danh sach ten cac file luu data game cua nguoi choi
void readFileGame(const char* filePath) {
	ifstream fIn;
	fIn.open(filePath, ios::beg);
	while (!fIn.eof()) {
		nFileGame++;
		fIn >> filegame[nFileGame - 1];
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
	setColor(0, 14);
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
	setColor(0, 6);
	drawArtVideoGame(x - 15, 1);
	drawArtVideoGame(x + 55, 1);
	// draw board
	setColor(0, 7);
	y += 4;
	drawBoard(x, y, 50, 12);
	// show list
	x = (WIDTH_WINDOW - 24) / 2, y += 2;
	for (int i = 0; i < nFileGame; i++) {
		if (i == 0) setColor(0, 12);
		gotoXY(x, y + i);
		cout << filegame[i];
		y++;
		if (i == 0) setColor(0, 7);
	}
}
// Chuc nang: chon file de load game bang cac phim len xuong
string chooseFileLoadGame() {
	int choice = 0;
	int temp;
	int x = (WIDTH_WINDOW - 24) / 2, y;
	while (true) {
		temp = _getch();
		switch (temp) {
		case 72:
			choice--;
			if (choice < 0) choice = 0;
			break;
		case 80:
			choice++;
			if (choice >= nFileGame) choice = nFileGame - 1;
			break;
		case 13:
			return filegame[choice];
		}
		y = 13;
		for (int i = 0; i < nFileGame; i++) {
			if (i == choice) setColor(0, 12);
			gotoXY(x, y + i);
			cout << filegame[i];
			y++;
			if (i == choice) setColor(0, 7);
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
	setColor(0, 14);
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
	setColor(0, 4);
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
	setColor(0, 6);
	drawArtTrophy(8, 1);
	drawArtTrophy(WIDTH_WINDOW - 24, 1);
	// draw board
	setColor(0, 7);
	x += 2, y += 2;
	drawBoard(x, y, 54, 12);
	drawVerLine(219, 11, x + 4, y + 1);
	drawVerLine(219, 11, x + 40, y + 1);
	// show list of high scores
	setColor(0, 7);
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
	setColor(0, 14);
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
	setColor(0, 7);
	x = (WIDTH_WINDOW - 55) / 2, y += 6;
	drawBoard(x, y, 54, 9);
	// art
	setColor(0, 2);
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
	setColor(0, 7);
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
	srand((unsigned)time(NULL));
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
	char c;
	while (!fIn.eof()) {
		fIn >> c;
		CHAR_LOCK = (int)c;
		fIn >> c;
		MOVING = (int)c;
		fIn >> SPEED >> LEVEL >> FOOD_INDEX >> SIZE_SNAKE >> SCORE;
		for (int i = 0; i < SIZE_SNAKE; i++) {
			fIn >> snake[i].x >> snake[i].y;
		}
	}
	fIn.close();
	GenerateFood();
}
void loadGame(string filePath) {
	system("cls");
	readData(filePath);
	drawGame();
	STATE = 1;//Start running Thread
}
void ProcessDead() {
	STATE = 0;
	gotoXY(0, HEIGH_CONSOLE + 2);
	printf("Dead, type y to continue or anykey to exit");
}
void Eat() {
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1) {
		FOOD_INDEX = 0;
		SIZE_SNAKE++;
		DrawGateIn(triggerCount);
		/*
		if (LEVEL == MAX_LEVEL) {
			LEVEL = 1;
			SPEED = SPEED - 3; // tang 1 so voi speed ban dau cua vong truoc
			SIZE_SNAKE = 3;
		}
		else {
			//LEVEL++;
			//SPEED++;

			Level2(OBScount);
			Level3(OBScount);
			Level4(OBScount);
			Level5(OBScount);

		}

		//GenerateFood();
	*/
	}
	else {
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
	//if (trigger.x == 0 && trigger.y == 0) GenerateFood();
	//else food[FOOD_INDEX] = { 0,HEIGH_CONSOLE + 1 };
	SCORE++;
}
void ResetData() {
	//Initialize the global values
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, WIDTH_CONSOLE = 70,
		HEIGH_CONSOLE = 20, SIZE_SNAKE = 3, SCORE = 0, LEVEL = 1;
	OBScount = 0; triggerCount = 0; trigger = { 0,0 };
	// Initialize default values for snake
	snake[0] = { 10, 1 }; snake[1] = { 11, 1 };
	snake[2] = { 12, 1 };
	GenerateFood();//Create food array
}
//Ham moi/chinh sua
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
		drawVerOBS(219, 3, rand() % (WIDTH_CONSOLE - 5), rand() % (HEIGH_CONSOLE - 5), count);
		drawHorOBS(220, 5, rand() % (WIDTH_CONSOLE - 5), rand() % (HEIGH_CONSOLE - 5), count);
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
bool TouchBody(int x, int y)//Cham than
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
	else if (snake[0].x == trigger.x && snake[0].y + 1 == trigger.y) {
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
	snake[SIZE_SNAKE - 1] = { WIDTH_CONSOLE - 8,1 };
	if (LEVEL == MAX_LEVEL) {
		LEVEL = 1;
		SPEED = SPEED - 3; // tang 1 so voi speed ban dau cua vong truoc
		SIZE_SNAKE = 3;
	}
	else {
		LEVEL++; SPEED++;
		SCORE += 5;
	}

}
void clearGate() {

	triggerCount = 0;
	setColor(0, 0);
	drawHorLine(219, 3, trigger.x - 1, trigger.y + 1);
	drawHorLine(219, 3, trigger.x - 1, trigger.y);
	setColor(0, 7);
	trigger = { 0,0 };
	GenerateFood();
	switch (LEVEL)
	{
	case 2:
		Level2(OBScount);
		break;
	case 3:
		Level3(OBScount);
		break;
	case 4:
		Level4(OBScount);
		break;
	case 5:
		Level5(OBScount);
		break;
	default:
		break;
	}
}
bool IsValidGate(int x, int y) {
	for (int j = -1; j <= 1; j++)
	{
		for (int i = 0; i < SIZE_SNAKE; i++) {
			if (snake[i].x == x + j && snake[i].y == y + j)
				return false;
		}
		for (int i = 0; i < OBScount; i++) {
			if (OBSTACLE[i].x == x + j && OBSTACLE[i].y == y + j)
				return false;
		}
		for (int i = 0; i < triggerCount; i++) {
			if (TRIGGER[i].x == x + j && TRIGGER[i].y == y + j)
				return false;
		}
	}
	return true;
}
void DrawGateIn(int& count)
{
	setColor(0, 9);
	do
	{
		trigger.x = rand() % (WIDTH_CONSOLE - 5) + 2;
		trigger.y = rand() % (HEIGH_CONSOLE - 7) + 4;
	} while (!IsValid(trigger.x, trigger.y));
	gotoXY(trigger.x, trigger.y);
	cout << 'x';
	drawHorTRIG(219, 3, trigger.x - 1, trigger.y + 1, count);
	drawVerTRIG(219, 1, trigger.x - 1, trigger.y, count);
	drawVerTRIG(219, 1, trigger.x + 1, trigger.y, count);
	setColor(0, 7);
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
	setColor(0, 10);
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
	setColor(0, 7);
}
void DrawFood() {
	gotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	setColor(0, 4);
	cout << (char)3;
	setColor(0, 7);
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
	int x = (WIDTH_WINDOW - 40) / 2, y = HEIGH_CONSOLE + 4;
	while (1) {
		if (STATE == 1) {//If my snake is alive
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
			gotoXY(x + 10, y + 1);
			cout << LEVEL;
			gotoXY(x + 37, y + 1);
			cout << SCORE;
			Sleep(800 / SPEED);//Sleep function with SPEEED variable
		}
	}
}
/*
bool TouchGate()//Ham nay mo phong, luc ghep ham thi se chinh sua lai
{
   //Toa do loi vao cua cong:
   int x, y;
   POINT gate[6];
   gate[0].x = x-1;
   gate[0].y = y-1;
   gate[1].x = x;
   gate[1].y = y-1;
   gate[2].x = x+1;
   gate[2].y = y-1;
   gate[3].x = x-1;
   gate[3].y = y;
   gate[4].x = x;
   gate[4].y = y;
   gate[5].x = x+1;
   gate[5].y = y;

	if(snake[0].x == gate[4].x && snake[0].y == gate[4].y)
		return true;//LEVEL++ ... ;

	for(int i=0;i<6;++i)
	{
		if(i!=4)
		{
			if(snake[0].x == gate[i].x && snake[0].y == gate[i].y)
			{
				return false;
			}
		}
	}
}
*/


void swapScore(int& x, int& y)//Doi vi tri thu tu diem
{
	x = x + y;
	y = x - y;
	x = x - y;
}
void swapName(string& x, string& y)//Doi vi tri thu tu nguoi choi
{
	string temp = x;
	x = y;
	y = temp;
}
bool checkScore(HIGHSCORE a[], int n, int score)//Kiem tra diem co nam trong top 5 khong
{
	for (int i = 0; i < n; ++i)
	{
		if (score > a[i].score)
			return 1;
	}
	return 0;
}
void saveHighScore(HIGHSCORE a[], int n, int score)//Luu diem nam trong top 5
{
	if (checkScore(a, n, score))
	{
		cout << "Diem cua ban nam trong top 5" << endl;
		cout << "Nhap ten: ";
		getline(cin, a[n - 1].name);
		a[n - 1].score = score;
	}
}
void sortListHighScore(HIGHSCORE a[], int n)//sap xep thu tu diem cao den thap
{
	for (int i = 0; i < n - 1; ++i)
	{
		for (int j = i + 1; j < n; ++j)
		{
			if (a[i].score < a[j].score)
			{
				swapScore(a[i].score, a[j].score);
				swapName(a[i].name, a[j].name);
			}
		}
	}
}
void WriteHighScore(const char* filePath, HIGHSCORE a[], int n)//Luu list high score vao file
{
	ofstream fOut;
	fOut.open(filePath);
	for (int i = 0; i < n; ++i)
	{
		fOut << a[i].name << endl << a[i].score << endl;
	}
	fOut.close();
}
void checkFileGame(string FileGame[], int nFileGame, string filenametemp)//Kiem tra file game co bi trung ten hay khong
{
	for (int i = 0; i < nFileGame; ++i)
	{
		if (filenametemp.compare(FileGame[i]) == 1)
		{
			cout << "File name have already existed. Do you want to replace it? " << endl;
			char choice[1];
			cin >> choice;
			if (choice == "y")
				FileGame[i] = filenametemp;
			break;
		}
	}
}
void SaveGame(const char* filePath, int CHAR_LOCK, int MOVING, int SPEED, int LEVEL, int FOOD_INDEX, int SIZE_SNAKE, int SCORE, int x, int y)
{
	ofstream fOut;
	fOut.open(filePath);
	fOut << CHAR_LOCK << endl;
	fOut << MOVING << endl;
	fOut << SPEED << endl;
	fOut << LEVEL << endl;
	fOut << FOOD_INDEX << endl;
	fOut << SIZE_SNAKE << endl;
	fOut << SCORE << endl;
	for (int i = 0; i < SIZE_SNAKE; ++i)
	{
		fOut << snake[i].x << " " << snake[i].y << endl;
	}
	fOut.close();
}
void StartGame() {
	system("cls");
	ResetData();
	drawGame();
	STATE = 1;//Start running Thread
}
int main() {
	//readFileGame("FileGame.txt");
	//readHighScore("HighScore.txt");

	setWindowSize(WIDTH_WINDOW, HEIGHT_WINDOW);
	setScreenBufferSize(WIDTH_WINDOW, HEIGHT_WINDOW);
	fixConsoleWindow();
	showConsoleCursor(false);
	drawMenu();
	int choice = chooseMenu();
	if (choice == 1) {
		StartGame();
		thread t1(ThreadFunc); //Create thread for snake
		HANDLE handle_t1 = t1.native_handle(); //Take handle of thread
		int temp;
		while (1) {
			temp = toupper(_getch());
			if (STATE == 1) {
				if (temp == 'P') {
					PauseGame(handle_t1);
				}
				else if (temp == 27) {
					ExitGame(handle_t1);
					return 0;
				}
				else {
					ResumeThread(handle_t1);
					if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp ==
						'W' || temp == 'S'))
					{
						if (temp == 'D') CHAR_LOCK = 'A';
						else if (temp == 'W') CHAR_LOCK = 'S';
						else if (temp == 'S') CHAR_LOCK = 'W';
						else CHAR_LOCK = 'D';
						MOVING = temp;
					}
				}
			}
			else {
				if (temp == 'Y') StartGame();
				else {
					ExitGame(handle_t1);
					return 0;
				}
			}
		}
	}

	if (choice == 2) {
		drawLoadGame();
	}

	if (choice == 3) {
		drawHighScore();
	}

	if (choice == 4) {
		drawAboutUs();
	}
}