//----------------------------------------------------------------------------
// File: ChatPeer.cpp
//
// Desc: The main game file for the ChatPeer sample.  It connects 
//       players together with two dialog boxes to prompt users on the 
//       connection settings to join or create a session. After the user 
//       connects to a session, the sample displays a multiplayer stage. 
// 
//       After a new game has started the sample begins a very simplistic 
//       chat session where users can send text to each other.
//
// Copyright (c) 1999-2001 Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef STRICT
#define STRICT
#endif // !STRICT

#include <windows.h>
#include <commctrl.h>
#include <basetsd.h>

#include "resource.h"


//-----------------------------------------------------------------------------
// Platform-dependent includes
//-----------------------------------------------------------------------------
#ifdef UNDER_CE
    #include <aygshell.h>
#else
    #include <richedit.h>
#endif // UNDER_CE


#include "NetworkConnection.h"


//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------
#define MAX_CHAT_STRINGS        50
#define MAX_CHAT_STRING_LENGTH  508
#define WM_APP_UPDATE_STATS     (WM_APP + 0)
#define WM_APP_CHAT             (WM_APP + 1)
#define CHATPEER_PORT           2502


// This GUID allows DirectPlay to find other instances of the same game on
// the network.  So it must be unique for every game, and the same for 
// every instance of that game.  // {876A3036-FFD7-46bc-9209-B42F617B9BE7}
GUID g_guidApp = { 0x876a3036, 0xffd7, 0x46bc, { 0x92, 0x9, 0xb4, 0x2f, 0x61, 0x7b, 0x9b, 0xe7 } };


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

HINSTANCE          g_hInst                       = NULL;    // HINST of app
HWND               g_hDlg                        = NULL;    // HWND of main dialog
TCHAR              g_strAppName[256]             = TEXT("ChatPeer");
HRESULT            g_hrDialog;                              // Exit code for app 



//-----------------------------------------------------------------------------
// App specific DirectPlay messages and structures 
//-----------------------------------------------------------------------------
#define GAME_MSGID_CHAT    1

// Change compiler pack alignment to be BYTE aligned, and pop the current value
#pragma pack( push, 1 )

struct GAMEMSG_GENERIC
{
    BYTE nType;
};

struct GAMEMSG_CHAT : public GAMEMSG_GENERIC
{
    char strChatString[MAX_CHAT_STRING_LENGTH];
};

// Pop the old pack alignment
#pragma pack( pop )



CNetworkConnection g_NetConnect;






