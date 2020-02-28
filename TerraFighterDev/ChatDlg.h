// ChatDlg.h: interface for the CChatDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATDLG_H__2326F622_E3D7_4164_A0B8_0DE5739FE9C4__INCLUDED_)
#define AFX_CHATDLG_H__2326F622_E3D7_4164_A0B8_0DE5739FE9C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinEditBox.h"
#include "WinListBox.h"
#include "NetworkConnection.h"

class CChatDlg : public CWinDialog  
{
public:
   CChatDlg();
   virtual ~CChatDlg();
   BOOL Create(C2DDraw *pDisplay, const CPoint& p);
   BOOL ValidButtonID(int Id);
   CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);
   void OnButtonPressed(int ButtonID);

   void SetNetworkConnection(CNetworkConnection *pNetConnect){m_pNetConnect = pNetConnect;}

   void Reset(){m_List.ResetContent();}

   void AddString(const TCHAR* str,D3DCOLOR Color);

private:
   CNetworkConnection *m_pNetConnect;

   CWinListBox m_List;
   CWinEditBox m_Edit;

};

#endif // !defined(AFX_CHATDLG_H__2326F622_E3D7_4164_A0B8_0DE5739FE9C4__INCLUDED_)
