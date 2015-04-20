// DEMO7_18.CPP Windowed DirectDraw Demo
#define WIN32_LEAN_AND_MEAN  // just say no to MFC

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <iostream.h> // include important C/C++ stuff
#include <stdlib.h>
#include <stdio.h> 
#include <math.h>
#include "resource.h"


void MainWnd_Paint(HWND hwnd);

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define WINDOW_CLASS_NAME "WINCLASS1"
#define WINDOW_WIDTH    382
#define WINDOW_HEIGHT   422
#define MM()    MessageBox(my_hwnd,"sdlfk","sldkfj",MB_OK | MB_ICONQUESTION)

#define Poit_Sco         10
#define Step_Sco          20
#define Left_Sco         0.6
#define Down_Sco         11//减分
#define Down_Sco2        7//减分2
#define Sco_50           1000
#define Sco_51           900
#define Sco_40           800
#define Sco_41           700
#define Sco_40_0         600
#define Sco_41_0         500
//#define 
int my_sco[20];
typedef struct sss
 {int stat;//2是Computer，是白色的。1是玩家，是黑色的。0就是空。
 }my_stru,*lpmy_stru;

my_stru bb[15][15]; 

typedef struct sssj
{int sco;
 int stat;
}sco_stru,*lpsco_stru;

sco_stru p_bb[15][15],c_bb[15][15];

int xx[15],ww[20],mouse_x,mouse_y,p_num=0,c_num=0;
int my_stat=0;//my_stat=0表示到电脑走了 my_stat=1表示到玩家走了
HWND      my_hwnd        =  NULL; // globally track main window
HINSTANCE my_hinstance   =  NULL; // globally track hinstance

#include "ShowBmp.h"

extern void* qipanBmp, *whiteBmp, *blackBmp;
//void ShowBMP(char* bmpcontent, int XPos, int YPos);
//void ShowBMP(void* bmpcontent, int XPos, int YPos, bool fromfile);

int Pain_back(int i,int j)
{
		ShowBMP(blackBmp, xx[j],xx[i], 0);
		return (0);
}
int Pain_white(int i,int j)
{ 
		ShowBMP(whiteBmp,xx[j],xx[i], 0);
		return (0);
}


///////////////////////////////////////////////////////////
int My_work(int mouse_x,int mouse_y)
{  int flag=0,i,j;
	for(i=0;i<15;i++)
		 {if((mouse_y>=xx[i]) & (mouse_y<=xx[i]+21))
		     {flag=1;
		         break;
			 }
		  }
	if(i==15)//如果mouse_y不在选定范畴，则返回0（即要用户重新输入）
	   return (0);
	if(flag==1)
		 {for(j=0;j<15;j++)
		      {flag=0;
		       if((mouse_x>=xx[j]) & (mouse_x<=xx[j]+21))
			      {flag=1;
			       break;
				  }
			  }
		    }
   if(flag==0)
		return (0);//用户输入不在范畴内，重新输入,返回0
   else 
	   {if(bb[i][j].stat==0)
		   {
			bb[i][j].stat=1;
		    Pain_back(i,j);
			}
	   else 
		   return (0);
	   }
	   p_num++;
	   return (1);//用户输入合法，返回1

}

int Check_11(int i,int j,int stat)
{
	int b;
	int cx,cy,mm,num;
 for (mm=0;mm<8;mm++)
   { switch (mm)
      {case 0:
         cx=-1;cy=0;break;
	   case 1:
		 cx=-1;cy=1;break;
	   case 2:
		 cx=0;cy=1;break;
	   case 3:
		 cx=1;cy=1;break;
	   case 4:
		 cx=1;cy=0;break;
	   case 5:
		 cx=1;cy=-1;break;
	   case 6:
		 cx=0;cy=-1;break;
	   case 7:
		  cx=-1;cy=-1;break;
	   default:break;
	  }
	 num=1;
    for(b=1;b<5;b++)
     {if((bb[i+b*cx][j+b*cy].stat==stat) & (((i+b*cx)>=0) &((i+b*cx)<15)) & (((j+b*cy)>=0) &((j+b*cy)<15)))
         num++;
	 }
	 if(num==5)
        return(stat);
   }
 return 0;
// return stat;
}

int Check_win()
{
	int flag=0;	
	int i,j;	
	for(i=0;i<15;i++)		
	{
		for(j=0;j<15;j++)
		{
			if(bb[i][j].stat!=0)
			{
				if(Check_11(i,j,bb[i][j].stat)==bb[i][j].stat)    
					return(bb[i][j].stat);     
			}
		}
	}
	return (0);
}