//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
INT_PTR CALLBACK ChatDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
HRESULT        OnInitDialog( HWND hDlg );
HRESULT        SendChatMessage( HWND hDlg );
HRESULT        RefreshStatusBarText( DPNID idLocal, DPNID idHost );
HRESULT        GetHostnamePortString( TCHAR* str, LPDIRECTPLAY8ADDRESS pAddress, DWORD dwBufferLen );




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point for the application.  Since we use a simple dialog for 
//       user interaction we don't need to pump messages.
//-----------------------------------------------------------------------------
INT APIENTRY _tWinMain( HINSTANCE hInst, HINSTANCE hPrevInst, 
                        LPTSTR pCmdLine, INT nCmdShow )
{
    BOOL    bConnectSuccess = FALSE;

#ifdef UNDER_CE
    // This is needed along with a hidden control in the RC file to make the
    // soft keyboard pop up when the user selects an edit control.
    SHInitExtraControls();
#endif // UNDER_CE

    InitCommonControls();

    g_hInst = hInst;

   
    // Init COM so we can use CoCreateInstance
    CoInitializeEx( NULL, COINIT_MULTITHREADED );

    bConnectSuccess = g_NetConnect.Init(hInst,g_strAppName,&g_guidApp,CHATPEER_PORT);

    if( bConnectSuccess )
    {
        // App is now connected via DirectPlay, so start the game.  

        // For this sample, we just start a simple dialog box game.
        g_hrDialog = S_OK;
        DialogBox( hInst, MAKEINTRESOURCE(IDD_MAIN_GAME), NULL, (DLGPROC) ChatDlgProc );

        if( FAILED( g_hrDialog ) )
        {
            if( g_hrDialog == DPNERR_CONNECTIONLOST )
            {
                MessageBox( NULL, TEXT("The DirectPlay session was lost. ")
                            TEXT("The sample will now quit."),
                            TEXT("DirectPlay Sample"), MB_OK | MB_ICONERROR );
            }
            else
            {
                DEBUG_MSG_HR( TEXT("DialogBox"), g_hrDialog );
                MessageBox( NULL, TEXT("An error occured during the game. ")
                            TEXT("The sample will now quit."),
                            TEXT("DirectPlay Sample"), MB_OK | MB_ICONERROR );
            }
        }
    }

    // Cleanup DirectPlay and helper classes
    
    g_NetConnect.Shutdown();
    
    CoUninitialize();

    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: ChatDlgProc()
// Desc: Handles dialog messages
//-----------------------------------------------------------------------------
INT_PTR CALLBACK ChatDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg ) 
    {
        case WM_INITDIALOG:
        {
#if defined(WIN32_PLATFORM_PSPC) && (_WIN32_WCE >= 300)
            

            SHINITDLGINFO   shidi;
            memset( &shidi, 0, sizeof(SHINITDLGINFO) );
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
            shidi.hDlg = hDlg;

            SetForegroundWindow( hDlg );
            SHInitDialog( &shidi );
#endif // WIN32_PLATFORM_PSPC

            g_NetConnect.SetWnd(hDlg);
            g_hDlg = hDlg;
            if( FAILED( g_hrDialog = OnInitDialog( hDlg ) ) )
            {
                DEBUG_MSG_HR( TEXT("OnInitDialog"), g_hrDialog );
                EndDialog( hDlg, 0 );
            }
            break;
        }

        case WM_ADD_PLAYER:
        case WM_DESTROY_PLAYER:
        case WM_NET_MIGRATE:
        {
            // Update the number of players in the game
            TCHAR strNumberPlayers[32];

            wsprintf( strNumberPlayers, TEXT("%d"), g_NetConnect.m_lNumberOfActivePlayers );
            SetDlgItemText( hDlg, IDC_NUM_PLAYERS, strNumberPlayers );

            // Update the status bar text
            RefreshStatusBarText( g_NetConnect.m_dpnidLocalPlayer, g_NetConnect.m_dpnidHostPlayer );
            break;
        }

        case WM_NET_MESSAGE:
        {
            HRESULT          hr;
            
            CNetworkQueueData* pQueuedData = (CNetworkQueueData*) lParam;
            DPNID dpnidPlayer = pQueuedData->dpnidPlayer;

            CNetworkPlayerInfo* pPlayerInfo = NULL;

            g_NetConnect.PlayerLock(); // enter player context CS

            // Get the player context accosicated with this DPNID
            hr = g_NetConnect.GetPlayerContext( dpnidPlayer,&pPlayerInfo);

            PLAYER_ADDREF( pPlayerInfo ); // addref player, since we are using it now
            g_NetConnect.PlayerUnlock(); // leave player context CS

            

            if( FAILED(hr) || pPlayerInfo == NULL )
            {
                // The player who sent this may have gone away before this 
                // message was handled, so just ignore it
                if( pQueuedData )
                {
                     g_NetConnect.ReturnBuffer(pQueuedData);
                     SAFE_DELETE( pQueuedData );
                }
                break;
            }
            
            // Add the message to the local listbox
            HWND hWndChatBox = GetDlgItem( hDlg, IDC_CHAT_LISTBOX );
            int nCount = (int)SendMessage( hWndChatBox, LB_GETCOUNT, 0, 0 );
            if( nCount > MAX_CHAT_STRINGS )
                SendMessage( hWndChatBox, LB_DELETESTRING, 0, 0 );

            // Make the chat string from the player's name and the edit box string
            TCHAR strChatBuffer[MAX_PLAYER_NAME + MAX_CHAT_STRING_LENGTH + 32];

            // Validate incoming data: A malicious user could modify or create an application
            // to send bogus information; to help guard against logical errors and denial 
            // of service attacks, the size of incoming data should be checked against what
            // is expected.
            if( pQueuedData->dwSize < sizeof(GAMEMSG_CHAT) )
                break;

            GAMEMSG_CHAT* pMsg = (GAMEMSG_CHAT*) pQueuedData->pMsg;

            if(pMsg->nType != GAME_MSGID_CHAT)
                break;

            wsprintf( strChatBuffer, TEXT("<%s> %hs"), pPlayerInfo->strPlayerName, pMsg->strChatString );

            g_NetConnect.PlayerLock(); // enter player context CS
            PLAYER_RELEASE( pPlayerInfo );  // Release player and cleanup if needed
            g_NetConnect.PlayerUnlock(); // leave player context CS           

            // Add it, and make sure it is visible
            int nIndex = (int)SendMessage( hWndChatBox, LB_ADDSTRING, 0, (LPARAM)strChatBuffer );
            SendMessage( hWndChatBox, LB_SETTOPINDEX, nIndex, 0 );

            // Done with the buffer, so return it DirectPlay, 
            // so that the memory can be reused
            g_NetConnect.ReturnBuffer( pQueuedData );
            SAFE_DELETE( pQueuedData );
            break;
        }

        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {
                case IDC_CHAT_EDIT:
                    if( HIWORD(wParam) == EN_UPDATE )
                    {
                        BOOL bEnableSend;
                        if( 0 == GetWindowTextLength( GetDlgItem( hDlg, IDC_CHAT_EDIT ) ) )
                            bEnableSend = FALSE;
                        else
                            bEnableSend = TRUE;

                        EnableWindow( GetDlgItem( hDlg, IDC_SEND ), bEnableSend );
                    }
                    break;

                case IDC_SEND:
                    // The enter key was pressed, so send out the chat message
                    if( FAILED( g_hrDialog = SendChatMessage( hDlg ) ) )
                    {
                        DEBUG_MSG_HR( TEXT("SendChatMessage"), g_hrDialog );
                        EndDialog( hDlg, 0 );
                    }
                    break;

                case IDC_INFO:
                    g_NetConnect.ShowSessionInfo(hDlg);
                    return TRUE;

                case IDCANCEL:
                    g_hrDialog = S_OK;
                    EndDialog( hDlg, 0 );
                    return TRUE;
            }
            break;
    }

    return FALSE; // Didn't handle message
}

