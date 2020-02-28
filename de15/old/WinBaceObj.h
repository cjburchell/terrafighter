// WinBaceObj.h: interface for the CWinBaceObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINBACEOBJ_H__C28C28D9_2BF9_43C1_8B43_A02F3ED6FC7C__INCLUDED_)
#define AFX_WINBACEOBJ_H__C28C28D9_2BF9_43C1_8B43_A02F3ED6FC7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ddutil.h"
#include "types.h"
#include "player.h"

class CWinBaceObj  
{
public:
	CSize GetSize();
	virtual BOOL MouseOver(CPoint p);
	virtual BOOL MouseUp(CPoint p,int& key);
	virtual int OnMouseUp();
	virtual void OnMouseOver(BOOL bIsOver,CPoint p);
	CRect GetRect();
	CString GetText();
	void SetColors(COLORREF forg, COLORREF back);
	virtual int OnMouseDown();
	virtual BOOL MouseDown(CPoint p, int& key);
	CWinBaceObj* GetParent();
	void RemoveChild(CWinBaceObj* pWin);
	CPoint GetPos();
	void SetPos(CPoint p);
	CPoint GetDrawPoint();
	CRect GetDrawRect();
	void SetText(CString str);
	void SetSize(CSize s);
	virtual BOOL Create(CDisplay *pDisplay, CWinBaceObj* pParent,CPoint p, CSize s, CString str);
	void AddChild(CWinBaceObj* pWnd);
	virtual void Display(CPlayer *pPlayer, CDisplay *pDisplay);
	CWinBaceObj();
	virtual ~CWinBaceObj();
protected:
	COLORREF m_ForgoundColor;
	COLORREF m_BackgroundColor;
	CWinBaceObj* m_pParent;
private:
	BOOL m_MouseWasOver;
	BOOL m_bCreated;
	CString m_text;
	CPoint m_pos;
	CList <CWinBaceObj*,CWinBaceObj*> m_Childen;
	CSize m_size;
};

#endif // !defined(AFX_WINBACEOBJ_H__C28C28D9_2BF9_43C1_8B43_A02F3ED6FC7C__INCLUDED_)
