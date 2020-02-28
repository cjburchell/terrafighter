// DEWndowMgr.cpp: implementation of the CDEWndowMgr class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "DEWndowMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDEWndowMgr::CDEWndowMgr()
{

}

CDEWndowMgr::~CDEWndowMgr()
{
	
}

void CDEWndowMgr::Display(CPlayer *pPlayer, CDisplay *pDisplay)
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

	pos = m_ToolTips.GetHeadPosition();
	while(pos)
	{
		CWinBaceObj* pWin = m_ToolTips.GetNext(pos);
		if(pWin)
		{
			pWin->Display(pPlayer,pDisplay);
		}
	}
}

void CDEWndowMgr::AddChild(CWinBaceObj *pWnd)
{
	m_Childen.AddHead(pWnd);
}

void CDEWndowMgr::RemoveChild(CWinBaceObj *pRemoveWin)
{
	POSITION pos = m_Childen.GetHeadPosition();
	while(pos)
	{
		POSITION currentpos = pos;
		CWinBaceObj* pWin = m_Childen.GetNext(pos);
		if(pWin && pWin == pRemoveWin)
		{
			m_Childen.RemoveAt(currentpos);
			return;
		}
	}

	pos = m_ToolTips.GetHeadPosition();
	while(pos)
	{
		POSITION currentpos = pos;
		CWinBaceObj* pWin = m_ToolTips.GetNext(pos);
		if(pWin && pWin == pRemoveWin)
		{
			m_ToolTips.RemoveAt(currentpos);
			return;
		}
	}
}

BOOL CDEWndowMgr::MouseDown(CPoint p, int &key)
{
	BOOL isHanded = FALSE;
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
	return isHanded;
}

BOOL CDEWndowMgr::MouseUp(CPoint p, int &key)
{
	BOOL isHanded = FALSE;
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
	return isHanded;
}

BOOL CDEWndowMgr::MouseOver(CPoint p)
{
	BOOL isHanded = FALSE;
	POSITION pos = m_Childen.GetHeadPosition();
	while(pos)
	{
		CWinBaceObj* pWin = m_Childen.GetNext(pos);
		if(pWin)
		{
			isHanded = pWin->MouseOver(p);
		}
	}
	return isHanded;
}

void CDEWndowMgr::AddToolTip(CWinBaceObj *pTip)
{
	m_ToolTips.AddHead(pTip);
}
