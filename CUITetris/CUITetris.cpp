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

int score;

// 0: I, 1: O, 2: S, 3: Z, 4: T, 5: L, 6: 7
bool tetromino[19][4][4] = { {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}}, {{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
                             {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
                             {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}, {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
                             {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}, {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
                             {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, {{1,0,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}, {{1,1,1,0},{0,1,0,0},{0,0,0,0},{0,0,0,0}}, {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},
                             {{1,0,0,0},{1,0,0,0},{1,1,0,0},{0,0,0,0}}, {{1,1,1,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}}, {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}, {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
                             {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}, {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, {{1,1,0,0},{1,0,0,0},{1,0,0,0},{0,0,0,0}}, {{1,1,1,0},{0,0,1,0},{0,0,0,0},{0,0,0,0}} };

int tetIndex[7][4] = { {0, 1, 0, 1}, {2, 2, 2, 2}, {3, 4, 3, 4}, {5, 6, 5, 6}, {7, 8, 9, 10}, {11, 12, 13, 14}, {15, 16, 17, 18} };
const int maxY = 20, maxX = 10;
bool bricks[maxY + 4][maxX + 4];

int frame;

int line;
int xpos;

int curblock;
int curturn;

bool isgameover;

void reset(void);

void init(void);
void gotoxy(const int, const int);
void invalidate(void);

void fall(void);
int land(void);
int erase(void);

void rotate(const int);
void drop(const int);
void move(const int);

void countframe(void);
void resetframe(void);

int main(void)
{
    init();

    while (!isgameover)
    {
        // process
        fall();
        score += land();

        // input
        if (_kbhit())
        {
            const int input = _getch();

            rotate(input);
            drop(input);
            move(input);
        }

        // paint
        invalidate();

        // approximately 60 FPS
        Sleep(16);
        countframe();

        while (isgameover)
        {
            const int input = _getch();

            if (input == 'r')
            {
                reset();
            }
            else if(input == 'q')
            {
                break;
            }
        }
    }

    return 0;
}

void reset(void)
{
    for (int y = 0; y < maxY; y++)
    {
        for (int x = 0; x < maxX; x++)
        {
            bricks[y][x] = 0;
        }
    }

    isgameover = false;

    resetframe();
    init();
}

void init(void)
{
    srand((unsigned int)time(NULL));
    curblock = rand() % 7;

    line = 0;
    xpos = 3;

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (tetromino[tetIndex[curblock][curturn]][y][x])
            {
                // if already blocked, game over
                if (bricks[line + y][xpos + x])
                {
                    // game over
                    isgameover = true;
                }

                bricks[line + y][xpos + x] = 1;
            }
        }
    }
}

void gotoxy(const int x, const int y)
{
    COORD pos = { (SHORT)x, (SHORT)y };
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

    gotoxy(0, 21);
    printf("SCORE: %d\n", score);

    if (isgameover)
    {
        gotoxy(0, 23);
        printf("GAME OVER");
    }
}

void fall(void)
{
    if (frame >= 60)
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
        resetframe();
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
                //bricks[line + y][xpos + x] = tetromino[tetIndex[curblock][curturn]][y][x];
                bricks[line + y][xpos + x] = 1;
            }
        }
    }
}

int land(void)
{
    int fallpoints = 0;
    bool flag = false;

    for (int y = 0; y < 4 && !flag; y++)
    {
        for (int x = 0; x < 4 && !flag; x++)
        {
            if (y < 3 && tetromino[tetIndex[curblock][curturn]][y + 1][x])
            {
                // if there is another brick below
                continue;
            }

            if (tetromino[tetIndex[curblock][curturn]][y][x])
            {
                // if the lowest bricks land on a ground
                if (bricks[line + y + 1][xpos + x] || line + y + 1 >= maxY)
                {
                    flag = true;
                }
            }
        }
    }

    if (flag)
    {
        fallpoints += (3 * (maxY - line));
        switch (erase())
        {
        case 4:
            fallpoints += 400;
        case 3:
            fallpoints += 300;
        case 2:
            fallpoints += 200;
        case 1:
            fallpoints += 100;
        }
        init();
    }

    return fallpoints;
}

int erase(void)
{
    int result = 0;

    // erase lines
    for (int y = 0; y < maxY; y++)
    {
        int tiles = 0;

        for (int x = 0; x < maxX; x++)
        {
            tiles += (int)bricks[y][x];
        }

        if (tiles >= maxX)
        {
            result++;

            // pull all above
            for (int i = y; i > 0; i--)
            {
                for (int j = 0; j < maxX; j++)
                {
                    bricks[i][j] = bricks[i - 1][j];
                }
            }

            for (int j = 0; j < maxX; j++)
            {
                bricks[0][j] = 0;
            }
        }
    }

    return result;
}

void rotate(const int input)
{
    if (input == 'w')
    {
        // erase the current brick first
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

        int tmpturn = (curturn + 1) % 4;
        int shiftlimit = (curblock == 0) ? 4 : 3;
        bool turnable = true;

        // counting columns to shift to left
        int shifttoleft = 0;
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (tetromino[tetIndex[curblock][tmpturn]][y][x])
                {
                    if (bricks[line + y][xpos + x] || xpos + x >= maxX)
                    {
                        if (shifttoleft < shiftlimit - x)
                        {
                            shifttoleft = shiftlimit - x;
                        }
                    }
                }
            }
        }

        // shift and check again
        int tmpxpos = xpos - shifttoleft;
        for (int y = 0; y < 4 && turnable; y++)
        {
            for (int x = 0; x < 4 && turnable; x++)
            {
                if (tetromino[tetIndex[curblock][tmpturn]][y][x])
                {
                    if (bricks[line + y][tmpxpos + x] || tmpxpos + x >= maxX)
                    {
                        turnable = false;
                    }
                }
            }
        }
        
        if (turnable)
        {
            curturn = tmpturn;
            xpos = tmpxpos;
        }

        // draw bricks back
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (tetromino[tetIndex[curblock][curturn]][y][x])
                {
                    bricks[line + y][xpos + x] = 1;
                }
            }
        }
    }
}

