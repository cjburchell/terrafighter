// SelectDeviceDlg.cpp: implementation of the CSelectDeviceDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "SelectDeviceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectDeviceDlg::CSelectDeviceDlg() : CWinDialog()
{

}

CSelectDeviceDlg::~CSelectDeviceDlg()
{

}

BOOL CSelectDeviceDlg::ValidButtonID(int Id)
{
   if(Id == IDOK || Id == IDCANCEL)
      return TRUE;
   else
      return FALSE;
}

CWinBaceObj* CSelectDeviceDlg::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   switch(id)
   {
      WIN_USE_CTRL(IDC_ADAPTER_COMBO, m_AdapterList);
      WIN_USE_CTRL(IDC_DEVICE_COMBO, m_DeviceList);
      WIN_USE_CTRL(IDC_FULLSCREENMODES_COMBO, m_FullscreenModeList);
      WIN_USE_CTRL(IDC_WINDOW, m_WindowedRadio);
      WIN_USE_CTRL(IDC_FULLSCREEN, m_FullscreenRadio);
      WIN_USE_CTRL(IDC_MULTISAMPLE_COMBO, m_MultiSampleList);
   }
   return NULL;
}

BOOL CSelectDeviceDlg::Create(C2DDraw *pDisplay, const CPoint& p,CD3DApplication* pApp)
{
   LoadDialog(IDD_SELECTDEVICE,pDisplay);
   m_pApp = pApp;

   // Working variables
   D3DAdapterInfo* pAdapter;
   D3DDeviceInfo*  pDevice;

   dwOldAdapter = pApp->m_dwAdapter;
   pAdapter     = &pApp->m_Adapters[dwOldAdapter];

   dwOldDevice  = pAdapter->dwCurrentDevice;
   pDevice      = &pAdapter->devices[dwOldDevice];

   dwOldMode    = pDevice->dwCurrentMode;
   bOldWindowed = pDevice->bWindowed;
   OldMultiSampleTypeWindowed = pDevice->MultiSampleTypeWindowed;
   OldMultiSampleTypeFullscreen = pDevice->MultiSampleTypeFullscreen;

   // New state is initially the same as the old state
   dwNewAdapter = dwOldAdapter;
   dwNewDevice  = dwOldDevice;
   dwNewMode    = dwOldMode;
   bNewWindowed = bOldWindowed;
   NewMultiSampleTypeWindowed = OldMultiSampleTypeWindowed;
   NewMultiSampleTypeFullscreen = OldMultiSampleTypeFullscreen;

   UpdateControls(TRUE);

   return CWinDialog::Create(pDisplay,NULL,p,GetSize(),GetText());
}

void CSelectDeviceDlg::OnButtonPressed(int ButtonID)
{
   // Get current UI state
   bNewWindowed  = m_WindowedRadio.IsChecked();
   // Working variables
   D3DAdapterInfo* pAdapter;
   if( IDOK == ButtonID )
   {
      // Handle the case when the user hits the OK button. Check if any
      // of the options were changed
      if( dwNewAdapter != dwOldAdapter || dwNewDevice  != dwOldDevice  ||
         dwNewMode    != dwOldMode    || bNewWindowed != bOldWindowed ||
         NewMultiSampleTypeWindowed != OldMultiSampleTypeWindowed ||
         NewMultiSampleTypeFullscreen != OldMultiSampleTypeFullscreen )
      {
         m_pApp->m_dwAdapter = dwNewAdapter;

         pAdapter = &m_pApp->m_Adapters[dwNewAdapter];
         pAdapter->dwCurrentDevice = dwNewDevice;

         pAdapter->devices[dwNewDevice].dwCurrentMode = dwNewMode;
         pAdapter->devices[dwNewDevice].bWindowed     = bNewWindowed;
         pAdapter->devices[dwNewDevice].MultiSampleTypeWindowed = NewMultiSampleTypeWindowed;
         pAdapter->devices[dwNewDevice].MultiSampleTypeFullscreen = NewMultiSampleTypeFullscreen;
      }
   }
   else if( ButtonID == IDC_ADAPTER_COMBO )
   {
      dwNewAdapter = m_AdapterList.GetCurSel();
      pAdapter     = &m_pApp->m_Adapters[dwNewAdapter];

      dwNewDevice  = pAdapter->dwCurrentDevice;
      dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
      bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
   }
   else if( ButtonID == IDC_DEVICE_COMBO )
   {
      pAdapter     = &m_pApp->m_Adapters[dwNewAdapter];

      dwNewDevice  = m_DeviceList.GetCurSel();
      dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
      bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
   }
   else if( ButtonID == IDC_FULLSCREENMODES_COMBO )
   {
      dwNewMode = m_FullscreenModeList.GetCurSel();
   }
   else if( ButtonID == IDC_MULTISAMPLE_COMBO )
   {
      DWORD dwItem = m_MultiSampleList.GetCurSel( );
      if( bNewWindowed )
         NewMultiSampleTypeWindowed = (D3DMULTISAMPLE_TYPE)dwItem;
      else
         NewMultiSampleTypeFullscreen = (D3DMULTISAMPLE_TYPE)dwItem;
   }
   else if(ButtonID == IDC_WINDOW)
   {
      bNewWindowed  = TRUE;
   }
   else if(ButtonID == IDC_FULLSCREEN)
   {
      bNewWindowed  = FALSE;
   }
   UpdateControls();
}

