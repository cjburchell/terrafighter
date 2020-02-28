// NetworkConnection.cpp: implementation of the CNetworkConnection class.
//
//////////////////////////////////////////////////////////////////////
#include <stdafx.h>
#include "NetworkConnection.h"
#include "SayDlg.h"
#include "TerraFighterApp.h"

#define DPLAY_SAMPLE_KEY        TEXT("Software\\Redpoint\\Terrafighter")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetworkConnection::CNetworkConnection()
{
   m_pDP                         = NULL;    // DirectPlay peer object
   m_pNetConnectWizard           = NULL;    // Connection wizard
   m_pSessionInfo                = NULL;    // Session Information
   m_pLobbiedApp    = NULL;    // DirectPlay lobbied app 
   m_bWasLobbyLaunched           = FALSE;   // TRUE if lobby launched
   m_dpnidLocalPlayer            = 0;       // DPNID of local player
   m_dpnidHostPlayer             = 0;       // DPNID of host player
   m_lNumberOfActivePlayers      = 0;       // Number of players currently in game
   m_hWnd = NULL;
   InitializeCriticalSection( &m_csPlayerContext );
   InitializeCriticalSection( &m_csQueue );

   m_MessageEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

}

CNetworkConnection::~CNetworkConnection()
{
   Shutdown();
   DeleteCriticalSection( &m_csPlayerContext );
   DeleteCriticalSection( &m_csQueue );
   CloseHandle(m_MessageEvent);
}

BOOL CNetworkConnection::Init(HINSTANCE hInst,LPTSTR strAppName,GUID* pGuidApp, DWORD PortNumb, CInput* pInput,CD3DApplication* pApp ,C2DDraw* p2dDraw )
{
   BOOL bConnectSuccess = FALSE;
   DXUtil::ReadStringRegKey(TEXT("Player Name"), 
      m_strLocalPlayerName, MAX_PATH, TEXT("TestPlayer") );
   DXUtil::ReadStringRegKey(TEXT("Session Name"), 
      m_strSessionName, MAX_PATH, TEXT("TestGame") );
   DXUtil::ReadStringRegKey(TEXT("Preferred Provider"), 
      m_strPreferredProvider, MAX_PATH, TEXT("DirectPlay8 TCP/IP Service Provider") );

   // Create helper class
   m_pNetConnectWizard = new CNetConnectWizard(pGuidApp );

   HRESULT hr;
   if( FAILED( hr = InitDirectPlay() ) )
   {
      sys_msg_hr( TEXT("InitDirectPlay"), hr );
      WinMessageBox(TEXT("Failed initializing IDirectPlay8Peer"),
         TEXT("Terra Fighter"),p2dDraw,pInput,pApp,GetApp()->GetWindowMgr(),NULL);
      return FALSE;
   }

   // Create session information helper
   m_pSessionInfo = new CSessionInfo( m_pDP );

   // Check if we were launched from a lobby client
   if( m_bWasLobbyLaunched && m_pNetConnectWizard->HaveConnectionSettingsFromLobby() )
   {
      // If were lobby launched then DPL_MSGID_CONNECT has already been
      // handled, so we can just tell the wizard to connect to the lobby
      // that has sent us a DPL_MSGID_CONNECT msg.
      if( FAILED( hr = m_pNetConnectWizard->ConnectUsingLobbySettings() ) )
      {
         sys_msg_hr( TEXT("ConnectUsingLobbySettings"), hr );
         WinMessageBox(TEXT("Failed to connect using lobby settings"),
            TEXT("Terra Fighter"),p2dDraw,pInput,pApp,GetApp()->GetWindowMgr(),NULL);

         bConnectSuccess = FALSE;
      }
      else
      {
         // Read information from m_pNetConnectWizard
         _tcsncpy( m_strLocalPlayerName, m_pNetConnectWizard->GetPlayerName(), MAX_PATH-1 );
         m_strLocalPlayerName[ MAX_PATH-1 ] = 0;

         bConnectSuccess = TRUE; 
      }
   }
   else
   {
      // If not lobby launched, prompt the user about the network 
      // connection and which session they would like to join or 
      // if they want to create a new one.

      // Setup connection wizard
      m_pNetConnectWizard->SetPlayerName( m_strLocalPlayerName );
      m_pNetConnectWizard->SetSessionName( m_strSessionName );
      m_pNetConnectWizard->SetPreferredProvider( m_strPreferredProvider );
      m_pNetConnectWizard->SetDefaultPort( PortNumb );

      // Do the connection wizard
      hr = m_pNetConnectWizard->DoConnectWizard( FALSE ,pInput, pApp, p2dDraw);        
      if( FAILED( hr ) ) 
      {
         sys_msg_hr( TEXT("DoConnectWizard"), hr );
         WinMessageBox(TEXT("Multiplayer connect failed."),
            TEXT("Terra Fighter"),p2dDraw,pInput,pApp,GetApp()->GetWindowMgr(),NULL);
         bConnectSuccess = FALSE;
      } 
      else if( hr == NCW_S_QUIT ) 
      {
         // The user canceled the Multiplayer connect, so quit 
         bConnectSuccess = FALSE;
      }
      else
      {
         bConnectSuccess = TRUE; 

         // Read information from m_pNetConnectWizard
         _tcsncpy( m_strLocalPlayerName, m_pNetConnectWizard->GetPlayerName(), MAX_PATH-1 );
         m_strLocalPlayerName[ MAX_PATH-1 ] = 0;

         _tcsncpy( m_strSessionName, m_pNetConnectWizard->GetSessionName(), MAX_PATH-1 );
         m_strSessionName[ MAX_PATH-1 ] = 0;

         _tcsncpy( m_strPreferredProvider, m_pNetConnectWizard->GetPreferredProvider(), MAX_PATH-1 );
         m_strPreferredProvider[ MAX_PATH-1 ] = 0;

         // Write information to the registry
         DXUtil::WriteStringRegKey(TEXT("Player Name"), m_strLocalPlayerName );
         DXUtil::WriteStringRegKey(TEXT("Session Name"), m_strSessionName );
         DXUtil::WriteStringRegKey(TEXT("Preferred Provider"), m_strPreferredProvider );
      }
   }
   return bConnectSuccess;

}

