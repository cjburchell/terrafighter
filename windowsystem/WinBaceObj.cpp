///////////////////////////////////////////////////////////////////////////////
///  WinBaceObj.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 9:58:24 AM by Christiaan Burchell
///
///  PURPOSE:  implementation of the CWinBaceObj class.
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

// WinBaceObj.cpp: implementation of the CWinBaceObj class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "WinBaceObj.h"
#include "WinButton.h"
#include "WinStatic.h"
#include "WinCheckBox.h"
#include "WinGroupBox.h"
#include "WinScrollBar.h"
#include "WinListBox.h"
#include "WinComboBox.h"
#include "WinEditBox.h"
#include "dxutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinBaceObj
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 9:58:33 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinBaceObj::CWinBaceObj()
{
   m_pParent = NULL;
   m_text = "";
   m_bCreated = FALSE;


   DXUtil::ReadIntRegKey("ForgoundColor",&m_ForgoundColor,D3DCOLOR_XRGB(255,255,255)); // text color
   DXUtil::ReadIntRegKey("HighlightColor",&m_HighlightColor,D3DCOLOR_XRGB(0,255,255)); //highlighted text color
   DXUtil::ReadIntRegKey("DisabledColor",&m_DisabledColor,D3DCOLOR_XRGB(0,128,128)); // disabled text color

   // border colors
   DXUtil::ReadIntRegKey("BorderColor",&m_BorderColor,D3DCOLOR_XRGB(0,0,0));
   DXUtil::ReadIntRegKey("BorderColor2",&m_BorderColor2,D3DCOLOR_XRGB(0,255,255));
   DXUtil::ReadIntRegKey("BorderColor3",&m_BorderColor3,D3DCOLOR_XRGB(0,0,100));

   // solid colors
   DXUtil::ReadIntRegKey("EditAreaColor",&m_EditAreaColor,D3DCOLOR_ARGB(192,0,0,128));
   DXUtil::ReadIntRegKey("BackgroundColor",&m_BackgroundColor,D3DCOLOR_ARGB(192,0,0,128));
   DXUtil::ReadIntRegKey("TitleBarColor",&m_TitleBarColor,D3DCOLOR_ARGB(192,0,0,255));
   DXUtil::ReadIntRegKey("SelectColor",&m_SelectColor,D3DCOLOR_ARGB(255,128,128,128));



   m_MouseWasOver = FALSE;
   m_bDinamic = FALSE;
   m_ID = WIN_BUTTON_ESC;
   m_pDisplay = NULL;
   m_bShowWindow = TRUE;
   m_bActive = FALSE;
   m_bEnable = TRUE;
   m_bShowBackground = FALSE;

}



/////////////////////////////////////////////////
///
/// NAME: ~CWinBaceObj
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 9:58:35 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinBaceObj::~CWinBaceObj()
{
   try
   {
      std::list<CWinBaceObj*>::iterator pos = m_Childen.begin();

      while( pos != m_Childen.end() )
      {
         CWinBaceObj* pWin = (*pos);
         if(pWin->m_bDinamic)
         {
            delete pWin;
         }
         pos ++;
      } 

      m_pDisplay = NULL;
      m_pParent = NULL;        
   }
   catch(...)
   {
      sys_msg("Excpeion in ~CWinBaceObj"); 
   }

}



/////////////////////////////////////////////////
///
/// NAME: GetControl
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Gets Child control using id
///
/// CREATED:     02/06/2004 9:59:01 AM
///
/// PARAMETERS: 
///             int id
///
/// RETURN: CWinBaceObj* 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinBaceObj* CWinBaceObj::GetControl(int id)
{
   for(std::list<CWinBaceObj*>::iterator pos = m_Childen.begin() ;pos != m_Childen.end();pos++ )
   {
      CWinBaceObj* pWin = (*pos);
      if(pWin)
      {
         if(pWin->GetID() == id)
            return pWin;
      }

   }
   return NULL;
}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Displays the window
///
/// CREATED:     02/06/2004 9:59:11 AM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::Display(C2DDraw *pDisplay)
{
   for(std::list<CWinBaceObj*>::iterator pos = m_Childen.begin() ;pos != m_Childen.end();pos++ )
   {
      CWinBaceObj* pWin = (*pos);
      if(pWin)
      {
         if(pWin->IsShown())
            pWin->Display(pDisplay);
      }
   }
}



