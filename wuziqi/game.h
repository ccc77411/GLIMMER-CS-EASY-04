#ifndef UTILS_H          
#define UTILS_H
#define BOARD_SIZE 9
#define WIN_LENGTH 5
#define EMPTY_CELL '.'

typedef enum {
    GAME_RUNNING, // 游戏进行中
    GAME_X_WINS, // X胜出
    GAME_O_WINS, // O胜出
    GAME_DRAW //平局
} GameStatus; // 枚举游戏状态

typedef enum {
    MOVE_ACCEPTED, // 可以下棋
    MOVE_OUT_OF_BOUNDS, // 当前位置在棋盘外
    MOVE_OCCUPIED // 当前位置已占用
} MoveResult; // 下棋结果

typedef struct {
    char cells[BOARD_SIZE][BOARD_SIZE]; // 棋盘信息
    char current_player;
    int move_count; // 步数统计
    GameStatus status;
} Game;
GameStatus win(char cells[BOARD_SIZE][BOARD_SIZE],Game game);//判断游戏输赢

#endif