//-----------------------------------------------------------------------------
// Name: OnInitDialog()
// Desc: Inits the dialog for the chat client.
//-----------------------------------------------------------------------------
HRESULT OnInitDialog( HWND hDlg )
{
    // Load and set the icon
    HICON hIcon = LoadIcon( g_hInst, MAKEINTRESOURCE( IDI_MAIN ) );
    SendMessage( hDlg, WM_SETICON, ICON_BIG,   (LPARAM) hIcon );  // Set big icon
    SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM) hIcon );  // Set small icon

    // Display local player's name
    SetDlgItemText( hDlg, IDC_PLAYER_NAME, g_NetConnect.m_strLocalPlayerName );

    PostMessage( hDlg, WM_APP_UPDATE_STATS, 0, 0 );
    SetFocus( GetDlgItem( hDlg, IDC_CHAT_EDIT ) );

#ifndef UNDER_CE
    SendMessage( GetDlgItem( hDlg, IDC_CHAT_EDIT ), EM_SETEVENTMASK, 0, ENM_UPDATE );
#endif // !UNDER_CE
    EnableWindow( GetDlgItem( hDlg, IDC_SEND ), FALSE );

    if( g_NetConnect.IsHostPlayer() )
        SetWindowText( hDlg, TEXT("ChatPeer (Host)") );
    else
        SetWindowText( hDlg, TEXT("ChatPeer") );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: SendChatMessage()
