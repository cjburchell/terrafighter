// AddressDlg.h: interface for the CAddressDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDRESSDLG_H__CBC69173_4CAA_48F1_B537_A32544B37A7F__INCLUDED_)
#define AFX_ADDRESSDLG_H__CBC69173_4CAA_48F1_B537_A32544B37A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinEditBox.h"

class CAddressDlg : public CWinDialog  
{
public:
   CAddressDlg();
   virtual ~CAddressDlg();

   CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);
   BOOL ValidButtonID(int Id);
   BOOL Create(C2DDraw *pDisplay, const CPoint& p);

   CWinEditBox m_Name;
   CWinEditBox m_Port;

};

#endif // !defined(AFX_ADDRESSDLG_H__CBC69173_4CAA_48F1_B537_A32544B37A7F__INCLUDED_)
