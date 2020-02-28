// NPC.h: interface for the CNPC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__98D969D7_F79E_42AE_984F_DAD78BE50091__INCLUDED_)
#define AFX_NPC_H__98D969D7_F79E_42AE_984F_DAD78BE50091__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"
#include "player.h"

class CNPC : public CMonster  
{
public:
	CString ConvertString(CString str);
	virtual BOOL HitPlayer(CPlayer* player, C2DDraw* pDisplay);
	virtual BOOL HitOtherMonster(CMonster* pMonster,C2DDraw* pDisplay, IGameMap* pMap, BOOL& IsDead);
	virtual BOOL HitStaticObject();
	virtual void DoMove(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay);	
	virtual BOOL Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap);
	CNPC();
	virtual ~CNPC();

};

#endif // !defined(AFX_NPC_H__98D969D7_F79E_42AE_984F_DAD78BE50091__INCLUDED_)
