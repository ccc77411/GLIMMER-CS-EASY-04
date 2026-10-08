# C-EASY-4：小应用之五子棋游戏  
## 思考思路 
### step1_项目要求和项目结构  
1.其实刚看完这一题，整个人都是蒙的完全不知道这个多文件项目是怎么运行，怎么联系在一起的。所以我的第一步是先去了解了多文件项目各个文件之间是怎么相互联系的（求助AI）。然后，我才知道文件中不同的c语言文件是靠头文件联系在一起的。比如game.c中定义的函数要先在game.h中声明然后game.c和main.c一起引用头文件game.h，这样才能在main.c中调用函数。  
2.搞清楚这个后，我就先建立好了这五个文件，然后把题中给的结构体，枚举型的定义写在了game.c中，之后我没先写main.c的框架而是直接到step2，因为感觉对整体的框架还是有点不清楚。  
### step2_引入坐标，绘制棋盘
**1.初始化棋盘，将game初始化，cells全部赋值为“.”，currentplayer为”X”,move_count为0，status为GAME_RUNNING**  
- 这里我就先定义了一个初始化棋盘的函数`Game initgame()`然后按要求进行赋值，然后返回一个Game类型的变量。
- 展示  
````
Game initgame(void)         //初始化游戏的内容
{
    Game game;
    memset(game.cells, EMPTY_CELL, sizeof game.cells);
    game.current_player = 'X';
    game.move_count = 0;
    game.status = GAME_RUNNING;
    return game;
}
````

**2.绘制棋盘**  
- 因为要绘制棋盘，所以先定义了一个函数`void drawboard()`然后我就想要怎么绘制棋盘，可以看出棋盘的排列是很规则的，所以我可以写出一小段，再按这样接着打印下去，所以我用了两个循环结构打印出了这个9*9的棋盘。函数如下  
````
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
````  

**3.读取鼠标左键位置，将光标坐标转化为棋盘坐标，将棋盘坐标对应的cells数组位置赋值为currentplayer，currentplayer变为相反元素**  
- 这一部分可以说是step2中最难的一步了首先就是这个Windows Console API，我让AI写了一段读取光标位置的代码，然后让它一行行解释给我，然后我根据它给我的代码该写一下用于读取鼠标左键点击时的光标位置。  
- 然后是将光标坐标转换为棋盘坐标，这里我就发现了一个问题（其实是在后面试运行时发现的，写在这里是为了更方便解释）这里光标位置是从整个终端开始的，而不是从当前命令行位置开始算的，所以运行时终端中的代码行数会影响坐标的转换，所以我想有没有一个能读取当前光标位置的函数，这样我就可以以此为基准来进行转换。然后依旧询问AI老师，果真找到了，所以我就定义了`void GetPos(int *x,int *y)`这个函数，如下  
````
void GetPos(int *x,int *y){                             
     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);  
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hOut, &info);
    *x = info.dwCursorPosition.X;  
    *y = info.dwCursorPosition.Y;
}
````  
- 然后依据读取的坐标写出如下的坐标转换  
````
int i = pos.X / 4;
int j = (pos.Y-cy) / 2;
````  
- 接着还要判断这个坐标是否符合标准，是否点在了棋盘内。于是我又定义了`MoveResult putresult(COORD pos, char cell,int cy) `这个函数来判断落子是否正确。如下  
````
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
````  
- 最后再将棋盘坐标对应的cells数组位置赋值为currentplayer并打印出来，接着根据步数的奇偶性来让currentplayer变为相反元素（但好像也有更直接的方法，只是不爱写了）。最后将这些合在一起就成了`void changegame(Game *game,HANDLE hIn, INPUT_RECORD rec,DWORD nRead,HANDLE hOut,int cy)`（其中加入了打印current_player和game_status等信息的部分）即重新绘制棋盘。如下    
````
changegame(Game *game,HANDLE hIn, INPUT_RECORD rec,DWORD nRead,HANDLE hOut,int cy)  
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
````  
**4.循环直到达成游戏结束的条件**  
- 这里把循环结构写在了main.c中。因为还没写判断胜利的条件，所以我先把条件写成1，让其进行死循环来进行试运行，这就发现了上面的问题。  
### step3_游戏结束的条件判定  
- 这一部分主要处理的应该就是判断胜负的代码了，这里我只想到了通过判断横向，竖向，左上到右下，左下到右上五个数组元素相同这四种情况来利用循环结构走遍每一格，看是否满足了这四种情况之一，然后看是五个"x"还是五个“O”，接着返回相应结果，这里同时还要看若步数达到81，还未有条件达成，则返回平局。根据这一思路写出了下面的函数。(感觉很繁琐，但我也只能想到这一方法了，感觉自己太蠢了)  
````
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

````
- 最后将这一函数用于主函数循环结构来判断循环条件和打印结果的部分，然后写打印结果的部分，最后加上清空控制台的函数，初始化控制台。  
- 还要再补充一些细节如头文件之间的引用，和头文件中函数的声明等，就完成了。 
## 回答step3中问题  
### 1.你需要在上述的哪一步对cells数组进行检测？你要做怎样的检测？   
- 在判断循环是否继续的时候进行检测，这样可以让循环利用其让循环结束。   
- 通过判断横向，竖向，左上到右下，左下到右上五个数组元素相同这四种情况来利用循环结构走遍每一格，看是否满足了这四种情况之一。  
### 2.请你说说为什么要设计棋盘坐标和字符坐标两种坐标？  
- 棋盘坐标是为方便存入cells数组中且便于看是否游戏结束，字符坐标是为了便于读取光标位置。    
### 3.为什么是在棋盘下面？  
- 在棋盘下面方便打印棋盘（棋盘位置不需要位移）和方便通过棋盘位置确定打印信息的位置，同时也方便看信息。  
#### 最后的最后，展示一下运行结果。  
<img width="1028" height="711" alt="屏幕截图 2026-10-08 222201" src="https://github.com/user-attachments/assets/204c0a59-befd-4628-a16f-65c6fc10de82" />
 
--- 
PS：只能说这又是一个令人头大的题目，依旧开始懵懵懂懂，做到后面焕然大悟(觉得自己很蠢)。我觉得主要难点有以下几点：  
1.最大的就是那个Windows Console API读取鼠标左键光标位置，看半天才看懂。  
2.还有就是那个读取当前光标位置的函数，没加这个之前，我试了半天，都会出现下面几行按不了的情况，然后我看了半天的代码感觉没问题啊，后面灵机一动，才想到可能是光标定位的问题。   
3.就是胜利条件的判断，这个个人感觉还是很繁琐，但我真的想不到还有啥，我也不想直接抄AI的，因为感觉不是自己掌握的，心里没底。
