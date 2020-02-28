// Monster.h: interface for the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTER_H__5AB67880_8FBF_4CAC_8290_DAC2868E50E4__INCLUDED_)
#define AFX_MONSTER_H__5AB67880_8FBF_4CAC_8290_DAC2868E50E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sprite.h"

enum TYPE_NPC{
	MON		=0,
	HEAL	=1,
	MURCH	=2,
	SAVE	=3,
	KEY		=4,
	NPC		=5,
	ENDMON	=6,
	FIRE	=7,
	LIT		=8,
	PRO		=9
};

class CMonster : public CSprite
{
public:
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
			 TYPE_NPC npctype);
	virtual void Display(CDC* pDC);
	CMonster();
	virtual ~CMonster();
	long m_hp;
	int m_ha;
	unsigned long m_xp;
	CString m_name;
	BOOL m_bCanMove;
	TYPE_NPC m_npctype;
	unsigned long m_gold;
	int m_MoveNumb;
    CBitmap* m_pBitmap;
};

#endif // !defined(AFX_MONSTER_H__5AB67880_8FBF_4CAC_8290_DAC2868E50E4__INCLUDED_)
