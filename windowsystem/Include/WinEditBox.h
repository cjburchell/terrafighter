///////////////////////////////////////////////////////////////////////////////
///  WinEditBox.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:30:08 PM by Christiaan Burchell
///
///  PURPOSE:  The Edit Control
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

// WinEditBox.h: interface for the CWinEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINEDITBOX_H__98A7E194_8242_4008_A73C_43108B7A5460__INCLUDED_)
#define AFX_WINEDITBOX_H__98A7E194_8242_4008_A73C_43108B7A5460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"

class CWinEditBox : public CWinBaceObj  
{
public:
   virtual void OnCharDown(char c, DWORD keycode);
   virtual void Display(C2DDraw *pDisplay);
   CWinEditBox();
   virtual ~CWinEditBox();

};

#endif // !defined(AFX_WINEDITBOX_H__98A7E194_8242_4008_A73C_43108B7A5460__INCLUDED_)
