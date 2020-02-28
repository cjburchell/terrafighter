// MultiplayerGamesDlg.cpp: implementation of the CMultiplayerGamesDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MultiplayerGamesDlg.h"
#include "TerraFighterApp.h"
#include "SayDlg.h"
#include "AddressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiplayerGamesDlg::CMultiplayerGamesDlg() : CWinDialog()
{
   m_bSearchingForSessions = FALSE;
   m_bConnecting       = FALSE;
   m_hEnumAsyncOp      = NULL;
   ZeroMemory(&m_guidSP, sizeof(GUID));
   m_strHostname = TEXT("");
   InitializeCriticalSection( &m_csHostEnum );
   m_hConnectCompleteEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

   // Setup the m_DPHostEnumHead circular linked list
   ZeroMemory( &m_DPHostEnumHead, sizeof( DPHostEnumInfo ) );
   m_DPHostEnumHead.pNext = &m_DPHostEnumHead;

   m_dwEnumHostExpireInterval = 0;

   m_hConnectAsyncOp   = NULL;
   m_bMigrateHost      = TRUE;
   m_dwPort            = 0;
   m_bUseDPNSVR		= FALSE;
   m_eSigningType		= SIGN_FAST;

   // Set the max players unlimited by default.  This can be changed by the app
   // by calling SetMaxPlayers()
   m_dwMaxPlayers   = 0;

   m_bUpdateHostsTimer = FALSE;
   m_ConnectCompleteTimer = FALSE;
}

CMultiplayerGamesDlg::~CMultiplayerGamesDlg()
{
   StopEnumHosts();
   DeleteCriticalSection( &m_csHostEnum );
   CloseHandle( m_hConnectCompleteEvent );
}

BOOL CMultiplayerGamesDlg::ValidButtonID(int Id)
{
   return (Id == IDC_BACK || Id == IDC_BACK);
}

CWinBaceObj* CMultiplayerGamesDlg::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   switch(id)
   {
      WIN_USE_CTRL(IDC_GAMES_LIST, m_GameList);
   }
   return NULL;
}

BOOL CMultiplayerGamesDlg::Create(C2DDraw *pDisplay, const CPoint& p,IDirectPlay8Peer* pDP)
{
   LoadDialog(IDD_MULTIPLAYER_GAMES,pDisplay);
   if(!CWinDialog::Create(pDisplay,NULL,p,GetSize(),GetText()))
      return FALSE;

   m_pDP = pDP;

   m_bSearchingForSessions = FALSE;

   // Check to see if a former search is still waiting to end
   if(m_hEnumAsyncOp != NULL)
   {
      GetControl(IDC_SEARCH_CHECK)->EnableWin(FALSE);
      GetControl(IDC_SEARCH_CHECK)->SetText(TEXT("Stopping...") );
   }



   InitListbox();

   return TRUE;
}

void CMultiplayerGamesDlg::OnButtonPressed(int ButtonID)
{
   HRESULT hr;
   switch( ButtonID )
   {
   case IDC_SEARCH_CHECK:
      m_bSearchingForSessions = !m_bSearchingForSessions;

      if( m_bSearchingForSessions )
      {
         // Ask the user for the remote address
         if( CNetConnectWizard::SPRequiresPort( &m_guidSP ) )
         {

            CAddressDlg dlg;

            dlg.Create(GetDisplay(),CPoint(10,10));

            dlg.m_Name.SetText(m_strHostname);

            CString tmp;
            tmp.Format("%d",m_dwPort);
            dlg.m_Port.SetText(tmp);

            int nResult =dlg.DoModal(GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),this);

            // If the user cancelled the remote address dialog box, 
            // don't start the search
            if( nResult == IDCANCEL )
            {
               m_bSearchingForSessions = FALSE;
               break;
            }

            m_strHostname = dlg.m_Name.GetText();
            m_dwPort = atoi(dlg.m_Port.GetText());
         }

         GetControl(IDC_SEARCH_CHECK)->SetText("Searching...");
         m_bUpdateHostsTimer = TRUE;

         // Start the async enumeration
         if( FAILED( hr = EnumHosts() ) )
         {
            sys_msg_hr( TEXT("SessionsDlgEnumHosts"), hr );
            WinMessageBox(TEXT("Error enumerating DirectPlay games."),
               TEXT("Terra Figher"),
               GetDisplay(),GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),this);
         }
      }
      else
      {
         StopEnumHosts();
      }

      break;

   case IDC_JOIN:
      if( FAILED( hr = JoinGame() ) )
      {
         sys_msg_hr( TEXT("SessionsDlgJoinGame"), hr );
         WinMessageBox(TEXT("Unable to join game."),
            TEXT("Terra Figher"),
            GetDisplay(),GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),this);
      }
      break;

   case IDC_CREATE:
      if( FAILED( hr = CreateGame() ) )
      {
         sys_msg_hr( TEXT("SessionsDlgCreateGame"), hr );
         WinMessageBox(TEXT("Unable to create game."),
            "Terra Figher",
            GetDisplay(),GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),this);
      }
      break;
   }
}

