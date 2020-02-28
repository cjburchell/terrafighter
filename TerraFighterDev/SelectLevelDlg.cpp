// SelectLevelDlg.cpp: implementation of the CSelectLevelDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectLevelDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectLevelDlg::CSelectLevelDlg(CLevelData* pMaps,DWORD MaxMaps)
{
   m_pLevel = pMaps;
   m_MaxMaps = MaxMaps;
   m_SelMap = 0;
}

CSelectLevelDlg::~CSelectLevelDlg()
{

}

BOOL CSelectLevelDlg::ValidButtonID(int Id)
{
   if(Id == IDOK || Id == IDCANCEL)
      return TRUE;
   else
      return FALSE;
}

CWinBaceObj* CSelectLevelDlg::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   switch(id)
   {
      WIN_USE_CTRL(IDC_NEXT,m_next);
      WIN_USE_CTRL(IDC_PREV,m_prev);

      WIN_USE_CTRL(IDC_LEVEL_NAME,m_LevelName);
      WIN_USE_CTRL(IDC_DISCRIPTION,m_Discription);


      WIN_USE_CTRL(IDC_BITMAP,m_bmp);
   }
   return NULL;
}

BOOL CSelectLevelDlg::Create(C2DDraw *pDisplay, const CPoint& p)
{
   CPoint newpoint(pDisplay->GetScreenWidth()/2,p.y);
   newpoint.Offset(-200,0);
   LoadDialog(IDD_SELECT_LEVEL,pDisplay);

   m_next.SetType(WINARROW_RIGHT);
   m_prev.SetType(WINARROW_LEFT);



   if(!CWinDialog::Create(pDisplay,NULL,newpoint,GetSize(),GetText()))
      return FALSE;

   UpdateControls();
   return TRUE;
}

void CSelectLevelDlg::UpdateControls()
{
   m_bmp.SetBitmap(m_pLevel[m_SelMap].m_MapFile, GetDisplay()->GetTempDevice(),&GetApp()->m_Zip);
   m_LevelName.SetText(m_pLevel[m_SelMap].m_Name);
   m_Discription.SetText(m_pLevel[m_SelMap].m_Discription);
}

void CSelectLevelDlg::OnButtonPressed(int ButtonID)
{
   switch(ButtonID)
   {
   case IDC_NEXT:
      {
         m_SelMap++;
         if(m_SelMap>m_MaxMaps-1)
            m_SelMap = m_MaxMaps-1;
         UpdateControls();
      }
      break;
   case IDC_PREV:
      {
         m_SelMap--;
         if(m_SelMap<0)
            m_SelMap = 0;
         UpdateControls();
      }
      break;
   }
}
