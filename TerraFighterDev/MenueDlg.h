// MenueDlg.h: interface for the CMenueDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUEDLG_H__484B7092_8385_4744_953B_CC254DBAF00D__INCLUDED_)
#define AFX_MENUEDLG_H__484B7092_8385_4744_953B_CC254DBAF00D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinButton.h"

/*#define WIN_BUTTON_NEW WIN_BUTTON_CUSTUM+1
#define WIN_BUTTON_QUIT WIN_BUTTON_CUSTUM+2
#define WIN_BUTTON_RESUME WIN_BUTTON_CUSTUM+3
#define WIN_BUTTON_OPTIONS WIN_BUTTON_CUSTUM+4*/


class CMenueDlg : public CWinDialog  
{
public:
   BOOL Create(C2DDraw *pDisplay,CPoint p);
   BOOL ValidButtonID(int Id);
   CMenueDlg();
   virtual ~CMenueDlg();
   CWinButton m_resume;
private:
   CWinButton m_new;
   CWinButton m_quit;
   CWinButton m_options;

};

#endif // !defined(AFX_MENUEDLG_H__484B7092_8385_4744_953B_CC254DBAF00D__INCLUDED_)
