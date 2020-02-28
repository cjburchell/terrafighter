// YesNoDlg.cpp: implementation of the CYesNoDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "YesNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYesNoDlg::CYesNoDlg() : CWinDialog()
{

}

CYesNoDlg::~CYesNoDlg()
{

}

BOOL CYesNoDlg::Create(C2DDraw *pDisplay, CPoint p, CString text)
{
   ShowTitleBar(FALSE);
   CRect r = pDisplay->GetTextRect(text);
   m_textctrl.Create(pDisplay,this,CPoint(5,5),r.Size(),text);

   CSize s = r.Size();

   s.cx+=10;
   s.cx = max(s.cx,115);
   s.cy+=50;

   CPoint yespos(5,r.Size().cy+20);
   m_yes.Create(pDisplay,this,yespos,CSize(50,20),"Yes");
   m_yes.SetID(IDYES);

   CPoint nopos(60,r.Size().cy+20);
   m_no.Create(pDisplay,this,nopos,CSize(50,20),"No");
   m_no.SetID(IDNO);

   return CWinDialog::Create(pDisplay,NULL,p,s,"Question");
}

BOOL CYesNoDlg::ValidButtonID(int Id)
{
   return Id == IDYES || Id == IDNO?TRUE:FALSE;
}
