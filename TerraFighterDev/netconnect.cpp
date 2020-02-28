//-----------------------------------------------------------------------------
// File: NetConnect.cpp
//
// Desc: This is a class that given a IDirectPlay8Peer, then DoConnectWizard()
//       will enumerate service providers, enumerate hosts, and allow the
//       user to either join or host a session.  The class uses
//       dialog boxes and GDI for the interactive UI.  Most games will
//       want to change the graphics to use Direct3D or another graphics
//       layer, but this simplistic sample uses dialog boxes.  Feel 
//       free to use this class as a starting point for adding extra 
//       functionality.
//
// Copyright (c) 2000-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef STRICT
#define STRICT
#endif // !STRICT
#include "stdafx.h"
#include <basetsd.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dplay8.h>
#include <dpaddr.h>
#include <dplobby8.h>
#include "NetConnect.h"
#include "resource.h"
#include "DXUtil.h"
#include "MultiplayerConnectDlg.h"
#include "LobbyWaitDlg.h"
#include "SayDlg.h"
#include "TerraFighterApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
CNetConnectWizard* g_pNCW = NULL;           // Pointer to the net connect wizard




//-----------------------------------------------------------------------------
// Name: CNetConnectWizard
// Desc: Init the class
//-----------------------------------------------------------------------------
CNetConnectWizard::CNetConnectWizard( GUID* pGuidApp )
{
   g_pNCW              = this;
   m_pDP               = NULL;
   m_pLobbiedApp       = NULL;
   m_bHaveConnectionSettingsFromLobby = FALSE;
   m_hLobbyClient      = NULL;
   m_MPGDlg.m_guidApp           = *pGuidApp;

   m_strPreferredProvider = TEXT("DirectPlay8 TCP/IP Service Provider");

   m_hLobbyConnectionEvent = CreateEvent( NULL, FALSE, FALSE, NULL );


}




//-----------------------------------------------------------------------------
// Name: ~CNetConnectWizard
// Desc: Cleanup the class
//-----------------------------------------------------------------------------
CNetConnectWizard::~CNetConnectWizard()
{
   CloseHandle( m_hLobbyConnectionEvent );
}



//-----------------------------------------------------------------------------
// Name: Init
// Desc:
//-----------------------------------------------------------------------------
HRESULT CNetConnectWizard::Init( IDirectPlay8Peer* pDP,
                                IDirectPlay8LobbiedApplication* pLobbiedApp )
{
   if( NULL == pDP || NULL == pLobbiedApp )
      return E_INVALIDARG;

   m_pDP               = pDP;
   m_pLobbiedApp       = pLobbiedApp;
   m_bHaveConnectionSettingsFromLobby = FALSE;
   m_hLobbyClient      = NULL;

   return S_OK;
}



//-----------------------------------------------------------------------------
// Name: Shutdown
// Desc: Releases the DirectPlay interfaces
//-----------------------------------------------------------------------------
VOID CNetConnectWizard::Shutdown()
{
}




