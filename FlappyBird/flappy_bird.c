#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <ncurses.h>

#define BIRD  "*@>"
#define WALL '*'
#define GAMEWIN_YLEN   22
#define GAMEWIN_XLEN   72
#define WALL_WIDTH 7
//bird
struct flappy_bird
{
	char shape[3];
	int x;
	int y;
};
//wall
typedef struct Wall_Node * wall_node;
struct Wall_Node
{
	int x;
	int y;
	wall_node next;
};
wall_node wall_head;
struct flappy_bird bird={BIRD,10,11};

WINDOW  *gamewin;                    //游戏窗口
int g_score;                        //游戏分数

void init_wall()
{
    int i;
    wall_node tmp, p;
	wall_head=(wall_node)malloc(sizeof(struct Wall_Node));
	wall_head->next=NULL;

    p = wall_head;
    for(i = 1; i < GAMEWIN_XLEN-2; i += (2*WALL_WIDTH)) {
        tmp = (wall_node)malloc(sizeof(struct Wall_Node));
        tmp->x = i;
        do {
            tmp->y = rand() % 16;
        }while(tmp->y < 5);
        p->next = tmp;
        tmp->next = NULL;
        p = tmp;
    }
}
int loop_c( int n)
{
	n=n%(GAMEWIN_XLEN-2);
	if(n==0)
	  return GAMEWIN_XLEN-2;
	else
	  return n;
}
void draw_single_wall(wall_node p)
{
	
	int i;
	if(p->x)
	for(i=1;i<p->y;i++)
	  mvwaddch(gamewin,i,p->x,'*');

	for(i=p->x;i<p->x+WALL_WIDTH;i++)
	  mvwaddch(gamewin,p->y,loop_c(i),'*');
	
	for(i=1;i<p->y;i++)
	  mvwaddch(gamewin,i,loop_c((p->x+WALL_WIDTH)-1),'*');

	for(i=p->y+5;i<GAMEWIN_YLEN-1;i++)
	  mvwaddch(gamewin,i,p->x,'*');

	for(i=p->x;i<p->x+WALL_WIDTH;i++)
	  mvwaddch(gamewin,p->y+5,loop_c(i),'*');

	for(i=p->y+5;i<GAMEWIN_YLEN-1;i++)
	  mvwaddch(gamewin,i,loop_c((p->x+WALL_WIDTH)-1),'*');
    wrefresh(gamewin);
}
void draw_wall()
{
	wall_node p = wall_head->next;
	while(p != NULL){
		draw_single_wall(p);
		p=p->next;
	}
}
void move_wall()
{
	int y;
	wall_node p = wall_head->next;
	while(p!=NULL){
		for(y=1;y<=p->y;y++)
		  mvwaddch(gamewin,y,p->x,' ');
		for(y=p->y+5;y<GAMEWIN_YLEN-1;y++)
		  mvwaddch(gamewin,y,p->x,' ');
		for(y=1;y<=p->y;y++)
		  mvwaddch(gamewin,y,loop_c((p->x+WALL_WIDTH)-1),' ');
		for(y=p->y+5;y<GAMEWIN_YLEN-1;y++)
		  mvwaddch(gamewin,y,loop_c((p->x+WALL_WIDTH)-1),' ');

		if(p->x == 1)
		  p->x=GAMEWIN_XLEN-2;
		else
		  --p->x;
		draw_single_wall(p);
		p=p->next;
	}
	wrefresh(gamewin);
}

void init_scr()
{
	initscr();                               //初始化,进入ncurses模式
	raw();                                  //禁止行缓冲
	noecho();                               //不在终端上显示控制字符
	keypad(stdscr,TRUE);                    //允许用户在终端中使用键盘
	curs_set(0);                            //设置光标不可见
	refresh();                              //刷新屏幕
}


void init_gamewin()
{
	int i;
	gamewin=newwin(GAMEWIN_YLEN, GAMEWIN_XLEN, 1, 3);
	box(gamewin,0,0); 
	mvwprintw(gamewin, 0, 2, " GMAE ");
	mvwprintw(gamewin, 0, GAMEWIN_XLEN-12, " SCORE: %d ", g_score);
	wrefresh(gamewin);
}

void clean_gamewin_line(int y, int x, int length)
{

    char EMPTYLINE[GAMEWIN_XLEN] = {0};                 // LOGBUF_LEN=57
    memset(EMPTYLINE, ' ', length);
    mvwprintw(gamewin, y, x, EMPTYLINE);
    wrefresh(gamewin); 
}

void begain_game()
{
	mvwprintw(gamewin, (int)(GAMEWIN_YLEN/2-3), 31, "WELCOME!");
	mvwprintw(gamewin, (int)(GAMEWIN_YLEN/2-1), 24, "Press any key to begain...");
	wrefresh(gamewin);
	getch();
	clean_gamewin_line((int)(GAMEWIN_YLEN/2-3),31,12);
	clean_gamewin_line((int)(GAMEWIN_YLEN/2-1),24,30);
    wrefresh(gamewin);

}
void draw_bird()
{
	static old_x=10,old_y=11;
	clean_gamewin_line(old_y,old_x-3,3);
	mvwprintw(gamewin,bird.y,bird.x-3,bird.shape);
	old_x=bird.x;
	old_y=bird.y;
	wrefresh(gamewin);
}
int wether_crash()
{
	int bx=bird.x;
	int by=bird.y;
	int wx,wy;
    wall_node p =wall_head->next;
	while(p!=NULL){
		wx=p->x;
		wy=p->y;
		if((wx <= bx)&&(bx <= wx+WALL_WIDTH-1)){
			if((wy <= by)&&(by <=wy +5))
			  ;
			else
			  return 1;
	
		}
		p=p->next;

	}
	return 0;
}
int refresh_game()
{
    char key;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec= 4* 100*1000;
	
	if (select(1, &set, NULL, NULL, &timeout) < 0)
        return -1;
	if(FD_ISSET(0,&set)){
		while((key=getch())==-1);

		if(key==' '||key== 'w'||key == 'W')
		  bird.y -=1;
		else if(key=='q'||key=='Q')
		  return -1;
	}
	else
	  bird.y +=1;
    
	move_wall();
	draw_bird();
	if(wether_crash()==1)
	  return -2;
	return 1;

}
void game_over()
{

    mvwprintw(gamewin, (int)(GAMEWIN_YLEN/2-3), 31, "GAME OVER!");
	mvwprintw(gamewin, (int)(GAMEWIN_YLEN/2-1), 24, "Press any key to quite...");
	wrefresh(gamewin);
	getch();
}
int main()
{
	int state=0;
	init_scr();
	init_gamewin();
	init_wall();
	begain_game();
	draw_wall();
	draw_bird();
    char c=getch();
	while((state=refresh_game())>0)
	  ;
	if(state==-2)
	  game_over();

    endwin();
	return 0;
	
}

