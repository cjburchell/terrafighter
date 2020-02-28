#include <io.h>
#include <dxutil.h>
#include <d3dutil.h>
#include "Types.h"
#include "player.h"
#include "deapp.h"
#include "Items.h"
#include "MenueDlg.h"
#include "saydlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void CPlayer::levelck(C2DDraw* pDisplay,IGameMap* pMap)
{
	if(xp >= nextlevel)
	{
		CSayDlg saydlg;
		saydlg.Create(pDisplay,CPoint(10,10),"Level up");
		saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
	}
   while(xp >= nextlevel)
   {
	  ++level;
	  spells->newspell((level/2)-1);
	  ha=ha+(rand()%7);
	  maxmp+=rand()%6+5;
	  mp=maxmp;
	  maxhp=maxhp+(rand()%7)+1;
	  defence=defence+(rand()%4)+1;
	  agility=agility+(rand()%3);
      nextlevel=(nextlevel*2)+(rand()%10);
   }
}
CPlayer::CPlayer() : CSprite()
{
	m_bPlayer = TRUE;
	PlayerOK=TRUE;
	spells=NULL;
	m_SeekMonster = NULL;
	m_pUseObj = NULL;
}
CPlayer::~CPlayer()
{
	if(spells!=NULL)
		delete spells;
}

void CPlayer::intplayer(C2DDraw* pDisplay)
{
	m_pos.x=11<<ICONSIZEB;
	m_pos.y=11<<ICONSIZEB;
	m_lastpos.x =m_pos.x;
	m_lastpos.y =m_pos.y;
   equip[SWORD]=FALSE;
   equip[ARMOR]=FALSE;
   equip[SHELD]=FALSE;
   agility=rand()%5+1;
   xp=0;
   k=0;
   RandMonstOn=TRUE;
   ha=rand()%5+5;
	level=1;
	map=0;
   m_hp=rand()%5+40;
   maxmp=5;
#ifdef _DEBUG
   maxmp=500;
#endif
   mp=maxmp;
   
   gold=200;
   defence=rand()%5+5;
   maxhp=m_hp;
   nextlevel=50;
   ship=SHIP_OUT;
   m_icon=0;
   dirction=NORTH;
   monster=FALSE;
   PlayerOK=TRUE;
   for(int count=0;count<10;count++)
   {
      items[count].name = "";
      items[count].equip=FALSE;
   }
   if(spells)
	   delete spells;
   spells = new CSpells(pDisplay);
   return;
}

void CPlayer::AninIcon(float time)
{
	float angle =ToDeg(m_Angle);
	if(ship!=SHIP_IN)
	{
		
		if(angle >=45 && angle <135)
			m_icon = 0;
		
		if(angle >=225 && angle<315)
			m_icon = 2;

		if(angle >=135 && angle<255)
			m_icon = 6;

		if(angle >=315 || angle<45)
			m_icon = 4;


		if(((m_icon)%2)!=0)
			m_icon--;
		else
			m_icon++;
	}
	else
	{
		if((angle >=135 && angle<255) || (angle >=315 || angle<45))
			m_icon = 9;

		if((angle >=45 && angle <135) || (angle >=225 && angle<315))
			m_icon = 8;	
	}
}

BOOL CPlayer::playermove(IGameMap* pMap)
{
		BOOL bCanMove=FALSE;;
		m_lastpos=GetDrawPoint();
#define	VELOCITY ((32.0/1000.0) *4.0)

		float fTime = pMap->GetElapsedTime(); // Get current time in seconds
		float distance = fTime * VELOCITY;
		MoveToGoTo(distance,pMap);
		dirction=NORTH;

      if(pMap->chkpos()==TRUE)
		{
			if((monster==FALSE)&&(RandMonstOn==TRUE))
				if(rand()%750==5)
				{
					#ifndef _DEBUG
					pMap->startfight();
					m_move_state = STOPPED;
					#endif
				}
			bCanMove=TRUE;
		}
		else
			m_move_state = STOPPED;
		
		return bCanMove;
}

