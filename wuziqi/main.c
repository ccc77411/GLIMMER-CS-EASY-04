#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include"game.h"
#include"console.h"

int main(void)
{
    int cx,cy;
    Game game = initgame();             //初始化游戏
    
    hIn  = GetStdHandle(STD_INPUT_HANDLE);  
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);     //获取输入和输出的句柄   
    GetConsoleMode(hIn, &oldMode);              //存储当前的控制台模式
    GetPos(&cx,&cy);                            //获取当前光标的坐标，方便之后定位
    drawboard();                               //绘制棋盘
    
    SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);  //设置输入模式，启用鼠标，禁用快速编辑模式
    SetConsoleCursorPosition(hOut, (COORD){0, 25+cy});               //将光标移动到第（25+cy）行并打印现在的玩家是谁
    printf("current_player is %c     GAME_RUNNING\n",game.current_player);
    

    while (win(game.cells, game) == GAME_RUNNING)                   //判断是否结束游戏，若否，则继续循环，若是则进入打印结果阶段
    {
      changegame(&game, hIn, rec,nRead,hOut,cy);
      if (game.move_count>80)                          //这里是用来让循环在平局时结束
      {
        break;
      }
      
    }
    SetConsoleCursorPosition(hOut, (COORD){0, 25+cy});
    printf("current_player is %c                   \n",game.current_player);
    SetConsoleCursorPosition(hOut, (COORD){0, 26+cy});     //让光标移动到第（26+cy）行来打印结果
    if (win(game.cells, game) == GAME_X_WINS)              
    {
      printf("The winner is x\n");
    }
    else if (win(game.cells, game) == GAME_O_WINS)
    {
      printf("The winner is O\n");
    }
    else
    {
      printf("Draw\n");
    }
    printf("The move_conut is:%d\n",game.move_count);     
    system("pause");                                    //按任意键继续，让玩家可以看结果
    SetConsoleMode(hIn, oldMode);
    system("cls");                                 //清空控制台
    return 0;
}


