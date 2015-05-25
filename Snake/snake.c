/**
 * Snake
 * author :glider
 * date   :2015.4.20
 * type   :game
*/

#include "snake.h"



int start_game(snake Tsnake,food Tfood);

int main()
{
    INITSCR();
    INITINFOWIN();  
    INITGAMEWIN();
    food Tfood=creat_food();
    snake Tsnake = init_snake();         //创建贪吃蛇
    draw_food(Tfood,Tsnake);

    /* 开始游戏 */
	while(start_game(Tsnake,Tfood)>0)
	  ;

    getch();
    destory_snake(Tsnake);
	endwin();
	return 0;

}

int start_game(snake psnake,food Tfood)
{
   move_snake(psnake);
   draw_snake(psnake);
   //检测按键:当按键为Q/q时,返回-1
   if(wait_for_press(psnake)==-1){
	   print_infowin("Quit Game!");
       gameover(gamewin, "Quit Game!");
	   return -1;
   }
   //检测贪吃蛇状态:1.是否吃到食物;2.是否触碰自己或者边界.
   if(check_snake(psnake,Tfood)<0){
	 gameover(gamewin,"GAME OVER!!!");
	 print_infowin("QUIT GMAE!");
	 return -1;
   }
   else
	 return 1;
}