void CPlayer::loadgame(IGameMap* pMap, C2DDraw* pDisplay)
{
   intplayer(pDisplay);
   FILE* data;
   int numb;
   CString filename;
   filename = name;
   filename += ".sav";
   if((data=fopen(filename,"r"))!=NULL)
   {
	  char tmpname[256];
	  int x,y;
      fscanf(data,"%d %d %d %lU %d %d %d %lU %d %lU %d %d %d %d %d %d %d %d %d %d %d %s %d %d %d %d %d\n",
            &m_hp,
            &defence,
			&maxhp,
			&gold,
			&ha,
			&level,
            &k,
			&xp,
			&agility,
			&nextlevel,
			&map,
            &monster,
			&equip[0],
			&equip[1],
			&equip[2],
			&x,
            &y,
			&m_lastpos.x,
			&m_lastpos.y,
			&m_savepos.x,
			&m_savepos.y,
			tmpname,
            &ship,
			&m_icon,
			&RandMonstOn,
			&mp,
			&maxmp);
	  name = tmpname;
	  m_pos.x = x;
	  m_pos.y = y;
      for(numb=0;numb<10;numb++)
      {
		 char tmpitemname[256];
		 char filename[100];
		 char buffer[256];
		 fgets(buffer,256,data);
         sscanf(buffer,"%s %d %d %d %d %lU %d %d %s\n",
			 tmpitemname,
			 &items[numb].hp,
			 &items[numb].defence,
             &items[numb].ha,
			 &items[numb].agility,
			 &items[numb].cost,
			 &items[numb].equip,
			 &items[numb].type,
			 filename);
		 items[numb].m_image_file = filename;
		 items[numb].name = tmpitemname;
         if(items[numb].name == "#")
			items[numb].name = "";
      }
	  spells->load(data);
      filename.Format("%s%d.map",name,map);
      if(!pMap->getmap(filename,pDisplay))
	  {
		pMap->getmap("map0.dat",pDisplay);
		intplayer(pDisplay);
	  }
	  else
	  {
		  filename.Format("%s%d.fog",name,map);
		  pMap->LoadFogMap(filename);
	  }

	  for(int i=0;i<MAXMAPS;i++)
	  {
		filename.Format("temp%d.map",i);
		remove(filename);
		filename.Format("temp%d.fog",i);
		remove(filename);
	  }
      fclose(data);
   }
   else
   {
      pMap->getmap("map0.dat",pDisplay);
      intplayer(pDisplay);
   }
}

BOOL CPlayer::GetInput(CInput *pInput,IGameMap *pMap, C2DDraw *pDisplay)
{
	
	//#define KeyPressed(key) (GetAsyncKeyState(key) & 0x8000)
	
	/*if(pInput->Keys[DIK_UP] & 0x80)
	{
		pInput->Keys[DIK_UP] = 0;
		playermove(VK_UP,pMap);
		m_move_state = STOPPED;
	}
	if(pInput->Keys[DIK_DOWN] & 0x80)
	{
		pInput->Keys[DIK_DOWN] = 0;
		playermove(VK_DOWN,pMap);
		m_move_state = STOPPED;
	}
	if(pInput->Keys[DIK_RIGHT] & 0x80)
	{
		pInput->Keys[DIK_RIGHT] = 0;
		playermove(VK_RIGHT,pMap);
		m_move_state = STOPPED;
	}
	if(pInput->Keys[DIK_LEFT] & 0x80)
	{
		pInput->Keys[DIK_LEFT] = 0;
		playermove(VK_LEFT,pMap);
		m_move_state = STOPPED;
	}*/

   while(!pInput->IsButtonPressEmpty())
	{
      int key = pInput->RemoveButtonPress();
		switch(key)
		{
		/*case VK_UP:
		case VK_DOWN:
		case VK_RIGHT:
		case VK_LEFT:		
			playermove(key,pMap);
		break;*/

		case 'Q':
		{
			if(!startnew(pDisplay,pInput,pMap))
				return FALSE;
			m_move_state = STOPPED;
		} 
		break;
		case VK_ESCAPE:
		case 'O':
		{
		} 
		break;
		
		case VK_RETURN:
		{
			ShowStatus(pDisplay);
		} 
		break;
		case 'P':
		{
			//PlaySound("Start.wav", NULL, SND_SYNC);
		} 
		break;
		case 'E':
		{
			if(monster==TRUE && map<6)
				if((rand()%(agility))>2)
					pMap->endfight();
			m_move_state = STOPPED;
		} 
		break;
		case 'I':
		{
			Citems* pitems=NULL;
			if((pitems = new Citems())!=NULL)
			{
				if(!pitems->showitems(this,pMap,pDisplay))
				{
					delete pitems;
					return FALSE;
				}
				delete pitems;
			} 
		} 
		break;
		case 'L':
		{
			spells->use(this,pMap,pDisplay);
		} 
		break;
		
		case 'K':
		{
			spells->select(pMap,pDisplay);
		} 
		break;
		case 'N':
			if((monster==FALSE)&&(RandMonstOn==TRUE))
				pMap->startfight();
			m_move_state = STOPPED;
			break;
		break;

		}
	}

	if(m_move_state == MOVING)
	{
		playermove(pMap);
	}
	else if(m_move_state == SEEKMONSTER && m_SeekMonster)
	{
		if(CanSee(m_SeekMonster->GetCenterPoint(),pMap))
		{
			SeekPoint(m_SeekMonster->GetCenterPoint(),pMap,TRUE);
			if(m_move_state == MOVING)
				m_move_state = SEEKMONSTER;
		}
		playermove(pMap);
		if(m_SeekMonster)
		{
			CRect monstrect = m_SeekMonster->GetRect();
			monstrect.InflateRect(10,10);
			CRect rectInter;
			if(rectInter.IntersectRect(monstrect,GetRect()))
			{
				CheckMonster(m_SeekMonster,pDisplay,pMap);
			}
		}
	}
	else if(m_move_state == USEOBJECT && m_pUseObj)
	{
		playermove(pMap);
		if(m_pUseObj)
		{
			CRect monstrect = m_pUseObj->GetRect();
			monstrect.InflateRect(10,10);
			CRect rectInter;
			if(rectInter.IntersectRect(monstrect,GetRect()))
			{
				if(!m_pUseObj->Use(this,pDisplay,pMap))
				{
					static_cast<CGameMap*>(pMap)->KillObject(m_pUseObj);
				}
				m_pUseObj = NULL;
				m_move_state = STOPPED;
			}
		}
	}
	int offsety = pDisplay->GetScreenHight()/2;
	int offsetx = pDisplay->GetScreenWidth()/2;
	pMap->SetMapPos(m_pos.x-offsetx,m_pos.y-offsety,offsetx,offsety);
	pMap->ClearFog(GetCenterPoint());
	return TRUE;
}