int CMultiplayerGamesDlg::DoIdle()
{
   // Upon this timer message, then refresh the list of hosts
   // by expiring old hosts, and displaying the list in the
   // dialog box
   HRESULT hr;
   if(m_bUpdateHostsTimer)
   {
      // Don't refresh if we are not enumerating hosts
      if(m_bSearchingForSessions )
      {
         // Expire all of the hosts that haven't
         // refreshed in a certain period of time
         ExpireOldHostEnums();

         // Display the list of hosts in the dialog
         if( FAILED( hr = DisplayEnumList() ) )
         {
            sys_msg_hr( TEXT("SessionsDlgDisplayEnumList"), hr );
            m_bUpdateHostsTimer = FALSE;
            WinMessageBox(TEXT("Error enumerating DirectPlay games."),
               TEXT("Terra Figher"),
               GetDisplay(),GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),this);
         }
      }
   }

   if(m_ConnectCompleteTimer)
   {
      // Check to see if the MessageHandler has set an event to tell us the
      // DPN_MSGID_CONNECT_COMPLETE has been processed.  Now m_hrConnectComplete
      // is valid.
      if( WAIT_OBJECT_0 == WaitForSingleObject( m_hConnectCompleteEvent, 0 ) )
      {
         m_bConnecting = FALSE;

         // Re-enable create button
         GetControl(IDC_CREATE)->EnableWin(TRUE);

         if( FAILED( m_hrConnectComplete ) )
         {
            sys_msg_hr( TEXT("DPN_MSGID_CONNECT_COMPLETE"), m_hrConnectComplete );
            WinMessageBox(TEXT("Unable to join game."),
               TEXT("Terra Figher"),
               GetDisplay(),GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),this);
         }
         else
         {
            // DirectPlay connect successful, so end dialog
            EndDialog(IDOK);
         }

         m_ConnectCompleteTimer = FALSE;
      }
   }
   return CWinDialog::DoIdle();
}


//-----------------------------------------------------------------------------
// Name: SessionsDlgInitListbox()
// Desc: Initializes the listbox
//-----------------------------------------------------------------------------
VOID CMultiplayerGamesDlg::InitListbox()
{
   // Clear the contents from the list box, and
   // display "Looking for games" text in listbox
   m_GameList.ResetContent();
   if( m_bSearchingForSessions )
   {
      m_GameList.AddString("Looking for games...");
   }
   else
   {
      m_GameList.AddString("Click Start Search to see a list of games.");
      m_GameList.AddString("Click Create to start a new game.");
   }
   m_GameList.SetCurSel(0);

   // Disable the join button until sessions are found
   if(GetControl(IDC_JOIN))
      GetControl(IDC_JOIN)->EnableWin(FALSE);
}

