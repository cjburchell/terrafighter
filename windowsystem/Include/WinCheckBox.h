///////////////////////////////////////////////////////////////////////////////
///  WinCheckBox.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:17:32 PM by Christiaan Burchell
///
///  PURPOSE:  The check box control
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

// WinCheckBox.h: interface for the CWinCheckBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINCHECKBOX_H__3887B852_E292_436A_B04E_3E16B810E229__INCLUDED_)
#define AFX_WINCHECKBOX_H__3887B852_E292_436A_B04E_3E16B810E229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinButton.h"

class CWinCheckBox : public CWinButton  
{
public:
   CWinCheckBox();
   virtual ~CWinCheckBox();
   BOOL IsChecked() const {return m_bChecked;}
   void SetChecked(BOOL check = TRUE){m_bChecked = check;}
   virtual void Display(C2DDraw *pDisplay);
   virtual int OnMouseUp(const CPoint&  p);
protected:
   BOOL m_bChecked;

};

#endif // !defined(AFX_WINCHECKBOX_H__3887B852_E292_436A_B04E_3E16B810E229__INCLUDED_)
