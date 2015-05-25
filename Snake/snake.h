/* *
 *  contains some snakestruct;
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <ncurses.h>



#define TBool            int
#define True             1
#define False            0    
#define SHAPE_FOOD       '@'
#define SHAPE_SNAKE      '#'
#define GAMEWIN_YLEN     15
#define GAMEWIN_XLEN     60
#define INFOWIN_YLEN      7
#define INFOWIN_XLEN      (GAMEWIN_XLEN)
#define INFOBUF_NUM       (INFOWIN_YLEN-2)
#define INFOBUF_LEN       (GAMEWIN_XLEN-2)
#define MAXLEVEL         12

WINDOW *infowin;                             //信息窗口
WINDOW *gamewin;                             //游戏窗口
int g_level;

typedef struct SnakeNode * snakenode;
typedef struct Snake * snake;
typedef struct Food *food;

void INITSCR();                               //初始化窗口,初始化设置    
void ENDWIN();                                //关闭窗口
void INITINFOWIN();                           //初始化通知窗口
void INITGAMEWIN();                           //初始化游戏窗口
void print_infowin(char *str); 
void cleanline(WINDOW * win, int y, int x);
int check_if_food(food Tfood,snake Tsnake);
food creat_food();
snake init_snake();
void destory_snake(snake S);
snakenode newsnakenode();
snakenode GetSnakeTail(snake s);:w
void draw_snake(snake s);
void move_snake(snake s);
void draw_food(food Tfood,snake Tsnake);
int check_snake(snake s,food pfood);
int check_snake_touch(snake s);
void gameover(WINDOW *win, char *str);
int wait_for_press(snake psnake);