HRESULT CMultiplayerGamesDlg::EnumHosts()
{

   HRESULT hr = S_OK;

   m_bEnumListChanged = TRUE;

   IDirectPlay8Address*   pDP8AddressHost  = NULL;
   IDirectPlay8Address*   pDP8AddressLocal = NULL;
   WCHAR*                 wszHostName      = NULL;

   // Create the local device address object
   if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
      CLSCTX_ALL, IID_IDirectPlay8Address,
      (LPVOID*) &pDP8AddressLocal ) ) )
   {
      sys_msg_hr( TEXT("CoCreateInstance"), hr );
      goto LCleanup;
   }

   // Set local service provider
   if( FAILED( hr = pDP8AddressLocal->SetSP( &m_guidSP ) ) )
   {
      sys_msg_hr( TEXT("SetSP"), hr );
      goto LCleanup;
   }

   // Create the remote host address object
   if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
      CLSCTX_ALL, IID_IDirectPlay8Address,
      (LPVOID*) &pDP8AddressHost ) ) )
   {
      sys_msg_hr( TEXT("CoCreateInstance"), hr );
      goto LCleanup;
   }

   // Set remote service provider
   if( FAILED( hr = pDP8AddressHost->SetSP( &m_guidSP ) ) )
   {
      sys_msg_hr( TEXT("SetSP"), hr );
      goto LCleanup;
   }

   // If we're using a TCP/IP (including network simulator) or IPX
   // service provider, the user was given an option for hostname and
   // port before the search started. 
   if( CNetConnectWizard::SPRequiresPort( &m_guidSP ) ) 
   {
      // Add the hostname. If this is blank, DirectPlay will attempt
      // to search the local network.
      if( _tcscmp(m_strHostname, TEXT("")) != 0 )
      {
         WCHAR wszHostName[MAX_PATH];
         DXUtil::ConvertGenericStringToWide( wszHostName, m_strHostname, MAX_PATH );
         hr = pDP8AddressHost->AddComponent( DPNA_KEY_HOSTNAME, wszHostName, 
            (DWORD) (wcslen(wszHostName)+1)*sizeof(WCHAR), 
            DPNA_DATATYPE_STRING );
         if( FAILED(hr) )
         {
            sys_msg_hr( TEXT("AddComponent"), hr );
            goto LCleanup;
         }
      }

      // Add the requested port value. The port value is required in order to
      // receive any search hits if DPNSVR isn't running on the remote machine.
      // Games will typically hard code the port so the user need not know it
      if( m_dwPort != 0 )
      {
         hr = pDP8AddressHost->AddComponent( DPNA_KEY_PORT, 
            &m_dwPort, sizeof(m_dwPort),
            DPNA_DATATYPE_DWORD );
         if( FAILED(hr) )
         {
            sys_msg_hr( TEXT("AddComponent"), hr );
            goto LCleanup;
         }
      }
   }

   // Enumerate hosts
   DPN_APPLICATION_DESC    dnAppDesc;
   ZeroMemory( &dnAppDesc, sizeof(DPN_APPLICATION_DESC) );
   dnAppDesc.dwSize          = sizeof(DPN_APPLICATION_DESC);
   dnAppDesc.guidApplication = m_guidApp;

   DWORD dwFlags;
   dwFlags = 0;

   // For certain service providers the user has not been
   // asked to fill in the components of the remote address,
   // so DirectPlay should ask for required fields.
   if( !CNetConnectWizard::SPRequiresPort( &m_guidSP ) )
   {
      dwFlags = DPNENUMHOSTS_OKTOQUERYFORADDRESSING;
   }

   // Enumerate all the active DirectPlay games on the selected connection
   hr = m_pDP->EnumHosts( &dnAppDesc,                            // application description
      pDP8AddressHost,                       // host address
      pDP8AddressLocal,                      // device address
      NULL,                                  // pointer to user data
      0,                                     // user data size
      INFINITE,                              // retry count (forever)
      0,                                     // retry interval (0=default)
      INFINITE,                              // time out (forever)
      NULL,                                  // user context
      &m_hEnumAsyncOp,                       // async handle
      dwFlags                                // flags
      );
   if( FAILED(hr) )
   {
      sys_msg_hr( TEXT("EnumHosts"), hr );
      goto LCleanup;
   }

LCleanup:
   SAFE_RELEASE( pDP8AddressHost);
   SAFE_RELEASE( pDP8AddressLocal );
   SAFE_DELETE( wszHostName );

   return hr;
}


//-----------------------------------------------------------------------------
// Name: SessionsDlgStopEnumHosts()
// Desc: Stops the running session enumeration
//-----------------------------------------------------------------------------
VOID CMultiplayerGamesDlg::StopEnumHosts()
{
   HRESULT hr;

   // Update the UI
   if(GetControl(IDC_SEARCH_CHECK))
   {
      GetControl(IDC_SEARCH_CHECK)->EnableWin(FALSE);
      GetControl(IDC_SEARCH_CHECK)->SetText("Stopping...");
   }

   // Stop the timer, and stop the async enumeration
   m_bUpdateHostsTimer = FALSE;

   // Until the CancelAsyncOperation returns, it is possible
   // to still receive host enumerations. Instruct DirectPlay to
   // stop the current enumeration and handle the rest of the cleanup
   // when receiving the ASYNC_OP_COMPLETE message. If this method
   // fails we should assume there's no active enumeration and call
   // the finalization method directly.
   if( m_hEnumAsyncOp )
   {
      hr = m_pDP->CancelAsyncOperation( m_hEnumAsyncOp, 0 );
      if( FAILED(hr) )
         FinalizeEnumHosts();
   }
   else
   {
      FinalizeEnumHosts();
   }
}




//-----------------------------------------------------------------------------
// Name: SessionsDlgFinalizeEnumHosts()
// Desc: This method should be called when we receive confirmation from 
//       DirectPlay that the enumeration has completed. It reset the search
//       UI and state variables, and enables the search button.
//-----------------------------------------------------------------------------
VOID CMultiplayerGamesDlg::FinalizeEnumHosts()
{
   // Clear the data list
   EnumListCleanup();

   // Reset the search state variables
   m_hEnumAsyncOp = NULL;
   m_bSearchingForSessions = FALSE;

   // Reset the search portion of the dialog
   InitListbox(); 
   if(GetControl(IDC_SEARCH_CHECK))
   {
      GetControl(IDC_SEARCH_CHECK)->EnableWin(TRUE);
      GetControl(IDC_SEARCH_CHECK)->SetText("Start Search");
   }
}

