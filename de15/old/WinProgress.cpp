// WinProgress.cpp: implementation of the CWinProgress class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "WinProgress.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinProgress::CWinProgress() : CWinBaceObj()
{
	m_h = 0;
}

CWinProgress::~CWinProgress()
{

}

void CWinProgress::Display(CPlayer *pPlayer, CDisplay *pDisplay)
{
	CRect rect = GetDrawRect();
	CRect temprect = rect;
	pDisplay->DrawFilledRect(rect,m_BackgroundColor);

	temprect.top += GetSize().cy - m_h;
	pDisplay->DrawFilledRect(temprect,m_color);
	
	pDisplay->DrawLine(CPoint(rect.left+1,rect.top+1),CPoint(rect.left+1,rect.bottom),RGB(128,128,128));
	pDisplay->DrawLine(CPoint(rect.left+1,rect.top+1),CPoint(rect.right,rect.top+1),RGB(128,128,128));
	pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.left-1,rect.bottom),RGB(255,255,255));
	pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.right,rect.top-1),RGB(255,255,255));
	pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.left,rect.bottom+1),RGB(0,0,0));
	pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.right+1,rect.top),RGB(0,0,0));

	CWinBaceObj::Display(pPlayer,pDisplay);
}

void CWinProgress::SetProgress(int progress)
{
	 m_h = (double(progress)/100.0)*double(GetSize().cy);
}

void CWinProgress::SetColor(COLORREF color)
{
	m_color = color;
}

void CWinProgress::OnMouseOver(BOOL bIsOver, CPoint p)
{
	m_ToolTip.Show(bIsOver);
}

BOOL CWinProgress::Create(CDisplay *pDisplay, CWinBaceObj *pParent, CPoint p, CSize s, CString str)
{
	if(!CWinBaceObj::Create(pDisplay,pParent,p,s,str))
		return FALSE;
	m_ToolTip.Create(pDisplay,this,CPoint(1,1),CSize(0,0),str);
	m_ToolTip.Show(FALSE);
	return TRUE;
}

void CWinProgress::SetToolTipText(CString s)
{
	m_ToolTip.SetText(s);
}
