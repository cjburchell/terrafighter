// SayDlg.cpp: implementation of the CSayDlg class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "SayDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSayDlg::CSayDlg()
{

}

CSayDlg::~CSayDlg()
{

}

BOOL CSayDlg::Create(CDisplay *pDisplay, CPoint p, CString text)
{
	ShowTitleBar(FALSE);
	CRect r = pDisplay->GetTextRect(text);
	m_textctrl.Create(pDisplay,this,CPoint(5,5),r.Size(),text);

	CSize s = r.Size();

	s.cx+=10;
	s.cx = max(s.cx,70);
	s.cy+=50;

	CPoint okpos(5,r.Size().cy+20);

	m_ok.Create(pDisplay,this,okpos,CSize(50,20),"OK");
	m_ok.SetButtonResponce(VK_RETURN);

	return CWinDialog::Create(pDisplay,NULL,p,s,"Say");
}

BOOL CSayDlg::RegKeyPress(int key)
{
	return key == VK_RETURN;
}

void CSayDlg::SetText(CString str)
{
	m_textctrl.SetText(str);
}
