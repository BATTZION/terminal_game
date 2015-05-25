#include "snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum Direction {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

struct SnakeNode{
	int x,y;
	snakenode front;
};
struct Snake{
	int length;
	snakenode head;
	enum Direction dir;
};
struct Food{
	int x,y;
};
snake init_snake()
{
	int i;
	snakenode a[4];
	snake newsnake;
	newsnake=(snake)malloc(sizeof(struct Snake));
	for(i=0;i<4;i++){
	  a[i]=(snakenode)malloc(sizeof(struct SnakeNode));
	  a[i]->x=10-i;
	  a[i]->y=6;
	  if(i!=0)
		a[i]->front=a[i-1];
	}
   a[0]->front=a[3];
   newsnake->length=4;
   newsnake->head=a[0];
   newsnake->dir=DIR_RIGHT;
   return newsnake;
}
void destory_snake(snake s)
{
	snakenode Dsnakenode=s->head;
	snakenode temp=NULL;
	while((s->length)--){
	  temp=Dsnakenode;
	  Dsnakenode=Dsnakenode->front;
	  free(temp);
	}
	Dsnakenode=NULL;
	free(s);
}

snakenode GetSnakeTail(snake s)
{
	return (s->head)->front;
}

void draw_snake(snake psnake)
{
    static int taily = 0;
    static int tailx = 0;
    if (taily != 0 && tailx != 0) {
    /* mvwaddch是把光标移动到指定窗口中(win)的指定位置(taily,tailx)然后输出字符 */
        mvwaddch(gamewin, taily, tailx, ' ');
    }
      
    /* #define GetSnakeTail(s)  ((s)->head->front) */  
    snakenode psnode = GetSnakeTail(psnake);
      
    int i = 0;
    for (; i < psnake->length; ++i) {
        mvwaddch(gamewin, psnode->y, psnode->x, SHAPE_SNAKE);
        psnode = psnode->front;
    }
      
    taily = GetSnakeTail(psnake)->y;
    tailx = GetSnakeTail(psnake)->x;
  
    wrefresh(gamewin);
}
void move_snake(snake s)
{
	int nowx=s->head->x;
	int nowy=s->head->y;
	snakenode newhead;
	newhead=GetSnakeTail(s);
	switch(s->dir){
		case DIR_DOWN:
			newhead->x=nowx;
			newhead->y=nowy+1;
			break;
		case DIR_UP:
			newhead->x=nowx;
			newhead->y=nowy-1;
			break;
		case DIR_LEFT:
			newhead->x=nowx-1;
			newhead->y=nowy;
			break;
		case DIR_RIGHT:
			newhead->x=nowx+1;
			newhead->y=nowy;
			break;
		default:
			break;
	}
	s->head=newhead;
}
void INITSCR()
{
	initscr();       //初始化,进入ncurses模式
	raw();           //禁止行缓冲
	noecho();        //不在终端上显示控制字符
	keypad(stdscr,TRUE);      //允许用户在终端中使用键盘
	curs_set(0);              //设置光标不可见
	refresh();                 //刷新屏幕
}
void INITINFOWIN()
{
    infowin= newwin(INFOWIN_YLEN,INFOWIN_XLEN,GAMEWIN_YLEN + 2, 3);
	box(infowin,0,0);
	mvwprintw(infowin,0,2," INFO ");
	wrefresh(infowin);
	print_infowin("  Press 'q' or 'Q' to quit.");
    print_infowin("  Press 'w/s/a/d' or 'W/S/A/D' to move the snake.");
    print_infowin("Info:");

}
void INITGAMEWIN()
{
	gamewin=newwin(GAMEWIN_YLEN, GAMEWIN_XLEN, 1, 3);
	box(gamewin,0,0);
	mvwprintw(gamewin, 0, 2, " GMAE ");
	mvwprintw(gamewin, GAMEWIN_YLEN-1, 2, "level: %d", g_level);
	mvwprintw(gamewin, GAMEWIN_YLEN-1, 30, "speed: %d", (int)(g_level/3));
	wrefresh(gamewin);
}

void print_infowin(char *str)
{
	static char infobuf[INFOBUF_NUM][INFOBUF_LEN]={0};
	static int index=0;
	strcpy(infobuf[index],str);
	int i=0;
	for(;i<INFOBUF_NUM;i++){
		cleanline(infowin,i+1,1);
		mvwprintw(infowin,i+1,1,infobuf[(index+i)%INFOBUF_NUM]);
		wrefresh(infowin);
	}
	index=(index + INFOBUF_NUM -1)%INFOBUF_NUM;
}
/* 将窗口win的坐标点(x,y)清空 */
void cleanline(WINDOW *win, int y, int x)
{
    char EMPTYLINE[INFOBUF_LEN] = {0}; // INFOBUF_LEN=57
    
    /* 将数组的0-56位置为空字符 */
    memset(EMPTYLINE, ' ', INFOBUF_LEN-1);
    
    /* 将光标移动到窗口win的(y,x)然后打印字符串EMPTYLINE */
    mvwprintw(win, y, x, EMPTYLINE);

    /* 在指定的窗口上，显示内容 */
    wrefresh(win); 
}
food creat_food()
{
	food FOOD;
	FOOD=(food)malloc(sizeof(struct Food));
	FOOD->x=30;
	FOOD->y=12;
	return FOOD;
}
void draw_food(food Tfood, snake Tsnake)
{
    do {
        Tfood->y = random() % (GAMEWIN_YLEN - 2) + 1;
        Tfood->x = random() % (GAMEWIN_XLEN - 2) + 1;
    } while (False == checkfood(Tfood, Tsnake));
      
    mvwaddch(gamewin, Tfood->y, Tfood->x, SHAPE_FOOD);
    wrefresh(gamewin);
}

