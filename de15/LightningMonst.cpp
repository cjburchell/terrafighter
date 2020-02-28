// LightningMonst.cpp: implementation of the CLightningMonst class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include "LightningMonst.h"
#include "map.h"

#define TIME_TO_NEXT_SHOT 200

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLightningMonst::CLightningMonst() : CRangeAttackMonster()
{
}

CLightningMonst::~CLightningMonst()
{

}

void CLightningMonst::RangeAttack(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay)
{
	// if we are within 6 spaces
	m_lastshot += pMap->GetElapsedTime();
	if(MoveCheck(6,player)==TRUE)
	{
		// fire a projectile
		if(m_lastshot > TIME_TO_NEXT_SHOT)
		{
			if(rand()%4==0)
			{
				static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(this,player,pMap,pDisplay,m_npctype,GetDrawPoint());
			}
			m_lastshot = 0;
		}
	}
}