BOOL CPlayer::startnew(C2DDraw* pDisplay,CInput* pInput,IGameMap* pMap)
{
	GetWindowMgr()->RemoveChild(GetApp()->GetHud());
	static_cast<CGameMap*>(pMap)->m_bMapLoaded = FALSE;
	pMap->FadeIn(500);
	CMenueDlg dlg;
	if(dlg.Create(pDisplay,CPoint(160,247)))
	{
		int key = dlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
		if(key != -1)
		{
			if(key == VK_RETURN || key == 'L')
			{
				if(!GetName(name,pDisplay))
				{
					pMap->FadeOut(500);
					static_cast<CGameMap*>(pMap)->m_bMapLoaded = TRUE;
					pMap->FadeIn(500);
					pMap->ResetTime();
					GetWindowMgr()->AddChild(GetApp()->GetHud());
					return FALSE;
				}

				if(key==VK_RETURN)
				{
					pMap->FadeOut(500);
					pMap->getmap("map0.dat",pDisplay);
					intplayer(pDisplay);
					for(int i=0;i<MAXMAPS;i++)
					{
						char file[30];
						sprintf(file,"%s%d.map",name,i);
						remove(file);
						sprintf(file,"%s%d.fog",name,i);
						remove(file);
					}
					static_cast<CGameMap*>(pMap)->m_pmonst->end();
				}
				else
				{
					pMap->FadeOut(500);
					loadgame(pMap,pDisplay);	
					static_cast<CGameMap*>(pMap)->m_pmonst->start(this,pDisplay,map,NUMB_TOWN_NPC,FALSE);
					
				}

				for(int i=0;i<MAXMAPS;i++)
				{
					char file[30];
					sprintf(file,"temp%d.map",i);
					remove(file);
					sprintf(file,"temp%d.fog",i);
					remove(file);
				}
				PlayerOK=TRUE;
				pMap->GoToPos(GetDrawPoint());
				pMap->ClearFog(GetCenterPoint());
				GetWindowMgr()->AddChild(GetApp()->GetHud());
				pMap->FadeIn(500);
			}
			else
			{
				pMap->FadeOut(500);
				pMap->getmap("map0.dat",pDisplay);
				intplayer(pDisplay);
				return TRUE;
			}

		}
		else
		{
			pMap->FadeOut(500);
			static_cast<CGameMap*>(pMap)->m_bMapLoaded = TRUE;
			pMap->FadeIn(500);
			pMap->ResetTime();
			GetWindowMgr()->AddChild(GetApp()->GetHud());
			return FALSE;
		}
	}
	pMap->ResetTime();
	return TRUE;
}
BOOL CPlayer::GetName(CString& string ,C2DDraw* pDisplay)
{
   string = "Test1";
   CSayDlg SayDlg;
   SayDlg.Create(pDisplay,CPoint(10,10),"Enter in your name\n                    ");
   SayDlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr()); 
   return TRUE;
}

void CPlayer::SetSavedPos()
{
	m_savepos = GetLastCenterPoint();
	//m_savepos.x = (m_savepos.x>>ICONSIZEB)<<ICONSIZEB;
	//m_savepos.y = (m_savepos.y>>ICONSIZEB)<<ICONSIZEB;
}

