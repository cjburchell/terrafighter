// MenueDlg.h: interface for the CMenueDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUEDLG_H__484B7092_8385_4744_953B_CC254DBAF00D__INCLUDED_)
#define AFX_MENUEDLG_H__484B7092_8385_4744_953B_CC254DBAF00D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinDialog.h>
#include <WinButton.h>

class CMenueDlg : public CWinDialog  
{
public:
	BOOL Create(C2DDraw *pDisplay,CPoint p);
	BOOL RegKeyPress(int key);
	CMenueDlg();
	virtual ~CMenueDlg();
private:
	CWinButton m_new;
	CWinButton m_load;
	CWinButton m_quit;

};

#endif // !defined(AFX_MENUEDLG_H__484B7092_8385_4744_953B_CC254DBAF00D__INCLUDED_)
