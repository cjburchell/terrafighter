//-----------------------------------------------------------------------------
// File: NetConnect.h
//
// Desc:
//
// Copyright (C) 2000-2001 Microsoft Corporation. All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef NETCONNECT_H
#define NETCONNECT_H


#include <windows.h>
#include <dplay8.h>
#include <dpaddr.h>
#include <tchar.h>
#include "D3DApp.h"
#include "input.h"
#include "2DDraw.h"
#include "MultiplayerGamesDlg.h"


//-----------------------------------------------------------------------------
// Defines, structures, and error codes
//-----------------------------------------------------------------------------

#define NCW_S_FORWARD      ( (HRESULT) 0x01000001 ) // Dialog success, so go forward
#define NCW_S_BACKUP       ( (HRESULT) 0x01000002 ) // Dialog canceled, show previous dialog
#define NCW_S_QUIT         ( (HRESULT) 0x01000003 ) // Dialog quit, close app
#define NCW_S_LOBBYCONNECT ( (HRESULT) 0x01000004 ) // Dialog connected from lobby, connect success



class CNetConnectWizard
{
public:
   CNetConnectWizard(GUID* pGuidApp );
   virtual ~CNetConnectWizard();

   HRESULT WINAPI MessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );
   HRESULT WINAPI LobbyMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );

   HRESULT Init( IDirectPlay8Peer* pDP, IDirectPlay8LobbiedApplication* pLobbiedApp );
   VOID    Shutdown();
   HRESULT DoConnectWizard( BOOL bBackTrack, CInput* pInput,CD3DApplication* pApp  ,C2DDraw* p2dDraw);
   HRESULT ConnectUsingLobbySettings();

   void   SetMaxPlayers( DWORD dwMaxPlayers )                 { m_MPGDlg.m_dwMaxPlayers = dwMaxPlayers; }
   void   SetPlayerName( const TCHAR* strPlayerName )               { m_MPGDlg.m_strLocalPlayerName = strPlayerName; }
   void   SetSessionName( const TCHAR* strSessionName )             {m_MPGDlg.m_strSessionName = strSessionName;}
   void   SetPreferredProvider( const TCHAR* strPreferredProvider ) {m_strPreferredProvider = strPreferredProvider;}
   void   SetDefaultPort( DWORD dwDefaultPort )               { m_MPGDlg.m_dwPort = dwDefaultPort; }

   CString GetPlayerName()                                     { return m_MPGDlg.m_strLocalPlayerName; }
   CString GetSessionName()                                    { return m_MPGDlg.m_strSessionName; }
   CString GetPreferredProvider()                              { return m_strPreferredProvider; }
   BOOL   IsHostPlayer()                                      { return m_MPGDlg.m_bHostPlayer; }
   BOOL   IsMigrateHost()                                     { return m_MPGDlg.m_bMigrateHost; }
   BOOL   HaveConnectionSettingsFromLobby()                   { return m_bHaveConnectionSettingsFromLobby; }
   static BOOL SPRequiresPort( GUID* pSP )                    { return (*pSP != CLSID_DP8SP_MODEM && *pSP != CLSID_DP8SP_SERIAL && *pSP != CLSID_DP8SP_BLUETOOTH); } 

protected:
   IDirectPlay8Peer*       m_pDP;
   IDirectPlay8LobbiedApplication* m_pLobbiedApp;
   CString                   m_strPreferredProvider;

   HANDLE                  m_hLobbyConnectionEvent;
   BOOL                    m_bHaveConnectionSettingsFromLobby;
   DPNHANDLE	            m_hLobbyClient;

   CMultiplayerGamesDlg m_MPGDlg;
};


#endif // NETCONNECT_H

