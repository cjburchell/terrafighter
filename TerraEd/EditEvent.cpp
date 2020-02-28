///////////////////////////////////////////////////////////////////////////////
///  EditEvent.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:22:25 PM by Christiaan Burchell
///
///  PURPOSE:  Edit Event Dialog
///
///  COPYRIGHT NOTICE:   Copyright (C) 2004 Redpoint Games 
///
///   This software is provided 'as-is', without any express or implied 
///   warranty.  In no event will the author be held liable for any damages 
///   arising from the use of this software. 
/// 
///   Permission is granted to anyone to use this software for any purpose,
///   excluding commercial applications, and to alter it and redistribute it
///   freely, subject to the following restrictions:
/// 
///   1. The origin of this software must not be misrepresented; you must not
///      claim that you wrote the original software. If you use this software
///      in a product, an acknowledgment in the product documentation would be
///      appreciated but is not required.
///   2. Altered source versions must be plainly marked as such, and must not be
///      misrepresented as being the original software.
///   3. This notice may not be removed or altered from any source distribution.
///   4. The author permission is required to use this software in commercial 
///      applications 
///
///  LAST CHANGED: $Date$
///
///  REVISION HISTORY:
///  $Log$
/// 

// EditEvent.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "EditEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditEvent dialog




/////////////////////////////////////////////////
///
/// NAME: CEditEvent
///
/// CLASS: CEditEvent
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:22:30 PM
///
/// PARAMETERS: 
///             CGameEvent* pEvent
///             CLevelData* pLevel
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CEditEvent::CEditEvent(CGameEvent* pEvent,CLevelData* pLevel,CWnd* pParent /*=NULL*/)
: CDialog(CEditEvent::IDD, pParent)
{
   //{{AFX_DATA_INIT(CEditEvent)
   //}}AFX_DATA_INIT

   m_pEvent = pEvent;
   m_pLevel = pLevel;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CEditEvent
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:22:32 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditEvent::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CEditEvent)
   DDX_Control(pDX, IDC_TYPE, m_Type_Ctrl);
   DDX_Control(pDX, IDC_ACTIVATE_EVENT, m_ActvateEventCtrl);
   DDX_Control(pDX, IDC_RESP_NAME, m_RespCtrl);
   DDX_Control(pDX, IDC_OBJ_NAME, m_ObjNameCtrl);
   //}}AFX_DATA_MAP

   DDX_Text(pDX, IDC_NAME, m_pEvent->m_Name);

   int temp = m_pEvent->m_EventType;
   DDX_CBIndex(pDX, IDC_TYPE, temp);
   m_pEvent->m_EventType = (EventType_e)temp;

   temp = m_pEvent->m_GameState;
   DDX_CBIndex(pDX, IDC_GAME_STATE, temp);
   m_pEvent->m_GameState = (GameState_e)temp;

   DDX_Check(pDX, IDC_ACTIVE, m_pEvent->m_bActive);
   DDX_Text(pDX, IDC_TIMER, m_pEvent->m_Timer);
   DDX_CBString(pDX, IDC_OBJ_NAME, m_pEvent->m_ObjectName);
   DDX_CBString(pDX, IDC_RESP_NAME, m_pEvent->m_ResponsName);

   DDX_Text(pDX, IDC_MESSAGE, m_pEvent->m_Message);
   DDX_CBString(pDX, IDC_ACTIVATE_EVENT, m_pEvent->m_EventToActivate);
}


BEGIN_MESSAGE_MAP(CEditEvent, CDialog)
   //{{AFX_MSG_MAP(CEditEvent)
   ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditEvent message handlers



/////////////////////////////////////////////////
///
/// NAME: OnInitDialog
///
/// CLASS: CEditEvent
///
/// DESCRIPTION: Initializes the dialog
///
/// CREATED:     02/06/2004 5:22:35 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CEditEvent::OnInitDialog() 
{
   CDialog::OnInitDialog();

   m_RespCtrl.AddString("CURRENT_PLAYER");
   m_ObjNameCtrl.AddString("CURRENT_PLAYER");

   for(int i=0;i<m_pLevel->m_Sprites.size();i++)
   {
      m_RespCtrl.AddString(m_pLevel->m_Sprites[i]->m_Name);
      m_ObjNameCtrl.AddString(m_pLevel->m_Sprites[i]->m_Name);
   }

   for(i=0;i<m_pLevel->m_Events.size();i++)
   {
      if(m_pLevel->m_Events[i]->m_Name != m_pEvent->m_Name)
      {
         m_ActvateEventCtrl.AddString(m_pLevel->m_Events[i]->m_Name);
      }

      for(int j=0;j<m_pLevel->m_Events[i]->m_CreateSprites.size();j++)
      {
         m_RespCtrl.AddString(m_pLevel->m_Events[i]->m_CreateSprites[j]->m_Name);
         m_ObjNameCtrl.AddString(m_pLevel->m_Events[i]->m_CreateSprites[j]->m_Name);
      }
   }

   UpdateData(FALSE);

   OnSelchangeType();

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}



/////////////////////////////////////////////////
///
/// NAME: OnSelchangeType
///
/// CLASS: CEditEvent
///
/// DESCRIPTION: User selected a new type
///
/// CREATED:     02/06/2004 5:22:53 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditEvent::OnSelchangeType() 
{
   switch(m_Type_Ctrl.GetCurSel())
   {
   case EVENT_SPRITE_DISTROYED:
      GetDlgItem(IDC_TIMER)->EnableWindow(FALSE);
      GetDlgItem(IDC_OBJ_NAME)->EnableWindow();
      GetDlgItem(IDC_RESP_NAME)->EnableWindow(FALSE);
      break;
   case EVENT_HIT_WAYPOINT:
      GetDlgItem(IDC_TIMER)->EnableWindow(FALSE);
      GetDlgItem(IDC_OBJ_NAME)->EnableWindow();
      GetDlgItem(IDC_RESP_NAME)->EnableWindow();
      break;
   case EVENT_ALL_ENEMYS_DISTOYED:
      GetDlgItem(IDC_TIMER)->EnableWindow(FALSE);
      GetDlgItem(IDC_OBJ_NAME)->EnableWindow(FALSE);
      GetDlgItem(IDC_RESP_NAME)->EnableWindow(FALSE);
      break;
   case EVENT_TIMEOUT:
      GetDlgItem(IDC_TIMER)->EnableWindow();
      GetDlgItem(IDC_OBJ_NAME)->EnableWindow(FALSE);
      GetDlgItem(IDC_RESP_NAME)->EnableWindow(FALSE);
      break;
   }

}
