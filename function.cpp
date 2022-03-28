#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
//#include <io.h>

using namespace std;

#define MAX_SIZE_SNAKE 28   
#define MAX_SIZE_FOOD 5
#define MAX_LEVEL 5
#define MAX_HIGH_SCORE 5
#define MAX_FILE_GAME 5
#define MAX_SPEED 10


POINT snake[10];
POINT food[4];
int CHAR_LOCK;
int MOVING;
int SPEED;
int HEIGHT_CONSOLE, WIDTH_CONSOLE;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE;


bool IsValid(int x, int y)
{
    for(int i=0;i<SIZE_SNAKE;++i)
    {
        if(snake[i].x == x && snake[i].y == y)
            return false;
        return true;
    }  
}

struct COORD
{
    int X,Y;
};

void GotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ProcessDead()
{
    STATE = 0;
    GotoXY(0,HEIGHT_CONSOLE + 2);
    cout<<"dead";   
}

void GenerateFood()
{
    int x,y;
    srand(time(NULL));
    for(int i=0;i<MAX_SIZE_FOOD;++i)
    {
        do
        {
            x = rand() % (WIDTH_CONSOLE - 1) + 1;
            y = rand() % (HEIGHT_CONSOLE - 1) + 1;
        }while(IsValid(x,y) == false);
        food[i] = {x,y};
    }
}

void Eat()
{
    snake[SIZE_SNAKE] = food[FOOD_INDEX];
    if(FOOD_INDEX == MAX_SIZE_FOOD - 1)
    {
        FOOD_INDEX = 0;
        SIZE_SNAKE = 6;
        if(SPEED == MAX_SPEED) SPEED = 1;
        else SPEED++;
        GenerateFood();
    }
    else
    {
        FOOD_INDEX++;
        SIZE_SNAKE++;
    }
}

bool TouchBody()//Cham than
{
    for(int i=3;i<SIZE_SNAKE;++i)
    {
        if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            return false;
    }
    return true;
}


bool TouchGate()//Ham nay mo phong, luc ghep ham thi se chinh sua lai
{ 
    /*
        ***
        * *
    */
   //Toa do loi vao cua cong: 
   int x, int y;
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

bool ChamChuongNgaiVat() //Chua hinh dung duoc @@, trong qua trinh ghep se tim ra
{

}

void MoveRight()
{
    if(snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE)
    {
        ProcessDead();
    }
    else if (TouchBody() == 0)
    {
        ProcessDead();
    }
    else if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE- 1].y == food[FOOD_INDEX].y)
    {
        Eat();
        for(int i=0;i<SIZE_SNAKE-1;++i)
        {
            snake[i].x = snake[i+1].x;
            snake[i].y = snake[i+1].y;
        }
        snake[SIZE_SNAKE - 1].x++;
    }
}

void MoveLeft()
{
    if(snake[SIZE_SNAKE - 1].x - 1 == WIDTH_CONSOLE)
    {
        ProcessDead();
    }
    else if (TouchBody() == 0)
    {
        ProcessDead();
    }
    else if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE- 1].y == food[FOOD_INDEX].y)
    {
        Eat();
        for(int i=0;i<SIZE_SNAKE-1;++i)
        {
            snake[i].x = snake[i+1].x;
            snake[i].y = snake[i+1].y;
        }
        snake[SIZE_SNAKE - 1].x--;
    }
}

void MoveUp()
{
    if(snake[SIZE_SNAKE - 1].y - 1 == HEIGHT_CONSOLE)
    {
        ProcessDead();
    }
    else if (TouchBody() == 0)
    {
        ProcessDead();
    }
    else if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE- 1].y - 1 == food[FOOD_INDEX].y)
    {
        Eat();
        for(int i=0;i<SIZE_SNAKE-1;++i)
        {
            snake[i].x = snake[i+1].x;
            snake[i].y = snake[i+1].y;
        }
        snake[SIZE_SNAKE - 1].y--;
    }
}

