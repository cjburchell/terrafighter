// RangeAttackMonster.cpp: implementation of the CRangeAttackMonster class.
//
//////////////////////////////////////////////////////////////////////

#include <afxwin.h>
#include "RangeAttackMonster.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRangeAttackMonster::CRangeAttackMonster() : CMonster()
{
	m_lastshot = 0;
}

CRangeAttackMonster::~CRangeAttackMonster()
{

}

void CRangeAttackMonster::DoMove(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay)
{
	// fire ball or lighting monsters
	// if the player is within 5 spaces
#define VELOCITY (32.0/1000.0)*2.0
	float Distace =  (VELOCITY*pMap->GetElapsedTime())*m_Speed;
	if(MoveCheck(3,player)==TRUE)
	{
		// run away player
		CPoint OldGoto = m_GotoPos;
		e_SpriteState Oldstate = m_move_state;
		double Oldangle = m_Angle;
		CPoint OldNext = m_NextPos;
		SeekPoint(player->GetCenterPoint(),pMap);
		if(m_move_state == MOVING)
			m_move_state = RUNAWAY;
		else
		{
			if(Oldstate != SEEKMONSTER || Oldstate != RUNAWAY)
			{
				m_GotoPos =  OldGoto;
	    		m_move_state = Oldstate;
				m_Angle =  Oldangle;
				m_NextPos = OldNext;
			}
		}
	}
	else if(MoveCheck(7,player)==TRUE)
	{
		// if we are within 7
		// move tward the player
		CPoint OldGoto = m_GotoPos;
		e_SpriteState Oldstate = m_move_state;
		double Oldangle = m_Angle;
		CPoint OldNext = m_NextPos;
		SeekPoint(player->GetCenterPoint(),pMap);
		if(m_move_state == MOVING)
			m_move_state =SEEKMONSTER;
		else
		{
			if(Oldstate != RUNAWAY || Oldstate != SEEKMONSTER)
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
		if(m_move_state == SEEKMONSTER || m_move_state == RUNAWAY)
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
	else if(m_move_state == RUNAWAY)
	{
		MoveToGoTo(-Distace,pMap);
	}
		
	RangeAttack(player,pMap,pDisplay);
}