//-----------------------------------------------------------------------------
// Name: InitDirectPlay()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CNetworkConnection::InitDirectPlay()
{
   DPNHANDLE hLobbyLaunchedConnection = NULL;
   HRESULT hr;

   // Create IDirectPlay8Peer
   if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Peer, NULL, 
      CLSCTX_INPROC_SERVER,
      IID_IDirectPlay8Peer, 
      (LPVOID*) &m_pDP ) ) )
      return sys_msg_hr( TEXT("CoCreateInstance"), hr );

   // Create IDirectPlay8LobbiedApplication
   if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8LobbiedApplication, NULL, 
      CLSCTX_INPROC_SERVER,
      IID_IDirectPlay8LobbiedApplication, 
      (LPVOID*) &m_pLobbiedApp ) ) )
      return sys_msg_hr( TEXT("CoCreateInstance"), hr );

   // Init the helper class, now that m_pDP and m_pLobbiedApp are valid
   m_pNetConnectWizard->Init( m_pDP, m_pLobbiedApp );

   // Turn off parameter validation in release builds
#ifdef _DEBUG
   const DWORD dwInitFlags = 0;
#else
   const DWORD dwInitFlags = DPNINITIALIZE_DISABLEPARAMVAL;
#endif // _DEBUG

   // Init IDirectPlay8Peer
   if( FAILED( hr = m_pDP->Initialize( (LPVOID)this, DirectPlayMessageHandler, dwInitFlags ) ) )
      return sys_msg_hr( TEXT("Initialize"), hr );

   // Init IDirectPlay8LobbiedApplication.  Before this Initialize() returns 
   // a DPL_MSGID_CONNECT msg may come in to the DirectPlayLobbyMessageHandler 
   // so be prepared ahead of time.
   if( FAILED( hr = m_pLobbiedApp->Initialize( (LPVOID)this, DirectPlayLobbyMessageHandler, 
      &hLobbyLaunchedConnection, dwInitFlags ) ) )
      return sys_msg_hr( TEXT("Initialize"), hr );

   // IDirectPlay8LobbiedApplication::Initialize returns a handle to a connnection
   // if we have been lobby launced.  Initialize is guanteeded to return after 
   // the DPL_MSGID_CONNECT msg has been processed.  So unless a we are expected 
   // multiple lobby connections, we do not need to remember the lobby connection
   // handle since it will be recorded upon the DPL_MSGID_CONNECT msg.
   m_bWasLobbyLaunched = ( hLobbyLaunchedConnection != NULL );

   return S_OK;
}


