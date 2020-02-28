// WinBaceObj.cpp: implementation of the CWinBaceObj class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "WinBaceObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinBaceObj::CWinBaceObj()
{
	m_pParent = NULL;
	m_text = "";
	m_bCreated = FALSE;
	m_ForgoundColor = RGB(0,0,0);
	m_BackgroundColor = RGB(192,192,192);
	m_MouseWasOver = FALSE;
}

CWinBaceObj::~CWinBaceObj()
{

}

void CWinBaceObj::Display(CPlayer *pPlayer, CDisplay *pDisplay)
{
	POSITION pos = m_Childen.GetHeadPosition();
	while(pos)
	{
		CWinBaceObj* pWin = m_Childen.GetNext(pos);
		if(pWin)
		{
			pWin->Display(pPlayer,pDisplay);
		}
	}
}

void CWinBaceObj::AddChild(CWinBaceObj *pWnd)
{
	m_Childen.AddHead(pWnd);
}

BOOL CWinBaceObj::Create(CDisplay *pDisplay, CWinBaceObj *pParent,CPoint p, CSize s, CString str)
{
	m_bCreated = TRUE;
	m_pParent = pParent;
	m_size = s;
	m_pos = p;
	m_text = str;
	if(pParent == NULL && p == CPoint(10,10))
	{
		// center the dialog;
		CPoint newpoint; 
		CRect r = GetRect();
		CRect scrrect(CPoint(0,0),CSize(pDisplay->m_dwWidth,pDisplay->m_dwHeight));
		newpoint = scrrect.CenterPoint() - r.CenterPoint();
		r.OffsetRect(newpoint);
		newpoint = r.TopLeft();
		m_pos =newpoint;
	}

	if(pParent)
	{
		pParent->AddChild(this);
	}
	return TRUE;
}

void CWinBaceObj::SetSize(CSize s)
{
	m_size = s;
}

void CWinBaceObj::SetText(CString str)
{
	m_text =str;
}

CRect CWinBaceObj::GetDrawRect()
{
	CRect r = GetRect();
	if(m_pParent)
	{
		r.OffsetRect(m_pParent->GetDrawPoint());
		r.OffsetRect(m_pos);
	}
	else
	{
		r.OffsetRect(m_pos);
	}
	return r;
}

CPoint CWinBaceObj::GetDrawPoint()
{
	CPoint p = m_pos;
	if(m_pParent)
	{
		p.Offset(m_pParent->GetDrawPoint());
	}
	return p;
}

void CWinBaceObj::SetPos(CPoint p)
{
	m_pos = p;
}

CPoint CWinBaceObj::GetPos()
{
	return m_pos;
}

void CWinBaceObj::RemoveChild(CWinBaceObj *pRemoveWin)
{
	POSITION pos = m_Childen.GetHeadPosition();
	while(pos)
	{
		POSITION currentpos = pos;
		CWinBaceObj* pWin = m_Childen.GetNext(pos);
		if(pWin && pWin == pRemoveWin)
		{
			pRemoveWin->m_pParent = NULL;
			m_Childen.RemoveAt(currentpos);
			break;
		}
	}
}

CWinBaceObj* CWinBaceObj::GetParent()
{
	return m_pParent;
}

BOOL CWinBaceObj::MouseDown(CPoint p, int &key)
{
	BOOL isHanded = FALSE;
	if(GetDrawRect().PtInRect(p))
	{
		POSITION pos = m_Childen.GetHeadPosition();
		while(pos)
		{
			CWinBaceObj* pWin = m_Childen.GetNext(pos);
			if(pWin)
			{
				isHanded = pWin->MouseDown(p,key);
				if(isHanded)
					break;
			}
		}
		if(!isHanded)
		{
			key = OnMouseDown();
			m_MouseWasOver = TRUE;
		}
		isHanded = TRUE;
	}
	return isHanded;
}

int CWinBaceObj::OnMouseDown()
{
	return VK_ESCAPE; //do nothing
}

void CWinBaceObj::SetColors(COLORREF forg, COLORREF back)
{
	m_ForgoundColor = forg;
	m_BackgroundColor = back;
}

CString CWinBaceObj::GetText()
{
	return m_text;
}

CRect CWinBaceObj::GetRect()
{
	CRect r(CPoint(0,0),m_size);
	return r;
}

void CWinBaceObj::OnMouseOver(BOOL bIsOver, CPoint p)
{

}

int CWinBaceObj::OnMouseUp()
{
	return VK_ESCAPE;
}

BOOL CWinBaceObj::MouseUp(CPoint p, int &key)
{
	BOOL isHanded = FALSE;
	if(GetDrawRect().PtInRect(p))
	{
		POSITION pos = m_Childen.GetHeadPosition();
		while(pos)
		{
			CWinBaceObj* pWin = m_Childen.GetNext(pos);
			if(pWin)
			{
				isHanded = pWin->MouseUp(p,key);
				if(isHanded)
					break;
			}
		}
		if(!isHanded)
		{
			key = OnMouseUp();
			m_MouseWasOver = TRUE;
		}
		isHanded = TRUE;
	}
	return isHanded;
}

BOOL CWinBaceObj::MouseOver(CPoint p)
{
	BOOL isHanded = FALSE;
	if(GetDrawRect().PtInRect(p))
	{
		POSITION pos = m_Childen.GetHeadPosition();
		while(pos)
		{
			CWinBaceObj* pWin = m_Childen.GetNext(pos);
			if(pWin)
			{
				isHanded = pWin->MouseOver(p);
			}
		}
		if(!isHanded)
		{
			OnMouseOver(TRUE,p);
		}
		m_MouseWasOver = TRUE;
		isHanded = TRUE;
	}
	else
	{
		if(m_MouseWasOver)
			OnMouseOver(FALSE,p);
		m_MouseWasOver = FALSE;
	}
	return isHanded;
}

CSize CWinBaceObj::GetSize()
{
	return m_size;
}
