// DEWndowMgr.h: interface for the CDEWndowMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEWNDOWMGR_H__0E236C4D_D8EE_4FA3_9E79_59486CC6BC2A__INCLUDED_)
#define AFX_DEWNDOWMGR_H__0E236C4D_D8EE_4FA3_9E79_59486CC6BC2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WinBaceObj.h"



class CDEWndowMgr
{
public:
	void AddToolTip(CWinBaceObj* pTip);
	virtual BOOL MouseOver(CPoint p);
	virtual BOOL MouseUp(CPoint p,int& key);
	void AddChild(CWinBaceObj* pWnd);
	void RemoveChild(CWinBaceObj* pWin);
	virtual BOOL MouseDown(CPoint p, int& key);
	virtual void Display(CPlayer *pPlayer, CDisplay *pDisplay);

	CDEWndowMgr();
	virtual ~CDEWndowMgr();
private:
	CList <CWinBaceObj*,CWinBaceObj*> m_Childen;
	CList <CWinBaceObj*,CWinBaceObj*> m_ToolTips;

};

CDEWndowMgr* GetWindowMgr();

#endif // !defined(AFX_DEWNDOWMGR_H__0E236C4D_D8EE_4FA3_9E79_59486CC6BC2A__INCLUDED_)
