// Hud.h: interface for the CHud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUD_H__4FD7447A_5B41_4DE1_A650_E7CDD5BF9724__INCLUDED_)
#define AFX_HUD_H__4FD7447A_5B41_4DE1_A650_E7CDD5BF9724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinButton.h"
#include "WinProgress.h"
#include "WinStatic.h"



class CHud : public CWinDialog  
{
public:
    void Show(BOOL bShow = TRUE);
	CWinButton m_Status;
	CWinStatic m_Text1;
	CWinStatic m_Text2;
	CWinProgress m_Armor;
	CWinProgress m_Shields;
	virtual BOOL Create(C2DDraw *pDisplay, CWinBaceObj* pParent);
	virtual void Display(C2DDraw *pDisplay);
	CHud();
	virtual ~CHud();

};

#endif // !defined(AFX_HUD_H__4FD7447A_5B41_4DE1_A650_E7CDD5BF9724__INCLUDED_)
