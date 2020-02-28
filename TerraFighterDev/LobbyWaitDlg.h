// LobbyWaitDlg.h: interface for the CLobbyWaitDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOBBYWAITDLG_H__E3746881_A18E_4E4B_92FF_8BD8A8023DAD__INCLUDED_)
#define AFX_LOBBYWAITDLG_H__E3746881_A18E_4E4B_92FF_8BD8A8023DAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"

class CLobbyWaitDlg : public CWinDialog  
{
public:
   virtual int DoIdle();
   CLobbyWaitDlg();
   virtual ~CLobbyWaitDlg();
   virtual BOOL ValidButtonID(int Id);
   virtual BOOL Create(C2DDraw *pDisplay, CPoint p);
   HANDLE	m_hEvent;



};

#endif // !defined(AFX_LOBBYWAITDLG_H__E3746881_A18E_4E4B_92FF_8BD8A8023DAD__INCLUDED_)
