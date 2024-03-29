///////////////////////////////////////////////////////////////////////////////
///  WinStatic.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:07:13 PM by Christiaan Burchell
///
///  PURPOSE:  Static control
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

// WinStatic.h: interface for the CWinStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSTATIC_H__1CEC680A_807E_4540_9815_20CAAD69565C__INCLUDED_)
#define AFX_WINSTATIC_H__1CEC680A_807E_4540_9815_20CAAD69565C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"

class CWinStatic : public CWinBaceObj
{
public:
   virtual void Display(C2DDraw *pDisplay);
   CWinStatic();
   virtual ~CWinStatic();

};

#endif // !defined(AFX_WINSTATIC_H__1CEC680A_807E_4540_9815_20CAAD69565C__INCLUDED_)
