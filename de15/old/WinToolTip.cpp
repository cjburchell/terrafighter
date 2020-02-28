// WinToolTip.cpp: implementation of the CWinToolTip class.
//
//////////////////////////////////////////////////////////////////////

#include <afxwin.h>
#include "player.h"
#include "WinToolTip.h"
#include <D3d8types.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinToolTip::CWinToolTip() : CWinBaceObj()
{
	m_bShow = TRUE;
	m_pBkgndSurface = NULL;
}

CWinToolTip::~CWinToolTip()
{
	if(m_pBkgndSurface)
		delete m_pBkgndSurface;
	GetWindowMgr()->RemoveChild(this);
}

BOOL CWinToolTip::MouseOver(CPoint p)
{
	return FALSE;
}

BOOL CWinToolTip::MouseDown(CPoint p, int &key)
{
	return FALSE;
}


void CWinToolTip::Display(CPlayer *pPlayer, CDisplay *pDisplay)
{
	if(m_bShow)
	{
		CRect r = pDisplay->GetTextRect(GetText());
		r.OffsetRect(GetDrawPoint());
		//if(m_pBkgndSurface)
		//{
			pDisplay->DrawBlendRect(r,0x7f000000);
			//pDisplay->Blt(GetDrawPoint().x,GetDrawPoint().y,m_pBkgndSurface,&r);
		//}
		pDisplay->OutTextc(m_BackgroundColor,RGB(255,255,255),GetDrawPoint().x,GetDrawPoint().y,TRUE,"%s",GetText());
		CWinBaceObj::Display(pPlayer,pDisplay);
	}
}

BOOL CWinToolTip::MouseUp(CPoint p, int &key)
{
	return FALSE;
}

void CWinToolTip::Show(BOOL bShow)
{
	m_bShow = bShow;
}

BOOL CWinToolTip::Create(CDisplay *pDisplay, CWinBaceObj *pParent, CPoint p, CSize s, CString str)
{
	//if(FAILED(pDisplay->CreateSurfaceFromBitmap(&m_pBkgndSurface,"blend2.bmp",200,100)))
	//{
		//return FALSE;
	//}
	//m_pBkgndSurface->SetColorKey(RGB(255,0,255));
	if(!CWinBaceObj::Create(pDisplay,pParent,p,s,str))
		return FALSE;
	pParent->RemoveChild(this);
	
	m_pParent = pParent;

	GetWindowMgr()->AddToolTip(this);
	return TRUE;
}