//-----------------------------------------------------------------------------
// Name: DirectPlayLobbyMessageHandler
// Desc: Handler for DirectPlay lobby messages.  This function is called by
//       the DirectPlay lobby message handler pool of threads, so be careful of 
//       thread synchronization problems with shared memory
//-----------------------------------------------------------------------------
HRESULT WINAPI CNetworkConnection::DirectPlayLobbyMessageHandler( PVOID pvUserContext, 
                                                                 DWORD dwMessageId, 
                                                                 PVOID pMsgBuffer )
{

   CNetworkConnection* pNetConnect = (CNetworkConnection*)pvUserContext;
   switch( dwMessageId )
   {
   case DPL_MSGID_CONNECT:
      {
         PDPL_MESSAGE_CONNECT pConnectMsg;
         pConnectMsg = (PDPL_MESSAGE_CONNECT)pMsgBuffer;

         // The CNetConnectWizard will handle this message for us,
         // so there is nothing we need to do here for this simple
         // sample.
         break;
      }

   case DPL_MSGID_DISCONNECT:
      {
         PDPL_MESSAGE_DISCONNECT pDisconnectMsg;
         pDisconnectMsg = (PDPL_MESSAGE_DISCONNECT)pMsgBuffer;

         // We should free any data associated with the lobby 
         // client here, but there is none.
         break;
      }

   case DPL_MSGID_RECEIVE:
      {
         PDPL_MESSAGE_RECEIVE pReceiveMsg;
         pReceiveMsg = (PDPL_MESSAGE_RECEIVE)pMsgBuffer;

         // The lobby client sent us data.  This sample doesn't
         // expected data from the client, but it is useful 
         // for more complex apps.
         break;
      }

   case DPL_MSGID_CONNECTION_SETTINGS:
      {
         PDPL_MESSAGE_CONNECTION_SETTINGS pConnectionStatusMsg;
         pConnectionStatusMsg = (PDPL_MESSAGE_CONNECTION_SETTINGS)pMsgBuffer;

         // The lobby client has changed the connection settings.  
         // This simple sample doesn't handle this, but more complex apps may
         // want to.
         break;
      }
   }

   // Make sure the DirectPlay MessageHandler calls the CNetConnectWizard handler, 
   // so the wizard can be informed of lobby messages such as DPL_MSGID_CONNECT
   if( pNetConnect && pNetConnect->m_pNetConnectWizard )
      return pNetConnect->m_pNetConnectWizard->LobbyMessageHandler( pvUserContext, dwMessageId, 
      pMsgBuffer );

   return S_OK;
}

HRESULT WINAPI CNetworkConnection::DirectPlayMessageHandler( PVOID pvUserContext, 
                                                            DWORD dwMessageId, 
                                                            PVOID pMsgBuffer )
{
   CNetworkConnection* pNetConnect = (CNetworkConnection*)pvUserContext;
   if(pNetConnect)
      return pNetConnect->OnMessage(dwMessageId,pMsgBuffer);
   return S_OK;
}

