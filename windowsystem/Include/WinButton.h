///////////////////////////////////////////////////////////////////////////////
///  WinButton.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:15:02 PM by Christiaan Burchell
///
///  PURPOSE:  The Button control
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

// WinButton.h: interface for the CWinButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINBUTTON_H__6EF0C9B3_4B81_4F54_A63A_8ABD2C8D9054__INCLUDED_)
#define AFX_WINBUTTON_H__6EF0C9B3_4B81_4F54_A63A_8ABD2C8D9054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"

#define WIN_BUTTON_ESC 0

class CWinButton  : public CWinBaceObj
{
public:
   BOOL m_isMouseOver;
   virtual int OnMouseUp(const CPoint&  p);
   virtual void OnMouseOver(BOOL bIsOver, const CPoint&  p);
   BOOL m_bPressed;
   virtual void Display(C2DDraw *pDisplay);
   virtual int OnMouseDown();
   CWinButton();
   virtual ~CWinButton();
};

#endif // !defined(AFX_WINBUTTON_H__6EF0C9B3_4B81_4F54_A63A_8ABD2C8D9054__INCLUDED_)