void MoveDown()
{
    if(snake[SIZE_SNAKE - 1].y + 1 == HEIGHT_CONSOLE)
    {
        ProcessDead();
    }
    else if (TouchBody() == 0)
    {
        ProcessDead();
    }
    else if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE- 1].y + 1 == food[FOOD_INDEX].y)
    {
        Eat();
        for(int i=0;i<SIZE_SNAKE-1;++i)
        {
            snake[i].x = snake[i+1].x;
            snake[i].y = snake[i+1].y;
        }
        snake[SIZE_SNAKE - 1].y++;
    }
}


void DrawSnakeAndFood(char* s)
{
    GotoXY(food[FOOD_INDEX].x,food[FOOD_INDEX].y);
    cout<<s;
    for(int i=0; i<SIZE_SNAKE;++i)
    {
        GotoXY(snake[i].x,snake[i].y);
        cout<<s;
    }
}

void ThreadFunc()
{
    while(1)
    {
        if(STATE == 1)
        {
            DrawSnakeAndFood(" ");
            switch(MOVING)
            {
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
            DrawSnakeAndFood("*");
        }
    }      
}


struct HIGHSCORE
{
    string name;
    int score;
};

void swapScore(int &x, int &y)//Doi vi tri thu tu diem
{
    x= x+y; 
    y= x-y;
    x= x-y;
}

void swapName(string &x, string &y)//Doi vi tri thu tu nguoi choi
{
    string temp = x;
    x = y;
    y = temp;
}

bool checkScore(HIGHSCORE a[], int n, int score)//Kiem tra diem co nam trong top 5 khong
{  
    for(int i=0;i<n;++i)
    {
        if(score > a[i].score)
            return 1;
    }       
    return 0;
}


void saveHighScore(HIGHSCORE a[], int n, int score)//Luu diem nam trong top 5
{
    if(checkScore(a,n,score))
    {
        cout<<"Diem cua ban nam trong top 5"<<endl;
        cout<<"Nhap ten: ";
        getline(cin,a[n-1].name);
        a[n-1].score = score;
    }
}

void sortListHighScore(HIGHSCORE a[], int n)//sap xep thu tu diem cao den thap
{
    for(int i=0;i<n-1;++i)
    {
        for(int j=i+1;j<n;++j)
        {
            if(a[i].score < a[j].score)
            {
                swapScore(a[i].score,a[j].score);
                swapName(a[i].name,a[j].name);
            }
        }
    } 
}

void WriteHighScore(const char *filePath, HIGHSCORE a[], int n)//Luu list high score vao file
{
    ofstream fOut;
    fOut.open(filePath);
    for(int i=0;i<n;++i)
    {
        fOut<<a[i].name<<endl<<a[i].score<<endl;
    }
    fOut.close();
}

string filegame[MAX_FILE_GAME];
int nFileGame = 0;

void checkFileGame(string FileGame[], int nFileGame ,string filenametemp)//Kiem tra file game co bi trung ten hay khong
{
    for(int i=0;i<nFileGame;++i)
    {
        if(filenametemp.compare(FileGame[i]) == 1)
        {
            cout<<"File name have already existed. Do you want to replace it? "<<endl;
            char choice[1];
            cin>>choice;
            if(choice == "y")
                FileGame[i] = filenametemp;
            break;
        }
    }
}

void SaveGame(const char *filePath, int CHAR_LOCK, int MOVING, int SPEED, int LEVEL, int FOOD_INDEX, int SIZE_SNAKE, int SCORE, int x, int y)
{
    ofstream fOut;
    fOut.open(filePath);

    fOut<<CHAR_LOCK<<endl;
    fOut<<MOVING<<endl;
    fOut<<SPEED<<endl;
    fOut<<LEVEL<<endl;
    fOut<<FOOD_INDEX<<endl;
    fOut<<SIZE_SNAKE<<endl;
    fOut<<SCORE<<endl;
    for(int i=0;i<SIZE_SNAKE;++i)
    {
        fOut<<snake[i].x<<" "<<snake[i].y<<endl;
    }
    fOut.close();
}
