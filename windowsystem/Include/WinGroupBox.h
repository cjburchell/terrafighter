///////////////////////////////////////////////////////////////////////////////
///  WinGroupBox.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:31:00 PM by Christiaan Burchell
///
///  PURPOSE:  The group box control
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

// WinGroupBox.h: interface for the CWinGroupBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINGROUPBOX_H__4FBAE239_4800_41E1_9886_4608EAA1967F__INCLUDED_)
#define AFX_WINGROUPBOX_H__4FBAE239_4800_41E1_9886_4608EAA1967F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinStatic.h"

class CWinGroupBox : public CWinStatic  
{
public:
   virtual void Display(C2DDraw *pDisplay);
   CWinGroupBox();
   virtual ~CWinGroupBox();

};

#endif // !defined(AFX_WINGROUPBOX_H__4FBAE239_4800_41E1_9886_4608EAA1967F__INCLUDED_)