//-----------------------------------------------------------------------------
// Name: DoConnectWizard
// Desc: This is the main external function.  This will launch a series of
//       dialog boxes that enumerate service providers, enumerate hosts,
//       and allow the user to either join or host a session
//-----------------------------------------------------------------------------
HRESULT CNetConnectWizard::DoConnectWizard( BOOL bBackTrack, CInput* pInput,CD3DApplication* pApp,C2DDraw* p2dDraw )
{
   if( m_pDP == NULL )
      return E_INVALIDARG;


   CMultiplayerConnectDlg dlg;

   dlg.m_Name = m_MPGDlg.m_strLocalPlayerName;
   dlg.m_strPreferredProvider = m_strPreferredProvider;
   dlg.Create(p2dDraw,CPoint(10,10),m_pDP);
   if(dlg.DoModal(pInput,pApp,GetApp()->GetWindowMgr(),NULL) == IDOK)
   {
      LRESULT iIndex;
      HRESULT hr;

      SetPlayerName((const char*)dlg.m_NameCtrl.GetText());
      iIndex = dlg.m_ConnectionList.GetCurSel();
      SetPreferredProvider((const char*)dlg.m_ConnectionList.GetCurString());

      GUID* pGuid = (GUID*) dlg.m_ConnectionList.GetItemData(iIndex);
      if( NULL == pGuid )
      {
         // 'Wait for lobby launch' SP has been selected, so wait for a connection
         if( FAILED( hr = m_pLobbiedApp->SetAppAvailable( TRUE, 0 ) ) )
            return sys_msg_hr( TEXT("SetAppAvailable"), hr );


         // Display the multiplayer connect dialog box.
         CLobbyWaitDlg WaitDlg;
         WaitDlg.m_hEvent = m_hLobbyConnectionEvent;
         WaitDlg.Create(p2dDraw,CPoint(10,10));
         if(WaitDlg.DoModal(pInput,pApp,GetApp()->GetWindowMgr(),NULL) != IDOK)
         {
            return NCW_S_QUIT;
         }

         if( m_bHaveConnectionSettingsFromLobby )
         {
            if( FAILED( hr = ConnectUsingLobbySettings() ) )
               return sys_msg_hr( TEXT("ConnectUsingLobbySettings"), hr );

            return NCW_S_LOBBYCONNECT;
         }

         // 'Wait for lobby launch' was canceled, so don't wait for a connection anymore
         if( FAILED( hr = m_pLobbiedApp->SetAppAvailable( FALSE, 0 ) ) )
            return sys_msg_hr( TEXT("SetAppAvailable"), hr );
      }
      else
      {
         // Query for the enum host timeout for this SP
         DPN_SP_CAPS dpspCaps;
         ZeroMemory( &dpspCaps, sizeof(DPN_SP_CAPS) );
         dpspCaps.dwSize = sizeof(DPN_SP_CAPS);
         if( FAILED( hr = m_pDP->GetSPCaps( pGuid, &dpspCaps, 0 ) ) )
            return sys_msg_hr( TEXT("GetSPCaps"), hr );

         // Set the host expire time to around 3 times
         // length of the dwDefaultEnumRetryInterval
         m_MPGDlg.m_dwEnumHostExpireInterval = dpspCaps.dwDefaultEnumRetryInterval * 3;
         m_MPGDlg.m_guidSP = *pGuid;
      }

      m_MPGDlg.Create(p2dDraw,CPoint(10,10),m_pDP);
      if(m_MPGDlg.DoModal(pInput,pApp,GetApp()->GetWindowMgr(),NULL) != IDOK)
      {
         return NCW_S_QUIT;
      }
      // The SP has been chosen, so move forward in the wizard
      // Display Dialog
   }
   else
      return NCW_S_QUIT;

   return S_OK;
}

