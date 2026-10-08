
#include<stdio.h>
#include <string.h>
#include<windows.h>
#include "game.h"

extern DWORD oldMode;
extern INPUT_RECORD rec;
extern DWORD nRead;
extern HANDLE hIn;
extern HANDLE hOut;

void drawboard();//绘制棋盘
Game initgame();//初始化游戏
void GetPos(int *x,int *y);//获取初始光标位置
void changegame(Game *game,HANDLE hIn, INPUT_RECORD rec,DWORD nRead,HANDLE hOut,int cy);//改变游戏状态
MoveResult putresult(COORD pos, char cell,int cy);//判断落子情况


