// Chest.h: interface for the CChest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHEST_H__94472949_C3E4_4A23_BE16_6BC79F09579C__INCLUDED_)
#define AFX_CHEST_H__94472949_C3E4_4A23_BE16_6BC79F09579C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DEStatic.h"

enum e_chest_type
{
	CHEST_GOLD = 0,
	CHEST_KEY = 1,
	CHEST_ITEM = 2
};

class CChest : public CDEStatic  
{
public:
	int m_itemnumber;
	UINT m_gold;
	BOOL m_bDestroyAfterUse;
	BOOL m_bIsOpen;
	e_chest_type m_Type;
	virtual BOOL Create(CPoint p, CSize s, BOOL bOpen = FALSE, BOOL bKey = FALSE, BOOL bDestroyAfterUse = FALSE);
	CChest();
	virtual ~CChest();

};

#endif // !defined(AFX_CHEST_H__94472949_C3E4_4A23_BE16_6BC79F09579C__INCLUDED_)
