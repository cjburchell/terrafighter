// YesNoDlg.h: interface for the CYesNoDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YESNODLG_H__04038FDD_34B9_4F09_B41D_DCA652C456F7__INCLUDED_)
#define AFX_YESNODLG_H__04038FDD_34B9_4F09_B41D_DCA652C456F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinButton.h"
#include "WinStatic.h"
 

class CYesNoDlg : public CWinDialog  
{
public:
	CYesNoDlg();
	virtual ~CYesNoDlg();
	BOOL RegKeyPress(int key);
	virtual BOOL Create(CDisplay *pDisplay, CPoint p, CString text);
protected:
	CWinButton m_yes;
	CWinButton m_no;
	CWinStatic m_textctrl;

};

#endif // !defined(AFX_YESNODLG_H__04038FDD_34B9_4F09_B41D_DCA652C456F7__INCLUDED_)
