// BuySellDlg.cpp: implementation of the CBuySellDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "BuySellDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuySellDlg::CBuySellDlg():CYesNoDlg()
{

}

CBuySellDlg::~CBuySellDlg()
{

}

BOOL CBuySellDlg::Create(C2DDraw *pDisplay, CPoint p, CString text)
{
	if(!CYesNoDlg::Create(pDisplay,p,text))
		return FALSE;
	m_yes.SetText("Buy");
	m_no.SetText("Sell");
	return TRUE;
}