// Desc: Create chat string based on the editbox and send it to everyone 
//-----------------------------------------------------------------------------
HRESULT SendChatMessage( HWND hDlg )
{
    // Get length of item text
    DWORD dwEditboxBufferSize = (DWORD)SendDlgItemMessage( hDlg, IDC_CHAT_EDIT, 
                                                           WM_GETTEXTLENGTH, 0, 0 );
    if( dwEditboxBufferSize == 0 )
        return S_OK;  // Don't do anything for blank messages 

    GAMEMSG_CHAT msgChat;
    msgChat.nType = GAME_MSGID_CHAT;
#ifndef UNICODE
    GetDlgItemText( hDlg, IDC_CHAT_EDIT, msgChat.strChatString, MAX_CHAT_STRING_LENGTH );
#else
    WCHAR szTempBuffer[MAX_CHAT_STRING_LENGTH];
    GetDlgItemText( hDlg, IDC_CHAT_EDIT, szTempBuffer, MAX_CHAT_STRING_LENGTH );
    DXUtil_ConvertWideStringToAnsiCch(msgChat.strChatString, szTempBuffer, MAX_CHAT_STRING_LENGTH);
#endif // UNICODE

    g_NetConnect.SendMessageToAll(sizeof(GAMEMSG_CHAT),(BYTE*)&msgChat);

    // Blank out edit box
    SetDlgItemText( hDlg, IDC_CHAT_EDIT, TEXT("") );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RefreshStatusBarText()
// Desc: Refresh the current status bar text based on connection state
//-----------------------------------------------------------------------------
HRESULT RefreshStatusBarText( DPNID idLocal, DPNID idHost )
{
    HRESULT hr = S_OK; 
    TCHAR strStatus[ 1024 ] = {0};
    TCHAR strAddress[ 256 ] = {0};
    DWORD dwNumAddresses = 0; // Count of address objects
    LPDIRECTPLAY8ADDRESS *rpAddresses = NULL; // Range of addresses    
    DWORD i = 0; // Loop variable

    // Margin the status text
    _tcsncpy( strStatus, TEXT("   "), 1023 );
    strStatus[ 1023 ] = 0;

    if( idLocal == NULL || idHost == NULL )
    {
        // If the host or local dpnid is not set, this should mean the player
        // creation messages have not yet arrived
        _tcsncat( strStatus, TEXT("Not connected"), 1023 - lstrlen( strStatus ) );
        strStatus[ 1023 ] = 0;
        dwNumAddresses = 0;
    }
    else if( idLocal == idHost )
    {
        // Else if the host and local dpnid's are equal, the local player is
        // the session host
        _tcsncat( strStatus, TEXT("Hosting"), 1023 - lstrlen( strStatus ) );
        strStatus[ 1023 ] = 0;
        dwNumAddresses = 0;

        // Determine the buffer size needed to hold the addresses
        hr = g_NetConnect.GetDP()->GetLocalHostAddresses( rpAddresses, &dwNumAddresses, 0 );
        if( DPNERR_BUFFERTOOSMALL != hr )
            goto LCleanReturn;  

        // Allocate the array
        rpAddresses = new LPDIRECTPLAY8ADDRESS[ dwNumAddresses ];
        if( NULL == rpAddresses )
        {
            hr = E_OUTOFMEMORY;
            goto LCleanReturn;
        }

        ZeroMemory( rpAddresses, dwNumAddresses * sizeof(LPDIRECTPLAY8ADDRESS) );

        // Retrieve the addresses
        hr = g_NetConnect.GetDP()->GetLocalHostAddresses( rpAddresses, &dwNumAddresses, 0 );
        if( FAILED(hr) )
            goto LCleanReturn;  
    }
    else
    {
        // Else the local player is connected to a remote session
        _tcsncat( strStatus, TEXT("Connected"), 1023 - lstrlen( strStatus ) );
        strStatus[ 1023 ] = 0;
        dwNumAddresses = 1;

        // Allocate the array
        rpAddresses = new LPDIRECTPLAY8ADDRESS[ dwNumAddresses ];
        if( NULL == rpAddresses )
        {
            hr = E_OUTOFMEMORY;
            goto LCleanReturn;
        }

        ZeroMemory( rpAddresses, dwNumAddresses * sizeof(LPDIRECTPLAY8ADDRESS) );

        // Retrieve the address
        hr = g_NetConnect.GetDP()->GetPeerAddress( idHost, &rpAddresses[0], 0 );
        if( FAILED(hr) )
            goto LCleanReturn;
    }

    // If we have addresses to report, determine a reasonable way to output them
    if( dwNumAddresses > 0 )
    {
        // Get the provider from the DirectPlay address
        GUID guidSP;
        hr = rpAddresses[0]->GetSP( &guidSP );
        if( FAILED(hr) )
            goto LCleanReturn;

        TCHAR strProvider[ 256 ] = {0};
  
        if( CLSID_DP8SP_IPX == guidSP )
            _tcsncpy( strProvider, TEXT(" via IPX"), 256 );
        else if( CLSID_DP8SP_TCPIP == guidSP )
            _tcsncpy( strProvider, TEXT(" via TCP/IP"), 256 );
        else if( CLSID_DP8SP_MODEM == guidSP )
            _tcsncpy( strProvider, TEXT(" via Modem"), 256 );
        else if( CLSID_DP8SP_SERIAL == guidSP )
            _tcsncpy( strProvider, TEXT(" via Serial"), 256 );
        else if( CLSID_DP8SP_BLUETOOTH == guidSP )
            _tcsncpy( strProvider, TEXT(" via Bluetooth"), 256 );
        
        _tcsncat( strStatus, strProvider, 1023 - lstrlen( strStatus ) );
        strStatus[ 1023 ] = 0;

        // If we are using TCP/IP or IPX, display the address list
        if( CLSID_DP8SP_IPX == guidSP ||
            CLSID_DP8SP_TCPIP == guidSP )
        {
            // Append the addresses to the status text
            for( i=0; i < dwNumAddresses; i++ )
            {
                // Get the IP address
                if( FAILED( GetHostnamePortString( strAddress, rpAddresses[ i ], 256 ) ) )
                    break;

                // Add formatting characters
                TCHAR* strSpacing = ( i != 0 ) ? TEXT(", ") : 
                                    ( idLocal == idHost ) ? TEXT(" at ") : TEXT(" to ");                      
        
                _tcsncat( strStatus, strSpacing, 1023 - lstrlen( strStatus ) );
                strStatus[ 1023 ] = 0;


                // Add the IP address string
                _tcsncat( strStatus, strAddress, 1023 - lstrlen( strStatus ) );
                strStatus[ 1023 ] = 0;
     
            }
        }
    }

    // Margin the status text
    _tcsncat( strStatus, TEXT("   "), 1023 - lstrlen( strStatus ) );
    strStatus[ 1023 ] = 0;

    // Set the text
    SendDlgItemMessage( g_hDlg, IDC_STATUS_BAR_TEXT, WM_SETTEXT, 0, (LPARAM) strStatus );
    hr = S_OK;
  
LCleanReturn:
    for( i=0; i < dwNumAddresses; i++ )
        SAFE_RELEASE( rpAddresses[i] );

    SAFE_DELETE_ARRAY( rpAddresses );

    return hr;
}




//-----------------------------------------------------------------------------
// Name: GetHostnamePortString
// Desc: Stores the hostname and port number of the given DirectPlay 
//       address in the provided string.
//-----------------------------------------------------------------------------
HRESULT GetHostnamePortString( TCHAR* str, LPDIRECTPLAY8ADDRESS pAddress, DWORD dwBufferLen )
{
    HRESULT hr = S_OK;

    // Sanity check
    if( NULL == str || NULL == pAddress )
        return E_FAIL;

    // Get the hostname string from the DirectPlay address
    WCHAR wstrHostname[ 256 ] = {0};
    DWORD dwSize = sizeof(wstrHostname);
    DWORD dwDataType = 0;
    hr = pAddress->GetComponentByName( L"hostname", wstrHostname, &dwSize, &dwDataType );
    if( FAILED(hr) )
        return hr;

    // Convert from wide character to generic
    TCHAR strHostname[ 256 ] = {0};
    DXUtil_ConvertWideStringToGenericCch( strHostname, wstrHostname, 256 );

    // Get the port value from the DirectPlay address
    DWORD dwPort = 0;
    dwSize = sizeof(DWORD);
    hr = pAddress->GetComponentByName( L"port", &dwPort, &dwSize, &dwDataType );
    if( FAILED(hr) )
        return hr;

    // Copy the address string and null terminate the result
    _sntprintf( str, dwBufferLen, TEXT("%s:%d"), strHostname, dwPort );
    str[ dwBufferLen-1 ] = TEXT('\0');

    return S_OK;
}