void CPlayer::GotoSavedPos()
{
	GotoPos(m_savepos);
}

BOOL CPlayer::Save(IGameMap* pMap)
{
   FILE* data;
   char filename[24];
   strcpy(filename,name);
   strcat(filename,".sav");
   if((data=fopen(filename,"w"))!=NULL)
   {
	   int x = m_pos.x;
	   int y = m_pos.y;
		fprintf(data,"%d %d %d %lu %d %d %d %lu %d %lu %d %d %d %d %d %d %d %d %d %d %d %s %d %d %d %d %d\n",
			m_hp,
			defence,
			maxhp,
			gold,
			ha,
			level,
			k,
			xp,
			agility,
			nextlevel,
			map,
			monster,
			equip[0],
			equip[1],
			equip[2],
			x,
            y,
			m_lastpos.x,
			m_lastpos.y,
			m_savepos.x,
			m_savepos.y,
			name,
            ship,
			m_icon,
			RandMonstOn,
			mp,
			maxmp);
         for(int numb=0;numb<10;numb++)
         {
            if(items[numb].name == "")
            {
               items[numb].name ="#";
			   items[numb].m_image_file = "#";
            }
            fprintf(data,"%s %d %d %d %d %lu %d %d %s\n",
				items[numb].name,
				items[numb].hp,
				items[numb].defence,
				items[numb].ha,
				items[numb].agility,
				items[numb].cost,
				items[numb].equip,
				items[numb].type,
				items[numb].m_image_file);
            if(items[numb].name == "#")
			{
               items[numb].name = "";
			   items[numb].m_image_file = "";
			}
         }
		 spells->save(data);
		 for(int i=0;i<MAXMAPS;i++)
		 {
			char filename2[24];
			sprintf(filename2,"temp%d.map",i);
			if(_access(filename2,0)!= -1)
			{
				sprintf(filename,"%s%d.map",name,i);
				remove(filename);
				rename(filename2,filename);
			}
			sprintf(filename2,"temp%d.fog",i);
			if(_access(filename2,0)!= -1)
			{
				sprintf(filename,"%s%d.fog",name,i);
				remove(filename);
				rename(filename2,filename);
			}
		 }
         sprintf(filename,"%s%d.map",name,map);
         pMap->savemap(filename);
		 sprintf(filename,"%s%d.fog",name,map);
		 pMap->SaveFogMap(filename);
         fclose(data);
		 return TRUE;
   }
   return FALSE;	 
}

void CPlayer::SeekMonster(CMonster *pMonst,IGameMap* pMap)
{
	SeekPoint(pMonst->GetCenterPoint(),pMap,TRUE);
	if(m_move_state == MOVING)
		m_move_state = SEEKMONSTER;
	m_SeekMonster = pMonst;
}

void CPlayer::StopSeekingMonster()
{
	m_SeekMonster = NULL;
	m_pUseObj = NULL;
	m_move_state = STOPPED;
}

CMonster* CPlayer::GetSeekMonster()
{
	return m_SeekMonster;
}

void CPlayer::UseSpell(IGameMap *pMap, C2DDraw *pDisplay, CPoint p)
{
	spells->use(this,pMap,pDisplay,TRUE,p);
}

void CPlayer::ShowStatus(C2DDraw *pDisplay)
{
	CString status;
	status.Format("Name    %-20s\nHitPoints    %-3d\nLevel        %-2d\nXp           %-9lu\nAgility      %-2d\nAttack power %-2d\nDefence      %-3d\nGold         %-6lu\nKeys         %-2d\nMp           %-3d",
		name,
		m_hp,
		level,
		xp,
		agility,
		ha,
		defence,
		gold,
		k,
		mp);
	CSayDlg saydlg;
	saydlg.Create(pDisplay,CPoint(10,10),status);
	saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
}

BOOL CPlayer::CheckMonster(CMonster *pMonst, C2DDraw *pDisplay, IGameMap *pMap)
{	
	if(pMonst == m_SeekMonster && m_move_state == SEEKMONSTER)
	{
		TRACE1("player uses %s\n",m_SeekMonster->m_name);
		if(!m_SeekMonster->Use(this,pDisplay,pMap))
		{
			static_cast<CGameMap*>(pMap)->m_pmonst->KillMonster(m_SeekMonster,this);
		}
		m_SeekMonster = NULL;
		m_move_state = STOPPED;
		return TRUE;
	}
	return FALSE;
}

void CPlayer::SeekObject(CDEStatic *pObj, IGameMap* pMap)
{
	SeekPoint(pObj->GetCenterPoint(),pMap,TRUE);
	if(m_move_state == MOVING)
			m_move_state = USEOBJECT;
	m_pUseObj = pObj;
}
