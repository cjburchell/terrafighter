///////////////////////////////////////////////////////////////////////////////
///  WinDialog.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:20:12 PM by Christiaan Burchell
///
///  PURPOSE:  the dialog window
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

// WinDialog.cpp: implementation of the CWinDialog class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <WinDialog.h>
#include <WindowMgr.h>
#include <WinButton.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TITLE_BAR_FONT_OFFSET 3
#define TITLE_BAR_SIZE (FONT_SIZE_Y + TITLE_BAR_FONT_OFFSET + TITLE_BAR_FONT_OFFSET) 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinDialog
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:20:17 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinDialog::CWinDialog() :CWinBaceObj()
{
   m_bTitleBar = TRUE;
   m_bDrag = FALSE;
   m_bCanMove = TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinDialog
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:20:19 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinDialog::~CWinDialog()
{

}



/////////////////////////////////////////////////
///
/// NAME: DoModal
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: displays a modal dialog
///
/// CREATED:     02/06/2004 12:20:35 PM
///
/// PARAMETERS: 
///             CInput* pInput
///             CD3DApplication* pApp
///             CWinBaceObj* pParent
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinDialog::DoModal(CInput* pInput,CD3DApplication* pApp, CWindowMgr* pMgr, CWinBaceObj* pParent)
{
   if(pParent)
      pParent->EnableWin(FALSE);
   m_bEndDialog = FALSE;
   int Id = -1;
   pMgr->AddChild(this);
   pInput->ClearButtonPressBuffer();
   BOOL bInputFound = FALSE;
   while(!bInputFound)
   {
      pApp->Render();
      pApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
      Id = DoIdle();
      if(ValidButtonID(Id))
         return Id;
      if(!pApp->LookForMSG())
         return -1;
      while(!pInput->IsButtonPressEmpty())
      {
         Id  = pInput->RemoveButtonPress();
         if(Id == WIN_BUTTON_ESC)
         {
            bInputFound = FALSE;
         }
         else
            bInputFound = ValidButtonID(Id);
      }
      if(m_bEndDialog)
      {
         Id = m_Endcode;
         bInputFound = TRUE;
      }
   }
   OnDistroy();
   pMgr->RemoveChild(this);
   if(pParent)
      pParent->EnableWin(TRUE);
   return Id;
}



/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Create the window
///
/// CREATED:     02/06/2004 12:21:45 PM
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
BOOL CWinDialog::Create(C2DDraw *pDisplay, CWinBaceObj *pParent, const CPoint& p, const CSize& s, const TCHAR* str)
{
   if(!CWinBaceObj::Create(pDisplay, pParent, p,  s,  str))
      return FALSE;

   CRect draw = GetDrawRect();
   m_BarRec = CRect(CPoint(0,-TITLE_BAR_SIZE),CSize(draw.Size().cx,TITLE_BAR_SIZE));
   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: GetBarRect
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Gets the Title bar rect
///
/// CREATED:     02/06/2004 12:22:01 PM
///
/// RETURN: CRect 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CRect CWinDialog::GetBarRect()
{
   CRect BarRec = m_BarRec;
   BarRec.OffsetRect(GetDrawPoint());
   return BarRec;
}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Displays the window
///
/// CREATED:     02/06/2004 12:22:13 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinDialog::Display(C2DDraw *pDisplay)
{
   CRect draw = GetDrawRect();
   if(m_bTitleBar)
   {
      CRect BarRec = GetBarRect();
      pDisplay->DrawGradientRectBlend(BarRec,m_TitleBarColor,m_BackgroundColor,GR_BR|GR_TR);
      pDisplay->DrawRect(BarRec,m_BorderColor);
      pDisplay->OutTextc(m_HighlightColor,m_HighlightColor,BarRec.TopLeft().x+TITLE_BAR_FONT_OFFSET,BarRec.TopLeft().y+TITLE_BAR_FONT_OFFSET,FALSE,"%s",GetText());

   }
   pDisplay->DrawBlendRect(draw,m_BackgroundColor);
   pDisplay->DrawRect(draw,m_BorderColor);
   CWinBaceObj::Display(pDisplay);
}




/////////////////////////////////////////////////
///
/// NAME: ValidButtonID
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Handes if the button is valid
///
/// CREATED:     02/06/2004 12:22:38 PM
///
/// PARAMETERS: 
///             int Id
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinDialog::ValidButtonID(int Id)
{
   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: ShowTitleBar
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Show the Title bar
///
/// CREATED:     02/06/2004 12:22:56 PM
///
/// PARAMETERS: 
///             BOOL show
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinDialog::ShowTitleBar(BOOL show)
{
   m_bTitleBar = show;
}



/////////////////////////////////////////////////
///
/// NAME: MouseOver
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: handels mouse over message
///
/// CREATED:     02/06/2004 12:23:06 PM
///
/// PARAMETERS: 
///             CPoint p
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinDialog::MouseOver(const CPoint& p)
{
   if(m_bDrag)
   {
      CPoint diff = p - m_DragStart;
      m_DragStart = p;
      CPoint newpos = GetDrawPoint();
      newpos.Offset(diff);
      SetPos(newpos);
      return TRUE;
   }
   else
      return CWinBaceObj::MouseOver(p);
}


/////////////////////////////////////////////////
///
/// NAME: MouseUp
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Handes mous up message
///
/// CREATED:     02/06/2004 12:23:16 PM
///
/// PARAMETERS: 
///             CPoint p
///             int& Id
///             BOOL bForceCheck
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinDialog::MouseUp(const CPoint& p,int& Id,BOOL bForceCheck)
{
   m_bDrag = FALSE;
   return CWinBaceObj::MouseUp(p,Id,TRUE);
}



/////////////////////////////////////////////////
///
/// NAME: MouseDown
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Handels mouse down message
///
/// CREATED:     02/06/2004 12:23:28 PM
///
/// PARAMETERS: 
///             CPoint p
///             int& Id
///             BOOL bForceCheck
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinDialog::MouseDown(const CPoint& p, int& Id,BOOL bForceCheck)
{
   if(GetBarRect().PtInRect(p) && m_bTitleBar && m_bCanMove)
   {
      m_bDrag = TRUE;
      m_DragStart = p;
      Id = WIN_BUTTON_ESC;
      return TRUE;
   }
   else
      return CWinBaceObj::MouseDown(p,Id,TRUE);
}







/////////////////////////////////////////////////
///
/// NAME: LoadDialog
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Loads the dialog from the resource file
///
/// CREATED:     02/06/2004 12:23:56 PM
///
/// PARAMETERS: 
///             int id
///             C2DDraw *pDisplay
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinDialog::LoadDialog(int id,C2DDraw *pDisplay)
{
   HRSRC  hDlgSrc = FindResource(NULL,MAKEINTRESOURCE(id),MAKEINTRESOURCE(RT_DIALOG));
   HGLOBAL hDlg = LoadResource(NULL,hDlgSrc);
   LPVOID pDlg = LockResource(hDlg);

   LPDLGTEMPLATE pDlgTemplate = (LPDLGTEMPLATE)pDlg;

   PWORD pMenu, pClass, pTitle, pPointSize, pTypeface;
   char szAnsiString[256];
   unsigned i;

   if ( HIWORD(pDlgTemplate->style) != 0xFFFF )// Is it a regular DLGTEMPLATE?
   {
      //
      // Start out by printing all the fixed fields in the DLGTEMPLATE
      //  

      //m_Style = pDlgTemplate->style;
      //m_ExtStyle = pDlgTemplate->dwExtendedStyle;
      //m_NumbCtrls = pDlgTemplate->cdit;

      CSize Size;
      Size.cx = pDlgTemplate->cx;
      Size.cy = pDlgTemplate->cy;
      SetSize(Size);
      CPoint Pos;
      Pos.x = pDlgTemplate->x;
      Pos.y = pDlgTemplate->y;
      SetPos(Pos);

      // Following the fixed DLGTEMPLATE, first up is the menu
      pMenu = (PWORD)(pDlgTemplate + 1);  // ptr math!
      if ( *pMenu )
      {
         if ( 0xFFFF == *pMenu )
         {
            pMenu++;
            int MenuID = *pMenu++;
         }
         else
         {
            pMenu = GetDlgEmbeddedString(   pMenu, szAnsiString,
               sizeof(szAnsiString) );
         }
      }
      else
         pMenu++;

      // Following the menu array is the "window class" array
      pClass = pMenu;
      if ( *pClass )
      {
         if ( 0xFFFF == *pClass )
            signed short Class =  (signed short)*pClass++;
         else
         {
            pClass = GetDlgEmbeddedString(  pClass, szAnsiString,
               sizeof(szAnsiString) );
         }
      }
      else
         pClass++;

      // Following the window class array is the title array
      pTitle = pClass;
      if ( *pTitle )
      {
         pTitle = GetDlgEmbeddedString(  pTitle, szAnsiString,
            sizeof(szAnsiString) );

         SetText(szAnsiString);
      }
      else
         pTitle++;

      // Following the title array is the point size and font (if DS_SETFONT
      // is set in pDlgTemplate->Style
      if ( pDlgTemplate->style & DS_SETFONT )
      {       
         pPointSize = pTitle;
         pTypeface = pPointSize+1;

         pTypeface = GetDlgEmbeddedString( pTypeface, szAnsiString,
            sizeof(szAnsiString) );
      }
      else
         pTypeface = pTitle; 

      LPDLGITEMTEMPLATE pDlgItemTemplate = (LPDLGITEMTEMPLATE)pTypeface;

      // Now iterate through each of the controls, calling DumpDialogItem
      for ( i=0; i < pDlgTemplate->cdit; i++ )
      {
         pDlgItemTemplate = ALIGN_DWORD(LPDLGITEMTEMPLATE, pDlgItemTemplate);
         pDlgItemTemplate = CWinBaceObj::LoadCtrl( pDlgItemTemplate,this, pDisplay);
      }
   }
   else 
   {
      // Left as an exercise for the reader... :-)
      AfxMessageBox( "DLGTEMPLATEEX type not yet handled\n" );
   }

   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: DoIdle
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Idle message
///
/// CREATED:     02/06/2004 12:24:22 PM
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinDialog::DoIdle()
{
   return WIN_BUTTON_ESC;
}



/////////////////////////////////////////////////
///
/// NAME: OnDistroy
///
/// CLASS: CWinDialog
///
/// DESCRIPTION: Distroys the window
///
/// CREATED:     02/06/2004 12:24:32 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinDialog::OnDistroy()
{
}
