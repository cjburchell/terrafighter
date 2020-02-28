// WinBitmapButton.cpp: implementation of the CWinBitmapButton class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "WinBitmapButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinBitmapButton::CWinBitmapButton() : CWinButton()
{
	m_bOwnBmp=FALSE;
	m_bmp = NULL;
}

CWinBitmapButton::~CWinBitmapButton()
{
	if(m_bOwnBmp && m_bmp)
		delete m_bmp;
}

void CWinBitmapButton::Display(CPlayer *pPlayer, CDisplay *pDisplay)
{
	m_ToolTip.Show(m_isMouseOver);
	CRect rect = GetDrawRect();
	if(m_bmp)
	{
		CPoint scr = rect.TopLeft();
		CRect r = GetRect();
		pDisplay->Blt(scr.x,scr.y,m_bmp,&r);
	}
	else
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
	CWinBaceObj::Display(pPlayer,pDisplay);
}

BOOL CWinBitmapButton::LoadBitmap(CString file,CDisplay* pDisplay)
{
	if(m_bOwnBmp && m_bmp)
		delete m_bmp;

	CSize size = GetSize();
	if(FAILED(pDisplay->CreateSurfaceFromBitmap(&(m_bmp),file,size.cx,size.cy)))
	{
		  pDisplay->FlipToGDISurface();
		  CString str;
		  str.Format("uable to open %s",file);
		  MessageBox(0,str,"ERROR",MB_ICONERROR|MB_OK);
		  return FALSE;
	}
	if(m_bmp!=NULL)
	{
		m_bmp->SetColorKey(RGB(255,0,255));
	}
	m_bOwnBmp = TRUE;
	return TRUE;
}

BOOL CWinBitmapButton::LoadBitmap(CSurface *bmp)
{
	if(m_bOwnBmp && m_bmp)
		delete m_bmp;
	m_bOwnBmp = FALSE;
	m_bmp = bmp;
	return TRUE;
}

BOOL CWinBitmapButton::Create(CDisplay *pDisplay, CWinBaceObj *pParent, CPoint p, CSize s, CString str)
{
	if(!CWinButton::Create(pDisplay,pParent,p,s,str))
		return FALSE;
	m_ToolTip.Create(pDisplay,this,CPoint(0,0),CSize(0,0),str);
	m_ToolTip.Show(FALSE);
	return TRUE;
}
