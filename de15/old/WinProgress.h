// WinProgress.h: interface for the CWinProgress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINPROGRESS_H__6C7225D3_07C9_4FE2_88C6_E89F258A6151__INCLUDED_)
#define AFX_WINPROGRESS_H__6C7225D3_07C9_4FE2_88C6_E89F258A6151__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"
#include "WinToolTip.h"

class CWinProgress : public CWinBaceObj
{
public:
	void SetToolTipText(CString s);
	BOOL Create(CDisplay *pDisplay, CWinBaceObj *pParent,CPoint p, CSize s, CString str);
	virtual void OnMouseOver(BOOL bIsOver, CPoint p);
	COLORREF m_color;
	void SetColor(COLORREF color);
	int m_h;
	void SetProgress(int progress);
	virtual void Display(CPlayer *pPlayer, CDisplay *pDisplay);
	CWinProgress();
	virtual ~CWinProgress();
protected:
	CWinToolTip m_ToolTip;

};

#endif // !defined(AFX_WINPROGRESS_H__6C7225D3_07C9_4FE2_88C6_E89F258A6151__INCLUDED_)
