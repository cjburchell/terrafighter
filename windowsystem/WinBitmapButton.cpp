///////////////////////////////////////////////////////////////////////////////
///  WinBitmapButton.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:11:59 PM by Christiaan Burchell
///
///  PURPOSE:  implementation of the CWinBitmapButton class.
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

#include "stdafx.h"

#include <WinBitmapButton.h>
#include <dxutil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinBitmapButton
///
/// CLASS: CWinBitmapButton
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:12:12 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinBitmapButton::CWinBitmapButton() : CWinButton()
{
   m_bOwnBmp=FALSE;
   m_bmp = NULL;
   m_bShowToolTip = TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinBitmapButton
///
/// CLASS: CWinBitmapButton
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:12:14 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinBitmapButton::~CWinBitmapButton()
{
   if(m_bOwnBmp && m_bmp)
      delete m_bmp;
}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinBitmapButton
///
/// DESCRIPTION: Displays the window
///
/// CREATED:     02/06/2004 12:12:20 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBitmapButton::Display(C2DDraw *pDisplay)
{

   m_ToolTip.Show(m_isMouseOver && m_bShowToolTip);
   CRect rect = GetDrawRect();
   if(m_bmp)
   {
      CPoint scr = rect.TopLeft();
      CRect r = GetRect();
      r.bottom--;
      m_bmp->Draw(pDisplay->GetTempDevice(),scr.x,scr.y,&r);
   }
   else if(ShowBackground())
      pDisplay->DrawBlendRect(rect,m_BackgroundColor);

   if(m_bPressed)
   {
      DrawBorder(pDisplay,rect);
   }
   else
   {
      pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.left,rect.bottom),m_BorderColor2);
      pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.right,rect.top),m_BorderColor2);

      pDisplay->DrawLine(CPoint(rect.right,rect.bottom-1),CPoint(rect.left,rect.bottom-1),m_BorderColor3);
      pDisplay->DrawLine(CPoint(rect.right-1,rect.bottom-1),CPoint(rect.right-1,rect.top),m_BorderColor3);

      pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.left-1,rect.bottom),m_BorderColor);
      pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.right,rect.top-1),m_BorderColor);
   }
   CWinBaceObj::Display(pDisplay);
}



/////////////////////////////////////////////////
///
/// NAME: SetBitmap
///
/// CLASS: CWinBitmapButton
///
/// DESCRIPTION: Sets the bitmap
///
/// CREATED:     02/06/2004 12:12:32 PM
///
/// PARAMETERS: 
///             CString file
///             C2DDraw* pDisplay
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinBitmapButton::SetBitmap(const TCHAR* file, IDirect3DDevice9* pDisplay,CZipArchive* pZip)
{
   if(m_bOwnBmp && m_bmp)
      delete m_bmp;

   CSize size = GetSize();
   m_bmp = new CSurface();

   if(FAILED(m_bmp->Init(file,size.cx,size.cy,pDisplay,pZip)))
   {
      sys_msg1("unable to open %s",file);
      return FALSE;
   }
   if(m_bmp!=NULL)
   {
      m_bmp->SetColorKey(D3DCOLOR_XRGB(255,0,255));
   }
   m_bOwnBmp = TRUE;
   return TRUE;
}


/////////////////////////////////////////////////
///
/// NAME: SetBitmap
///
/// CLASS: CWinBitmapButton
///
/// DESCRIPTION: Sets the bitmap using a surface
///
/// CREATED:     02/06/2004 12:13:12 PM
///
/// PARAMETERS: 
///             CSurface *bmp
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinBitmapButton::SetBitmap(CSurface *bmp)
{
   if(m_bOwnBmp && m_bmp)
      delete m_bmp;
   m_bOwnBmp = FALSE;
   m_bmp = bmp;
   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CWinBitmapButton
///
/// DESCRIPTION: Creates the window
///
/// CREATED:     02/06/2004 12:13:30 PM
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
BOOL CWinBitmapButton::Create(C2DDraw *pDisplay, CWinBaceObj *pParent, CPoint p, CSize s, CString str)
{
   if(!CWinButton::Create(pDisplay,pParent,p,s,str))
      return FALSE;
   m_ToolTip.Create(pDisplay,this,CPoint(0,s.cy),CSize(0,0),str);
   m_ToolTip.Show(FALSE);
   return TRUE;
}
