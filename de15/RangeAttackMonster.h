// RangeAttackMonster.h: interface for the CRangeAttackMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANGEATTACKMONSTER_H__D5130C61_1EFF_46C3_81DF_49B095EF33A6__INCLUDED_)
#define AFX_RANGEATTACKMONSTER_H__D5130C61_1EFF_46C3_81DF_49B095EF33A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"
#include "player.h"

class CRangeAttackMonster : public CMonster  
{
public:
	virtual void DoMove(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay);
	DWORD m_lastshot;
	virtual void RangeAttack(CPlayer* player, IGameMap* pMap, C2DDraw* pDisplay) = 0;
	CRangeAttackMonster();
	virtual ~CRangeAttackMonster();

};

#endif // !defined(AFX_RANGEATTACKMONSTER_H__D5130C61_1EFF_46C3_81DF_49B095EF33A6__INCLUDED_)