//-----------------------------------------------------------------------------
// Name: SessionsDlgJoinGame()
// Desc: Joins the selected DirectPlay session
//-----------------------------------------------------------------------------
HRESULT CMultiplayerGamesDlg::JoinGame()
{
   HRESULT         hr;
   DPHostEnumInfo* pDPHostEnumSelected = NULL;
   int             nItemSelected;

   m_bHostPlayer = FALSE;

   // Add status text in list box
   nItemSelected = m_GameList.GetCurSel();

   EnterCriticalSection( &m_csHostEnum );

   pDPHostEnumSelected = (DPHostEnumInfo*)m_GameList.GetItemData(nItemSelected);

   if( NULL == pDPHostEnumSelected )
   {
      WinMessageBox(TEXT("There are no games to join."),
         TEXT("Terra Figher"),
         GetDisplay(),GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),this);
      hr = S_OK;
      goto LCleanReturn;
   }

   m_bConnecting = TRUE;

   // Set the peer info
   WCHAR wszPeerName[MAX_PATH];
   DXUtil::ConvertGenericStringToWide( wszPeerName, m_strLocalPlayerName, MAX_PATH );

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

   ResetEvent( m_hConnectCompleteEvent );

   // Connect to an existing session. DPNCONNECT_OKTOQUERYFORADDRESSING allows
   // DirectPlay to prompt the user using a dialog box for any device address
   // or host address information that is missing
   // We also pass in copies of the app desc and host addr, since pDPHostEnumSelected
   // might be deleted from another thread that calls SessionsDlgExpireOldHostEnums().
   // This process could also be done using reference counting instead.
   hr = m_pDP->Connect( pDPHostEnumSelected->pAppDesc,       // the application desc
      pDPHostEnumSelected->pHostAddr,      // address of the host of the session
      pDPHostEnumSelected->pDeviceAddr,    // address of the local device the enum responses were received on
      NULL, NULL,                          // DPN_SECURITY_DESC, DPN_SECURITY_CREDENTIALS
      NULL, 0,                             // user data, user data size
      NULL,                                // player context,
      NULL, &m_hConnectAsyncOp,            // async context, async handle,
      DPNCONNECT_OKTOQUERYFORADDRESSING ); // flags
   if( FAILED(hr) && hr != E_PENDING )
   {
      sys_msg_hr( TEXT("Connect"), hr );
      goto LCleanReturn;
   }

   // Set a timer to wait for m_hConnectCompleteEvent to be signaled.
   // This will tell us when DPN_MSGID_CONNECT_COMPLETE has been processed
   // which lets us know if the connect was successful or not.
   m_ConnectCompleteTimer = TRUE;
   //SetTimer( hDlg, TIMERID_CONNECT_COMPLETE, 100, NULL );

   // Disable the create/join buttons until connect succeeds or fails
   GetControl(IDC_JOIN)->EnableWin(FALSE);
   //GetControl(IDC_CREATE)->EnableWin(FALSE);

   hr = S_OK;

LCleanReturn:
   LeaveCriticalSection( &m_csHostEnum );

   return hr;
}




