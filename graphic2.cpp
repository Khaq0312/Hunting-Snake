#include <Windows.h>
#include "graphic2.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <io.h>
#include <fcntl.h>
using namespace std;
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

void drawMenu() {
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
	drawBoard(x, y, 24, 10);
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
			if (choice > 4) choice = 4;
			break;
		case 13:
			return choice;
		}
		for (int i = 1; i <= 4; i++) {
			if (i == choice) {
				setColor(0, 12);
			}
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
			}
			if (i == choice) {
				setColor(0, 7);
			}
		}
	}
}

void eraseDrawing(POINT start, POINT end) {
	for (int i = start.x; i <= end.x; i++) {
		for (int j = start.y; j < end.y; j++) {
			gotoXY(i, j);
			wcout << L" ";
		}
	}
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
	cout << "LEVEL: "; // ======== level duoc cap nhat trong ham ThreadFunc() =========
	gotoXY(x + 30, y);
	cout << "SCORE: "; // ======== score duoc cap nhat trong ham ThreadFunc() =========
	setColor(0, 7);
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

bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		if (snake[i].x == x && snake[i].y == y) {
			return false;
		}
	}
	return true;
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