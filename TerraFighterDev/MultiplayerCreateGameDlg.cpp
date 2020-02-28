// MultiplayerCreateGameDlg.cpp: implementation of the CMuliplayerCreateGameDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MultiplayerCreateGameDlg.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMultiplayerCreateGameDlg::CMultiplayerCreateGameDlg()
{

}

CMultiplayerCreateGameDlg::~CMultiplayerCreateGameDlg()
{

}

BOOL CMultiplayerCreateGameDlg::ValidButtonID(int Id)
{
   return (Id == IDOK || Id == IDCANCEL);
}

CWinBaceObj* CMultiplayerCreateGameDlg::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   switch(id)
   {
      WIN_USE_CTRL(IDC_EDIT_SESSION_NAME, m_Name);
      WIN_USE_CTRL(IDC_MIGRATE_HOST, m_MigrateHost);
      WIN_USE_CTRL(IDC_USE_DPNSVR, m_DPNSVR);
      WIN_USE_CTRL(IDC_LOCAL_PORT, m_Port);
      WIN_USE_CTRL(IDC_SIGNING_FULL, m_SigningFull);
      WIN_USE_CTRL(IDC_SIGNING_FAST, m_SigningFast);
      WIN_USE_CTRL(IDC_DISABLED, m_SigningDisabled);
   }
   return NULL;
}

BOOL CMultiplayerCreateGameDlg::Create(C2DDraw *pDisplay, const CPoint& p)
{
   LoadDialog(IDD_MULTIPLAYER_CREATE,pDisplay);
   if(!CWinDialog::Create(pDisplay,NULL,p,GetSize(),GetText()))
      return FALSE;
   return TRUE;
}

void CMultiplayerCreateGameDlg::OnButtonPressed(int ButtonID)
{
   switch(ButtonID)
   {
   case IDC_SIGNING_FULL:
      m_eSigningType = SIGN_FULL;
      m_SigningFast.SetChecked(FALSE);
      m_SigningDisabled.SetChecked(FALSE);
      break;
   case IDC_SIGNING_FAST:
      m_eSigningType = SIGN_FAST;
      m_SigningFull.SetChecked(FALSE);
      m_SigningDisabled.SetChecked(FALSE);
      break;
   case IDC_DISABLED:
      m_eSigningType = SIGN_NONE;
      m_SigningFull.SetChecked(FALSE);
      m_SigningFast.SetChecked(FALSE);
      break;
   }
}