//-----------------------------------------------------------------------------
// Name: SessionsDlgCreateGame()
// Desc: Asks the user the session name, and creates a new DirectPlay session
//-----------------------------------------------------------------------------
HRESULT CMultiplayerGamesDlg::CreateGame()
{
   HRESULT hr = S_OK;

   // Display a modal multiplayer connect dialog box.

   CMultiplayerCreateGameDlg dlg;

   dlg.Create(GetDisplay(),CPoint(10,10));
   dlg.m_Name.SetText(m_strSessionName);
   dlg.m_DPNSVR.SetChecked(m_bUseDPNSVR);
   CString porttext;
   porttext.Format("%d",m_dwPort);
   dlg.m_Port.SetText(porttext);
   dlg.SetSigningType(m_eSigningType);
   dlg.m_MigrateHost.SetChecked(m_bMigrateHost);
   // Hide the port field for service providers which don't use it
   if( !CNetConnectWizard::SPRequiresPort( &m_guidSP ) )
   {
      dlg.GetControl(IDC_LOCAL_PORT)->ShowWindow(FALSE);
      dlg.GetControl(IDC_LOCAL_PORT_TEXT)->ShowWindow(FALSE);
   }

   if( dlg.DoModal(GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),this) == IDCANCEL)
      return S_OK;

   m_strSessionName = dlg.m_Name.GetText();
   m_bUseDPNSVR = dlg.m_DPNSVR.IsChecked();
   m_dwPort= atoi(dlg.m_Port.GetText());
   m_eSigningType = dlg.m_eSigningType;
   m_bMigrateHost = dlg.m_MigrateHost.IsChecked();

   // Stop the search if we are about to connect
   if( m_bSearchingForSessions )
   {
      OnButtonPressed(IDC_SEARCH_CHECK);
   }

   m_bHostPlayer = TRUE;

   IDirectPlay8Address*   pDP8AddressHost  = NULL;
   WCHAR*                 wszHostName      = NULL;

   // Create the local host address object
   if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
      CLSCTX_ALL, IID_IDirectPlay8Address,
      (LPVOID*) &pDP8AddressHost ) ) )
   {
      sys_msg_hr( TEXT("CoCreateInstance"), hr );
      goto LCleanup;
   }

   // Set service provider
   if( FAILED( hr = pDP8AddressHost->SetSP( &m_guidSP ) ) )
   {
      sys_msg_hr( TEXT("SetSP"), hr );
      goto LCleanup;
   }

   // If were are using a service provider that requires a port value, 
   // and the user has requested a particular port number, set that
   // port as a component of the host address. If no port is specified, 
   // DirectPlay will automatically select an open port
   if( m_dwPort != 0 && CNetConnectWizard::SPRequiresPort( &m_guidSP ) )
   {
      hr = pDP8AddressHost->AddComponent( DPNA_KEY_PORT, 
         &m_dwPort, 
         sizeof(m_dwPort),
         DPNA_DATATYPE_DWORD );
      if( FAILED(hr) )
      {
         sys_msg_hr( TEXT("AddComponent"), hr );
         goto LCleanup;
      }
   }

   // Set peer info name
   WCHAR wszPeerName[MAX_PATH];
   DXUtil::ConvertGenericStringToWide( wszPeerName, m_strLocalPlayerName, MAX_PATH );

   DPN_PLAYER_INFO dpPlayerInfo;
   ZeroMemory( &dpPlayerInfo, sizeof(DPN_PLAYER_INFO) );
   dpPlayerInfo.dwSize = sizeof(DPN_PLAYER_INFO);
   dpPlayerInfo.dwInfoFlags = DPNINFO_NAME;
   dpPlayerInfo.pwszName = wszPeerName;

   // Set the peer info, and use the DPNOP_SYNC since by default this
   // is an async call.  If it is not DPNOP_SYNC, then the peer info may not
   // be set by the time we call Host() below.
   if( FAILED( hr = m_pDP->SetPeerInfo( &dpPlayerInfo, NULL, NULL, DPNOP_SYNC ) ) )
   {
      sys_msg_hr( TEXT("SetPeerInfo"), hr );
      goto LCleanup;
   }

   WCHAR wszSessionName[MAX_PATH];
   DXUtil::ConvertGenericStringToWide( wszSessionName, m_strSessionName, MAX_PATH );

   // Setup the application desc
   DPN_APPLICATION_DESC dnAppDesc;
   ZeroMemory( &dnAppDesc, sizeof(DPN_APPLICATION_DESC) );
   dnAppDesc.dwSize          = sizeof(DPN_APPLICATION_DESC);
   dnAppDesc.guidApplication = m_guidApp;
   dnAppDesc.pwszSessionName = wszSessionName;
   dnAppDesc.dwMaxPlayers    = m_dwMaxPlayers;
   dnAppDesc.dwFlags         = 0;
   if( m_bMigrateHost )
      dnAppDesc.dwFlags |= DPNSESSION_MIGRATE_HOST;

   if( !m_bUseDPNSVR )
      dnAppDesc.dwFlags |= DPNSESSION_NODPNSVR;

   if( SIGN_FAST == m_eSigningType )
      dnAppDesc.dwFlags |= DPNSESSION_FAST_SIGNED;
   else if( SIGN_FULL == m_eSigningType )
      dnAppDesc.dwFlags |= DPNSESSION_FULL_SIGNED;


   // Host a game on m_pDeviceAddress as described by dnAppDesc
   // DPNHOST_OKTOQUERYFORADDRESSING allows DirectPlay to prompt the user
   // using a dialog box for any device address information that is missing
   if( FAILED( hr = m_pDP->Host( &dnAppDesc,               // the application desc
      &pDP8AddressHost,         // array of addresses of the local devices used to connect to the host
      1,                        // number in array
      NULL, NULL,               // DPN_SECURITY_DESC, DPN_SECURITY_CREDENTIALS
      NULL,                     // player context
      DPNHOST_OKTOQUERYFORADDRESSING ) ) ) // flags
   { 
      // This error is often caused by a port conflict
      if( hr == DPNERR_INVALIDDEVICEADDRESS && 
         m_dwPort != 0 && CNetConnectWizard::SPRequiresPort( &m_guidSP ) )
      {
         WinMessageBox(	  TEXT("This error is often caused by a port conflict.\n\n")
            TEXT("If another application is already using the port you specified,\n")
            TEXT("try creating the game using a different port number."),
            TEXT("Invalid Device Address"),
            GetDisplay(),GetApp()->GetInput(),GetApp(),GetApp()->GetWindowMgr(),this);
      }

      sys_msg_hr( TEXT("Host"), hr );
      goto LCleanup;
   }

   // DirectPlay connect successful, so end dialog
   EndDialog( IDOK );

