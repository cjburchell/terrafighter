///////////////////////////////////////////////////////////////////////////////
///  WinArrowButton.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 10:09:39 AM by Christiaan Burchell
///
///  PURPOSE:  interface for the CWinArrowButton class.
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

#if !defined(AFX_WINARROWBUTTON_H__1481EAF6_C087_49EC_808A_0595CD43C8EF__INCLUDED_)
#define AFX_WINARROWBUTTON_H__1481EAF6_C087_49EC_808A_0595CD43C8EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinButton.h"

enum WinArrow_Type_e
{
   WINARROW_UP = 0,
   WINARROW_DOWN = 1,
   WINARROW_RIGHT = 2,
   WINARROW_LEFT = 3
};

class CWinArrowButton : public CWinButton  
{
public:
   CWinArrowButton();
   virtual ~CWinArrowButton();
   void SetType(WinArrow_Type_e Type){m_Type=Type;}
   WinArrow_Type_e GetType() const {return m_Type; }
   virtual void Display(C2DDraw *pDisplay);
private:
   WinArrow_Type_e m_Type;


};

#endif // !defined(AFX_WINARROWBUTTON_H__1481EAF6_C087_49EC_808A_0595CD43C8EF__INCLUDED_)
