// AddressDlg.cpp: implementation of the CAddressDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AddressDlg.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CAddressDlg::CAddressDlg()
{

}

CAddressDlg::~CAddressDlg()
{

}

BOOL CAddressDlg::ValidButtonID(int Id)
{
   return (Id == IDOK || Id == IDCANCEL);
}

BOOL CAddressDlg::Create(C2DDraw *pDisplay, const CPoint& p)
{
   LoadDialog(IDD_MULTIPLAYER_ADDRESS,pDisplay);
   if(!CWinDialog::Create(pDisplay,NULL,p,GetSize(),GetText()))
      return FALSE;
   return TRUE;
}

CWinBaceObj* CAddressDlg::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   switch(id)
   {
      WIN_USE_CTRL(IDC_REMOTE_HOSTNAME, m_Name);
      WIN_USE_CTRL(IDC_REMOTE_PORT, m_Port);
   }
   return NULL;
}

