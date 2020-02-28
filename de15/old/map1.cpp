#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <afxwin.h>

#include "map1.h"
#include "monst.h"
#include "types.h"
#include "icons.h"
#include "win.h"
#include "items.h"
#include "save.h"
#include "mouse.h"
#include "dxutil.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



extern BOOL TimerOn;
extern BOOL Timer2On;

void map1::getnewmap(int map)
{
   char filename[24];
   sprintf(filename,"temp%d.map\0",player->map);
   if((player->map==0)||(player->map>5))
	savemap(filename);
   player->map=map;
   sprintf(filename,"temp%d.map\0",map);
   if(_access(filename,0)== -1)
	   sprintf(filename,"%s%d.map\0",player->name,map);
   if(getmap(filename)==0)
   {
		sprintf(filename,"map%d.dat\0",map);
        getmap(filename);
   }
}
BOOL map1::chkpos()
{
	int retval=FALSE;
	int newmap;
	unsigned char choice=bkgnd[player->pos[yq]][player->pos[xq]];
	switch(choice)
	{
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
            if(player->ship==SHIP_IN)
               ship();
			m_pmonst->end();
			player->savepos[xq]=player->pos[xq];
			player->savepos[yq]=player->pos[yq];
            getnewmap(choice-'I'+6);
            gotopos(startpos[xq],startpos[yq]);
			m_pmonst->start(player,pDirDraw,choice-'I'+6,NUMB_TOWN_NPC,FALSE);
			player->monster=TRUE;
            player->RandMonstOn=FALSE;
			//pDirDraw->ResetPal();
			Timer2On=FALSE;
            break;
		case 128: 
		case 129:
		case 130:
		case 131:
		case 132:
		case 133:
			if(player->ship==SHIP_IN)
               ship();
			m_pmonst->end();
			player->savepos[xq]=player->pos[xq];
			player->savepos[yq]=player->pos[yq];
            getnewmap((choice-128)+24);
            gotopos(startpos[xq],startpos[yq]);
			m_pmonst->start(player,pDirDraw,choice-128+24,NUMB_TOWN_NPC,FALSE);
			player->monster=TRUE;
            player->RandMonstOn=FALSE;
			//pDirDraw->ResetPal();
			Timer2On=FALSE;
            break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(player->ship==SHIP_IN)
               ship();
			m_pmonst->end();
			if(inout[choice - '1'][xq]==0 && inout[choice - '1'][yq]==0)
				gotopos(player->savepos[xq],player->savepos[yq]);
			else
				gotopos(inout[choice - '1'][xq],inout[choice - '1'][yq]);
			newmap = inout[choice - '1'][2];
			getnewmap(inout[choice - '1'][2]);
			if(newmap!=0)
			{	
				m_pmonst->start(player,pDirDraw,newmap,NUMB_TOWN_NPC,FALSE);
				player->monster=TRUE;
				player->RandMonstOn=FALSE;
				//pDirDraw->ResetPal();
				Timer2On=FALSE;
			}
			else
			{
				Timer2On=TRUE;
				player->RandMonstOn=TRUE;
				player->monster=FALSE;
			}
				
			break;
		case 'z':
			player->savepos[xq]++;
		case 'y':
			outside();
            break;
		case '#':ship();break;
		case 'd':
			retval=dore(1,1);
			if(player->ship==SHIP_IN&&retval==TRUE)
				ship();
			break;
		case 0:
		case 10:
		case 'w':
         if(player->ship==SHIP_IN)
		 {
            if(player->monster==TRUE)
			{ 
				if((m_pmonst->checkpos(player,pDirDraw,this))==1)
				{		
					if(m_pmonst->NumbMonst==0 && player->RandMonstOn==TRUE)
						endfight();
					retval=TRUE;
				}
				else
					retval=FALSE;
			}
			else
				retval=TRUE;
		 }
         else
         {
            player->pos[0]=player->lastpos[0];
			player->pos[1]=player->lastpos[1];
         }
         break;
	  case '<':
      case ':':
      case '_':
      case ',':
	  case 'H':
	  case 'F':
	  case 't':
	  case 'k':
	  case 'b':
      case 'g':
      case 'G':
	  case '0':
	  case '*':
      case 'f':
      case 'B':
      case ' ':
	  case '.':
	  case 'i':
         if(player->ship==SHIP_OUT)
		 {
            if(player->monster==TRUE)
			{ 
				if((m_pmonst->checkpos(player,pDirDraw,this))==1)
				{		
					if(m_pmonst->NumbMonst==0 && player->RandMonstOn==TRUE)
						endfight();
					retval=TRUE;
				}
				else
					retval=FALSE;
			}
			else
				retval=TRUE;
		 }
         else
            ship();
         break;
		default:
				player->pos[0]=player->lastpos[0];
				player->pos[1]=player->lastpos[1];
			break;
	}
	return(retval);
}
void map1::outside()
{
	Timer2On=TRUE;
    getnewmap(0);
    gotopos(player->savepos[xq],player->savepos[yq]);
	if(player->monster==TRUE)
		m_pmonst->end();
    player->RandMonstOn=TRUE;
	player->monster=FALSE;
}

