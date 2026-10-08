#include<stdio.h>
#include<windows.h>
#include <string.h>
#include"console.h"

DWORD oldMode;
INPUT_RECORD rec;
DWORD nRead;
HANDLE hIn;
HANDLE hOut;

void drawboard(void)
{
    for (int i = 0; i < 19; i++)
    {
        if (i % 2 == 0)
        {
            printf("+");
            for (int j = 0; j < 9; j++)
            {
                printf("---+");
            }
            printf("\n");
        }
        else
        {
            printf("|");
            for (int j = 0; j < 9; j++)
            {
                printf(" . |");
            }
            printf("\n");
        }
    }
}

Game initgame(void)         //初始化游戏的内容
{
    Game game;
    memset(game.cells, EMPTY_CELL, sizeof game.cells);
    game.current_player = 'X';
    game.move_count = 0;
    game.status = GAME_RUNNING;
    return game;
}





MoveResult putresult(COORD pos, char cell,int cy)       
{
    if ( pos.X <= 35 && pos.Y <= 18+cy)  //先判断是否在棋盘内
    {
        if (cell == EMPTY_CELL)      //判断位置是否被占用
        {
            return MOVE_ACCEPTED;
        }
        return MOVE_OCCUPIED;
    }
    return MOVE_OUT_OF_BOUNDS;
}




void GetPos(int *x,int *y){                             
     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);  
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hOut, &info);
    *x = info.dwCursorPosition.X;  
    *y = info.dwCursorPosition.Y;
}





void changegame(Game *game,HANDLE hIn, INPUT_RECORD rec,DWORD nRead,HANDLE hOut,int cy)  
{
    ReadConsoleInput(hIn, &rec, 1, &nRead);                                //读取一条输入记录
    if (!ReadConsoleInput(hIn, &rec, 1, &nRead) || nRead == 0) {      
        return;}
    if (rec.EventType != MOUSE_EVENT){                                   //只读取鼠标信息
        return;
    }
    MOUSE_EVENT_RECORD *m = &rec.Event.MouseEvent;

    if (m->dwEventFlags == 0 &&
        (m->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))               //只读取鼠标按键且为左键信息
    {
        COORD pos = m->dwMousePosition;                                   //取出鼠标点击的位置信息
        int i = pos.X / 4;
        int j = (pos.Y-cy) / 2;                                           //将光标坐标转换成棋盘坐标
        if (putresult(pos,game->cells[j][i],cy)==MOVE_OUT_OF_BOUNDS)        //打印出落子位置是否正确
        {
            SetConsoleCursorPosition(hOut, (COORD){0, 24+cy});
            printf("MOVE_OUT_OF_BOUNDS\n");
        }
        else if (putresult(pos,game->cells[j][i],cy)==MOVE_OCCUPIED)
        {
            SetConsoleCursorPosition(hOut, (COORD){0, 24+cy});
            printf("MOVE_OCCUPIED          \n");
        }
        
        
            if(pos.X<=35&&pos.X%4==2&&pos.Y<=18+cy&&(pos.Y-cy)%2==1){         //如果正确，则改变棋盘信息，将对应数组赋值，在棋盘相应位置打印出对应玩家符号，让步数加一
            SetConsoleCursorPosition(hOut, (COORD){0, 24+cy});
            printf("MOVE_ACCEPTED          \n");
            if (game->cells[j][i] == EMPTY_CELL)
            {
                game->cells[j][i] = game->current_player;
                SetConsoleCursorPosition(hOut, pos);
                printf("%c", game->cells[j][i]);
                if (game->move_count%2==0)
                {
                    game->current_player='O';
                }
                else{
                    game->current_player='X';
                }
                game->move_count++;
                
            }
         }
         SetConsoleCursorPosition(hOut, (COORD){0, 25+cy});
         printf("current_player is %c     GAME_RUNNING\n",game->current_player);      //打印出current_player和game_status
        }

    }

