// WinBitmapButton.h: interface for the CWinBitmapButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINBITMAPBUTTON_H__9CEE652E_4D35_4421_A90D_3E345609BA8C__INCLUDED_)
#define AFX_WINBITMAPBUTTON_H__9CEE652E_4D35_4421_A90D_3E345609BA8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinButton.h"
#include "WinToolTip.h"	// Added by ClassView

class CWinBitmapButton : public CWinButton  
{
public:
	CWinToolTip m_ToolTip;
	BOOL Create(CDisplay *pDisplay, CWinBaceObj *pParent, CPoint p, CSize s, CString str);
	
	BOOL LoadBitmap(CSurface* bmp);
	BOOL LoadBitmap(CString file,CDisplay* pDisplay);
	virtual void Display(CPlayer *pPlayer, CDisplay *pDisplay);
	CWinBitmapButton();
	virtual ~CWinBitmapButton();

protected:
	BOOL m_bOwnBmp;
	CSurface* m_bmp;
};

#endif // !defined(AFX_WINBITMAPBUTTON_H__9CEE652E_4D35_4421_A90D_3E345609BA8C__INCLUDED_)