void map1::ship()
{
   UINT dir = VK_UP;
   if(player->pos[yq]>player->lastpos[yq])
   {
      dir=VK_DOWN;
      player->icon=0;
   }
   else
      if(player->pos[yq]<player->lastpos[yq])
      {
         dir=VK_UP;
         player->icon=2;
      }
   if(player->pos[xq]>player->lastpos[xq])
   {
      dir=VK_RIGHT;
      player->icon=4;
   }
   else
      if(player->pos[xq]<player->lastpos[xq])
      {
         dir=VK_LEFT;
         player->icon=6;
      }
	if(player->ship==SHIP_OUT)
   {
      player->ship=SHIP_IN;
	  move(dir);
	  bkgnd[player->pos[yq]][player->pos[xq]]='w';
      player->icon=8; 
   }
   else
   {
      player->ship=SHIP_OUT;
      bkgnd[player->lastpos[yq]][player->lastpos[xq]]='#';
	  move(dir);
   }
}
/////////////////////////////////////////////////////////////////////////////
// map1

map1::map1(CDisplay* pDD, HWND hWin,cursor* p_curs,CPlayer* lp_player):map(pDD,hWin,p_curs,lp_player)
{	
	if(FAILED(pDD->CreateSurface( &MoveSurface, 800,600)))
	{
		pDirDraw->FlipToGDISurface();
		MessageBox(hWin,"unable to create a Move Surface","ERROR",MB_ICONERROR|MB_OK);
	}
}

map1::~map1()
{
	char file[20];
	for(int i=0;i<MAXMAPS;i++)
	{
		sprintf(file,"temp%d.map",i);
		remove(file);
	}
	SAFE_DELETE(MoveSurface);
}


void map1::move(UINT nChar)
{
	if(MoveSurface!=0)
	{
		int tempx,tempy,start;
		int xpos,ypos;
		tempx=player->pos[xq]-MAP_PLAYER_OFFSET_X-1;
		if(tempx<0)
		   tempx+=(sizex);
		tempy=player->pos[yq]-MAP_PLAYER_OFFSET_Y-1;
		if(tempy<0)
		   tempy+=(sizey);
		start=tempy;
		for(xpos=0;xpos<22;xpos++)
		{
		  if(tempx>(sizex-1))
			   tempx=0;
			for(ypos=0;ypos<17;ypos++)	
			{
				if(tempy>(sizey-1))
					 tempy=0;
				pcicons->puticon(MoveSurface,mapitem(bkgnd[tempy++][tempx]),xpos,ypos);
			}
			tempy=start;
			tempx++;
		}
		CPoint point(0,0);
		CRect rec;
		int point1x=32;
		int point1y=32;
		int point2x = 0;
		int point2y = 0;
		int posy=0;
		int posx=0;
		BOOL sbit=TRUE;
		for(int numb=4;numb<32;numb+=4)
		{
			switch(nChar)
			{
			case VK_UP:
				point2x=672;
				point1y=64-numb;
				point2y=544-numb;
				posy=numb-32;
				break;
			case VK_DOWN:
				point2x=672;
				point1y=numb;
				point2y=480+numb;
				posy=-numb+32;
				break;
			case VK_RIGHT:
				point1x=numb;
				point2x=640+numb;
				point2y=512;
				posx=-numb+32;
				break;
			case VK_LEFT:
				point1x=64-numb;
				point2x=704-numb;
				point2y=512;
				posx=numb-32;
				break;
			}
			if(TimerOn==TRUE)
			{
				TimerOn=FALSE;
				if(LookForMSG()==FALSE)
					return;
				TimerOn=TRUE;
			}
			else
				if(LookForMSG()==FALSE)
					return;
			rec.SetRect(point1x,point1y,point2x,point2y);
			pDirDraw->Blt(point.x,point.y,MoveSurface,&rec);
			if(sbit==TRUE)
			{
				player->AninIcon();
				sbit=FALSE;
			}
			else
				sbit=TRUE;
			pcicons->putplayericon(pDirDraw,player->icon,MAP_PLAYER_OFFSET_X,MAP_PLAYER_OFFSET_Y,0,0);
			if(player->monster==TRUE)
				m_pmonst->display(posx,posy,player,pDirDraw);
			lc_curs->showcursor(pDirDraw);
			pDirDraw->Present();
		}	
	}
}

