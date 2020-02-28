// MultiplayerConnectDlg.h: interface for the CMultiplayerConnectDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIPLAYERCONNECTDLG_H__67C49A71_BD38_4F94_8D76_AD729B1BF552__INCLUDED_)
#define AFX_MULTIPLAYERCONNECTDLG_H__67C49A71_BD38_4F94_8D76_AD729B1BF552__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinEditBox.h"
#include "WinListBox.h"
#include "NetConnect.h"

class CMultiplayerConnectDlg : public CWinDialog  
{
public:
   CMultiplayerConnectDlg();
   virtual ~CMultiplayerConnectDlg();
   CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);
   BOOL ValidButtonID(int Id);

   BOOL Create(C2DDraw *pDisplay, const CPoint& p, IDirectPlay8Peer* pDP);

   CString m_Name;
   CString m_strPreferredProvider;
   CWinEditBox m_NameCtrl;
   CWinListBox m_ConnectionList;

private:

   HRESULT FillListBox(IDirectPlay8Peer* pDP);


};

#endif // !defined(AFX_MULTIPLAYERCONNECTDLG_H__67C49A71_BD38_4F94_8D76_AD729B1BF552__INCLUDED_)
