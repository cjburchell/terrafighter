// LobbyWaitDlg.cpp: implementation of the CLobbyWaitDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "LobbyWaitDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLobbyWaitDlg::CLobbyWaitDlg() : CWinDialog()
{

}

CLobbyWaitDlg::~CLobbyWaitDlg()
{

}

BOOL CLobbyWaitDlg::ValidButtonID(int Id)
{
   return  Id == IDCANCEL || Id == IDOK;
}

BOOL CLobbyWaitDlg::Create(C2DDraw *pDisplay, CPoint p)
{
   LoadDialog(IDD_LOBBY_WAIT_STATUS,pDisplay);
   return CWinDialog::Create(pDisplay,NULL,p,GetSize(),GetText());
}

int CLobbyWaitDlg::DoIdle()
{
   DWORD dwResult = WaitForSingleObject( m_hEvent, 0 );
   if( dwResult != WAIT_TIMEOUT )
      return IDOK;
   return CWinDialog::DoIdle();
}
