///////////////////////////////////////////////////////////////////////////////
///  WinBitmapButton.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:11:06 PM by Christiaan Burchell
///
///  PURPOSE:  interface for the CWinBitmapButton class.
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

#if !defined(AFX_WINBITMAPBUTTON_H__9CEE652E_4D35_4421_A90D_3E345609BA8C__INCLUDED_)
#define AFX_WINBITMAPBUTTON_H__9CEE652E_4D35_4421_A90D_3E345609BA8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinButton.h>
#include <WinToolTip.h>	// Added by ClassView
#include <Surface.h>
#include <ZipArchive.h>

class CWinBitmapButton : public CWinButton  
{
public:
   CWinToolTip m_ToolTip;
   BOOL Create(C2DDraw *pDisplay, CWinBaceObj *pParent, CPoint p, CSize s, CString str);

   BOOL SetBitmap(CSurface* bmp);
   BOOL SetBitmap(const TCHAR* file,IDirect3DDevice9* pDisplay,CZipArchive* pZip);
   virtual void Display(C2DDraw *pDisplay);
   CWinBitmapButton();
   virtual ~CWinBitmapButton();

   void ShowToolTip(BOOL bShowToolTip = TRUE){m_bShowToolTip = bShowToolTip;}


protected:
   BOOL m_bShowToolTip;
   BOOL m_bOwnBmp;
   CSurface* m_bmp;
};

#endif // !defined(AFX_WINBITMAPBUTTON_H__9CEE652E_4D35_4421_A90D_3E345609BA8C__INCLUDED_)