int sco_mm(int m,int n,int stat)
{ int cx,cy,mm;
  int nn,nnn;
  int i,j;
  for (mm=0;mm<8;mm++)
   { switch (mm)
      {case 0:cx=-1;cy=0;break;
	   case 1:cx=-1;cy=1;break;
	   case 2:cx=0;cy=1;break;
	   case 3:cx=1;cy=1;break;
	   case 4:cx=1;cy=0;break;
	   case 5:cx=1;cy=-1;break;
	   case 6:cx=0;cy=-1;break;
	   case 7:cx=-1;cy=-1;break;
	   default:break;
	  }
	nnn=2;nn=Poit_Sco;
	i=m;j=n;
    i+=cx;j+=cy;
    if(stat==2)
    {
      while(i>=0 && i<15 && j>=0 && j<15 && nnn>0)
        {  if(c_bb[i][j].stat==stat)
             {//nn+=Step_Sco;
	            if (nnn==2)
			       nn+=Step_Sco;
	            else if (nnn==1)
			      {c_bb[i-cx][j-cy].sco+=10;
			        nnn=-3;//不做了，退出。
			      }
		     }
	        else if(c_bb[i][j].stat==0)
	           {if(nnn<2)
		            nn=(int)(nn*Left_Sco);
			     c_bb[i][j].sco+=nn;
                 nnn--;
		       }
	        else 
		        nnn=-3;//不做了，退出。
	        i+=cx;
	        j+=cy;
		}
      
     }
    else if(stat==1)
    {
       while(i>=0 && i<15 && j>=0 && j<15 && nnn>0)
         {  if(p_bb[i][j].stat==stat)
              {//nn+=Step_Sco;
	            if (nnn==2)
			       nn+=Step_Sco;
	            else if (nnn==1)
			      {p_bb[i-cx][j-cy].sco+=10;
			       nnn=-3;
			      }
			  }
	        else if(p_bb[i][j].stat==0)
	           {if(nnn<2)
		           nn=(int)(nn*Left_Sco);
			     p_bb[i][j].sco+=nn;
                 nnn--;
		        }
	         else 
		        nnn=-3;//不做了，退出。
	       i+=cx;
		   j+=cy;
		 }
	}
   }
return (0);
}


int my_sco_add(int m,int n)
{ int cx,cy,mm;
  int nn,nnn;
  int i,j;
  for (mm=0;mm<8;mm++)
   { switch (mm)
      {case 0:cx=-1;cy=0;break;
	   case 1:cx=-1;cy=1;break;
	   case 2:cx=0;cy=1;break;
	   case 3:cx=1;cy=1;break;
	   case 4:cx=1;cy=0;break;
	   case 5:cx=1;cy=-1;break;
	   case 6:cx=0;cy=-1;break;
	   case 7:cx=-1;cy=-1;break;
	   default:break;
	  }
     i=m;j=n;
	 nn=0;
	 i+=ww[8]*cx;j+=ww[8]*cx;
	 //if(i<0 | i>14 | j<0 | j>14)
		// continue;
	 for(nnn=0;nnn<ww[9];nnn++)
	    {  if(c_bb[i][j].stat==ww[nnn])
	             nn++;
	       else 
			  break;
	       i+=cx;j+=cy;
		   if(i<0 || i>14 || j<0 || j>14)
		       break;
		}
	 if(nn==ww[9])//说明配对成功；下一步是加分
	    { i=m;j=n;
	      i+=ww[8]*cx;j+=ww[8]*cx;
		  if(ww[19]==2)//要求加电脑的分，c_bb[i][j].sco+=?--------
		     {for(nnn=0;nnn<ww[9];nnn++)
		          {c_bb[i][j].sco+=ww[10+nnn];
                   i+=cx;j+=cy;
				  }
			 }
		  else if (ww[19]==1) //要求加玩家的分，p_bb[i][j].sco+=?--------
		     {for(nnn=0;nnn<ww[9];nnn++)
		          { p_bb[i][j].sco+=ww[10+nnn];
			        i+=cx;j+=cy;
				  }
			 }
		}//----end 加分---------
   }//----end  for (mm=0;mm<8;mm++)------
   return (1);
}



