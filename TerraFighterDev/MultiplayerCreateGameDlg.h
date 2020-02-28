// MultiplayerCreateGameDlg.h: interface for the CMuliplayerCreateGameDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULIPLAYERCREATEGAMEDLG_H__A3AE472B_ED78_47C7_AE9D_8952C44D93CA__INCLUDED_)
#define AFX_MULIPLAYERCREATEGAMEDLG_H__A3AE472B_ED78_47C7_AE9D_8952C44D93CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinCheckBox.h"
#include "WinEditBox.h"
#include <dplay8.h>

enum SigningType { SIGN_NONE, SIGN_FAST, SIGN_FULL };


class CMultiplayerCreateGameDlg : public CWinDialog  
{
public:
   CMultiplayerCreateGameDlg();
   virtual ~CMultiplayerCreateGameDlg();
   BOOL ValidButtonID(int Id);
   CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);
   BOOL Create(C2DDraw *pDisplay, const CPoint& p);
   void OnButtonPressed(int ButtonID);

   void SetSigningType(SigningType type)
   {
      m_eSigningType = type;
      m_SigningFull.SetChecked(FALSE);
      m_SigningFast.SetChecked(FALSE);
      m_SigningDisabled.SetChecked(FALSE);
      switch(type)
      {
      case SIGN_FULL:
         m_SigningFull.SetChecked(TRUE);
         break;
      case SIGN_FAST:
         m_SigningFast.SetChecked(TRUE);
         break;
      case SIGN_NONE:
         m_SigningDisabled.SetChecked(TRUE);
         break;
      }
   }

   CWinEditBox m_Name;
   CWinEditBox m_Port;
   CWinCheckBox m_MigrateHost;
   CWinCheckBox m_DPNSVR;
   CWinCheckBox m_SigningFull;
   CWinCheckBox m_SigningFast;
   CWinCheckBox m_SigningDisabled;

   SigningType	m_eSigningType;

};

#endif // !defined(AFX_MULIPLAYERCREATEGAMEDLG_H__A3AE472B_ED78_47C7_AE9D_8952C44D93CA__INCLUDED_)