/////////////////////////////////////////////////
///
/// NAME: AddChild
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Adds a child window
///
/// CREATED:     02/06/2004 9:59:23 AM
///
/// PARAMETERS: 
///             CWinBaceObj *pWnd
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::AddChild(CWinBaceObj *pWnd)
{
   m_Childen.push_front(pWnd);
}



/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Creates the window
///
/// CREATED:     02/06/2004 9:59:41 AM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///             CWinBaceObj *pParent
///             CPoint p
///             CSize s
///             CString str
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinBaceObj::Create(C2DDraw *pDisplay, CWinBaceObj *pParent,const CPoint& p, const CSize& s, const TCHAR* str)
{
   m_pDisplay = pDisplay;
   m_bCreated = TRUE;
   m_pParent = pParent;
   m_size = s;
   m_pos = p;
   m_text = str;
   if(pParent == NULL && p == CPoint(10,10))
   {
      // center the dialog;
      CPoint newpoint; 
      CRect r = GetRect();
      CRect scrrect(CPoint(0,0),CSize(pDisplay->GetScreenWidth(),pDisplay->GetScreenHight()));
      newpoint = scrrect.CenterPoint() - r.CenterPoint();
      r.OffsetRect(newpoint);
      newpoint = r.TopLeft();
      m_pos =newpoint;
   }

   if(pParent)
   {
      pParent->AddChild(this);
   }
   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: SetSize
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Sets the size of the window
///
/// CREATED:     02/06/2004 9:59:56 AM
///
/// PARAMETERS: 
///             CSize s
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::SetSize(const CSize& s)
{
   m_size = s;
}



/////////////////////////////////////////////////
///
/// NAME: SetText
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Sets the windows text
///
/// CREATED:     02/06/2004 10:00:05 AM
///
/// PARAMETERS: 
///             CString str
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::SetText(const TCHAR* str)
{
   m_text =str;
}



/////////////////////////////////////////////////
///
/// NAME: GetDrawRect
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Gets the windows draw rect
///
/// CREATED:     02/06/2004 10:00:14 AM
///
/// RETURN: CRect 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CRect CWinBaceObj::GetDrawRect() const
{
   CRect r = GetRect();
   if(m_pParent)
   {
      r.OffsetRect(m_pParent->GetDrawPoint());
      r.OffsetRect(m_pos);
   }
   else
   {
      r.OffsetRect(m_pos);
   }
   return r;
}



/////////////////////////////////////////////////
///
/// NAME: GetDrawPoint
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Gets the windows draw position
///
/// CREATED:     02/06/2004 10:00:35 AM
///
/// RETURN: CPoint 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CPoint CWinBaceObj::GetDrawPoint() const
{
   CPoint p = m_pos;
   if(m_pParent)
   {
      p.Offset(m_pParent->GetDrawPoint());
   }
   return p;
}



/////////////////////////////////////////////////
///
/// NAME: SetPos
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Sets the windows positon
///
/// CREATED:     02/06/2004 10:00:50 AM
///
/// PARAMETERS: 
///             CPoint p
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::SetPos(const CPoint& p)
{
   m_pos = p;
}



/////////////////////////////////////////////////
///
/// NAME: GetPos
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Gets the windows postion
///
/// CREATED:     02/06/2004 10:01:08 AM
///
/// RETURN: CPoint 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
const CPoint& CWinBaceObj::GetPos() const
{
   return m_pos;
}



/////////////////////////////////////////////////
///
/// NAME: RemoveChild
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Removes a child from the window
///
/// CREATED:     02/06/2004 10:01:32 AM
///
/// PARAMETERS: 
///             CWinBaceObj *pRemoveWin
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::RemoveChild(CWinBaceObj *pRemoveWin)
{
   pRemoveWin->m_pParent = NULL;
   m_Childen.remove(pRemoveWin);
}



/////////////////////////////////////////////////
///
/// NAME: GetParent
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Gets the windows parent
///
/// CREATED:     02/06/2004 10:01:45 AM
///
/// RETURN: CWinBaceObj* 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinBaceObj* CWinBaceObj::GetParent()
{
   return m_pParent;
}



/////////////////////////////////////////////////
///
/// NAME: MouseDown
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Handels the mouse down event
///
/// CREATED:     02/06/2004 10:01:55 AM
///
/// PARAMETERS: 
///             CPoint p
///             int &Id
///             BOOL bForceCheck
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinBaceObj::MouseDown(const CPoint& p, int &Id,BOOL bForceCheck)
{
   BOOL isHanded = FALSE;
   if(GetDrawRect().PtInRect(p) || bForceCheck)
   {

      for(std::list<CWinBaceObj*>::reverse_iterator pos = m_Childen.rbegin(); pos != m_Childen.rend();pos ++ )
      {
         CWinBaceObj* pWin = (*pos);
         if(pWin && pWin->IsEnabled())
         {
            if(pWin->IsShown())
               isHanded = pWin->MouseDown(p,Id)?TRUE:isHanded;
            //if(isHanded)
            //	break;
         }

      }
      if(!isHanded && GetDrawRect().PtInRect(p))
      {
         m_bActive = TRUE;
         Id = OnMouseDown();
         m_MouseWasOver = TRUE;
         isHanded = TRUE;
      }
      else
         m_bActive = FALSE;
   }
   else
      m_bActive = FALSE;
   return isHanded;
}



/////////////////////////////////////////////////
///
/// NAME: OnMouseDown
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Mouse down default handeler
///
/// CREATED:     02/06/2004 10:02:19 AM
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinBaceObj::OnMouseDown()
{
   return WIN_BUTTON_ESC; //do nothing
}



/////////////////////////////////////////////////
///
/// NAME: SetColors
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Sets the window colors
///
/// CREATED:     02/06/2004 10:02:31 AM
///
/// PARAMETERS: 
///             D3DCOLOR forg
///             D3DCOLOR back
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::SetColors(D3DCOLOR forg, D3DCOLOR back)
{
   m_ForgoundColor = forg;
   m_BackgroundColor = back;
}



/////////////////////////////////////////////////
///
/// NAME: GetText
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Gets the window text
///
/// CREATED:     02/06/2004 10:02:40 AM
///
/// RETURN: CString 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
const CString& CWinBaceObj::GetText() const
{
   return m_text;
}



/////////////////////////////////////////////////
///
/// NAME: GetRect
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Gets the window rect
///
/// CREATED:     02/06/2004 10:02:50 AM
///
/// RETURN: CRect 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CRect CWinBaceObj::GetRect() const
{
   CRect r(CPoint(0,0),m_size);
   return r;
}



/////////////////////////////////////////////////
///
/// NAME: OnMouseOver
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Handels the mouse over event
///
/// CREATED:     02/06/2004 10:03:09 AM
///
/// PARAMETERS: 
///             BOOL bIsOver
///             CPoint p
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::OnMouseOver(BOOL, const CPoint&)
{

}



/////////////////////////////////////////////////
///
/// NAME: OnMouseUp
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Default mous up handeler
///
/// CREATED:     02/06/2004 10:03:22 AM
///
/// PARAMETERS: 
///             CPoint p
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinBaceObj::OnMouseUp(const CPoint& p)
{
   return WIN_BUTTON_ESC;
}



/////////////////////////////////////////////////
///
/// NAME: MouseUp
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Hanels the mouse up event
///
/// CREATED:     02/06/2004 10:03:36 AM
///
/// PARAMETERS: 
///             CPoint p
///             int &Id
///             BOOL bForceCheck
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinBaceObj::MouseUp(const CPoint& p, int &Id,BOOL bForceCheck)
{
   BOOL isHanded = FALSE;
   if(GetDrawRect().PtInRect(p) || bForceCheck)
   {
      std::list<CWinBaceObj*>::reverse_iterator pos = m_Childen.rbegin();
      while( pos != m_Childen.rend() )
      {
         CWinBaceObj* pWin = (*pos);
         if(pWin && pWin->IsEnabled())
         {
            if(pWin->IsShown())
               isHanded = pWin->MouseUp(p,Id)?TRUE:isHanded;
            //if(bHandled)
            //	break;

         }
         pos++;
      }
      if(!isHanded && GetDrawRect().PtInRect(p))
      {
         Id = OnMouseUp(p);
         m_MouseWasOver = TRUE;
         isHanded = TRUE;
      }
   }
   return isHanded;
}



/////////////////////////////////////////////////
///
/// NAME: MouseOver
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Handels the mouse move event
///
/// CREATED:     02/06/2004 10:03:51 AM
///
/// PARAMETERS: 
///             CPoint p
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinBaceObj::MouseOver(const CPoint& p)
{
   BOOL isHanded = FALSE;
   if(GetDrawRect().PtInRect(p))
   {
      std::list<CWinBaceObj*>::iterator pos = m_Childen.begin();
      while( pos != m_Childen.end() )
      {
         CWinBaceObj* pWin = (*pos);
         if(pWin && pWin->IsEnabled())
         {
            if(pWin->IsShown())
               isHanded = pWin->MouseOver(p);
         }
         pos++;
      }
      if(!isHanded)
      {
         OnMouseOver(TRUE,p);
      }
      m_MouseWasOver = TRUE;
      isHanded = TRUE;
   }
   else
   {
      if(m_MouseWasOver)
         OnMouseOver(FALSE,p);
      m_MouseWasOver = FALSE;
   }
   return isHanded;
}



/////////////////////////////////////////////////
///
/// NAME: GetSize
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Gets the Size of the window
///
/// CREATED:     02/06/2004 10:04:02 AM
///
/// RETURN: CSize 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CSize CWinBaceObj::GetSize() const
{
   return m_size;
}



/////////////////////////////////////////////////
///
/// NAME: OnButtonPressed
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Default button pressed handeler
///
/// CREATED:     02/06/2004 10:04:32 AM
///
/// PARAMETERS: 
///             int ButtonID
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::OnButtonPressed(int ButtonID)
{
   if(m_pParent)
      m_pParent->OnButtonPressed(ButtonID);
}




/////////////////////////////////////////////////
///
/// NAME: GetDlgEmbeddedString
///
/// DESCRIPTION: Gets an embedded string
///
/// CREATED:     02/06/2004 10:05:17 AM
///
/// PARAMETERS: 
///             PWORD pwsz 
///             PSTR pszBuffer 
///             unsigned cbLen
///
/// RETURN: PWORD 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
PWORD GetDlgEmbeddedString( PWORD pwsz, PSTR pszBuffer, unsigned cbLen )
{
   if ( !cbLen || !pszBuffer )
      return FALSE;

   *pszBuffer = 0;

   while ( *pwsz )     // Loop through all chars in the unicode string
   {
      if ( cbLen )    // While there's room, copy to the output buffer
         *pszBuffer++ = (BYTE)*pwsz;         
      pwsz++;
   }

   // Null terminate the output string
   if ( cbLen )                
      *pszBuffer = 0;         // Space was left over
   else
      *(pszBuffer-1) = 0;     // Wrote to the entire buffer.  Back up and
   // null out the last character

   return pwsz+1;  // return next WORD past the null terminator
}



/////////////////////////////////////////////////
///
/// NAME: LoadCtrl
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Loads the control from the resorce file
///
/// CREATED:     02/06/2004 10:06:18 AM
///
/// PARAMETERS: 
///              LPDLGITEMTEMPLATE pDlgItemTemplate
///             CWinBaceObj* pParent
///             C2DDraw *pDisplay
///
/// RETURN: LPDLGITEMTEMPLATE 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
LPDLGITEMTEMPLATE CWinBaceObj::LoadCtrl( LPDLGITEMTEMPLATE pDlgItemTemplate, CWinBaceObj* pParent,C2DDraw *pDisplay )
{
   //
   // Start out by printing all the fixed fields in the DLGITEMTEMPLATE
   //          


   DWORD Style = pDlgItemTemplate->style;
   //m_ExtStyle = pDlgItemTemplate->dwExtendedStyle;

   CSize Size;
   Size.cx = pDlgItemTemplate->cx;
   Size.cy = pDlgItemTemplate->cy;

   CPoint Pos;
   Pos.x = pDlgItemTemplate->x;
   Pos.y = pDlgItemTemplate->y;

   int id = pDlgItemTemplate->id;



   PWORD pClass, pTitle, pCreationData;
   char szAnsiString[256];

   CString Class = _T("");
   int classId = -1;

   // Following the fixed DLGITEMTEMPLATE, first up is the window class
   pClass = (PWORD)(pDlgItemTemplate+1);   // ptr math!
   if ( *pClass )
   {
      if ( 0xFFFF == *pClass )
      {
         pClass++;
         classId = *pClass++;
      }
      else
      {
         pClass = GetDlgEmbeddedString(  pClass, szAnsiString,
            sizeof(szAnsiString) );

         Class =  szAnsiString;
      }
   }
   else
      pClass++;




   CString Title;
   // Following the window class array is the title array
   pTitle = pClass;
   if ( *pTitle )
   {
      if ( 0xFFFF == *pTitle )
      {
         pTitle++;
         Title.Format( "%u", *pTitle++ );
      }
      else
      {
         pTitle = GetDlgEmbeddedString(  pTitle, szAnsiString,
            sizeof(szAnsiString) );

         Title = szAnsiString;
      }
   }
   else
      pTitle++;

   pCreationData = pTitle;

   // Following the title array is the (optional) creation data.  We won't
   // show it here, because it's usually not meaningful as ASCII text
   if ( *pCreationData )
   {
      // Adjust for the creation data.  *pCreationData is in bytes, rather
      // than WORDs, so we divide by sizeof(DWORD) to make the ptr addition
      // come out correctly
      pCreationData = (PWORD)( (PBYTE)pCreationData + *pCreationData );
   }
   else
      pCreationData++;


   CWinBaceObj* pCtrl = pParent->CreateControl(id,classId,Class,Style);

   switch ( classId )
   {
   case 0x0080: //return "Button";
      {
         int ButtonStyle = Style & 0xf;
         if(ButtonStyle  == BS_GROUPBOX)
         {
            if(pCtrl == NULL)
            {
               pCtrl = new CWinGroupBox();
               pCtrl->SetDinamicCreation(TRUE);
            }
            pCtrl->Create(pDisplay,pParent,Pos,Size,Title);
            pCtrl->SetID(id);
         }
         else if(ButtonStyle == BS_AUTOCHECKBOX || ButtonStyle ==  BS_AUTORADIOBUTTON)
         {
            if(pCtrl == NULL)
            {
               pCtrl = new CWinCheckBox();
               pCtrl->SetDinamicCreation(TRUE);
            }
            pCtrl->Create(pDisplay,pParent,Pos,CSize(15,15),Title);
            pCtrl->SetID(id);



            Pos.Offset(20,3);
            CWinBaceObj* pStatic = new CWinStatic();
            pStatic->Create(pDisplay,pParent,Pos,Size,Title);
            pStatic->SetDinamicCreation(TRUE);

         }
         else
         {

            if(pCtrl == NULL)
            {
               pCtrl = new CWinButton();
               pCtrl->SetDinamicCreation(TRUE);
            }
            pCtrl->Create(pDisplay,pParent,Pos,Size,Title);
            pCtrl->SetID(id);
         }
      }
      break;

   case 0x0081: //edit control
      {
         if(pCtrl == NULL)
         {
            pCtrl = new CWinEditBox();
            pCtrl->SetDinamicCreation(TRUE);
         }
         pCtrl->Create(pDisplay,pParent,Pos,Size,Title);
         pCtrl->SetID(id);
      }
      break;
   case 0x0082: //return "Static";
      {
         if(pCtrl == NULL)
         {
            pCtrl = new CWinStatic();
            pCtrl->SetDinamicCreation(TRUE);
         }
         pCtrl->Create(pDisplay,pParent,Pos,Size,Title);
         pCtrl->SetID(id);
      }
      break;

   case 0x0084: //return "Scroll bar";
      {
         if(pCtrl == NULL)
         {
            pCtrl = new CWinScrollBar();
            pCtrl->SetDinamicCreation(TRUE);
         }
         pCtrl->Create(pDisplay,pParent,Pos,Size,Title);
         pCtrl->SetID(id);
      }
      break;
   case 0x0083: //return "List box";
      {
         if(pCtrl == NULL)
         {
            pCtrl = new CWinListBox();
            pCtrl->SetDinamicCreation(TRUE);
         }
         pCtrl->Create(pDisplay,pParent,Pos,Size,Title);
         pCtrl->SetID(id);

      }
      break;
   case 0x0085: //return "Combo box";
      {
         if(pCtrl == NULL)
         {
            pCtrl = new CWinComboBox();
            pCtrl->SetDinamicCreation(TRUE);
         }
         pCtrl->Create(pDisplay,pParent,Pos,Size,Title);
         pCtrl->SetID(id);

      }
      break;
   default:
      {
         if(pCtrl != NULL)
         {
            pCtrl->Create(pDisplay,pParent,Pos,Size,Title);
            pCtrl->SetID(id);
         }
      }
   }









   // Return value is next byte after the DLGITEMTEMPLATE and its
   // variable len  fields     
   return (LPDLGITEMTEMPLATE)pCreationData;
}



/////////////////////////////////////////////////
///
/// NAME: CreateControl
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Default handeler for creating the controls
///
/// CREATED:     02/06/2004 10:06:53 AM
///
/// PARAMETERS: 
///             int id
///             int classId
///             CString Class
///             DWORD Style
///
/// RETURN: CWinBaceObj* 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinBaceObj* CWinBaceObj::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   return NULL;
}




