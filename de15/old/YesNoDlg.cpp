// YesNoDlg.cpp: implementation of the CYesNoDlg class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "YesNoDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYesNoDlg::CYesNoDlg() : CWinDialog()
{

}

CYesNoDlg::~CYesNoDlg()
{

}

BOOL CYesNoDlg::Create(CDisplay *pDisplay, CPoint p, CString text)
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
	m_yes.SetButtonResponce('y');

	CPoint nopos(60,r.Size().cy+20);
	m_no.Create(pDisplay,this,nopos,CSize(50,20),"No");
	m_no.SetButtonResponce('n');

	return CWinDialog::Create(pDisplay,NULL,p,s,"Question");
}

BOOL CYesNoDlg::RegKeyPress(int key)
{
	return key == 'y' || key == 'n'?TRUE:FALSE;
}
