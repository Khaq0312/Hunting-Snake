#pragma once
#pragma comment(lib, "winmm.lib")
void readFileName();
void readHighScore();
bool IsValid(int x, int y);
void GenerateFood();
void readData(std::string filePath);
void loadGame(std::string filePath);
void Eat();
void ResetData();
bool TouchOBS(int x, int y);
bool TouchBody(int x, int y);
void MoveRight();
void MoveLeft();
void MoveUp();
void MoveDown();
void moveGate();
void clearGate();
void ThreadFunc();
void swapScore(int& x, int& y);
void swapName(std::string& x, std::string& y);
void SaveGame(std::string filePath);
void StartGame(std::string f);
void ProcessDead();
int processGame(std::string file, void (*mode)(std::string));