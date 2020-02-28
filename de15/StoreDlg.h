// StoreDlg.h: interface for the CStoreDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOREDLG_H__767FCF4F_B99A_44DC_8687_BD817919268E__INCLUDED_)
#define AFX_STOREDLG_H__767FCF4F_B99A_44DC_8687_BD817919268E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinBitmapButton.h"
#include "WinStatic.h"
#include "item.h"

class CStoreDlg : public CWinDialog  
{
public:
	virtual BOOL RegKeyPress(int key);
   virtual BOOL Create(C2DDraw *pDisplay, CPoint p, CItem** pItems,int count);
	CStoreDlg();
	virtual ~CStoreDlg();
private:
	CWinBitmapButton* m_pbuttons;
	CWinStatic* m_pStatic;
	CWinButton m_close;


};

#endif // !defined(AFX_STOREDLG_H__767FCF4F_B99A_44DC_8687_BD817919268E__INCLUDED_)