void CSelectDeviceDlg::UpdateControls(BOOL bInit)
{
   // Working variables
   D3DAdapterInfo* pAdapter;
   D3DDeviceInfo*  pDevice;
   // Reset the content in each of the combo boxes
   m_AdapterList.ResetContent();
   m_DeviceList.ResetContent();
   m_FullscreenModeList.ResetContent();
   m_MultiSampleList.ResetContent();

   pAdapter = &m_pApp->m_Adapters[dwNewAdapter];
   pDevice  = &pAdapter->devices[dwNewDevice];

   // Add a list of adapters to the adapter combo box
   for( DWORD a=0; a < m_pApp->m_dwNumAdapters; a++ )
   {
      // Add device name to the combo box
      DWORD dwItem = m_AdapterList.AddString(m_pApp->m_Adapters[a].d3dAdapterIdentifier.Description);

      // Set the combobox selection on the current adapater
      if( a == dwNewAdapter )
         m_AdapterList.SetCurSel(dwItem);
   }

   // Add a list of devices to the device combo box
   for( DWORD d=0; d < pAdapter->dwNumDevices; d++ )
   {
      // Add device name to the combo box
      DWORD dwItem = m_DeviceList.AddString(pAdapter->devices[d].strDesc );

      // Set the combobox selection on the current device
      if( d == dwNewDevice )
         m_DeviceList.SetCurSel(dwItem );
   }

   // Add a list of modes to the mode combo box
   for( DWORD m=0; m < pDevice->dwNumModes; m++ )
   {
      DWORD BitDepth = 16;
      if( pDevice->modes[m].Format == D3DFMT_X8R8G8B8 ||
         pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
         pDevice->modes[m].Format == D3DFMT_R8G8B8 )
      {
         BitDepth = 32;
      }

      // Add mode desc to the combo box
      TCHAR strMode[80];
      _stprintf( strMode, _T("%ld x %ld x %ld"), pDevice->modes[m].Width,
         pDevice->modes[m].Height,
         BitDepth );
      DWORD dwItem = m_FullscreenModeList.AddString(strMode );

      // Set the combobox selection on the current mode
      if( m == dwNewMode )
         m_FullscreenModeList.SetCurSel( dwItem );
   }

   // Add a list of multisample modes to the multisample combo box
   for( m=0; m <= 16; m++ )
   {
      TCHAR strDesc[50];

      D3DFORMAT fmt;
      if( bNewWindowed )
         fmt = m_pApp->m_Adapters[dwNewAdapter].d3ddmDesktop.Format;
      else
         fmt = pDevice->modes[dwNewMode].Format;

      if ( m == 1 ) // 1 is not a valid multisample type
         continue;

      if( SUCCEEDED( m_pApp->m_pD3D->CheckDeviceMultiSampleType( dwNewAdapter,
         pDevice->DeviceType, fmt, bNewWindowed, (D3DMULTISAMPLE_TYPE)m , NULL) ) )
      {
         if( m == 0 )
            lstrcpy( strDesc, _T("none") );
         else
            wsprintf( strDesc, _T("%d samples"), m );

         // Add device name to the combo box
         DWORD dwItem = m_MultiSampleList.AddString(strDesc );

         // Set the combobox selection on the current multisample type
         if( bNewWindowed )
         {
            if( (D3DMULTISAMPLE_TYPE)m == NewMultiSampleTypeWindowed || m == 0 )
               m_MultiSampleList.SetCurSel( dwItem );
         }
         else
         {
            if( (D3DMULTISAMPLE_TYPE)m == NewMultiSampleTypeFullscreen || m == 0 )
               m_MultiSampleList.SetCurSel( dwItem );
         }
      }
   }
   DWORD dwItem = m_MultiSampleList.GetCurSel();
   if( bNewWindowed )
      NewMultiSampleTypeWindowed = (D3DMULTISAMPLE_TYPE)dwItem;
   else
      NewMultiSampleTypeFullscreen = (D3DMULTISAMPLE_TYPE)dwItem;

   m_WindowedRadio.ShowWindow(pDevice->bCanDoWindowed);

   m_MultiSampleList.ShowWindow(m_MultiSampleList.GetCount() > 1);
   CWinBaceObj* pGpCtrl = GetControl(IDC_MULTI_SMPL_GB);
   if(pGpCtrl)
      pGpCtrl->ShowWindow(m_MultiSampleList.GetCount() > 1);

   CWinBaceObj* pTxtCtrl = GetControl(IDC_MULTISAMPLE_TXT);
   if(pTxtCtrl)
      pTxtCtrl->ShowWindow(m_MultiSampleList.GetCount() > 1);



   if( bNewWindowed )
   {
      if(bInit)
         m_WindowedRadio.SetChecked(TRUE);
      m_FullscreenRadio.SetChecked(FALSE);
      m_FullscreenModeList.ShowWindow(FALSE);
   }
   else
   {
      m_WindowedRadio.SetChecked(FALSE);
      if(bInit)
         m_FullscreenRadio.SetChecked(TRUE);
      m_FullscreenModeList.ShowWindow(TRUE);
   }
}