LCleanup:
   SAFE_RELEASE( pDP8AddressHost );
   SAFE_DELETE( wszHostName );

   return hr;
}


//-----------------------------------------------------------------------------
// Name: SessionsDlgEnumListCleanup()
// Desc: Deletes the linked list, g_DPHostEnumInfoHead
//-----------------------------------------------------------------------------
VOID CMultiplayerGamesDlg::EnumListCleanup()
{
   DPHostEnumInfo* pDPHostEnum = m_DPHostEnumHead.pNext;
   DPHostEnumInfo* pDPHostEnumDelete;

   while ( pDPHostEnum != &m_DPHostEnumHead )
   {
      pDPHostEnumDelete = pDPHostEnum;
      pDPHostEnum = pDPHostEnum->pNext;

      if( pDPHostEnumDelete->pAppDesc )
      {
         SAFE_DELETE_ARRAY( pDPHostEnumDelete->pAppDesc->pwszSessionName );
         SAFE_DELETE( pDPHostEnumDelete->pAppDesc );
      }

      // Changed from array delete to Release
      SAFE_RELEASE( pDPHostEnumDelete->pHostAddr );
      SAFE_RELEASE( pDPHostEnumDelete->pDeviceAddr );
      SAFE_DELETE( pDPHostEnumDelete );
   }

   // Re-link the g_DPHostEnumInfoHead circular linked list
   m_DPHostEnumHead.pNext = &m_DPHostEnumHead;
}

//-----------------------------------------------------------------------------
// Name: SessionsDlgExpireOldHostEnums
// Desc: Check all nodes to see if any have expired yet.
//-----------------------------------------------------------------------------
VOID CMultiplayerGamesDlg::ExpireOldHostEnums()
{
   DWORD dwCurrentTime = timeGetTime();

   // This is called from the dialog UI thread, SessionsDlgNoteEnumResponse
   // is called from the DirectPlay message handler threads so
   // they may also be inside it at this time, so we need to go into the
   // critical section first
   EnterCriticalSection( &m_csHostEnum );

   DPHostEnumInfo* pDPHostEnum = m_DPHostEnumHead.pNext;
   while ( pDPHostEnum != &m_DPHostEnumHead )
   {
      // Check the poll time to expire stale entries.  Also check to see if
      // the entry is already invalid.  If so, don't note that the enum list
      // changed because that causes the list in the dialog to constantly redraw.
      if( ( pDPHostEnum->bValid != FALSE ) &&
         ( pDPHostEnum->dwLastPollTime < dwCurrentTime - m_dwEnumHostExpireInterval ) )
      {
         // This node has expired, so invalidate it.
         pDPHostEnum->bValid = FALSE;
         m_bEnumListChanged  = TRUE;
      }

      pDPHostEnum = pDPHostEnum->pNext;
   }

   LeaveCriticalSection( &m_csHostEnum );
}

