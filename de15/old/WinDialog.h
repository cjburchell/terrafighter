// WinDialog.h: interface for the CWinDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDIALOG_H__3D7FDA13_4F13_4759_A94A_63B97242EA3F__INCLUDED_)
#define AFX_WINDIALOG_H__3D7FDA13_4F13_4759_A94A_63B97242EA3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"

class CWinDialog : public CWinBaceObj  
{
public:
	void ShowTitleBar(BOOL show = TRUE);
	virtual BOOL RegKeyPress(int key);
	virtual int DoModal(CInput2* pInput,CGameMap* pMap);
	virtual void Display(CPlayer *pPlayer, CDisplay *pDisplay);
	CWinDialog();
	virtual ~CWinDialog();
	virtual BOOL MouseOver(CPoint p);
	virtual BOOL MouseUp(CPoint p,int& Id);
	virtual BOOL MouseDown(CPoint p, int& Id);
	virtual BOOL Create(CDisplay *pDisplay, CWinBaceObj *pParent,CPoint p, CSize s, CString str);
	CRect GetBarRect();

protected:
	BOOL m_bTitleBar;
	BOOL m_bDrag;
	CPoint m_DragStart;
	CRect m_BarRec;
	BOOL m_bCanMove;
};

#endif // !defined(AFX_WINDIALOG_H__3D7FDA13_4F13_4759_A94A_63B97242EA3F__INCLUDED_)
