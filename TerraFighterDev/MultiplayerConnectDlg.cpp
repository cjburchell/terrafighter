// MultiplayerConnectDlg.cpp: implementation of the CMultiplayerConnectDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MultiplayerConnectDlg.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMultiplayerConnectDlg::CMultiplayerConnectDlg() : CWinDialog()
{

}

CMultiplayerConnectDlg::~CMultiplayerConnectDlg()
{
   GUID*   pGuid = NULL;
   DWORD   iIndex;
   DWORD   dwCount;

   dwCount = m_ConnectionList.GetCount();
   for( iIndex = 0; iIndex < dwCount; iIndex++ )
   {
      pGuid = (GUID*) m_ConnectionList.GetItemData(iIndex);
      if(pGuid)
         SAFE_DELETE( pGuid );
   }
}

BOOL CMultiplayerConnectDlg::ValidButtonID(int Id)
{
   if(Id == IDOK || Id == IDCANCEL)
      return TRUE;
   else
      return FALSE;
}

CWinBaceObj* CMultiplayerConnectDlg::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   switch(id)
   {
      WIN_USE_CTRL(IDC_PLAYER_NAME_EDIT, m_NameCtrl);
      WIN_USE_CTRL(IDC_CONNECTION_LIST, m_ConnectionList);
   }
   return NULL;
}

BOOL CMultiplayerConnectDlg::Create(C2DDraw *pDisplay, const CPoint& p,IDirectPlay8Peer* pDP)
{
   LoadDialog(IDD_MULTIPLAYER_CONNECT,pDisplay);
   if(!CWinDialog::Create(pDisplay,NULL,p,GetSize(),GetText()))
      return FALSE;

   m_NameCtrl.SetText(m_Name);
   if(FillListBox(pDP) == S_OK)
      return TRUE;
   return FALSE;
}

HRESULT CMultiplayerConnectDlg::FillListBox(IDirectPlay8Peer* pDP)
{
   HRESULT                     hr;
   int                         iLBIndex;
   DWORD                       dwItems     = 0;
   DPN_SERVICE_PROVIDER_INFO*  pdnSPInfo   = NULL;
   DWORD                       dwSize      = 0;
   TCHAR                       strName[MAX_PATH];

   // Enumerate all DirectPlay service providers, and store them in the listbox

   // Get required space for all providers
   hr = pDP->EnumServiceProviders( NULL, NULL, pdnSPInfo, &dwSize,
      &dwItems, 0 );
   if( FAILED(hr) && hr != DPNERR_BUFFERTOOSMALL )
   {
      sys_msg_hr( TEXT("EnumServiceProviders"), hr );
      goto LCleanReturn;
   }

   // Allocate required space
   pdnSPInfo = (DPN_SERVICE_PROVIDER_INFO*) new BYTE[dwSize];
   if( NULL == pdnSPInfo )
   {
      hr = E_OUTOFMEMORY;
      sys_msg_hr( TEXT("ConnectionsDlgFillListBox"), hr );
      goto LCleanReturn;
   }

   // Perform the enumeration
   hr = pDP->EnumServiceProviders( NULL, NULL, pdnSPInfo,
      &dwSize, &dwItems, 0 );
   if( FAILED(hr) )
   {
      sys_msg_hr( TEXT("EnumServiceProviders"), hr );
      goto LCleanReturn;
   }

   // For each detected provider, add an item to the listbox
   DPN_SERVICE_PROVIDER_INFO* pdnSPInfoEnum;
   pdnSPInfoEnum = pdnSPInfo;

   DWORD i;
   for ( i = 0; i < dwItems; i++ )
   {
      DXUtil::ConvertWideStringToGeneric( strName, pdnSPInfoEnum->pwszName, MAX_PATH );

      // Found a service provider, so put it in the listbox
      // Store pointer to GUID in listbox
      GUID* pGuid = new GUID;
      if( NULL == pGuid )
      {
         hr = E_OUTOFMEMORY;
         sys_msg_hr( TEXT("ConnectionsDlgFillListBox"), hr );
         goto LCleanReturn;
      }

      memcpy( pGuid, &pdnSPInfoEnum->guid, sizeof(GUID) );
      iLBIndex =m_ConnectionList.AddItemData(strName,(DWORD)pGuid);

      // Advance to next provider
      pdnSPInfoEnum++;
   }

   m_ConnectionList.AddItemData("Wait for Lobby Connection",0);

   iLBIndex = m_ConnectionList.FindString(m_strPreferredProvider);	

   if( iLBIndex != -1 )
      m_ConnectionList.SetCurSel(iLBIndex);
   else
      m_ConnectionList.SetCurSel(0);


   hr = S_OK;

LCleanReturn:
   SAFE_DELETE_ARRAY( pdnSPInfo );

   return hr;
}
