// NetworkConnection.h: interface for the CNetworkConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKCONNECTION_H__A422405E_4090_4C96_A197_F1991BB711DC__INCLUDED_)
#define AFX_NETWORKCONNECTION_H__A422405E_4090_4C96_A197_F1991BB711DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <dplay8.h>
#include <dplobby8.h>
#include "NetConnect.h"
#include "DXUtil.h"
#include "SessionInfo.h"
#pragma warning( disable : 4786 )
#include <list>
#include "resource.h"

#define PLAYER_ADDREF( pPlayerInfo )    if( pPlayerInfo ) pPlayerInfo->lRefCount++;
#define PLAYER_RELEASE( pPlayerInfo )   if( pPlayerInfo ) { pPlayerInfo->lRefCount--; if( pPlayerInfo->lRefCount <= 0 ) SAFE_DELETE( pPlayerInfo ); } pPlayerInfo = NULL;


#define MAX_PLAYER_NAME         14

struct CNetworkPlayerInfo
{
   LONG  lRefCount;                        // Ref count so we can cleanup when all threads 
   // are done w/ this object
   DPNID dpnidPlayer;                      // DPNID of player
   TCHAR strPlayerName[MAX_PLAYER_NAME];   // Player name
};


struct CNetworkQueueData 
{
   BYTE* pMsg;
   DWORD dwSize;
   DPNHANDLE hBufferHandle;
   DPNID dpnidPlayer;                      // DPNID of player
};



class CNetworkConnection  
{
public:
   BOOL Init(HINSTANCE hInst,LPTSTR strAppName,GUID* pGuidApp,DWORD PortNumb,CInput* pInput,CD3DApplication* pApp,C2DDraw* p2dDraw);
   CNetworkConnection();
   virtual ~CNetworkConnection();
   HRESULT InitDirectPlay();

   static HRESULT WINAPI DirectPlayMessageHandler( PVOID pvUserContext, 
      DWORD dwMessageId, 
      PVOID pMsgBuffer );

   static HRESULT WINAPI DirectPlayLobbyMessageHandler( PVOID pvUserContext, 
      DWORD dwMessageId, 
      PVOID pMsgBuffer );

   virtual HRESULT OnMessage(DWORD dwMessageId,PVOID pMsgBuffer);

   void Shutdown();

   void ShowSessionInfo(HWND hParent)
   {
      if( m_pSessionInfo )
         m_pSessionInfo->ShowDialog( hParent );
   }

   BOOL IsHostPlayer()
   {
      return m_pNetConnectWizard->IsHostPlayer();
   }
   DPNID              m_dpnidHostPlayer;       // DPNID of host player
   DPNID              m_dpnidLocalPlayer;       // DPNID of local player
   LONG               m_lNumberOfActivePlayers;       // Number of players currently in game
   TCHAR              m_strLocalPlayerName[MAX_PATH];          // Local player name

   void SetWnd(HWND hWnd){m_hWnd= hWnd;}

   void PlayerLock(){EnterCriticalSection(&m_csPlayerContext);};
   void PlayerUnlock(){LeaveCriticalSection(&m_csPlayerContext);};

   void ReturnBuffer(CNetworkQueueData* pData)
   {
      m_pDP->ReturnBuffer( pData->hBufferHandle,0 );
   }


   HRESULT GetPlayerContext(DPNID dpnidPlayer,CNetworkPlayerInfo** ppInfo)
   {
      return m_pDP->GetPlayerContext( dpnidPlayer, 
         (LPVOID* const) ppInfo,
         0);
   }

   void SendMessageToAll(DWORD dwSize, BYTE* pData)
   {
      DPN_BUFFER_DESC bufferDesc;
      bufferDesc.dwBufferSize = dwSize;
      bufferDesc.pBufferData  = pData;
      // Send it to all of the players include the local client
      // DirectPlay will tell via the message handler 
      // if there are any severe errors, so ignore any errors 
      DPNHANDLE hAsync;
      m_pDP->SendTo( DPNID_ALL_PLAYERS_GROUP, &bufferDesc, 1,
         0, NULL, &hAsync, DPNSEND_GUARANTEED );
   }

   void SendMessageToOne(DPNID dpnid ,DWORD dwSize, BYTE* pData)
   {
      DPN_BUFFER_DESC bufferDesc;
      bufferDesc.dwBufferSize = dwSize;
      bufferDesc.pBufferData  = pData;
      DPNHANDLE hAsync;
      m_pDP->SendTo( dpnid, &bufferDesc, 1,
         0, NULL, &hAsync, DPNSEND_GUARANTEED );
   }

   CNetworkQueueData* GetNextMessage()
   {
      CNetworkQueueData* pData = NULL;
      EnterCriticalSection(&m_csQueue);                  // enter player context CS
      if(m_queue.size() != 0)
      {
         pData = m_queue.front();
         m_queue.pop_front();
      }
      LeaveCriticalSection(&m_csQueue);
      return pData;
   }

   HANDLE m_MessageEvent;

   IDirectPlay8Peer* GetDP(){return m_pDP;}

private:

   IDirectPlay8Peer*  m_pDP;    // DirectPlay peer object


   HWND m_hWnd;
   CRITICAL_SECTION m_csQueue;
   std::list<CNetworkQueueData*> m_queue;

   CRITICAL_SECTION m_csPlayerContext;
   CNetConnectWizard* m_pNetConnectWizard;    // Connection wizard
   CSessionInfo*      m_pSessionInfo;    // Session Information
   IDirectPlay8LobbiedApplication* m_pLobbiedApp;    // DirectPlay lobbied app 
   BOOL               m_bWasLobbyLaunched;   // TRUE if lobby launched
   TCHAR              m_strSessionName[MAX_PATH];              // Session name
   TCHAR              m_strPreferredProvider[MAX_PATH];        // Provider string

};

#endif // !defined(AFX_NETWORKCONNECTION_H__A422405E_4090_4C96_A197_F1991BB711DC__INCLUDED_)
