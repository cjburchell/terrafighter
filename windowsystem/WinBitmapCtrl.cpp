///////////////////////////////////////////////////////////////////////////////
///  WinBitmapCtrl.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:15:33 PM by Christiaan Burchell
///
///  PURPOSE:  The Button Control
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

// WinBitmapCtrl.cpp: implementation of the CWinBitmapCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <Surface.h>
#include <WinBitmapCtrl.h>
#include <dxutil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinBitmapCtrl
///
/// CLASS: CWinBitmapCtrl
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:14:10 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinBitmapCtrl::CWinBitmapCtrl()
{
   m_bOwnBmp=FALSE;
   m_bmp = NULL;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinBitmapCtrl
///
/// CLASS: CWinBitmapCtrl
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:14:12 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinBitmapCtrl::~CWinBitmapCtrl()
{
   if(m_bOwnBmp && m_bmp)
      delete m_bmp;
}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinBitmapCtrl
///
/// DESCRIPTION: Displays the window
///
/// CREATED:     02/06/2004 12:14:21 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinBitmapCtrl::Display(C2DDraw *pDisplay)
{
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

   CWinBaceObj::Display(pDisplay);
}



/////////////////////////////////////////////////
///
/// NAME: SetBitmap
///
/// CLASS: CWinBitmapCtrl
///
/// DESCRIPTION: Sets the bitmap
///
/// CREATED:     02/06/2004 12:14:32 PM
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
BOOL CWinBitmapCtrl::SetBitmap(const TCHAR* file,IDirect3DDevice9* pDisplay,CZipArchive* pZip)
{
   if(m_bOwnBmp && m_bmp)
      delete m_bmp;

   CSize size = GetSize();
   m_bmp = new CSurface();

   if(FAILED(m_bmp->Init(file,size.cx,size.cy, pDisplay, pZip)))
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
/// CLASS: CWinBitmapCtrl
///
/// DESCRIPTION: Sets the bitmap using a surface
///
/// CREATED:     02/06/2004 12:14:44 PM
///
/// PARAMETERS: 
///             CSurface *bmp
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinBitmapCtrl::SetBitmap(CSurface *bmp)
{
   if(m_bOwnBmp && m_bmp)
      delete m_bmp;
   m_bOwnBmp = FALSE;
   m_bmp = bmp;
   return TRUE;
}