//-----------------------------------------------------------------------------
// Name: SessionsDlgDisplayEnumList
// Desc: Display the list of hosts in the dialog box
//-----------------------------------------------------------------------------
HRESULT CMultiplayerGamesDlg::DisplayEnumList()
{
   DPHostEnumInfo* pDPHostEnumSelected = NULL;
   GUID           guidSelectedInstance;
   BOOL           bFindSelectedGUID;
   BOOL           bFoundSelectedGUID;
   int            nItemSelected;

   // This is called from the dialog UI thread, SessionsDlgNoteEnumResponse
   // is called from the DirectPlay message handler threads so
   // they may also be inside it at this time, so we need to go into the
   // critical section first
   EnterCriticalSection( &m_csHostEnum );

   // Only update the display list if it has changed since last time
   if( !m_bEnumListChanged )
   {
      LeaveCriticalSection( &m_csHostEnum );
      return S_OK;
   }

   m_bEnumListChanged = FALSE;

   bFindSelectedGUID  = FALSE;
   bFoundSelectedGUID = FALSE;

   // Try to keep the same session selected unless it goes away or
   // there is no real session currently selected
   nItemSelected = m_GameList.GetCurSel();
   if( nItemSelected != LB_ERR )
   {
      pDPHostEnumSelected = (DPHostEnumInfo*) m_GameList.GetItemData(nItemSelected);
      if( pDPHostEnumSelected != NULL && pDPHostEnumSelected->bValid )
      {
         guidSelectedInstance = pDPHostEnumSelected->pAppDesc->guidInstance;
         bFindSelectedGUID = TRUE;
      }
   }

   // Test to see if any sessions exist in the linked list
   DPHostEnumInfo* pDPHostEnum = m_DPHostEnumHead.pNext;
   while ( pDPHostEnum != &m_DPHostEnumHead )
   {
      if( pDPHostEnum->bValid )
         break;
      pDPHostEnum = pDPHostEnum->pNext;
   }

   // If there are any sessions in list,
   // then add them to the listbox
   if( pDPHostEnum != &m_DPHostEnumHead )
   {
      // Clear the contents from the list box and enable the join button
      m_GameList.ResetContent();

      // Enable the join button only if not already connecting to a game
      if( !m_bConnecting )
         GetControl(IDC_JOIN)->EnableWin(TRUE);

      pDPHostEnum = m_DPHostEnumHead.pNext;
      while ( pDPHostEnum != &m_DPHostEnumHead )
      {
         // Add host to list box if it is valid
         if( pDPHostEnum->bValid )
         {
            int nIndex = m_GameList.AddItemData(pDPHostEnum->szSession,(LPARAM)pDPHostEnum);

            if( bFindSelectedGUID )
            {
               // Look for the session the was selected before
               if( pDPHostEnum->pAppDesc->guidInstance == guidSelectedInstance )
               {
                  m_GameList.SetCurSel(nIndex);
                  bFoundSelectedGUID = TRUE;
               }
            }
         }

         pDPHostEnum = pDPHostEnum->pNext;
      }

      if( !bFindSelectedGUID || !bFoundSelectedGUID )
         m_GameList.SetCurSel(0);
   }
   else
   {
      // There are no active session, so just reset the listbox
      InitListbox();
   }

   LeaveCriticalSection( &m_csHostEnum );

   return S_OK;
}




void CMultiplayerGamesDlg::OnDistroy()
{
   StopEnumHosts();
}

