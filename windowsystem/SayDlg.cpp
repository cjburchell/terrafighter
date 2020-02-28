// SayDlg.cpp: implementation of the CSayDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <SayDlg.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSayDlg::CSayDlg()
{

}

CSayDlg::~CSayDlg()
{

}

BOOL CSayDlg::Create(C2DDraw *pDisplay, const CPoint& p, const TCHAR* text)
{
   ShowTitleBar(FALSE);
   CRect r = pDisplay->GetTextRect(m_Text);
   m_textctrl.Create(pDisplay,this,CPoint(5,5),r.Size(),m_Text);

   CSize s = r.Size();

   s.cx+=10;
   s.cx = max(s.cx,120);
   s.cy+=50;

   CPoint okpos(5,r.Size().cy+20);

   m_ok.Create(pDisplay,this,okpos,CSize(50,20),"OK");
   m_ok.SetID(IDOK);
   ShowTitleBar(TRUE);

   return CWinDialog::Create(pDisplay,NULL,p,s,text);
}

BOOL CSayDlg::ValidButtonID(int Id)
{
   return Id == IDOK;
}

void CSayDlg::SetText(const TCHAR* str)
{
   m_textctrl.SetText(str);
   m_Text = str;
}
