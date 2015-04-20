#ifndef __FIVECHESS_H__
#define __FIVECHESS_H__

int		Pain_back(int i,int j);
int		Pain_white(int i,int j);
int		Game_Init();
void	Update_all();
int 	Computer_think();
int 	Computer_work();
int My_work(int mouse_x,int mouse_y);
int Check_win();

#endif //__FIVECHESS_H__