// WinButton.h: interface for the CWinButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINBUTTON_H__6EF0C9B3_4B81_4F54_A63A_8ABD2C8D9054__INCLUDED_)
#define AFX_WINBUTTON_H__6EF0C9B3_4B81_4F54_A63A_8ABD2C8D9054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"

class CWinButton  : public CWinBaceObj
{
public:
	BOOL m_isMouseOver;
	virtual int OnMouseUp();
	virtual void OnMouseOver(BOOL bIsOver, CPoint p);
	BOOL m_bPressed;
	virtual void Display(CPlayer *pPlayer, CDisplay *pDisplay);
	virtual int OnMouseDown();
	void SetButtonResponce(int r);
	CWinButton();
	virtual ~CWinButton();
protected:
	int m_Responce;

};

#endif // !defined(AFX_WINBUTTON_H__6EF0C9B3_4B81_4F54_A63A_8ABD2C8D9054__INCLUDED_)
