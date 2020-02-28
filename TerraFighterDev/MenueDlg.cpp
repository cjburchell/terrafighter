// MenueDlg.cpp: implementation of the CMenueDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "MenueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenueDlg::CMenueDlg() : CWinDialog()
{

}

CMenueDlg::~CMenueDlg()
{

}

BOOL CMenueDlg::ValidButtonID(int Id)
{
   if(Id == IDC_NEW || Id == IDC_QUIT || Id == IDC_RESUME || Id == IDC_MULTI_PLAYER)
      return TRUE;
   else
      return FALSE;
}

BOOL CMenueDlg::Create(C2DDraw *pDisplay, CPoint p)
{    
   LoadDialog(IDD_MENU,pDisplay);
   p = CPoint(pDisplay->GetScreenWidth()/2,p.y);
   p.Offset(-GetSize().cx/2,0);


   if(!CWinDialog::Create(pDisplay,NULL,p,GetSize(),GetText()))
      return FALSE;

   return TRUE;
}
