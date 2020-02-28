///////////////////////////////////////////////////////////////////////////////
///  WinBitmapCtrl.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:14:01 PM by Christiaan Burchell
///
///  PURPOSE:  the Bitmap control
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

#if !defined(AFX_WINBITMAPCTRL_H__99A83A9C_5B35_4588_8DBC_82412BA5B940__INCLUDED_)
#define AFX_WINBITMAPCTRL_H__99A83A9C_5B35_4588_8DBC_82412BA5B940__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinBaceObj.h>
#include <ZipArchive.h>

class CWinBitmapCtrl : public CWinBaceObj  
{
public:
   virtual void Display(C2DDraw *pDisplay);
   CWinBitmapCtrl();
   virtual ~CWinBitmapCtrl();
   BOOL SetBitmap(CSurface* bmp);
   BOOL SetBitmap(const TCHAR* file,IDirect3DDevice9* pDisplay,CZipArchive* pZip);
private:
   BOOL m_bOwnBmp;
   CSurface* m_bmp;
};

#endif // !defined(AFX_WINBITMAPCTRL_H__99A83A9C_5B35_4588_8DBC_82412BA5B940__INCLUDED_)
