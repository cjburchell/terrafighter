// WinButton.cpp: implementation of the CWinButton class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "WinButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinButton::CWinButton() : CWinBaceObj()
{
	m_Responce = VK_ESCAPE;
	m_bPressed = FALSE;
	m_isMouseOver = FALSE;
}

CWinButton::~CWinButton()
{

}

void CWinButton::SetButtonResponce(int r)
{
	m_Responce = r;
}

int CWinButton::OnMouseDown()
{
	m_bPressed = TRUE;
	return CWinBaceObj::OnMouseDown();
}

void CWinButton::Display(CPlayer *pPlayer, CDisplay *pDisplay)
{
	CRect rect = GetDrawRect();
	pDisplay->DrawFilledRect(rect,m_BackgroundColor);

	if(m_bPressed)
	{
		pDisplay->DrawLine(CPoint(rect.left+1,rect.top+1),CPoint(rect.left+1,rect.bottom),RGB(128,128,128));
		pDisplay->DrawLine(CPoint(rect.left+1,rect.top+1),CPoint(rect.right,rect.top+1),RGB(128,128,128));
		pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.left-1,rect.bottom),RGB(255,255,255));
		pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.right,rect.top-1),RGB(255,255,255));

		pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.left,rect.bottom+1),RGB(0,0,0));
		pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.right+1,rect.top),RGB(0,0,0));

	}
	else
	{
		pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.left,rect.bottom),RGB(255,255,255));
		pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.right,rect.top),RGB(255,255,255));


		pDisplay->DrawLine(CPoint(rect.right,rect.bottom-1),CPoint(rect.left,rect.bottom-1),RGB(128,128,128));
		pDisplay->DrawLine(CPoint(rect.right-1,rect.bottom-1),CPoint(rect.right-1,rect.top),RGB(128,128,128));

		pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.left-1,rect.bottom),RGB(0,0,0));
		pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.right,rect.top-1),RGB(0,0,0));
	}
	
	CPoint p; 
	CRect r = pDisplay->GetTextRect(GetText());
	r.OffsetRect(rect.TopLeft());
	p = rect.CenterPoint() - r.CenterPoint();
	r.OffsetRect(p);
	p = r.TopLeft();
	if(!m_isMouseOver)
		pDisplay->OutTextc(m_BackgroundColor,m_ForgoundColor,p.x,p.y, FALSE, "%s",GetText());
	else
		pDisplay->OutTextc(m_BackgroundColor,RGB(255,255,255),p.x,p.y, FALSE, "%s",GetText());
	CWinBaceObj::Display(pPlayer,pDisplay);
}

void CWinButton::OnMouseOver(BOOL bIsOver, CPoint p)
{
	m_isMouseOver = bIsOver;
	if(!bIsOver)
		m_bPressed = FALSE;
}

int CWinButton::OnMouseUp()
{
	if(m_bPressed)
	{
		m_bPressed = FALSE;
		return m_Responce;
	}
	else
		return VK_ESCAPE;
}
