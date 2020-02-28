// SayDlg.h: interface for the CSayDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAYDLG_H__6D0C8660_470C_4E51_A08E_C7E42178CF02__INCLUDED_)
#define AFX_SAYDLG_H__6D0C8660_470C_4E51_A08E_C7E42178CF02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinButton.h"
#include "WinStatic.h"

class CSayDlg : public CWinDialog  
{
public:
	void SetText(CString str);
	BOOL RegKeyPress(int key);
	virtual BOOL Create(CDisplay *pDisplay, CPoint p, CString text);
	CSayDlg();
	virtual ~CSayDlg();
protected:
	CWinButton m_ok;
	CWinStatic m_textctrl;
};

#endif // !defined(AFX_SAYDLG_H__6D0C8660_470C_4E51_A08E_C7E42178CF02__INCLUDED_)
