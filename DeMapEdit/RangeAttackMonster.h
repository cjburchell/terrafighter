// RangeAttackMonster.h: interface for the CRangeAttackMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANGEATTACKMONSTER_H__D5130C61_1EFF_46C3_81DF_49B095EF33A6__INCLUDED_)
#define AFX_RANGEATTACKMONSTER_H__D5130C61_1EFF_46C3_81DF_49B095EF33A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"

class CRangeAttackMonster : public CMonster  
{
public:
	DWORD m_lastshot;
	CRangeAttackMonster();
	virtual ~CRangeAttackMonster();

};

#endif // !defined(AFX_RANGEATTACKMONSTER_H__D5130C61_1EFF_46C3_81DF_49B095EF33A6__INCLUDED_)
