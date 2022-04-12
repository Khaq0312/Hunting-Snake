#include "graphic.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <conio.h>
using namespace std;
//an/hien con tro chuot
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
	gotoXY(x, y); cout << (char)220;
	for (int i = 1; i < width; i++) cout << (char)220;
	cout << (char)220;
	gotoXY(x, height + y); cout << (char)223;
	for (int i = 1; i < width; i++) cout << (char)223;
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
//ve duong thang dung
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
		gotoXY(x, y++);
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
	}
}
int menu() {
	system("cls");
	// Title
	setColor(BG_COLOR, 14);
	int x = (WIDTH_WINDOW - 100) / 2, y = 4;
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < 6; i++) {
		gotoXY(x, y++);
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
		Sleep(80);
	}
	mode = _setmode(_fileno(stdout), mode);
	int choice = 0, temp;
	int w = 24, h = 12;
	x = (WIDTH_WINDOW - w) / 2, y = (HEIGHT_WINDOW + y - h) / 2;
	// draw art snake
	setColor(BG_COLOR, 10);
	drawArtSnake(19, y + 2);
	drawArtSnake(WIDTH_WINDOW - 32, y + 2);
	// show menu
	setColor(BG_COLOR, 7);
	drawBoard(x, y, w, h);
	x += 8, y += 2;
	while (1) {
		for (int i = 0; i < 5; i++) {
			gotoXY(x, y + 2 * i);
			if (i == choice) setColor(BG_COLOR, 12);
			switch (i) {
			case 0:
				cout << "NEW GAME";
				break;
			case 1:
				cout << "LOAD GAME";
				break;
			case 2:
				cout << "HIGH SCORE";
				break;
			case 3:
				cout << "ABOUT US";
				break;
			case 4:
				cout << "EXIT";
				break;
			}
			if (i == choice) setColor(BG_COLOR, 7);
		}
		// choose
		do {
			temp = _getch();
		} while (temp == 224);
		switch (temp) {
		case 72:
			choice--;
			if (choice < 0) choice = 0;
			break;
		case 80:
			choice++;
			if (choice >= 5) choice = 4;
			break;
		case 13:
			return choice;
		default:
			break;
		}
	}
}
void eraseDrawing(POINT start, POINT end) {
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = start.x; i <= end.x; i++) {
		for (int j = start.y; j <= end.y; j++) {
			gotoXY(i, j);
			wcout << L" ";
		}
	}
	mode = _setmode(_fileno(stdout), mode);
}
// ve giao dien luc choi game
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
		Sleep(1000);
		eraseDrawing({ 1,1 }, { WIDTH_CONSOLE - 1, HEIGH_CONSOLE - 1 });
	}
	mode = _setmode(_fileno(stdout), mode);
	// Instruction
	setColor(BG_COLOR, 8);
	int width = WIDTH_WINDOW - WIDTH_CONSOLE - 5, xBeg = WIDTH_CONSOLE + 4;
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
	gotoXY(x, y + 1);
	cout << "Esc: Exit game";
	setColor(BG_COLOR, 8);
	// show level, score
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
void drawArtVideoGame(int x, int y) {
	for (int i = 0; i < 10; i++) {
		gotoXY(x, y++);
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
	}
}
// ve danh sach hien thi file load game va chon
string chooseFileLoadGame() {
	system("cls");
	// draw title
	setColor(BG_COLOR, 14);
	int x = (WIDTH_WINDOW - 51) / 2, y = 4;
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < 3; i++) {
		gotoXY(x, y++);
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
		Sleep(80);
	}
	mode = _setmode(_fileno(stdout), mode);
	// draw art video game
	setColor(BG_COLOR, 6);
	drawArtVideoGame(14, 1);
	drawArtVideoGame(WIDTH_WINDOW - 26, 1);
	// return to menu
	gotoXY((WIDTH_WINDOW - 30) / 2, HEIGHT_WINDOW - 2);
	setColor(BG_COLOR, 8);
	cout << "Press 'Esc' to return to menu";
	// draw board
	setColor(BG_COLOR, 7);
	y += 4;
	drawBoard(x, y, 50, 12);
	int choice = 0, temp;
	x = (WIDTH_WINDOW - 24) / 2, y += 2;
	while (1) {
		// show list
		for (int i = 0; i < nFileGame; i++) {
			if (i == choice) setColor(BG_COLOR, 12);
			gotoXY(x, y + i * 2);
			cout << filegame[i];
			if (i == choice) setColor(BG_COLOR, 7);
		}
		// choose
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
		case 27: return " ";
		default: break;
		}
	}
}
void drawArtTrophy(int x, int y) {
	for (int i = 0; i < 10; i++) {
		gotoXY(x, y++);
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
	}
}
// ve danh sach 5 nguoi choi dat diem cao
void drawHighScore() {
	system("cls");
	//draw title
	setColor(BG_COLOR, 14);
	int x = (WIDTH_WINDOW - 55) / 2, y = 4;
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < 3; i++) {
		gotoXY(x, y++);
		switch (i) {
		case 0:
			wcout << L"▒█░▒█ ▀█▀ ▒█▀▀█ ▒█░▒█   ▒█▀▀▀█ ▒█▀▀█ ▒█▀▀▀█ ▒█▀▀█ ▒█▀▀▀"; break;
		case 1:
			wcout << L"▒█▀▀█ ▒█░ ▒█░▄▄ ▒█▀▀█   ░▀▀▀▄▄ ▒█░░░ ▒█░░▒█ ▒█▄▄▀ ▒█▀▀▀"; break;
		case 2:
			wcout << L"▒█░▒█ ▄█▄ ▒█▄▄█ ▒█░▒█   ▒█▄▄▄█ ▒█▄▄█ ▒█▄▄▄█ ▒█░▒█ ▒█▄▄▄"; break;
		}
		Sleep(80);
	}
	x -= 2, y += 2;
	setColor(BG_COLOR, 4);
	for (int i = 0; i < 4; i++) {
		gotoXY(x, y++);
		switch (i) {
		case 0:
			wcout << L" __________  ___    ____   ___  __   _____  _________  ____"; break;
		case 1:
			wcout << L"/_  __/ __ \\/ _ \\  / __/  / _ \\/ /  / _ \\ \\/ / __/ _ \\/ __/"; break;
		case 2:
			wcout << L" / / / /_/ / ___/ /__ \\  / ___/ /__/ __ |\\  / _// , _/\\ \\  "; break;
		case 3:
			wcout << L"/_/  \\____/_/    /____/ /_/  /____/_/ |_|/_/___/_/|_/___/  "; break;
		}
	}
	mode = _setmode(_fileno(stdout), mode);
	// draw trophy art
	setColor(BG_COLOR, 6);
	drawArtTrophy(8, 1);
	drawArtTrophy(WIDTH_WINDOW - 24, 1);
	// return to menu
	gotoXY((WIDTH_WINDOW - 40) / 2, HEIGHT_WINDOW - 2);
	setColor(BG_COLOR, 8);
	cout << "Press 'Esc' or 'Enter' to return to menu";
	// draw board
	setColor(BG_COLOR, 7);
	x += 2, y += 2;
	drawBoard(x, y, 54, 12);
	drawVerLine(219, 11, x + 4, y + 1);
	drawVerLine(219, 11, x + 40, y + 1);
	// show list of high scores
	y += 2;
	for (int i = 0; i < nHighscore; i++) {
		if (i == 0) setColor(0, 10);
		if (i == 1 || i == 2) setColor(0, 3);
		gotoXY(x + 2, y + i);
		cout << i + 1;
		gotoXY(x + 7, y + i);
		cout << highscore[i].name;
		gotoXY(x + 43, y + i);
		cout << highscore[i].score;
		if (i == 0 || i == 1 || i == 2) setColor(0, 7);
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
		gotoXY(x, y++);
		switch (i)
		{
		case 0:
			wcout << L"░█▀▀█ ▒█▀▀█ ▒█▀▀▀█ ▒█░▒█ ▀▀█▀▀   ▒█░▒█ ▒█▀▀▀█"; break;
		case 1:
			wcout << L"▒█▄▄█ ▒█▀▀▄ ▒█░░▒█ ▒█░▒█ ░▒█░░   ▒█░▒█ ░▀▀▀▄▄"; break;
		case 2:
			wcout << L"▒█░▒█ ▒█▄▄█ ▒█▄▄▄█ ░▀▄▄▀ ░▒█░░   ░▀▄▄▀ ▒█▄▄▄█"; break;
		}
		Sleep(80);
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
		gotoXY(x, y++);
		switch (i) {
		case 0: wcout << L"▄▄▀█▄"; break;
		case 1: wcout << L"▀▀▀██"; break;
		case 2: wcout << L" ▄██▀  ███     ███"; break;
		case 3: wcout << L"███▀▄███ ███ ███ ███ ▄"; break;
		case 4: wcout << L"▀█████▀   ▀███▀   ▀██▀"; break;
		}
	}
	mode = _setmode(_fileno(stdout), mode);
	// info
	x = (WIDTH_WINDOW - 55) / 2 + 3, y += 1;
	for (int i = 0; i < 7; i++) {
		gotoXY(x, y++);
		switch (i) {
		case 0:
			setColor(BG_COLOR, 14);
			cout << "School: ";
			setColor(BG_COLOR, 7);
			cout << "Ho Chi Minh University of Science (HCMUS)";
			break;
		case 1:
			setColor(BG_COLOR, 14);
			cout << "Class: ";
			setColor(BG_COLOR, 7);
			cout << "21CLC7";
			x += 20; y--;
			break;
		case 2:
			setColor(BG_COLOR, 14);
			cout << "Team: ";
			setColor(BG_COLOR, 7);
			cout << "5";
			x -= 20;
			break;
		case 3:
			setColor(BG_COLOR, 14);
			cout << "Members:";
			setColor(BG_COLOR, 7);
			x += 3;
			break;
		case 4:
			cout << "Tran Thi Thanh Van - 21127478"; break;
		case 5:
			cout << "Ha Minh Khang - 21127623"; break;
		case 6:
			cout << "Nguyen Chau Vi Cuong - 21127237"; break;
		}
	}
	// return to menu
	gotoXY((WIDTH_WINDOW - 40) / 2, HEIGHT_WINDOW - 2);
	setColor(BG_COLOR, 8);
	cout << "Press 'Esc' or 'Enter' to return to menu";
	setColor(BG_COLOR, 7);
}
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
void Level2() {
	drawHorOBS(219, 5, WIDTH_CONSOLE / 2 - 2, HEIGH_CONSOLE / 2, OBScount, OBSTACLE);
	drawVerOBS(219, 5, WIDTH_CONSOLE / 2, HEIGH_CONSOLE / 2 - 2, OBScount, OBSTACLE);
}
void Level3() {
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
void Level4() {
	if (OBScount == 0) Level3();
	drawVerOBS(219, 3, WIDTH_CONSOLE / 2, 1, OBScount, OBSTACLE);
	drawHorOBS(219, 5, 1, HEIGH_CONSOLE / 2, OBScount, OBSTACLE);
	drawVerOBS(219, 3, WIDTH_CONSOLE / 2, HEIGH_CONSOLE - 3, OBScount, OBSTACLE);
	drawHorOBS(219, 5, WIDTH_CONSOLE - 5, HEIGH_CONSOLE / 2, OBScount, OBSTACLE);
}
// kiem tra trung cua chuong ngai vat level 5
bool IsValidOBS(int x, int y, int mode) {
	for (int j = 0; j < 5 - 2 * mode; j++)
	{
		int count = OBScount;
		if (SIZE_SNAKE > OBScount)
			count = SIZE_SNAKE;
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
void Level5() {
	srand(SEED);
	for (int i = 0; i < (LEVEL - 1) * 5 + LEVEL; i++) {
		rand(); rand();
	}
	if (OBScount == 0) Level4();
	int x, y;
	for (int i = 0; i < 2; i++) {
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
void DrawGateIn() {
	food[FOOD_INDEX] = { 0 , HEIGH_CONSOLE + 1 };
	setColor(BG_COLOR, 9);
	srand(SEED);
	for (int i = 1; i < LEVEL; i++) {
		rand(); rand();
	}
	do {
		trigger.x = rand() % (WIDTH_CONSOLE - 5) + 2;
		trigger.y = rand() % (HEIGH_CONSOLE - 7) + 4;
	} while (!IsValidGate(trigger.x, trigger.y));
	gotoXY(trigger.x, trigger.y);
	cout << 'x';
	drawHorOBS(219, 3, trigger.x - 1, trigger.y + 1, triggerCount, TRIGGER);
	drawVerOBS(219, 1, trigger.x - 1, trigger.y, triggerCount, TRIGGER);
	drawVerOBS(219, 1, trigger.x + 1, trigger.y, triggerCount, TRIGGER);
	setColor(BG_COLOR, 7);
}
void drawPointOut() {
	gotoXY(WIDTH_CONSOLE - 10, 0);
	cout << (char)220;
}
void DrawSnake() {
	char ms[25] = "211274782112762321127237";
	int j = 0;
	for (int i = SIZE_SNAKE - 1; i >= 0; i--) {
		gotoXY(snake[i].x, snake[i].y);
		cout << ms[j++];
		if (j == 24) j = 0;
	}
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
void drawPauseGameNoti() {
	int w = 30, h = 5;
	int x = (WIDTH_CONSOLE - w) / 2, y = (HEIGH_CONSOLE - h) / 2;
	setColor(BG_COLOR, 14);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, x, y + i);
		Sleep(80);
	}
	setColor(14, 0);
	gotoXY(x + 2, y + 2);
	cout << "Press any key to continue!";
	setColor(BG_COLOR, 7);
}
int chooseYesNo(int x, int y) { // yes->1, no->0
	int temp, choice = 1;
	const int bgColor = 14;
	while (1) {
		gotoXY(x, y);
		setColor(bgColor, 0);
		if (choice == 1) setColor(bgColor, 4);
		cout << "YES";
		gotoXY(x + 10, y);
		setColor(bgColor, 0);
		if (choice == 0) setColor(bgColor, 4);
		cout << "NO";
		do {
			temp = _getch();
		} while (temp == 224);
		switch (temp) {
		case 75: choice = 1; break;
		case 77: choice = 0; break;
		case 13: return choice;
		default: break;
		}
	}
}
int drawGameOver() { // yes(1) -> play again, no(0) -> turn back menu
	const int boardColor = 14;
	int w = 60, h = 9, xS = (WIDTH_CONSOLE - w) / 2 + 1, yS = (HEIGH_CONSOLE - h) / 2;
	int x = (WIDTH_CONSOLE - 26) / 2, y = HEIGH_CONSOLE + 1;
	for (int i = 0; i < 25; i++) {
		gotoXY(x + i, y); cout << " ";
	}
	x = xS, y = yS;
	setColor(boardColor, boardColor);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, x, y + i);
	}
	x += (w - 55) / 2, y += 2;
	int n = 0;
	while (n < 5) {
		if (n > 0) {
			setColor(boardColor, boardColor);
			for (int j = 1; j < 5; j += 2) {
				drawHorLine(219, w, xS, y + j);
			}
		}
		int mode = _setmode(_fileno(stdout), _O_U16TEXT);
		setColor(boardColor, 4);
		for (int i = 0; i < 5; i++) {
			if (n % 2 != 0 && i % 2 != 0) gotoXY(x + 1, y + i);
			else gotoXY(x, y + i);
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
		}
		mode = _setmode(_fileno(stdout), mode);
		Sleep(200);
		n++;
	}
	// check high score to save
	inputHighscoreInfo();
	// ask if play again?
	x = xS, y = yS;
	setColor(boardColor, boardColor);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, x, y + i);
		Sleep(80);
	}
	setColor(boardColor, 0);
	y += (h - 3) / 2;
	gotoXY(x + (w - 26) / 2, y);
	cout << "Do you want to play again?";
	y += 2, x += (w - 15) / 2;
	int ch = chooseYesNo(x, y);
	setColor(BG_COLOR, 7);
	return ch;
}
void reDrawGamePlay() {
	eraseDrawing({ 1,1 }, { WIDTH_CONSOLE - 1,HEIGH_CONSOLE - 1 });
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
			Sleep(80);
		}
		setColor(boardColor, 0);
		x += (w - 17) / 2;
		gotoXY(x, ++y);
		showConsoleCursor(true);
		cout << "Enter file name:";
		do {
			gotoXY(x, y + 1);
			getline(cin, name);
		} while (name == "");
		showConsoleCursor(false);
		name = name + ".txt";
		index = checkFileGame(name);
		if (index != -1) {
			x = xS + (w - 30) / 2;
			gotoXY(x, y);
			cout << "File name has already existed.";
			gotoXY(x, y + 1);
			cout << "Do you want to replace it?";
			x = xS + (w - 15) / 2, y += 3;
			choice = chooseYesNo(x, y);
			if (choice) changeNewFileToLast(index);
		}
		else {
			if (nFileGame < MAX_FILE_GAME) {
				filegame[nFileGame++] = name;
			}
			else {
				const char* str = filegame[0].c_str();
				remove(str);
				filegame[0] = name;
				changeNewFileToLast(0);
			}
			choice = 1;
		}
	} while (choice == 0);
	saveFileNamesToFile();
	setColor(BG_COLOR, boardColor);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, xS, yS + i);
	}
	gotoXY(xS + (w - 23) / 2, yS + 2);
	setColor(boardColor, 4);
	cout << "SAVE GAME SUCCESSFULLY!";
	setColor(BG_COLOR, 7);
	return name;
}
void inputHighscoreInfo() {
	int check = checkScore();
	if (check == 0) return;
	int w = 60, h = 9, x, y, exist, xS, yS;
	const int boardColor = 14;
	string name;
	xS = (WIDTH_CONSOLE - w) / 2 + 1, yS = (HEIGH_CONSOLE - h) / 2;
	do {
		x = xS, y = yS;
		setColor(BG_COLOR, boardColor);
		for (int i = 0; i < h; i++) {
			drawHorLine(219, w, xS, y + i);
			Sleep(80);
		}
		setColor(boardColor, 4);
		x = xS + (w - 40) / 2, y = yS + (h - 3) / 2;
		gotoXY(x, y);
		cout << "CONGRATULATIONS! YOUR SCORE IS IN TOP 5!";
		x = xS + (w - 17) / 2;
		gotoXY(x, ++y);
		showConsoleCursor(true);
		setColor(boardColor, 0);
		cout << "Enter your name: ";
		do {
			gotoXY(x, y + 1);
			getline(cin, name);
		} while (name == "");
		showConsoleCursor(false);
		exist = checkPlayerName(name);
		if (exist) {
			y += 2;
			gotoXY(xS + (w - 30) / 2, y);
			cout << "Your name has already existed.";
			Sleep(400);
		}
		else {
			if (check == 1) {
				highscore[nHighscore].score = SCORE;
				highscore[nHighscore].name = name;
				nHighscore++;
			}
			else {
				highscore[MAX_HIGH_SCORE - 1].score = SCORE;
				highscore[MAX_HIGH_SCORE - 1].name = name;
			}
			sortListHighScore();
			WriteHighScore();
		}
	} while (exist);
	setColor(BG_COLOR, boardColor);
	for (int i = 0; i < h; i++) {
		drawHorLine(219, w, xS, yS + i);
	}
	gotoXY(xS + (w - 48) / 2, yS + (h - 1) / 2);
	setColor(boardColor, 4);
	Sleep(200);
	cout << "GO TO 'HIGH SCORE' IN THE MENU TO SEE YOUR NAME!";
	setColor(BG_COLOR, 7);
	Sleep(700);
}