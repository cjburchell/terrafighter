// WinDialog.cpp: implementation of the CWinDialog class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "WinDialog.h"

#define BORDER RGB(0,0,0)

#define TITLE_BAR_FONT_OFFSET 3
#define TITLE_BAR_SIZE (FONT_SIZE_Y + TITLE_BAR_FONT_OFFSET + TITLE_BAR_FONT_OFFSET) 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinDialog::CWinDialog() :CWinBaceObj()
{
	m_bTitleBar = TRUE;
	m_bDrag = FALSE;
	m_bCanMove = TRUE;
}

CWinDialog::~CWinDialog()
{

}

BOOL CWinDialog::Create(CDisplay *pDisplay, CWinBaceObj *pParent,CPoint p, CSize s, CString str)
{
	if(!CWinBaceObj::Create(pDisplay, pParent, p,  s,  str))
		return FALSE;

	CRect draw = GetDrawRect();
	m_BarRec = CRect(CPoint(0,-TITLE_BAR_SIZE),CSize(draw.Size().cx,TITLE_BAR_SIZE));
	return TRUE;
}


int CWinDialog::DoModal(CInput2* pInput,CGameMap* pMap)
{
	int key = -1;
	pMap->GetWndowMgr()->AddChild(this);
	pInput->ClearBuffer();
	BOOL bInputFound = FALSE;
	while(!bInputFound)
	{
		pMap->pDirDraw->Clear(RGB(0,0,0));
		pMap->pDirDraw->StartRender();
		pMap->setscr();
		pMap->pDirDraw->Present();
		if(!LookForMSG())
			return -1;
		while(!pInput->IsEmpty())
		{
			key  = pInput->RemoveKeyPress();
			if(key == VK_ESCAPE)
			{
				bInputFound = FALSE;
			}
			else
				bInputFound = RegKeyPress(key);
		}
	}
	pMap->GetWndowMgr()->RemoveChild(this);
	pMap->ResetTime();
	return key;
}

CRect CWinDialog::GetBarRect()
{
	CRect BarRec = m_BarRec;
	BarRec.OffsetRect(GetDrawPoint());
	return BarRec;
}

void CWinDialog::Display(CPlayer *pPlayer, CDisplay *pDisplay)
{
	CRect draw = GetDrawRect();
	if(m_bTitleBar)
	{
		CRect BarRec = GetBarRect();
		pDisplay->DrawGradientRect(BarRec,RGB(0,255,0),m_BackgroundColor,GR_BR|GR_TR);
		pDisplay->DrawRect(BarRec,RGB(0,0,0));
		pDisplay->OutTextc(RGB(0,255,0),m_ForgoundColor,BarRec.TopLeft().x+TITLE_BAR_FONT_OFFSET,BarRec.TopLeft().y+TITLE_BAR_FONT_OFFSET,FALSE,"%s",GetText());

	}
	pDisplay->DrawFilledRect(draw,m_BackgroundColor);
	pDisplay->DrawRect(draw,RGB(0,0,0));
	CWinBaceObj::Display(pPlayer,pDisplay);
}


BOOL CWinDialog::RegKeyPress(int key)
{
	return TRUE;
}

void CWinDialog::ShowTitleBar(BOOL show)
{
	m_bTitleBar = show;
}

BOOL CWinDialog::MouseOver(CPoint p)
{
	if(m_bDrag)
	{
		CPoint diff = p - m_DragStart;
		m_DragStart = p;
		CPoint newpos = GetDrawPoint();
		newpos.Offset(diff);
		SetPos(newpos);
		return TRUE;
	}
	else
		return CWinBaceObj::MouseOver(p);
}
BOOL CWinDialog::MouseUp(CPoint p,int& Id)
{
	m_bDrag = FALSE;
	return CWinBaceObj::MouseUp(p,Id);
}

BOOL CWinDialog::MouseDown(CPoint p, int& Id)
{
	if(GetBarRect().PtInRect(p) && m_bTitleBar && m_bCanMove)
	{
		m_bDrag = TRUE;
		m_DragStart = p;
		Id = VK_ESCAPE;
		return TRUE;
	}
	else
		return CWinBaceObj::MouseDown(p,Id);
}