void CNetworkConnection::Shutdown()
{
   if(m_pNetConnectWizard)
      m_pNetConnectWizard->Shutdown();

   CNetworkQueueData* pData;
   while((pData = GetNextMessage()) != NULL)
   {
      ReturnBuffer(pData);
      SAFE_DELETE( pData );
   }

   if( m_pDP )
   {
      m_pDP->Close(0);
      SAFE_RELEASE( m_pDP );
   }

   if( m_pLobbiedApp )
   {
      m_pLobbiedApp->Close( 0 );
      SAFE_RELEASE( m_pLobbiedApp );
   }    

   // Don't delete the wizard until we know that 
   // DirectPlay is out of its message handlers.
   // This will be true after Close() has been called. 
   SAFE_DELETE( m_pNetConnectWizard );
   SAFE_DELETE( m_pSessionInfo );
}

HRESULT CNetworkConnection::OnMessage(DWORD dwMessageId,PVOID pMsgBuffer)
{
   // Try not to stay in this message handler for too long, otherwise
   // there will be a backlog of data.  The best solution is to 
   // queue data as it comes in, and then handle it on other threads
   // as this sample shows

   // This function is called by the DirectPlay message handler pool of 
   // threads, so be care of thread synchronization problems with shared memory

   // Sift this message through the SessionInfo helper class
   if( m_pSessionInfo && m_pSessionInfo->MessageHandler( dwMessageId, pMsgBuffer ) )
      return S_OK;



   HRESULT hReturn = S_OK;

   switch( dwMessageId )
   {
   case DPN_MSGID_CREATE_PLAYER:
      {
         HRESULT hr;
         PDPNMSG_CREATE_PLAYER pCreatePlayerMsg;
         pCreatePlayerMsg = (PDPNMSG_CREATE_PLAYER)pMsgBuffer;

         // Create a new and fill in a APP_PLAYER_INFO
         CNetworkPlayerInfo* pPlayerInfo = new CNetworkPlayerInfo;
         if( NULL == pPlayerInfo )
            break;

         ZeroMemory( pPlayerInfo, sizeof(CNetworkPlayerInfo) );
         pPlayerInfo->dpnidPlayer = pCreatePlayerMsg->dpnidPlayer;
         pPlayerInfo->lRefCount   = 1;

         // Get the peer info and extract its name
         DWORD dwSize = 0;
         DPN_PLAYER_INFO* pdpPlayerInfo = NULL;
         hr = DPNERR_CONNECTING;

         // GetPeerInfo might return DPNERR_CONNECTING when connecting, 
         // so just keep calling it if it does
         while( hr == DPNERR_CONNECTING ) 
            hr = m_pDP->GetPeerInfo( pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0 );                                

         if( hr == DPNERR_BUFFERTOOSMALL )
         {
            pdpPlayerInfo = (DPN_PLAYER_INFO*) new BYTE[ dwSize ];
            if( NULL == pdpPlayerInfo )
            {
               // Out of memory
               SAFE_DELETE( pPlayerInfo )
                  break;
            }

            ZeroMemory( pdpPlayerInfo, dwSize );
            pdpPlayerInfo->dwSize = sizeof(DPN_PLAYER_INFO);

            hr = m_pDP->GetPeerInfo( pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0 );
            if( SUCCEEDED(hr) )
            {
               // This stores a extra TCHAR copy of the player name for 
               // easier access.  This will be redundent copy since DPlay 
               // also keeps a copy of the player name in GetPeerInfo()
               DXUtil::ConvertWideStringToGeneric( pPlayerInfo->strPlayerName, 
                  pdpPlayerInfo->pwszName, MAX_PLAYER_NAME );    

               if( pdpPlayerInfo->dwPlayerFlags & DPNPLAYER_LOCAL )
                  m_dpnidLocalPlayer = pCreatePlayerMsg->dpnidPlayer;
               if( pdpPlayerInfo->dwPlayerFlags & DPNPLAYER_HOST )
                  m_dpnidHostPlayer = pCreatePlayerMsg->dpnidPlayer;
            }

            SAFE_DELETE_ARRAY( pdpPlayerInfo );
         }

         // Tell DirectPlay to store this pPlayerInfo 
         // pointer in the pvPlayerContext.
         pCreatePlayerMsg->pvPlayerContext = pPlayerInfo;

         // Update the number of active players, and 
         // post a message to the dialog thread to update the 
         // UI.  This keeps the DirectPlay message handler 
         // from blocking
         InterlockedIncrement( &m_lNumberOfActivePlayers );


         if( m_hWnd != NULL )
            PostMessage( m_hWnd, WM_ADD_PLAYER, 0, 0 );

         break;
      }

   case DPN_MSGID_DESTROY_PLAYER:
      {
         PDPNMSG_DESTROY_PLAYER pDestroyPlayerMsg;
         pDestroyPlayerMsg = (PDPNMSG_DESTROY_PLAYER)pMsgBuffer;
         CNetworkPlayerInfo* pPlayerInfo = (CNetworkPlayerInfo*) pDestroyPlayerMsg->pvPlayerContext;

         EnterCriticalSection(&m_csPlayerContext);                  // enter player context CS
         PLAYER_RELEASE( pPlayerInfo );  // Release player and cleanup if needed
         LeaveCriticalSection(&m_csPlayerContext);

         // Update the number of active players, and 
         // post a message to the dialog thread to update the 
         // UI.  This keeps the DirectPlay message handler 
         // from blocking
         InterlockedDecrement( &m_lNumberOfActivePlayers );
         if( m_hWnd != NULL )
            PostMessage( m_hWnd, WM_DESTROY_PLAYER, 0, 0 );

         break;
      }

   case DPN_MSGID_HOST_MIGRATE:
      {
         PDPNMSG_HOST_MIGRATE pHostMigrateMsg;
         pHostMigrateMsg = (PDPNMSG_HOST_MIGRATE)pMsgBuffer;

         m_dpnidHostPlayer = pHostMigrateMsg->dpnidNewHost;
         if( m_hWnd != NULL )
            PostMessage( m_hWnd, WM_NET_MIGRATE, 0, 0 );

         break;
      }

   case DPN_MSGID_TERMINATE_SESSION:
      {
         PDPNMSG_TERMINATE_SESSION pTerminateSessionMsg;
         pTerminateSessionMsg = (PDPNMSG_TERMINATE_SESSION)pMsgBuffer;

         if( m_hWnd != NULL )
            PostMessage( m_hWnd, WM_NET_TERMINATE, 0, 0 );
         break;
      }

   case DPN_MSGID_RECEIVE:
      {
         PDPNMSG_RECEIVE pReceiveMsg;
         pReceiveMsg = (PDPNMSG_RECEIVE)pMsgBuffer;
         CNetworkPlayerInfo* pPlayerInfo = (CNetworkPlayerInfo*) pReceiveMsg->pvPlayerContext;           
         if( NULL == pPlayerInfo )
            break;

         CNetworkQueueData* pQueuedData = new CNetworkQueueData;
         pQueuedData->pMsg = pReceiveMsg->pReceiveData;
         pQueuedData->dwSize = pReceiveMsg->dwReceiveDataSize;
         pQueuedData->hBufferHandle = pReceiveMsg->hBufferHandle;
         pQueuedData->dpnidPlayer = pPlayerInfo->dpnidPlayer;

         EnterCriticalSection(&m_csQueue);                  // enter player context CS
         m_queue.push_back(pQueuedData);
         LeaveCriticalSection(&m_csQueue);

         SetEvent(m_MessageEvent);

         if( m_hWnd != NULL )
            PostMessage( m_hWnd, WM_NET_MESSAGE, 0, (LPARAM) pQueuedData );

         hReturn = DPNSUCCESS_PENDING;
      }
   }

   // Make sure the DirectPlay MessageHandler calls the CNetConnectWizard handler, 
   // so it can be informed of messages such as DPN_MSGID_ENUM_HOSTS_RESPONSE.
   if( hReturn != DPNSUCCESS_PENDING && SUCCEEDED(hReturn) && m_pNetConnectWizard )
      hReturn = m_pNetConnectWizard->MessageHandler( NULL, dwMessageId, pMsgBuffer );

   return hReturn;
}
