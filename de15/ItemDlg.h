// ItemDlg.h: interface for the CItemDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDLG_H__E5974240_39D8_48E1_8C3B_BF1C61C549CA__INCLUDED_)
#define AFX_ITEMDLG_H__E5974240_39D8_48E1_8C3B_BF1C61C549CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinBitmapButton.h"
#include "WinStatic.h"

class CItemDlg : public CWinDialog  
{
public:
	virtual BOOL RegKeyPress(int key);
	virtual BOOL Create(C2DDraw *pDisplay, CPoint p, CItem* pItems,int count);
	CItemDlg();
	virtual ~CItemDlg();
private:
	CWinBitmapButton* m_pbuttons;
	CWinStatic m_ArmorText;
	CWinStatic m_WeaponText;
	CWinStatic m_ShieldText;
	CWinStatic m_title;

	CWinButton m_close;

};

#endif // !defined(AFX_ITEMDLG_H__E5974240_39D8_48E1_8C3B_BF1C61C549CA__INCLUDED_)
