// Hud.h: interface for the CHud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUD_H__A44D0A9F_A63A_4E04_BE13_8C5DAF7A3EA8__INCLUDED_)
#define AFX_HUD_H__A44D0A9F_A63A_4E04_BE13_8C5DAF7A3EA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "2DDraw.h"
#include "player.h"
#include "WinDialog.h"
#include "WinStatic.h"
#include "WinButton.h"
#include "WinBitmapButton.h"
#include "WinProgress.h"

class CHud  : public CWinDialog
{
public:
	void Show(BOOL bShow = TRUE);
	CWinBitmapButton m_Spells;
	CWinButton m_Items;
	CWinButton m_Status;
	CWinStatic m_Gold;
	CWinStatic m_Keys;
	CWinProgress m_Health;
	CWinProgress m_Mana;
	CWinStatic m_Stuff;
	virtual BOOL Create(C2DDraw *pDisplay, CWinBaceObj* pParent);
	virtual void Display(CPlayer* pPlayer,C2DDraw* pDisplay);
	CHud();
	virtual ~CHud();
};

#endif // !defined(AFX_HUD_H__A44D0A9F_A63A_4E04_BE13_8C5DAF7A3EA8__INCLUDED_)
