// Monster.h: interface for the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTER_H__5AB67880_8FBF_4CAC_8290_DAC2868E50E4__INCLUDED_)
#define AFX_MONSTER_H__5AB67880_8FBF_4CAC_8290_DAC2868E50E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <2DDraw.h>
#include "sprite.h"
#include "types.h"
#include "mouse.h"
#include "player.h"

class CPlayer;

class CMonster : public CSprite
{
public:
	virtual BOOL HitPlayer(CPlayer* player, C2DDraw* pDisplay);
	virtual BOOL HitOtherMonster(CMonster* pMonster,C2DDraw* pDisplay, IGameMap* pMap, BOOL& IsDead);
	virtual BOOL HitStaticObject();
	virtual void DoMove(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay);
	BOOL Create(
		     int hp1,
			 int constpoint,
			 int ha,
			 UINT xp,
			 CString bmp_file,
			 CString tmpname,
			 UINT gold,
			 CSize size,
			 CPoint pos,
			 BOOL movement,
			 TYPE_NPC npctype,
			 IDirect3DDevice9 *pDisplay);
	virtual void Display(IGameMap *pMap,C2DDraw* pDisplay,CCursor* pCursor);
	virtual BOOL Use(CPlayer* player, C2DDraw* pDisplay, IGameMap* pMap);
	BOOL MoveCheck(int spaces,CPlayer* player);
	virtual BOOL Move(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay);
	virtual BOOL Attack(CSprite* other,C2DDraw* pDisplay);
	CMonster();
	virtual ~CMonster();
	CSurface* m_pic;
	long m_hp;
	int m_ha;
	unsigned long m_xp;
	CString m_name;
	BOOL m_bCanMove;
	TYPE_NPC m_npctype;
	unsigned long m_gold;
	int m_MoveNumb;
};

#endif // !defined(AFX_MONSTER_H__5AB67880_8FBF_4CAC_8290_DAC2868E50E4__INCLUDED_)
