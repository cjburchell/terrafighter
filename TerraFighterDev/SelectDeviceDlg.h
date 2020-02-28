// SelectDeviceDlg.h: interface for the CSelectDeviceDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTDEVICEDLG_H__32E4293C_DE57_44CE_A082_28A8DB0395D8__INCLUDED_)
#define AFX_SELECTDEVICEDLG_H__32E4293C_DE57_44CE_A082_28A8DB0395D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinComboBox.h"
#include "WinCheckBox.h"

class CSelectDeviceDlg : public CWinDialog  
{
public:
   CSelectDeviceDlg();
   virtual ~CSelectDeviceDlg();

   CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);
   BOOL Create(C2DDraw *pDisplay,const CPoint& p,CD3DApplication* pApp);
   BOOL ValidButtonID(int Id);
   virtual void OnButtonPressed(int ButtonID);

   void UpdateControls(BOOL bInit = FALSE);
private:
   DWORD  dwOldAdapter, dwNewAdapter;
   DWORD  dwOldDevice,  dwNewDevice;
   DWORD  dwOldMode,    dwNewMode;
   BOOL   bOldWindowed, bNewWindowed;
   D3DMULTISAMPLE_TYPE OldMultiSampleTypeWindowed, NewMultiSampleTypeWindowed;
   D3DMULTISAMPLE_TYPE OldMultiSampleTypeFullscreen, NewMultiSampleTypeFullscreen;

   CWinComboBox m_AdapterList;
   CWinComboBox m_DeviceList;
   CWinComboBox m_FullscreenModeList;
   CWinCheckBox m_WindowedRadio;
   CWinCheckBox m_FullscreenRadio;
   CWinComboBox m_MultiSampleList;

   CD3DApplication* m_pApp;
};

#endif // !defined(AFX_SELECTDEVICEDLG_H__32E4293C_DE57_44CE_A082_28A8DB0395D8__INCLUDED_)
