// WinToolTip.h: interface for the CWinToolTip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINTOOLTIP_H__16294D54_ED70_4133_9A28_A496F74EDBEC__INCLUDED_)
#define AFX_WINTOOLTIP_H__16294D54_ED70_4133_9A28_A496F74EDBEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"

class CWinToolTip : public CWinBaceObj  
{
public:
	BOOL Create(CDisplay *pDisplay, CWinBaceObj *pParent,CPoint p, CSize s, CString str);
	BOOL m_bShow;
	void Show(BOOL bShow = TRUE);
	virtual BOOL MouseUp(CPoint p, int &key);
	virtual void Display(CPlayer *pPlayer, CDisplay *pDisplay);
	virtual BOOL MouseOver(CPoint p);
	virtual BOOL MouseDown(CPoint p, int &key);
	CWinToolTip();
	virtual ~CWinToolTip();
private:
	CSurface* m_pBkgndSurface;

};

#endif // !defined(AFX_WINTOOLTIP_H__16294D54_ED70_4133_9A28_A496F74EDBEC__INCLUDED_)
