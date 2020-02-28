// BuySellDlg.h: interface for the CBuySellDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUYSELLDLG_H__06BE9281_A067_4F6D_AD92_7A31593F15FD__INCLUDED_)
#define AFX_BUYSELLDLG_H__06BE9281_A067_4F6D_AD92_7A31593F15FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "YesNoDlg.h"

class CBuySellDlg : public CYesNoDlg  
{
public:
   virtual BOOL Create(C2DDraw *pDisplay, CPoint p, CString text);
	CBuySellDlg();
	virtual ~CBuySellDlg();

};

#endif // !defined(AFX_BUYSELLDLG_H__06BE9281_A067_4F6D_AD92_7A31593F15FD__INCLUDED_)
