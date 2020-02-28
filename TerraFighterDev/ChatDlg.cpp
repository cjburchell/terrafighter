// ChatDlg.cpp: implementation of the CChatDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatDlg.h"
#include "NetMessages.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatDlg::CChatDlg() : CWinDialog()
{
   m_pNetConnect = NULL;
}

CChatDlg::~CChatDlg()
{

}

BOOL CChatDlg::ValidButtonID(int Id)
{
   return Id == IDCANCEL;
}

BOOL CChatDlg::Create(C2DDraw *pDisplay, const CPoint& p)
{    
   LoadDialog(IDD_CHATBOX,pDisplay);
   if(!CWinDialog::Create(pDisplay,NULL,p,GetSize(),GetText()))
      return FALSE;
   return TRUE;
}

CWinBaceObj* CChatDlg::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   switch(id)
   {
      WIN_USE_CTRL(IDC_CHAT_LIST, m_List);
      WIN_USE_CTRL(IDC_CHAT_EDIT, m_Edit);
   }
   return NULL;
}

void CChatDlg::OnButtonPressed(int ButtonID)
{
   switch(ButtonID)
   {
   case IDC_CHAT_EDIT:
   case ID_SEND:
      {
         int index = m_List.AddString(m_Edit.GetText());
         m_List.SetVisable(index);
         if(m_pNetConnect)
         {
            GAMEMSG_CHAT msg;
            msg.nType = GAME_MSGID_CHAT;
            ::_snprintf(msg.strChatString,MAX_CHAT_STRING_LENGTH,m_Edit.GetText());
            m_pNetConnect->SendMessageToAll(sizeof(GAMEMSG_CHAT),(BYTE*)&msg);
         }
         m_Edit.SetText("");
      }
      break;
   }
}

void CChatDlg::AddString(const TCHAR* str,D3DCOLOR Color)
{
   int index = m_List.AddString(str,Color);
   m_List.SetVisable(index);
}
