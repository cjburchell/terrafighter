// SelectLevelDlg.h: interface for the CSelectLevelDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTLEVELDLG_H__DEB5BF2B_0350_4BF5_A04A_D46C75E4E6C1__INCLUDED_)
#define AFX_SELECTLEVELDLG_H__DEB5BF2B_0350_4BF5_A04A_D46C75E4E6C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinStatic.h"
#include "WinCheckBox.h"
#include "WinArrowButton.h"
#include "TerrafighterApp.h"
#include "WinListBox.h"
#include "WinComboBox.h"
#include "WinBitmapCtrl.h"


class CSelectLevelDlg : public CWinDialog  
{
public:
   CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);
   CSelectLevelDlg(CLevelData* pLevel,DWORD MaxMaps);
   virtual ~CSelectLevelDlg();
   BOOL Create(C2DDraw *pDisplay,const CPoint& p);
   BOOL ValidButtonID(int Id);
   virtual void OnButtonPressed(int ButtonID);
   int m_SelMap;
private:
   void UpdateControls();
   CLevelData* m_pLevel;
   int m_MaxMaps;
   CWinArrowButton m_next;
   CWinArrowButton m_prev;
   CWinBitmapCtrl m_bmp;
   CWinStatic m_LevelName;
   CWinStatic m_Discription;
};

#endif // !defined(AFX_SELECTLEVELDLG_H__DEB5BF2B_0350_4BF5_A04A_D46C75E4E6C1__INCLUDED_)