void drop(const int input)
{
    if (input == 's')
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
        resetframe();

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (tetromino[tetIndex[curblock][curturn]][y][x])
                {
                    bricks[line + y][xpos + x] = 1;
                }
            }
        }
    }
    else if (input == ' ')
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

        bool landed = false;
        while (!landed)
        {
            line++;

            for (int y = 0; y < 4 && !landed; y++)
            {
                for (int x = 0; x < 4 && !landed; x++)
                {
                    if (tetromino[tetIndex[curblock][curturn]][y][x])
                    {
                        if (bricks[line + y + 1][xpos + x] || line + y + 1 >= maxY)
                        {
                            landed = true;
                        }
                    }
                }
            }
        }

        resetframe();

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (tetromino[tetIndex[curblock][curturn]][y][x])
                {
                    bricks[line + y][xpos + x] = 1;
                }
            }
        }

        score += land();
    }
}

void move(const int input)
{
    bool moveable = true;

    switch (input)
    {
    case 'a':
        // move left
        for (int y = 0; y < 4 && moveable; y++)
        {
            for (int x = 0; x < 4 && moveable; x++)
            {
                if (x > 0 && tetromino[tetIndex[curblock][curturn]][y][x - 1])
                {
                    // if there is another brick beside
                    continue;
                }

                if (tetromino[tetIndex[curblock][curturn]][y][x])
                {
                    if (xpos + x <= 0 || bricks[line + y][xpos + x - 1])
                    {
                        moveable = false;
                    }
                }
            }
        }

        if (moveable)
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

            xpos--;

            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (tetromino[tetIndex[curblock][curturn]][y][x])
                    {
                        bricks[line + y][xpos + x] = 1;
                    }
                }
            }
        }
        break;

    case 'd':
        // move right
        for (int y = 0; y < 4 && moveable; y++)
        {
            for (int x = 0; x < 4 && moveable; x++)
            {
                if (x < 3 && tetromino[tetIndex[curblock][curturn]][y][x + 1])
                {
                    // if there is another brick beside
                    continue;
                }

                if (tetromino[tetIndex[curblock][curturn]][y][x])
                {
                    if (xpos + x >= maxX - 1 || bricks[line + y][xpos + x + 1])
                    {
                        moveable = false;
                    }
                }
            }
        }

        if (moveable)
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

            xpos++;

            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (tetromino[tetIndex[curblock][curturn]][y][x])
                    {
                        bricks[line + y][xpos + x] = 1;
                    }
                }
            }
        }

        break;
    };
}

void countframe(void)
{
    frame++;
}

void resetframe(void)
{
    frame = 0;
}