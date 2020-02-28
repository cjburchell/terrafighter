// SayDlg.h: interface for the CSayDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAYDLG_H__6D0C8660_470C_4E51_A08E_C7E42178CF02__INCLUDED_)
#define AFX_SAYDLG_H__6D0C8660_470C_4E51_A08E_C7E42178CF02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinButton.h"
#include "WinStatic.h"



class CSayDlg : public CWinDialog  
{
public:
   void SetText(const TCHAR* str);
   BOOL ValidButtonID(int Id);
   virtual BOOL Create(C2DDraw *pDisplay, const CPoint& p, const TCHAR* str);
   CSayDlg();
   virtual ~CSayDlg();
protected:
   CWinButton m_ok;
   CWinStatic m_textctrl;
   CString m_Text;
};

inline int WinMessageBox(const TCHAR* Text, const TCHAR* Title , C2DDraw *pDisplay,CInput* pInput,CD3DApplication* pApp, CWindowMgr* pMgr, CWinBaceObj* pParent = NULL)
{
   CSayDlg dlg;
   dlg.SetText(Text);
   dlg.Create(pDisplay,CPoint(10,10),Title);
   return dlg.DoModal(pInput,pApp, pMgr, pParent);
}

#endif // !defined(AFX_SAYDLG_H__6D0C8660_470C_4E51_A08E_C7E42178CF02__INCLUDED_)
