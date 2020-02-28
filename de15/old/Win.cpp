#include <afx.h>
#include <afxwin.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include "ddutil.h"
#include "win.h"
#include "types.h"
#include "monst.h"
#include "icons.h"
#include "map.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void mywindow(int type,CPlayer* player,CDisplay* pDisplay,CGameMap* pMap,int pos1,int pos2)
{
	mywin* lpmywin=NULL;
	if((lpmywin=new mywin(type,player,pDisplay,pMap,pos1,pos2))!=NULL)
		delete lpmywin;
	pMap->ResetTime();
}

void mywindow(int type,CPlayer* player,CDisplay* pDisplay,CGameMap* pMap,CString string,long var)
{
	mywin* lpmywin=NULL;
	if((lpmywin=new mywin(type,player,pDisplay,pMap))!=NULL)
	{
		if(type==WD_NOTHING)
		{
			if(var==WD_DEFALT)
				lpmywin->say(player,pMap,pDisplay,string);
			else
				lpmywin->say(player,pMap,pDisplay,string,var);
			PeekInput();
		}
		delete lpmywin;
	}
	pMap->ResetTime();
}

mywin::mywin(int type,CPlayer* player,CDisplay* pDisplay,CGameMap* pMap,int pos1,int pos2)
{
	switch(type)
	{
	case WD_STAT:
		DisStat(player,pDisplay,pos1,pos2);
		break;
	case WD_HEAL:
		heal(player,pMap,pDisplay,pos1,pos2);
		break;
	case WD_KEY:
		buykey(player,pMap,pDisplay,pos1,pos2);
		break;
	case WD_SAVE:
		savegame(player,pMap,pDisplay);
		break;
	}
}
mywin::~mywin()
{
	//nothing
}
BOOL mywin::heal(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay,int pos1,int pos2)
{
   player->MoveBack();
   say(player,pMap,pDisplay,"Whould you like to be healed#     For 25 gold peaces#         Y or N");
   UINT NewChar=0;
   do
   {
	if(!getinput(&NewChar))
	   return FALSE;
	if(NewChar=='N'||NewChar==VK_ESCAPE)
		return TRUE;
   }while((NewChar!='Y')&&(NewChar!=VK_RETURN));
   if(player->gold>=25)
   {
         player->m_hp=player->maxhp;
		 player->mp=player->maxmp;
		 say(player,pMap,pDisplay,"Thank you come again");
   }
   else
   {
         say(player,pMap,pDisplay,"You do not have 25 gold peices");  
   }
   return TRUE;
}
void mywin::DisStat(CPlayer* player,CDisplay* pDisplay,int pos1,int pos2)
{
	pDisplay->outtext(pos1,(pos2),"                             ");
	pDisplay->outtext(pos1,(pos2+1)," NAME    %-20s",player->name);
	pDisplay->outtext(pos1,(pos2+2)," HP           %-3d            ",player->m_hp);
	pDisplay->outtext(pos1,(pos2+3)," LEVEL        %-2d             ",player->level);
	pDisplay->outtext(pos1,(pos2+4)," XP           %-9lu      ",player->xp);
	pDisplay->outtext(pos1,(pos2+5)," AGILITY      %-2d             ",player->agility);
	pDisplay->outtext(pos1,(pos2+6)," Attack power %-2d             ",player->ha);
	pDisplay->outtext(pos1,(pos2+7)," Defence      %-3d            ",player->defence);
	pDisplay->outtext(pos1,(pos2+8)," Gold         %-6lu         ",player->gold);
	pDisplay->outtext(pos1,(pos2+9)," KEYS         %-2d             ",player->k);
	pDisplay->outtext(pos1,(pos2+10)," mp           %-3d            ",player->mp);
	pDisplay->outtext(pos1,(pos2+11),"                             ");
	pDisplay->DrawRectAroundText(pos1,pos2,pos1+29,pos2+12,RGB(255,255,255));
}

#ifdef __DEBUG
void Dismon(TYPE_MONSTER* monster,int pos1,int pos2)
{
	pDisplay->outtext(pos1,pos2,"                         ");
	pDisplay->outtext(pos1,pos2+1," %-20s    ",monster->name);
	pDisplay->outtext(pos1,pos2+2,"                         ");
	pDisplay->outtext(pos1,pos2+3,"x- %d",monster->m_pos.x);
	pDisplay->outtext(pos1,pos2+4,"y- %d",monster->m_pos.y);
	pDisplay->outtext(pos1,pos2+5,"hp- %d",monster->m_hp);
}
#endif

BOOL mywin::buykey(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay,int pos1,int pos2)
{
   player->MoveBack();
   say(player,pMap,pDisplay,"Whould you like to buy a key#     For 250 gold peaces#         Y or N");
   UINT NewChar=0;
   do
   {
	if(!getinput(&NewChar))
	   return FALSE;
	if(NewChar=='N'||NewChar==VK_ESCAPE)
		return TRUE;
   }while((NewChar!='Y')&&(NewChar!=VK_RETURN));
   if(player->gold>=25)
   {
         player->gold=player->gold-250;
         player->k++;
         say(player,pMap,pDisplay,"Thank you come again!");
   }
   else
   {
         say(player,pMap,pDisplay,"You do not have 250 gold peices");
   }
   return TRUE;	 
}

void mywin::sayraw(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay,const char* string)
{
	char buffer[201];
	char* token;
	int size=2;
	sprintf(buffer,"%s",string);
	pMap->setscr();
	pDisplay->outtext(1,1,"                                 ");
	token=strtok(buffer,"#");
    pDisplay->outtext(1,size++,"  %-31s",token);
	while((token=strtok(NULL,"#"))!=NULL)
	{
		pDisplay->outtext(1,size++,"  %-31s",token);
	}
	pDisplay->outtext(1,size++,"                                 ");
	pDisplay->DrawRectAroundText(1,1,34,size,RGB(255,255,255));
	pDisplay->Present();
}
void mywin::say(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay,const char* format ...)
{	
	va_list ap;
	char s[500];

	if(format==NULL)
		return;
	va_start(ap,format);
	vsprintf(s,format,ap);
	va_end(ap);
	sayraw(player,pMap,pDisplay,s);
}

BOOL mywin::savegame(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay)
{
   
   
   player->MoveBack();
   say(player,pMap,pDisplay,"Whould you like me to record#         your deeds#          Y or N");
   UINT NewChar=0;
   do
   {
	if(!getinput(&NewChar))
	   return FALSE;
	if(NewChar=='N'||NewChar==VK_ESCAPE)
		return TRUE;
   }while((NewChar!='Y')&&(NewChar!=VK_RETURN));

   if(!player->Save(pMap))
	   return FALSE;

   say(player,pMap,pDisplay,"  It has been recorded# You have %10lu xp#    to the next level",player->nextlevel-player->xp);
   
   return TRUE;
}

#ifdef _DEBUG
void log(const char* str)
{
	FILE* data;
	if((data=fopen("log.dat","a"))!=NULL)
	{
		  fprintf(data,"%s\n",str);
		  fclose(data);
	}
}
#endif
