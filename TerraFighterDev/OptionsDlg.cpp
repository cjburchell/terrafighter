// OptionsDlg.cpp: implementation of the COptionsDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionsDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionsDlg::COptionsDlg()
{

}

COptionsDlg::~COptionsDlg()
{

}

BOOL COptionsDlg::ValidButtonID(int Id)
{
   if(Id == IDOK || Id == IDC_DISPLAY)
      return TRUE;
   else
      return FALSE;
}

CWinBaceObj* COptionsDlg::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   switch(id)
   {
      WIN_USE_CTRL(IDC_CHECK_SHADOW,m_CheckShadows);
      WIN_USE_CTRL(IDC_CHECK_MULTEX,m_CheckMultiTex);
      WIN_USE_CTRL(IDC_CHECK_WATER,m_CheckWater);
      WIN_USE_CTRL(IDC_CHECK_HIGH_DEF,m_CheckLOD);
      WIN_USE_CTRL(IDC_CHECK_3D_SOUND,m_Check3DSound);
   }
   return NULL;
}

BOOL COptionsDlg::Create(C2DDraw *pDisplay, const CPoint& p)
{

   LoadDialog(IDD_OPTIONS,pDisplay);
   CPoint newpoint(pDisplay->GetScreenWidth()/2,p.y);
   newpoint.Offset(-GetSize().cx/2,0);


   if(!CWinDialog::Create(pDisplay,NULL,newpoint,GetSize(),GetText()))
      return FALSE;

   return TRUE;
}
