// Spell.h: interface for the CSpell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPELL_H__87D26A32_9608_4DF8_B114_F452FC5CC413__INCLUDED_)
#define AFX_SPELL_H__87D26A32_9608_4DF8_B114_F452FC5CC413__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <2DDraw.h>
#include <surface.h>

enum S_TYPE{S_HEAL,S_OUTSIDE,S_FIREBALL,S_LITBALL};


class CSpell  
{
public:
	BOOL Create(CString file, C2DDraw* pDisplay);
	CSpell();
	virtual ~CSpell();
	BOOL active;
	CString name;
	S_TYPE type;
	int power;
	int costmp;
	CSurface* m_pic;

};

#endif // !defined(AFX_SPELL_H__87D26A32_9608_4DF8_B114_F452FC5CC413__INCLUDED_)