//-----------------------------------------------------------------------------
// Name: MessageHandler
// Desc: Handler for DirectPlay messages.  This function is called by
//       the DirectPlay message handler pool of threads, so be careful of thread
//       synchronization problems with shared memory
//-----------------------------------------------------------------------------
HRESULT WINAPI CNetConnectWizard::MessageHandler( PVOID pvUserContext,
                                                 DWORD dwMessageId,
                                                 PVOID pMsgBuffer )
{
   UNREFERENCED_PARAMETER( pvUserContext );

   // Try not to stay in this message handler for too long, otherwise
   // there will be a backlog of data.  The best solution is to
   // queue data as it comes in, and then handle it on other threads.

   // This function is called by the DirectPlay message handler pool of
   // threads, so be careful of thread synchronization problems with shared memory

   switch(dwMessageId)
   {
   case DPN_MSGID_ENUM_HOSTS_RESPONSE:
      {
         PDPNMSG_ENUM_HOSTS_RESPONSE pEnumHostsResponseMsg;
         pEnumHostsResponseMsg = (PDPNMSG_ENUM_HOSTS_RESPONSE)pMsgBuffer;

         // Take note of the host response
         m_MPGDlg.NoteEnumResponse( pEnumHostsResponseMsg );
         break;
      }

   case DPN_MSGID_ASYNC_OP_COMPLETE:
      {
         PDPNMSG_ASYNC_OP_COMPLETE pAsyncOpCompleteMsg;
         pAsyncOpCompleteMsg = (PDPNMSG_ASYNC_OP_COMPLETE)pMsgBuffer;

         if( pAsyncOpCompleteMsg->hAsyncOp == m_MPGDlg.m_hEnumAsyncOp )
            m_MPGDlg.FinalizeEnumHosts();

         break;
      }

   case DPN_MSGID_CONNECT_COMPLETE:
      {
         PDPNMSG_CONNECT_COMPLETE pConnectCompleteMsg;
         pConnectCompleteMsg = (PDPNMSG_CONNECT_COMPLETE)pMsgBuffer;

         // Set m_hrConnectComplete, then set an event letting
         // everyone know that the DPN_MSGID_CONNECT_COMPLETE msg
         // has been handled
         m_MPGDlg.m_hrConnectComplete = pConnectCompleteMsg->hResultCode;
         SetEvent( m_MPGDlg.m_hConnectCompleteEvent );
         break;
      }
   }

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ConnectUsingLobbySettings
// Desc: Call this after the DPL_MSGID_CONNECT has been processed to carry out
//       the connection settings received by the lobby client.  DPL_MSGID_CONNECT
//       will have already been processed if we were lobby launched, or after
//       WaitForConnection returns without timing out.
//-----------------------------------------------------------------------------
HRESULT CNetConnectWizard::ConnectUsingLobbySettings()
{
   HRESULT hr;
   DPNHANDLE hAsync;

   if( m_hLobbyClient == NULL )
      return E_INVALIDARG;

   DPL_CONNECTION_SETTINGS* pSettings = NULL;
   DWORD dwSettingsSize = 0;

   // Get the connection settings from the lobby.
   hr = m_pLobbiedApp->GetConnectionSettings( m_hLobbyClient, pSettings, &dwSettingsSize, 0 );
   if( hr != DPNERR_BUFFERTOOSMALL )
   {
      sys_msg_hr( TEXT("GetConnectionSettings"), hr );
      goto LCleanReturn;
   }

   pSettings = (DPL_CONNECTION_SETTINGS*) new BYTE[dwSettingsSize];
   if( NULL == pSettings )
   {
      hr = E_OUTOFMEMORY;
      sys_msg_hr( TEXT("ConnectUsingLobbySettings"), hr );
      goto LCleanReturn;
   }

   if( FAILED( hr = m_pLobbiedApp->GetConnectionSettings( m_hLobbyClient, pSettings, &dwSettingsSize, 0 ) ) )
   {
      sys_msg_hr( TEXT("GetConnectionSettings"), hr );
      goto LCleanReturn;
   }

   // Check if the lobby told us to host the game
   m_MPGDlg.m_bHostPlayer = (pSettings->dwFlags & DPLCONNECTSETTINGS_HOST);

   // Set the peer info
   WCHAR wszPeerName[MAX_PATH];
   DXUtil::ConvertGenericStringToWide( wszPeerName, m_MPGDlg.m_strLocalPlayerName, MAX_PATH );
   DPN_PLAYER_INFO dpPlayerInfo;
   ZeroMemory( &dpPlayerInfo, sizeof(DPN_PLAYER_INFO) );
   dpPlayerInfo.dwSize = sizeof(DPN_PLAYER_INFO);
   dpPlayerInfo.dwInfoFlags = DPNINFO_NAME;
   dpPlayerInfo.pwszName = wszPeerName;

   // Set the peer info, and use the DPNOP_SYNC since by default this
   // is an async call.  If it is not DPNOP_SYNC, then the peer info may not
   // be set by the time we call Connect() below.
   if( FAILED( hr = m_pDP->SetPeerInfo( &dpPlayerInfo, NULL, NULL, DPNOP_SYNC ) ) )
   {
      sys_msg_hr( TEXT("SetPeerInfo"), hr );
      goto LCleanReturn;
   }

   if( m_MPGDlg.m_bHostPlayer )
   {
      // Enable host migrate by default.
      pSettings->dpnAppDesc.dwFlags |= DPNSESSION_MIGRATE_HOST;

      // Disable DPNSVR by default
      pSettings->dpnAppDesc.dwFlags |= DPNSESSION_NODPNSVR;

      // Host a game as described by pSettings
      if( FAILED( hr = m_pDP->Host( &pSettings->dpnAppDesc,               // the application desc
         pSettings->ppdp8DeviceAddresses,      // array of addresses of the local devices used to connect to the host
         pSettings->cNumDeviceAddresses,       // number in array
         NULL, NULL,                           // DPN_SECURITY_DESC, DPN_SECURITY_CREDENTIALS
         NULL,                                 // player context
         0 ) ) )                               // flags
      {
         sys_msg_hr( TEXT("Host"), hr );
         goto LCleanReturn;
      }
   }
   else
   {
      // Connect to an existing session. There should only be on device address in
      // the connection settings structure when connecting to a session, so just
      // pass in the first one.
      // The enumeration is automatically cancelled after Connect is called 
      hr = m_pDP->Connect( &pSettings->dpnAppDesc,              // the application desc
         pSettings->pdp8HostAddress,          // address of the host of the session
         pSettings->ppdp8DeviceAddresses[0],  // address of the local device used to connect to the host
         NULL, NULL,                          // DPN_SECURITY_DESC, DPN_SECURITY_CREDENTIALS
         NULL, 0,                             // user data, user data size
         NULL,                                // player context,
         NULL, &hAsync,                       // async context, async handle,
         0 );                                 // flags
      if( hr != E_PENDING && FAILED(hr) )
      {
         sys_msg_hr( TEXT("Connect"), hr );
         goto LCleanReturn;
      }

      hr = S_OK; // Accept E_PENDING.

      // Wait until the MessageHandler sets an event to tell us the
      // DPN_MSGID_CONNECT_COMPLETE has been processed.  Then m_hrConnectComplete
      // will be valid.
      WaitForSingleObject( m_MPGDlg.m_hConnectCompleteEvent, INFINITE );

      if( FAILED( m_MPGDlg.m_hrConnectComplete ) )
      {
         sys_msg_hr( TEXT("DPN_MSGID_CONNECT_COMPLETE"), m_MPGDlg.m_hrConnectComplete );
         WinMessageBox(TEXT("Unable to join game."),
            TEXT("Terra Fighter"),GetApp()->GetDisplay(),GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),NULL);
         hr = m_MPGDlg.m_hrConnectComplete;
      }
   }

LCleanReturn:
   // Cleanup the addresses and memory obtained from GetConnectionSettings

   if( pSettings )
   {
      SAFE_RELEASE( pSettings->pdp8HostAddress );

      for( DWORD dwIndex=0; dwIndex < pSettings->cNumDeviceAddresses; dwIndex++ )
         SAFE_RELEASE( pSettings->ppdp8DeviceAddresses[dwIndex] );

      SAFE_DELETE_ARRAY( pSettings );
   }

   return hr;
}