/////////////////////////////////////////////////
///
/// NAME: CharPressed
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: handels the Char pressed event
///
/// CREATED:     02/06/2004 10:07:14 AM
///
/// PARAMETERS: 
///             char c
///             DWORD keycode
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinBaceObj::CharPressed(char c, DWORD keycode)
{
   BOOL isHanded = FALSE;
   if(IsActive())
   {
      OnCharDown(c,keycode);
   }
   else
   {
      std::list<CWinBaceObj*>::reverse_iterator pos = m_Childen.rbegin();
      while( pos != m_Childen.rend() )
      {
         CWinBaceObj* pWin = (*pos);
         if(pWin)
         {
            if(pWin->IsShown())
               isHanded = pWin->CharPressed(c,keycode)?TRUE:isHanded;
            if(isHanded)
               break;
         }
         pos ++;
      }
   }
   return isHanded;
}



/////////////////////////////////////////////////
///
/// NAME: OnCharDown
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Default handeler of the CharDown event
///
/// CREATED:     02/06/2004 10:07:45 AM
///
/// PARAMETERS: 
///             char c
///             DWORD keycode
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::OnCharDown(char , DWORD )
{

}



/////////////////////////////////////////////////
///
/// NAME: DrawBorder
///
/// CLASS: CWinBaceObj
///
/// DESCRIPTION: Draws the border around the window
///
/// CREATED:     02/06/2004 10:07:58 AM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///             CRect rect
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBaceObj::DrawBorder(C2DDraw *pDisplay,const CRect& rect)
{
   pDisplay->DrawLine(CPoint(rect.left+1,rect.top+1),CPoint(rect.left+1,rect.bottom),m_BorderColor3);
   pDisplay->DrawLine(CPoint(rect.left+1,rect.top+1),CPoint(rect.right,rect.top+1),m_BorderColor3);
   pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.left-1,rect.bottom),m_BorderColor2);
   pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.right,rect.top-1),m_BorderColor2);
   pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.left,rect.bottom+1),m_BorderColor);
   pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.right+1,rect.top),m_BorderColor);
}