int Computer_think()
{int i,j;
	for( i=0;i<15;i++)
    {for( j=0;j<15;j++)
        {p_bb[i][j].sco=0;
         p_bb[i][j].stat=bb[i][j].stat;
		 c_bb[i][j].sco=0;
		 c_bb[i][j].stat=bb[i][j].stat;
		}
	}
 for( i=0;i<15;i++)
   {for(j=0;j<15;j++)
       {if(bb[i][j].stat!=0)
         {
			 sco_mm(i,j,bb[i][j].stat);
		 /* Sco_0_j(i,j,bb[i][j].stat);
          Sco_1_j(i,j,bb[i][j].stat);
          Sco_2_j(i,j,bb[i][j].stat);
          Sco_3_j(i,j,bb[i][j].stat);
          Sco_4_j(i,j,bb[i][j].stat);
          Sco_5_j(i,j,bb[i][j].stat);
          Sco_6_j(i,j,bb[i][j].stat);
          Sco_7_j(i,j,bb[i][j].stat);*/

	
//关于四个子的--------------------------------我我我我空---		 
		  memset(ww,0,20*sizeof(int));
		  ww[0]=2; ww[1]=2; ww[2]=2; ww[3]=2; ww[4]=0;   ww[8]=0; ww[9]=5;
		                                      ww[14]=Sco_50;      ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=1; ww[1]=1; ww[2]=1; ww[3]=1; ww[4]=0;   ww[8]=0;ww[9]=5;
                                              ww[14]=Sco_51;     ww[19]=1;
          my_sco_add(i,j);
//------------------------------我我我空我-------------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=2;  ww[1]=2;  ww[2]=2;  ww[3]=0;       ww[4]=2; ww[8]=0; ww[9]=5;
		                                ww[13]=Sco_50;                   ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=1;  ww[1]=1;  ww[2]=1;  ww[3]=0;       ww[4]=1; ww[8]=0; ww[9]=5;
		                                ww[13]=Sco_51;                   ww[19]=1;
          my_sco_add(i,j);
//------------------------------------------我我空我我---------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=2;  ww[1]=2;  ww[2]=0;         ww[3]=2;  ww[4]=2; ww[8]=0; ww[9]=5;
		                      ww[12]=Sco_50/2;                             ww[19]=2;
          my_sco_add(i,j);

          memset(ww,0,20*sizeof(int));
		  ww[0]=1;ww[1]=1;ww[2]=0;ww[3]=1;ww[4]=1;ww[8]=0;ww[9]=5;
		                  ww[14]=Sco_51/2;                  ww[19]=1;
          my_sco_add(i,j);
//end  关于四子----------------------------------
//关于三子--------------------------------------------------
//---------------------------------------------敌我我我空-----------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=2;ww[1]=2;ww[2]=2;ww[3]=0;ww[8]=0;ww[9]=4;
		                          ww[13]=Sco_40;  ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=1;ww[1]=1;ww[2]=1;ww[3]=0;ww[8]=0;ww[9]=4;
		                          ww[13]=Sco_41;  ww[19]=1;
          my_sco_add(i,j);
//----------------------------------------------敌我我我空敌------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=1;ww[1]=2;ww[2]=2;ww[3]=2;ww[4]=0;ww[5]=1;ww[8]=0;ww[9]=6;
		                                  ww[14]=-(Sco_40+90);  ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=2;ww[1]=1;ww[2]=1;ww[3]=1;ww[4]=0;ww[5]=2;ww[8]=0;ww[9]=6;
		                                  ww[14]=-(Sco_41+90);    ww[19]=1;
          my_sco_add(i,j);
//---------------------------------------敌我我我空空------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=1;ww[1]=2;ww[2]=2;ww[3]=2;ww[4]=0;ww[5]=0;ww[8]=0;ww[9]=6;
		                                  ww[14]=-(Sco_40-Sco_40_0);ww[15]=ww[14];ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=2;ww[1]=1;ww[2]=1;ww[3]=1;ww[4]=0;ww[5]=0;ww[8]=0;ww[9]=6;
		                                  ww[14]=-(Sco_41-Sco_41_0);ww[15]=ww[14];ww[19]=1;
          my_sco_add(i,j);
//---------------------------------------敌空我我我空-------------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=1;ww[1]=0;ww[2]=2;ww[3]=2;ww[4]=2;ww[5]=0;ww[8]=0;ww[9]=6;
		          ww[11]=-(Sco_40-Sco_40_0);                   ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=2;ww[1]=0;ww[2]=1;ww[3]=1;ww[4]=1;ww[5]=0;ww[8]=0;ww[9]=6;
		          ww[11]=-(Sco_41-Sco_41_0);                   ww[19]=1;
          my_sco_add(i,j);
//---------------------------------------空我我空我空-------------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=0; ww[1]=2; ww[2]=2; ww[3]=0; ww[4]=2; ww[5]=0;ww[8]=0;  ww[9]=6;
		  ww[10]=Sco_40_0;           ww[13]=Sco_40;    ww[15]=Sco_40_0;  ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=0; ww[1]=1; ww[2]=1; ww[3]=0; ww[4]=1; ww[5]=0; ww[8]=0; ww[9]=6;
		  ww[10]=Sco_41_0;           ww[13]=Sco_41;    ww[15]=Sco_41_0;  ww[19]=1;
          my_sco_add(i,j);

 //---------------------------------------敌我我空我敌-------------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=1; ww[1]=2; ww[2]=2; ww[3]=0; ww[4]=2; ww[5]=1; ww[8]=0;  ww[9]=6;
		                             ww[13]=-50;                          ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=2; ww[1]=1; ww[2]=1; ww[3]=0; ww[4]=1; ww[5]=2; ww[8]=0; ww[9]=6;
		                             ww[13]=-50;                         ww[19]=1;
          my_sco_add(i,j);
 //---------------------------------------敌我我空我空-------------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=1; ww[1]=2; ww[2]=2; ww[3]=0; ww[4]=2; ww[5]=0; ww[8]=0;  ww[9]=6;
		                             ww[13]=Sco_40_0;  ww[15]=Sco_40_0;   ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=2; ww[1]=1; ww[2]=1; ww[3]=0; ww[4]=1; ww[5]=0; ww[8]=0; ww[9]=6;
		                             ww[13]=Sco_41_0;  ww[15]=Sco_41_0;      ww[19]=1;
          my_sco_add(i,j);
 //---------------------------------------敌我空我我空-------------
		  memset(ww,0,20*sizeof(int));
		  ww[0]=1; ww[1]=2; ww[2]=0; ww[3]=2; ww[4]=2; ww[5]=0; ww[8]=0;  ww[9]=6;
		                    ww[12]=Sco_40_0;           ww[15]=Sco_40_0;   ww[19]=2;
          my_sco_add(i,j);

		  memset(ww,0,20*sizeof(int));
		  ww[0]=2; ww[1]=1; ww[2]=0; ww[3]=1; ww[4]=1; ww[5]=0; ww[8]=0; ww[9]=6;
		                    ww[12]=Sco_41_0;           ww[15]=Sco_41_0;  ww[19]=1;
          my_sco_add(i,j);
//----------------ene 关于三子------------------------
//
		 }
	   }
   }

	return (0);
}