void map1::startfight()
{
		
		UINT type;
		switch(mapitem(bkgnd[player->pos[yq]][player->pos[xq]]))
		{
		case 0:type=1;break;
		case 1:type=2;break;
		case 4:type=3;break;
		case 5:type=5;break;
		case 9:type=4;break;
		default:type=5;break;
		}
		getnewmap(type);
		player->savepos[xq]=player->pos[xq];
		player->savepos[yq]=player->pos[yq];
		player->pos[xq]=(rand()%19)+1;
		player->pos[yq]=(rand()%14)+1;
		player->monster=TRUE;
		m_pmonst->start(player,pDirDraw,type,0,TRUE);
		pDirDraw->Clear(255);
		pDirDraw->Present();
		pDirDraw->Clear(2);
		pDirDraw->Present();
		pDirDraw->Clear(255);
		pDirDraw->Present();
		pDirDraw->Clear(2);
		pDirDraw->Present();
		pDirDraw->Clear(255);
		pDirDraw->Present();
		pDirDraw->Clear(2);
		pDirDraw->Present();
		pDirDraw->Clear(255);
		pDirDraw->Present();
}

void map1::endfight()
{
	player->pos[xq]=player->savepos[xq];
	player->pos[yq]=player->savepos[yq];
	getnewmap(0);
	player->monster=FALSE;
	m_pmonst->end();
}



void map1::shearch()
{
	int gold;
	switch(bkgnd[player->pos[yq]][player->pos[xq]])
	{
		case 'k':
			fkey(2,2);
			break;
		case 'G':
		if(rand()%4!=0)
		{	
			  pDirDraw->outtext(2,2,"                              ");
		      pDirDraw->outtext(2,3,"       You got %d gold        ",gold=(rand()%100)+20);
			  pDirDraw->outtext(2,4,"                              ");
			  CRect rec(2<<3,2<<3,32<<3,(5<<3)+3);
			  pDirDraw->DrawRect(rec,RGB(255,255,255));
			  player->gold=player->gold+gold;
              bkgnd[player->pos[yq]][player->pos[xq]]='0';
			  pDirDraw->Present();
		}
		else
		{
			
			Citems* pCitems=NULL;
			if((pCitems = new Citems())!=NULL)
			{
				int itemnumb = (player->level/2)+5;
				if(itemnumb>MAXITEMS)
					itemnumb=MAXITEMS;
				if(pCitems->getitem(player,this,pDirDraw,rand()%(itemnumb))==TRUE)
					bkgnd[player->pos[yq]][player->pos[xq]]='0';
				delete pCitems;
			}
		}
	  break;
      default:
		  mywindow(WD_NOTHING,player,pDirDraw,this,"You found nothing");
         break;
	}
    
}
void map1::fkey(int pos1,int pos2)
{
	mywindow(WD_NOTHING,player,pDirDraw,this,"Found a key");
	player->k++;
	bkgnd[player->pos[yq]][player->pos[xq]]='0';
}


int map1::dore(int pos1,int pos2)
{
	if(player->k<=0)
	{
        player->pos[xq]=player->lastpos[xq];
		player->pos[yq]=player->lastpos[yq];
		mywindow(WD_NOTHING,player,pDirDraw,this,"don't have a key to open dore");
		TimerOn=FALSE;
		return FALSE;
	}
	else
	{
		player->k--;
		bkgnd[player->pos[yq]][player->pos[xq]]='b';
		return TRUE;
	}
}
BOOL map1::CheckPosMonst(CMonster* monst)
{
	switch(bkgnd[monst->pos[yq]+(monst->size-1)][monst->pos[xq]])
	{

		case 0:
		case 10:
		case 'w':
			if(player->ship!=SHIP_IN)
				return FALSE;
			break;
		case ':':
		case '_':
		case ',':
		case 'H':
		case 'F':
		case 't':
		case 'k':
		case 'b':
		case 'g':
		case 'G':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '*':
		case 'f':
		case 'B':
		case ' ':
		case '.':
		case 'i':
        break;
		default:
			return FALSE;
			break;
	}
	switch(bkgnd[monst->pos[yq]+monst->size-1][monst->pos[xq]+monst->size-1])
	{

		case 0:
		case 10:
		case 'w':
			if(player->ship!=SHIP_IN)
				return FALSE;
			break;
		case ':':
		case '_':
		case ',':
		case 'H':
		case 'F':
		case 't':
		case 'k':
		case 'b':
		case 'g':
		case 'G':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '*':
		case 'f':
		case 'B':
		case ' ':
		case '.':
		case 'i':
        break;
		default:
			return FALSE;
			break;
	}
	return TRUE;
}

