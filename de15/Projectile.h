// Projectile.h: interface for the CProjectile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTILE_H__6C1CFBF2_A877_4EA5_B673_2B75CEF3D12C__INCLUDED_)
#define AFX_PROJECTILE_H__6C1CFBF2_A877_4EA5_B673_2B75CEF3D12C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"
#include "player.h"

class CProjectile : public CMonster  
{
public:
	void SeekPoint(CPoint p);
	virtual BOOL HitPlayer(CPlayer* player, C2DDraw* pDisplay);
	virtual BOOL HitOtherMonster(CMonster* pMonster,C2DDraw* pDisplay, IGameMap* pMap, BOOL& IsDead);
	virtual BOOL HitStaticObject();
	virtual void DoMove(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay);	void ClearDirection();
	void MoveOneSpace();
	void MoveThisWay(double Angle);
	virtual BOOL Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap);
	void SeekPlayer(CPlayer* pPlayer);
	CProjectile(CSprite* pOwner);
	virtual ~CProjectile();
protected:
	CSprite* m_pOwner;
};

#endif // !defined(AFX_PROJECTILE_H__6C1CFBF2_A877_4EA5_B673_2B75CEF3D12C__INCLUDED_)
