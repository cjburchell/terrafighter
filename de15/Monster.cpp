// Monster.cpp: implementation of the CMonster class.
//
//////////////////////////////////////////////////////////////////////
#include <afx.h>
#include <afxwin.h>
#include "Monster.h"
#include "player.h"
#include "dxutil.h"
#include "map.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonster::CMonster() : CSprite()
{
	m_pic=NULL;
	m_MoveNumb=0;
	m_Speed=1;
}

CMonster::~CMonster()
{
	if(m_pic!=NULL)
		delete m_pic;
}

BOOL CMonster::Attack(CSprite *other,C2DDraw* pDisplay)
{
	if(other->IsPlayer())
	{
		TRACE1("%s attacks player\n",m_name);
		CPlayer* pPlayer = static_cast<CPlayer*>(other);
		int ma =rand()%m_ha;
		ma = ma-(ma*pPlayer->defence/100);
		if(ma<0)ma=0;
		if(ma>0)
		{

         // Flash screen Red!

         sys_msg("TODO: Flash screen Red!");
			/*pDisplay->Clear(RGB(0,0,0));
			pDisplay->StartRender();
			pDisplay->Present();
			pDisplay->Clear(RGB(255,255,255));
			pDisplay->StartRender();
			pDisplay->Present();
			pDisplay->Clear(RGB(0,0,0));
			pDisplay->StartRender();
			pDisplay->Present();*/
		}
		pPlayer->m_hp = pPlayer->m_hp - ma;
		if(pPlayer->m_hp <= 0)
			pPlayer->PlayerOK=FALSE;
	}
	else
	{
		TRACE2("%s attacks %s\n",m_name,static_cast<CMonster*>(other)->m_name);
		CMonster* pMonster = static_cast<CMonster*>(other);
		int ma =rand()%m_ha;
		if(ma<0)ma=0;
		if(ma>0)
		{	
			pMonster->m_hp=pMonster->m_hp-ma;
		}

		// if the fire is a projectile
		if(pMonster->m_hp<=0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMonster::Move(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay)
{
	BOOL alive = TRUE;
	m_lastpos=GetDrawPoint();

	DoMove(player,pMap,pDisplay);
	CSize size = pMap->GetSize();

	if(m_pos.y<0)
		m_pos.y=0;
	if(m_pos.y>size.cy)
		m_pos.y=size.cy;

	if(m_pos.x>size.cx)
		m_pos.x=size.cx;
	if(m_pos.x<0)
		m_pos.x=0;	

	if(!pMap->CheckPos(GetCenterPoint()))
	{
		alive = HitStaticObject();
	}
	CMonster* pOther=static_cast<CGameMap*>(pMap)->m_pmonst->OtherMonstCheck(this);
	if(pOther)
	{
		BOOL IsDead = FALSE;
		alive = HitOtherMonster(pOther,pDisplay,pMap,IsDead);
		if(IsDead)
			static_cast<CGameMap*>(pMap)->m_pmonst->KillMonster(pOther,player);
	}

	CRect rectInter;
	if(rectInter.IntersectRect(GetRect(),player->GetRect()))
	{
		alive = HitPlayer(player,pDisplay);
	}
	m_MoveNumb++;
	int nummove=player->ha-m_ha;

	if(nummove<=0)
		nummove= 1 << ICONSIZEB;
	else
		nummove*= 2 << ICONSIZEB;
	if(m_MoveNumb>nummove)
		m_Speed=1;
	return alive;
}

BOOL CMonster::MoveCheck(int spaces,CPlayer* player)
{
	// how far is the monster form the player?
	int temp1=(player->GetDrawPoint().x-GetDrawPoint().x)>>ICONSIZEB;
	int temp2=(player->GetDrawPoint().y-GetDrawPoint().y)>>ICONSIZEB;

	// if the player is within ceten number of "spaces" return TRUE 
	if((temp1<spaces)&&(temp1>(-spaces)))
	{
		if((temp2<spaces)&&(temp2>(-spaces)))
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

BOOL CMonster::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	int ha;
	if((rand()%(22-(player->agility/2)))==0)
		ha = (rand()%(player->ha+(20*player->level)))+10;
	else
		ha = (rand()%player->ha);
	if(ha>0)
	{
		m_hp = m_hp - ha;
	}
	if(m_hp<=0)
	{
		player->xp = player->xp + m_xp;
		player->levelck(pDisplay,pMap);
		CPoint p = GetDrawPoint();
		pMap->CreateChest(p,m_gold);
		return FALSE;
	}
	return TRUE;
}

void CMonster::Display(IGameMap *pMap, C2DDraw *pDisplay,CCursor* pCursor)
{
	if(pMap->IsInFog(GetCenterPoint()))
		return;
	CPoint scr = pMap->MapToScreen(GetDrawPoint());
	int h = GetSize().cy;
	int w = GetSize().cx;
   int maxh =  pDisplay->GetScreenHight();
   int maxw =  pDisplay->GetScreenWidth();
	if(scr.x < maxw && scr.y < maxh && scr.y+h>0 && scr.x+w>0)
	{
		int offsety = 0;
		int offsetx = 0;
		if(scr.x < 0)
		{
			w = w + scr.x;
			offsetx = -scr.x;
			scr.x = 0;
		}
		else if(scr.x+w > maxw)
		{
			w = maxw - scr.x;
		}

		if(scr.y < 0)
		{
			h = h + scr.y;
			offsety = -scr.y;
			scr.y = 0;
		}
		else if(scr.y+h > maxh)
		{
			h = maxh - scr.y;
		}

		CPoint rectPoint(0,0);
		CSize  size(w,h);
		CRect rect(rectPoint,size);
		rect.OffsetRect(offsetx,offsety);

		if(m_pic!=0)
			pDisplay->Blt(scr.x,scr.y,m_pic,&rect);

		CPoint cursor = pMap->ScreenToMap(pCursor->GetPos());
		if(GetRect().PtInRect(cursor))
		{
			int size = GetSize().cx>>ICONSIZEB;
			pCursor->ShowSel(scr.x,scr.y,rect,pDisplay,size);
		}
		DisplayPath(pMap,pDisplay);
	
	}
}

BOOL CMonster::Create(
		     int hp1,
			 int constpoint,
			 int ha,
			 UINT xp,
			 CString bmp_file,
			 CString name,
			 UINT gold,
			 CSize size,
			 CPoint pos,
			 BOOL movement,
			 TYPE_NPC npctype,
			 IDirect3DDevice9 *pDisplay)
{
	m_gold = gold;
	m_bCanMove = movement;
	m_ha = ha;
	m_name = name;
	m_npctype = npctype;
	m_xp = xp;
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_size = size;
	m_hp=0;
    for(int i=0;i<hp1;i++)
		m_hp=m_hp+(rand()%8)+1;
	m_hp=constpoint+m_hp;
	if(m_hp<=0)
		m_hp=1;
	if(m_pic!=NULL)
		delete m_pic;
	m_pic = new CSurface();
	if(FAILED(m_pic->Init(bmp_file,m_size.cx,m_size.cy,pDisplay)))
	{
		  sys_printf(__LOCATION__,0,"uable to open %s",bmp_file);
		  return FALSE;
	}
	if(m_pic!=NULL)
	{
		m_pic->SetColorKey(RGB(255,0,255));
	}
	if(m_pos.x==0)
	{
		m_pos.x=rand()%((17-(m_size.cx>>ICONSIZEB))<<ICONSIZEB)+1;
		m_pos.y=rand()%((12-(m_size.cy>>ICONSIZEB))<<ICONSIZEB)+1;
	}
	m_lastpos=GetDrawPoint();
	m_MoveNumb=0;
	m_Speed=1;
	return TRUE;
}

void CMonster::DoMove(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay)
{
#define VELOCITY (32.0/1000.0)*2.0
	float Distace =  (VELOCITY*pMap->GetElapsedTime())*m_Speed;

	if(MoveCheck(7,player)==TRUE) // if the player is within 7 spaces
	{
		// search and destroy player
		CPoint OldGoto = m_GotoPos;
		e_SpriteState Oldstate = m_move_state;
		double Oldangle = m_Angle;
		CPoint OldNext = m_NextPos;
		
		SeekPoint(player->GetCenterPoint(),pMap);
		if(m_move_state == MOVING)
		{
			m_move_state = SEEKMONSTER;
		}
		else
		{
			if(Oldstate != SEEKMONSTER)
			{
				m_GotoPos =  OldGoto;
	    		m_move_state = Oldstate;
				m_Angle =  Oldangle;
				m_NextPos = OldNext;
			}
		}
	}
	else
	{
		if(m_move_state == SEEKMONSTER)
			m_move_state = STOPPED;
	}
	

	if(m_move_state == STOPPED)
	{
		if(rand()%20!=0)
		{
			CPoint GotoPos = GetDrawPoint();
			GotoPos.Offset(rand()%RANGE - HRANGE,rand()%RANGE - HRANGE);
			SeekPoint(GotoPos,pMap);
		}
	}
	else if(m_move_state == MOVING)
	{
		if(rand()%20!=0)
		{
			MoveToGoTo(Distace,pMap);
		}
		else
			m_move_state = STOPPED;
	}
	else if(m_move_state == SEEKMONSTER)
	{
		MoveToGoTo(Distace,pMap);
	}
}

BOOL CMonster::HitStaticObject()
{
	MoveBack();
	return TRUE;
}

BOOL CMonster::HitOtherMonster(CMonster* pMonster,C2DDraw* pDisplay, IGameMap* pMap, BOOL& IsDead)
{
	IsDead = FALSE;
	MoveBack();
	return TRUE;
}

BOOL CMonster::HitPlayer(CPlayer* player, C2DDraw* pDisplay)
{
	MoveBack();
	Attack(player,pDisplay);
	m_Speed=(-1);
	m_MoveNumb=0;
	return TRUE;
}
