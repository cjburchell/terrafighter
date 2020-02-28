// NPC.cpp: implementation of the CNPC class.
//
//////////////////////////////////////////////////////////////////////
#include <afx.h>
#include "NPC.h"
#include "saydlg.h"
#include "deapp.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPC::CNPC() :CMonster()
{
	m_move_state = STOPPED;
}

CNPC::~CNPC()
{

}

BOOL CNPC::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	CSayDlg dlg;
	dlg.Create(pDisplay,CPoint(10,10),ConvertString(m_name));
	dlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
	return TRUE;
}


void CNPC::DoMove(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay)
{
	// can we move?

	if(m_bCanMove)
	{
		float VELOCITY  = ((32.0/1000.0)*1.0);
		float Distace =  (VELOCITY*pMap->GetElapsedTime())* m_Speed;
		if(Distace>32.00)
			Distace = 32.00;
		
		//move randomly
		
		if(m_move_state == STOPPED)
		{
			if(rand()%20!=0)
			{
				CPoint GotoPos = GetDrawPoint();
				GotoPos.Offset((rand()%100)-35,(rand()%100)-35);
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

			if(m_GotoPos == GetDrawPoint())
				m_move_state = STOPPED;
		}
	}
}

BOOL CNPC::HitPlayer(CPlayer* player, C2DDraw* pDisplay)
{
	m_move_state = STOPPED;
	MoveBack();
	return TRUE;
}

BOOL CNPC::HitOtherMonster(CMonster* pMonster,C2DDraw* pDisplay, IGameMap* pMap, BOOL& IsDead)
{
	m_move_state = STOPPED;
	return CMonster::HitOtherMonster(pMonster,pDisplay,pMap,IsDead);
}
BOOL CNPC::HitStaticObject()
{
	m_move_state = STOPPED;
	return CMonster::HitStaticObject();
}

CString CNPC::ConvertString(CString str)
{
	str.Replace('#','\n');
	str.Replace('-',' ');
	return str;
}