//-----------------------------------------------------------------------------
// Name: LobbyMessageHandler
// Desc: Handler for DirectPlay messages.  This function is called by
//       the DirectPlay lobby message handler pool of threads, so be careful of thread
//       synchronization problems with shared memory
//-----------------------------------------------------------------------------
HRESULT WINAPI CNetConnectWizard::LobbyMessageHandler( PVOID pvUserContext,
                                                      DWORD dwMessageId,
                                                      PVOID pMsgBuffer )
{
   UNREFERENCED_PARAMETER( pvUserContext );
   HRESULT hr = S_OK;

   switch(dwMessageId)
   {
   case DPL_MSGID_CONNECT:
      {
         // This message will be processed when a lobby connection has been
         // established. If you were lobby launched then
         // IDirectPlay8LobbiedApplication::Initialize()
         // waits until this message has been processed before returning, so
         // take care not to deadlock by making calls that need to be handled by
         // the thread who called Initialize().  The same is true for WaitForConnection()

         PDPL_MESSAGE_CONNECT pConnectMsg;
         pConnectMsg = (PDPL_MESSAGE_CONNECT)pMsgBuffer;
         PDPL_CONNECTION_SETTINGS pSettings = pConnectMsg->pdplConnectionSettings;

         m_hLobbyClient = pConnectMsg->hConnectId;

         if( FAILED( hr = m_pDP->RegisterLobby( m_hLobbyClient, m_pLobbiedApp,
            DPNLOBBY_REGISTER ) ) )
            return sys_msg_hr( TEXT("RegisterLobby"), hr );

         if( pSettings == NULL )
         {
            // There aren't connection settings from the lobby
            m_bHaveConnectionSettingsFromLobby = FALSE;
         }
         else
         {
            // Record the player name if found
            if( pSettings->pwszPlayerName != NULL )
            {
               TCHAR strPlayerName[MAX_PATH];
               DXUtil::ConvertWideStringToGeneric( strPlayerName, pSettings->pwszPlayerName, MAX_PATH );
               m_MPGDlg.m_strLocalPlayerName=strPlayerName;
            }
            else
            {
               m_MPGDlg.m_strLocalPlayerName=TEXT("Unknown player name");
            }

            m_bHaveConnectionSettingsFromLobby = TRUE;
         }

         // Tell everyone we have a lobby connection now
         SetEvent( m_hLobbyConnectionEvent );
         break;
      }
   }

   return S_OK;
}



