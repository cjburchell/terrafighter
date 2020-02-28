///////////////////////////////////////////////////////////////////////////////
///  WindowMgr.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:24:53 PM by Christiaan Burchell
///
///  PURPOSE:  the window manager
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

// WndowMgr.h: interface for the CDEWndowMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEWNDOWMGR_H__0E236C4D_D8EE_4FA3_9E79_59486CC6BC2A__INCLUDED_)
#define AFX_DEWNDOWMGR_H__0E236C4D_D8EE_4FA3_9E79_59486CC6BC2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WinBaceObj.h"



class CWindowMgr
{
public:
   virtual BOOL CharPressed(char c,DWORD keycode);
   void AddToolTip(CWinBaceObj* pTip);
   virtual BOOL MouseOver(const CPoint&  p);
   virtual BOOL MouseUp(const CPoint&  p,int& Id);
   void AddChild(CWinBaceObj* pWnd);
   void RemoveChild(CWinBaceObj* pWin);
   virtual BOOL MouseDown(const CPoint&  p, int& Id);
   virtual void Display(LPDIRECT3DDEVICE9 pd3dDevice,C2DDraw *pDisplay);


   BOOL FindChild(CWinBaceObj* pWnd);


   CWindowMgr();
   virtual ~CWindowMgr();
private:
   std::list <CWinBaceObj*> m_Childen;
   std::list <CWinBaceObj*> m_ToolTips;

};

CWindowMgr* GetWindowMgr();

#endif // !defined(AFX_DEWNDOWMGR_H__0E236C4D_D8EE_4FA3_9E79_59486CC6BC2A__INCLUDED_)
