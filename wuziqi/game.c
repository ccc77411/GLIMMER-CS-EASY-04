#include<stdio.h>
#include"game.h"

GameStatus win(char cells[9][9],Game game){      //判断游戏输赢
   
    for ( int i = 0; i < 9 ; i++)                          //横向判断
    {
        for (int j = 0; j < 5; j++)
        {
            if (cells[i][j] == cells[i][j + 1] &&
                cells[i][j] == cells[i][j + 2] &&
                cells[i][j] == cells[i][j + 3] &&
                cells[i][j] == cells[i][j + 4])
            {
                if (cells[i][j]!=EMPTY_CELL)
                {
                    if (cells[i][j]=='X')
                   {
                    return GAME_X_WINS;
                   }
                   else if(cells[i][j]=='O'){  
                   return GAME_O_WINS;
                   }
                }
                
            }
            
        }
    }
    for ( int i = 0; i < 5 ; i++)                        //竖向判断
    {
        for (int j = 0; j < 9; j++)
        {
            if (cells[i][j] == cells[i+1][j] &&
                cells[i][j] == cells[i+2][j] &&
                cells[i][j] == cells[i+3][j] &&
                cells[i][j] == cells[i+4][j])
            {
                if (cells[i][j]!=EMPTY_CELL)
                {
                    if (cells[i][j]=='X')
                   {
                    return GAME_X_WINS;
                   }
                    else if(cells[i][j]=='O'){ 
                        return GAME_O_WINS;
                    }
                }
            }
            
        }
        
    }


    for ( int i = 0; i < 5 ; i++)                          //从左上到右下判断
    {
        for (int j = 0; j < 5; j++)
        {
            if (cells[i][j] == cells[i+1][j+1] &&
                cells[i][j] == cells[i+2][j+2] &&
                cells[i][j] == cells[i+3][j+3] &&
                cells[i][j] == cells[i+4][j+4])
            {
                if (cells[i][j]!=EMPTY_CELL)
                {
                    if (cells[i][j]=='X')
                   {
                    return GAME_X_WINS;
                   }
                   else if(cells[i][j]=='O'){  
                   return GAME_O_WINS;
                   }
                }
            }
            
        }
        
    }

    for ( int i = 4; i < 9 ; i++)                        //从左下到右上判断
    {
        for (int j = 0; j < 5; j++)
        {
            if (cells[i][j] == cells[i-1][j+1] &&
                cells[i][j] == cells[i-2][j+2] &&
                cells[i][j] == cells[i-3][j+3] &&
                cells[i][j] == cells[i-4][j+4])
            {
                if (cells[i][j]!=EMPTY_CELL)
                {
                    if (cells[i][j]=='X')
                   {
                    return GAME_X_WINS;
                   }
                   else if(cells[i][j]=='O'){  
                     return GAME_O_WINS;
                   }
                }
            }
            
        }
        
    }
    if (game.move_count<81)                        
    {
        return GAME_RUNNING;
    }
    else{
        return GAME_DRAW;            
    }
}