/* 检查food出现的位置不能在snake上 */
TBool checkfood(food pfood, snake snake)
{
    snakenode pnode = GetSnakeTail(snake);
  
    int i = 0;
    for (; i < snake->length; ++i, pnode = pnode->front)
        if (pfood->y == pnode->y && pfood->x == pnode->x)
            return False;
  
    return True;
}
int check_if_eatfood(food Tfood,snake Tsnake)
{
	int fx=Tfood->x;
	int fy=Tfood->y;
	snakenode head=Tsnake->head;
    if(fx==(head->x)&&fy==(head->y))
		  return 1;
	else
	  return -1;
	
}
void add_snake_node(snake Tsnake)
{
	snakenode newnode =(snakenode)malloc(sizeof(snakenode));
    int nowx=Tsnake->head->x;
	int nowy=Tsnake->head->y;

	switch(Tsnake->dir){
		case DIR_DOWN:
			newnode->x=nowx;
			newnode->y=nowy+1;
			break;
		case DIR_UP:
			newnode->x=nowx;
			newnode->y=nowy-1;
			break;
		case DIR_LEFT:
			newnode->x=nowx-1;
			newnode->y=nowy;
			break;
		case DIR_RIGHT:
			newnode->x=nowx+1;
			newnode->y=nowy;
			break;
	}
	newnode->front=GetSnakeTail(Tsnake);
    Tsnake->head->front=newnode;
    Tsnake->head=newnode;
	++Tsnake->length;
}
int check_snake(snake psnake,food pfood)
{
	/*检查是否吃到食物*/
	if(check_if_eatfood(pfood,psnake)>0){
	   if (++g_level > MAXLEVEL) {
            print_infowin("Win!!!");
            gameover(gamewin, "Win!!!");
            return -1;
     }
	mvwprintw(gamewin, GAMEWIN_YLEN-1, 2, " Level: %d ", g_level);
    mvwprintw(gamewin, GAMEWIN_YLEN-1, 30, " Speed: %d ", (int)(g_level/3));

	   add_snake_node(psnake);
	   draw_food(pfood,psnake);
   }
	/* 检查是否吃到自己或者触碰边界*/
   return check_snake_touch(psnake);
}
int check_snake_touch(snake psnake)
{
	int hx=psnake->head->x;
	int hy=psnake->head->y;
	int i=psnake->length;
	snakenode temp=psnake->head->front;
	/*检查是否触碰自己*/
	for(;i>1;i--){
     if((hx==(temp->x))&&(hy==(temp->y)))
	    return -1;
	 else
	   temp=temp->front;
	}
	/*检查是否触碰边界*/
	if((hx==GAMEWIN_XLEN)||(hy==GAMEWIN_YLEN)||(hx==0)||(hy==0))
	  return -1;
	return 1;
}
void gameover(WINDOW *win, char *str)
{
    mvwprintw(win, (int)(GAMEWIN_YLEN/2), (GAMEWIN_XLEN/2 - strlen(str)/2), str);
    mvwprintw(win, (int)(GAMEWIN_YLEN/2 + 1), 20, "Press any key to quit...");
    wrefresh(win);
}
int wait_for_press(snake psnake)
{
	fd_set set;
	FD_ZERO(&set);
	FD_SET(0,&set);
    int key=-1; 
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = (6- (int)(g_level/3))*100*1000;

    if (select(1, &set, NULL, NULL, &timeout) < 0)
        return -1;   
    if (FD_ISSET(0, &set)) {
		while ((key = getch()) == -1) 
		  ;
		switch (key) {
        case 'w':
        case 'W':
            (psnake->dir == DIR_DOWN) ? : (psnake->dir = DIR_UP);
            break;
          
        case 's':
        case 'S':
            (psnake->dir == DIR_UP) ? : (psnake->dir = DIR_DOWN);
            break;
          
        case 'a':
        case 'A':
            (psnake->dir == DIR_RIGHT) ? : (psnake->dir = DIR_LEFT);
            break;
          
        case 'd':
        case 'D':
            (psnake->dir == DIR_LEFT) ? : (psnake->dir = DIR_RIGHT);
            break;
          
        case 'q':
        case 'Q':
			return -1;
          
        default:
            break;
        }
    }
	return 1;
}

