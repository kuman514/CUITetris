#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <Windows.h>

/*
1000 1100 1000 0100 0100 1000 0100
1000 1100 1100 1100 1110 1000 0100
1000 0000 0100 1000 0000 1100 1100
1000 0000 0000 0000 0000 0000 0000

1111 0000 0110 1100 1000 1110 1000
0000 0000 1100 0110 1100 1000 1110
0000 0000 0000 0000 1000 0000 0000
0000 0000 0000 0000 0000 0000 0000

0000 0000 0000 0000 1110 1100 1100
0000 0000 0000 0000 0100 0100 1000
0000 0000 0000 0000 0000 0100 1000
0000 0000 0000 0000 0000 0000 0000

0000 0000 0000 0000 0100 0010 1110
0000 0000 0000 0000 1100 1110 0010
0000 0000 0000 0000 0100 0000 0000
0000 0000 0000 0000 0000 0000 0000
*/

// 0: I, 1: O, 2: S, 3: Z, 4: T, 5: L, 6: 7
bool tetromino[19][4][4] = { {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}}, {{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
                             {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
                             {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}, {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
                             {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}, {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
                             {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, {{1,0,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}, {{1,1,1,0},{0,1,0,0},{0,0,0,0},{0,0,0,0}}, {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},
                             {{1,0,0,0},{1,0,0,0},{1,1,0,0},{0,0,0,0}}, {{1,1,1,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}}, {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}, {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
                             {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}, {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, {{1,1,0,0},{1,0,0,0},{1,0,0,0},{0,0,0,0}}, {{1,1,1,0},{0,0,1,0},{0,0,0,0},{0,0,0,0}} };

// [start, end)
int tetIndex[7][2] = { {0, 2}, {2, 3}, {3, 5}, {5, 7}, {7, 11}, {11, 15}, {15, 19} };
const int maxY = 20, maxX = 10;
bool bricks[maxY + 4][maxX + 4];

int frame;

int line;
int xpos;

int curblock;
int curturn;

void init(void);
void gotoxy(const int, const int);
void invalidate(void);

void fall(void);
void land(void);
void rotate(void);
void move(void);

void countframe(void);

int main(void)
{
    init();

    while (true)
    {
        // process
        fall();
        land();

        // input
        rotate();
        move();

        // paint
        invalidate();

        // 60 FPS
        Sleep(16);
        countframe();
    }

    return 0;
}

void init(void)
{
    srand(time(NULL));
    curblock = rand() % 7;

    line = 0;
    xpos = 3;

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            bricks[line + y][xpos + x] = tetromino[tetIndex[curblock][curturn]][y][x];
        }
    }
}

void gotoxy(const int x, const int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void invalidate(void)
{
    gotoxy(0, 0);

    for (int y = 0; y < maxY; y++)
    {
        printf("▣");

        for (int x = 0; x < maxX; x++)
        {
            printf("%s", bricks[y][x] ? "■" : "□");
        }

        printf("▣");
        putchar('\n');
    }

    for (int x = 0; x < maxX + 2; x++)
    {
        printf("▣");
    }
}

void fall(void)
{
    if (frame >= 59)
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (tetromino[tetIndex[curblock][curturn]][y][x])
                {
                    bricks[line + y][xpos + x] = 0;
                }
            }
        }

        line++;
    }
    else
    {
        return;
    }

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (tetromino[tetIndex[curblock][curturn]][y][x])
            {
                bricks[line + y][xpos + x] = tetromino[tetIndex[curblock][curturn]][y][x];
            }
        }
    }
}

void land(void)
{
    //line = 0;
    //xpos = 3;
}

void rotate(void)
{

}

void move(void)
{

}

void countframe(void)
{
    frame++;

    if (frame >= 60)
    {
        frame = 0;
    }
}