//-----------------------------------------------------------------------------
// Name: SessionsDlgNoteEnumResponse()
// Desc: Stores them in the linked list, m_DPHostEnumHead.  This is
//       called from the DirectPlay message handler so it could be
//       called simultaneously from multiple threads.
//-----------------------------------------------------------------------------
HRESULT CMultiplayerGamesDlg::NoteEnumResponse( PDPNMSG_ENUM_HOSTS_RESPONSE pEnumHostsResponseMsg )
{
   HRESULT hr = S_OK;
   BOOL    bFound;

   // This function is called from the DirectPlay message handler so it could be
   // called simultaneously from multiple threads, so enter a critical section
   // to assure that it we don't get race conditions.  Locking the entire
   // function is crude, and could be more optimal but is effective for this
   // simple sample
   EnterCriticalSection( &m_csHostEnum );

   DPHostEnumInfo* pDPHostEnum          = m_DPHostEnumHead.pNext;
   DPHostEnumInfo* pDPHostEnumNext      = NULL;
   const DPN_APPLICATION_DESC* pResponseMsgAppDesc =
      pEnumHostsResponseMsg->pApplicationDescription;

   // Look for a matching session instance GUID.
   bFound = FALSE;
   while ( pDPHostEnum != &m_DPHostEnumHead )
   {
      if( pResponseMsgAppDesc->guidInstance == pDPHostEnum->pAppDesc->guidInstance )
      {
         bFound = TRUE;
         break;
      }

      pDPHostEnumNext = pDPHostEnum;
      pDPHostEnum = pDPHostEnum->pNext;
   }

   if( !bFound )
   {
      m_bEnumListChanged = TRUE;

      // If there's no match, then look for invalid session and use it
      pDPHostEnum = m_DPHostEnumHead.pNext;
      while ( pDPHostEnum != &m_DPHostEnumHead )
      {
         if( !pDPHostEnum->bValid )
            break;

         pDPHostEnum = pDPHostEnum->pNext;
      }

      // If no invalid sessions are found then make a new one
      if( pDPHostEnum == &m_DPHostEnumHead )
      {
         // Found a new session, so create a new node
         pDPHostEnum = new DPHostEnumInfo;
         if( NULL == pDPHostEnum )
         {
            hr = E_OUTOFMEMORY;
            goto LCleanup;
         }

         ZeroMemory( pDPHostEnum, sizeof(DPHostEnumInfo) );

         // Add pDPHostEnum to the circular linked list, m_DPHostEnumHead
         pDPHostEnum->pNext = m_DPHostEnumHead.pNext;
         m_DPHostEnumHead.pNext = pDPHostEnum;
      }
   }

   // Update the pDPHostEnum with new information
   TCHAR strName[MAX_PATH];
   if( pResponseMsgAppDesc->pwszSessionName )
   {
      DXUtil::ConvertWideStringToGeneric( strName, pResponseMsgAppDesc->pwszSessionName, MAX_PATH );
   }

   // Cleanup any old enum
   if( pDPHostEnum->pAppDesc )
   {
      SAFE_DELETE_ARRAY( pDPHostEnum->pAppDesc->pwszSessionName );
      SAFE_DELETE( pDPHostEnum->pAppDesc );
   }
   SAFE_RELEASE( pDPHostEnum->pHostAddr );
   SAFE_RELEASE( pDPHostEnum->pDeviceAddr );

   //
   // Duplicate pEnumHostsResponseMsg->pAddressSender in pDPHostEnum->pHostAddr.
   // Duplicate pEnumHostsResponseMsg->pAddressDevice in pDPHostEnum->pDeviceAddr.
   //
   if( FAILED( hr = pEnumHostsResponseMsg->pAddressSender->Duplicate( &pDPHostEnum->pHostAddr ) ) )
   {
      sys_msg_hr( TEXT("Duplicate"), hr );
      goto LCleanup;
   }

   if( FAILED( hr = pEnumHostsResponseMsg->pAddressDevice->Duplicate( &pDPHostEnum->pDeviceAddr ) ) )
   {
      sys_msg_hr( TEXT("Duplicate"), hr );
      goto LCleanup;
   }

   // Deep copy the DPN_APPLICATION_DESC from
   pDPHostEnum->pAppDesc = new DPN_APPLICATION_DESC;
   if( NULL == pDPHostEnum->pAppDesc )
   {
      hr = E_OUTOFMEMORY;
      sys_msg_hr( TEXT("SessionsDlgNoteEnumResponse"), hr );
      goto LCleanup;
   }

   ZeroMemory( pDPHostEnum->pAppDesc, sizeof(DPN_APPLICATION_DESC) );
   memcpy( pDPHostEnum->pAppDesc, pResponseMsgAppDesc, sizeof(DPN_APPLICATION_DESC) );
   if( pResponseMsgAppDesc->pwszSessionName )
   {
      pDPHostEnum->pAppDesc->pwszSessionName = new WCHAR[ wcslen(pResponseMsgAppDesc->pwszSessionName)+1 ];
      wcscpy( pDPHostEnum->pAppDesc->pwszSessionName,
         pResponseMsgAppDesc->pwszSessionName );
   }

   // Update the time this was done, so that we can expire this host
   // if it doesn't refresh w/in a certain amount of time
   pDPHostEnum->dwLastPollTime = timeGetTime();

   // Check to see if the current number of players changed
   TCHAR szSessionTemp[MAX_PATH];
   if( pResponseMsgAppDesc->dwMaxPlayers > 0 )
   {
      _sntprintf( szSessionTemp, MAX_PATH-1, TEXT("%s (%d/%d) (%dms)"), strName,
         pResponseMsgAppDesc->dwCurrentPlayers,
         pResponseMsgAppDesc->dwMaxPlayers,
         pEnumHostsResponseMsg->dwRoundTripLatencyMS );

      // Null terminate
      szSessionTemp[ MAX_PATH-1 ] = 0;
   }
   else
   {
      _sntprintf( szSessionTemp, MAX_PATH-1, TEXT("%s (%d) (%dms)"), strName,
         pResponseMsgAppDesc->dwCurrentPlayers,
         pEnumHostsResponseMsg->dwRoundTripLatencyMS );

      // Null terminate
      szSessionTemp[ MAX_PATH-1 ] = 0;
   }

   // if this node was previously invalidated, or the session name is now
   // different the session list in the dialog needs to be updated
   if( ( pDPHostEnum->bValid == FALSE ) ||
      ( _tcscmp( pDPHostEnum->szSession, szSessionTemp ) != 0 ) )
   {
      m_bEnumListChanged = TRUE;
   }
   _tcscpy( pDPHostEnum->szSession, szSessionTemp );

   // This host is now valid
   pDPHostEnum->bValid = TRUE;

LCleanup:
   LeaveCriticalSection( &m_csHostEnum );

   return hr;
}
