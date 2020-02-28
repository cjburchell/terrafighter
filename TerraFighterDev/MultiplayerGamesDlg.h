// MultiplayerGamesDlg.h: interface for the CMultiplayerGamesDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIPLAYERGAMESDLG_H__D4A071EA_7BB9_4A21_8ADF_E01C6B9197B5__INCLUDED_)
#define AFX_MULTIPLAYERGAMESDLG_H__D4A071EA_7BB9_4A21_8ADF_E01C6B9197B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinEditBox.h"
#include "WinListBox.h"
#include "MultiplayerCreateGameDlg.h"

class CMultiplayerGamesDlg : public CWinDialog  
{
public:
   virtual void OnDistroy();
   CMultiplayerGamesDlg();
   virtual ~CMultiplayerGamesDlg();

   CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);
   BOOL ValidButtonID(int Id);

   BOOL Create(C2DDraw *pDisplay, const CPoint& p, IDirectPlay8Peer* pDP);

   void OnButtonPressed(int ButtonID);
   int DoIdle();

   // added input/outputs



   CString		m_strLocalPlayerName;
   BOOL		m_bHostPlayer;		
   DWORD		m_dwPort;
   GUID        m_guidApp;
   GUID        m_guidSP;
   DWORD       m_dwEnumHostExpireInterval;
   CString     m_strSessionName;
   DWORD       m_dwMaxPlayers;
   BOOL        m_bMigrateHost;
   HRESULT     m_hrConnectComplete;



   // thees funtions are run sometimes on an other thread
   HRESULT NoteEnumResponse( PDPNMSG_ENUM_HOSTS_RESPONSE pEnumHostsResponseMsg );
   DPNHANDLE               m_hEnumAsyncOp;
   VOID FinalizeEnumHosts();
   HANDLE                  m_hConnectCompleteEvent;
   DPNHANDLE               m_hConnectAsyncOp;

private:

   CRITICAL_SECTION        m_csHostEnum;
   BOOL					m_bConnecting;

   CString		m_strHostname;

   BOOL                    m_bSearchingForSessions;
   BOOL                    m_bUseDPNSVR;
   SigningType             m_eSigningType;


   struct DPHostEnumInfo
   {
      DWORD                 dwRef;
      DPN_APPLICATION_DESC* pAppDesc;
      IDirectPlay8Address* pHostAddr;
      IDirectPlay8Address* pDeviceAddr;
      TCHAR                szSession[MAX_PATH];
      DWORD                dwLastPollTime;
      BOOL                 bValid;
      DPHostEnumInfo*      pNext;
   };

   DPHostEnumInfo          m_DPHostEnumHead;

   VOID StopEnumHosts();
   HRESULT EnumHosts();
   VOID InitListbox();

   HRESULT JoinGame();
   HRESULT CreateGame();
   VOID EnumListCleanup();
   VOID ExpireOldHostEnums();
   HRESULT DisplayEnumList();
   BOOL m_bEnumListChanged;

   BOOL m_bUpdateHostsTimer;
   BOOL m_ConnectCompleteTimer;

   CWinListBox m_GameList;

   IDirectPlay8Peer* m_pDP;


};

#endif // !defined(AFX_MULTIPLAYERGAMESDLG_H__D4A071EA_7BB9_4A21_8ADF_E01C6B9197B5__INCLUDED_)
