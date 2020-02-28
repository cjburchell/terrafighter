// UseDlg.h: interface for the CUseDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USEDLG_H__32392BE1_CFFB_4BC5_9258_65A60D99917A__INCLUDED_)
#define AFX_USEDLG_H__32392BE1_CFFB_4BC5_9258_65A60D99917A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinButton.h"

class CUseDlg : public CWinDialog  
{
public:
	virtual BOOL RegKeyPress(int key);
	virtual BOOL Create(C2DDraw *pDisplay, CPoint p,CString Usetext);
	CUseDlg();
	virtual ~CUseDlg();
private:
	CWinButton usebut;
	CWinButton discardbut;

};

#endif // !defined(AFX_USEDLG_H__32392BE1_CFFB_4BC5_9258_65A60D99917A__INCLUDED_)
