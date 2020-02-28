// SpellDlg.h: interface for the CSpellDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPELLDLG_H__9E2FDA1E_5A87_44D6_B866_69A32B9961F4__INCLUDED_)
#define AFX_SPELLDLG_H__9E2FDA1E_5A87_44D6_B866_69A32B9961F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinBitmapButton.h"
#include "spells.h"

class CSpellDlg : public CWinDialog  
{
public:
	BOOL RegKeyPress(int key);
   virtual BOOL Create(C2DDraw *pDisplay, CPoint p,CSpells* pSpells);
	CSpellDlg();
	virtual ~CSpellDlg();
private:
	CWinBitmapButton m_buttons[6];
	CWinButton m_close;

};

#endif // !defined(AFX_SPELLDLG_H__9E2FDA1E_5A87_44D6_B866_69A32B9961F4__INCLUDED_)