int Computer_work()
{int i,j,m=0,n=0,max=0,m1=0,n1=0,max1=0;
 for(i=0;i<15;i++)
    {for(j=0;j<15;j++)
	  {if(max<c_bb[i][j].sco)
          {max=c_bb[i][j].sco;
           m=i;
		   n=j;
		  }
	   else if(max==c_bb[i][j].sco)
	       {if(p_bb[i][j].sco>max)
               {max=c_bb[i][j].sco;
                m=i;
				n=j;
			   }
		   }
        if(max1<p_bb[i][j].sco)
          {max1=p_bb[i][j].sco;
           m1=i;
		   n1=j;
		  }
		else if(max1==p_bb[i][j].sco)
		   {if(c_bb[i][j].sco>max1)
		      {max1=p_bb[i][j].sco;
		       m1=i;
			   n1=j;
			  }
		   }
	  }
    
	}
if(max1>max && max1>=50)
    {max=max1;
	 m=m1;
	 n=n1;
	}
 if(max>0)
    { if(bb[m][n].stat==0)
         {bb[m][n].stat=2;
	      Pain_white(m,n);
          return (1);
		 }
	}
if(max<=0 )
	{if(max1<=0)
        {bb[7][7].stat=2;
         Pain_white(7,7);
	     return (1);
		}
	else 
	   {bb[m1][n1].stat=2;
        Pain_white(m1,n1);
		return (1);
	   }
	}
 MessageBox(my_hwnd,"我没有走","sldfkjsldf",MB_OK | MB_ICONQUESTION);
   return (0);
}



////////////////////////////////////////////////////////////
void Update_all()
{
	int i = 0, j = 0;
	ShowBMP(qipanBmp, 0, 0, 0);
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{ 
			if( bb[i][j].stat==1)
			{ 
				//BitBlt(hDC,xx[j],xx[i],21,21,hMemDC,0,0,SRCAND);
				Pain_back(i,j);		
			}
			else if( bb[i][j].stat==2)
			{ 
				//BitBlt(hDC,xx[j],xx[i],21,21,hMemDC,0,0,SRCAND);
				Pain_white(i,j);
			}			       
		}
	}
}
int Game_Init()
{
	int i, j;	
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			bb[i][j].stat=0;
		}
		xx[i]=8+i*24;
	}
	Update_all();   
	return(1);	   
} // end Game_Init

/////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////

