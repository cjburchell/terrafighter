///////////////////////////////////////////////////////////////////////////////
///  WindowMgr.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:25:04 PM by Christiaan Burchell
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

// DEWndowMgr.cpp: implementation of the CWindowMgr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "WindowMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWindowMgr
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:25:08 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWindowMgr::CWindowMgr()
{

}



/////////////////////////////////////////////////
///
/// NAME: ~CWindowMgr
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:25:10 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWindowMgr::~CWindowMgr()
{

}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Displays the windows
///
/// CREATED:     02/06/2004 12:25:20 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE8 pd3dDevice
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWindowMgr::Display(LPDIRECT3DDEVICE9 pd3dDevice,C2DDraw *pDisplay)
{
   pDisplay->Start(pd3dDevice);

   std::list<CWinBaceObj*>::iterator pos = m_Childen.begin();
   while( pos != m_Childen.end() )
   {
      CWinBaceObj* pWin = (*pos);
      if(pWin)
      {
         if(pWin->IsShown())
            pWin->Display(pDisplay);
      }
      pos++;
   }

   pos = m_ToolTips.begin();
   while( pos != m_ToolTips.end() )
   {
      CWinBaceObj* pWin = (*pos);
      if(pWin)
      {
         if(pWin->IsShown())
            pWin->Display(pDisplay);
      }
      pos++;
   }

   pDisplay->End();
}



/////////////////////////////////////////////////
///
/// NAME: FindChild
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Finds a child
///
/// CREATED:     02/06/2004 12:25:28 PM
///
/// PARAMETERS: 
///             CWinBaceObj* pWnd
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWindowMgr::FindChild(CWinBaceObj* pWnd)
{
   BOOL bFound = FALSE;
   std::list<CWinBaceObj*>::iterator pos = m_Childen.begin();
   while( pos != m_Childen.end() )
   {
      CWinBaceObj* pTempWin = (*pos);
      if(pTempWin && pTempWin == pWnd)
      {
         bFound = TRUE;
         break;
      }
      pos++;
   }
   return bFound;
}



/////////////////////////////////////////////////
///
/// NAME: AddChild
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Adds a child
///
/// CREATED:     02/06/2004 12:25:37 PM
///
/// PARAMETERS: 
///             CWinBaceObj *pWnd
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWindowMgr::AddChild(CWinBaceObj *pWnd)
{
   if(!FindChild(pWnd))
      m_Childen.push_back(pWnd);
}



/////////////////////////////////////////////////
///
/// NAME: RemoveChild
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Remove child
///
/// CREATED:     02/06/2004 12:27:00 PM
///
/// PARAMETERS: 
///             CWinBaceObj *pRemoveWin
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWindowMgr::RemoveChild(CWinBaceObj *pRemoveWin)
{
   m_Childen.remove(pRemoveWin);
   m_ToolTips.remove(pRemoveWin);
}



/////////////////////////////////////////////////
///
/// NAME: MouseDown
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Handel mouse down
///
/// CREATED:     02/06/2004 12:26:48 PM
///
/// PARAMETERS: 
///             CPoint p
///             int &Id
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWindowMgr::MouseDown(const CPoint& p, int &Id)
{
   BOOL isHanded = FALSE;
   std::list<CWinBaceObj*>::reverse_iterator pos = m_Childen.rbegin();
   while( pos != m_Childen.rend() )
   {
      CWinBaceObj* pWin = (*pos);
      if(pWin && pWin->IsEnabled())
      {
         isHanded = pWin->MouseDown(p,Id);
         if(isHanded)
            break;
      }
      pos++;
   }
   return isHanded;
}



/////////////////////////////////////////////////
///
/// NAME: MouseUp
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: handel mouse up
///
/// CREATED:     02/06/2004 12:26:38 PM
///
/// PARAMETERS: 
///             CPoint p
///             int &Id
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWindowMgr::MouseUp(const CPoint& p, int &Id)
{
   BOOL isHanded = FALSE;
   std::list<CWinBaceObj*>::reverse_iterator pos = m_Childen.rbegin();
   while( pos != m_Childen.rend() )
   {
      CWinBaceObj* pWin = (*pos);
      {
         isHanded = pWin->MouseUp(p,Id);
         if(isHanded)
            break;
      }
      pos++;
   }
   return isHanded;
}



/////////////////////////////////////////////////
///
/// NAME: MouseOver
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Handel mouse over
///
/// CREATED:     02/06/2004 12:26:30 PM
///
/// PARAMETERS: 
///             CPoint p
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWindowMgr::MouseOver(const CPoint& p)
{
   BOOL isHanded = FALSE;
   std::list<CWinBaceObj*>::iterator pos = m_Childen.begin();
   while( pos != m_Childen.end() )
   {
      CWinBaceObj* pWin = (*pos);
      if(pWin && pWin->IsEnabled())
      {
         isHanded = pWin->MouseOver(p);
      }
      pos++;
   }
   return isHanded;
}



/////////////////////////////////////////////////
///
/// NAME: AddToolTip
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Add tool tip
///
/// CREATED:     02/06/2004 12:26:17 PM
///
/// PARAMETERS: 
///             CWinBaceObj *pTip
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWindowMgr::AddToolTip(CWinBaceObj *pTip)
{
   m_ToolTips.push_front(pTip);
}



/////////////////////////////////////////////////
///
/// NAME: CharPressed
///
/// CLASS: CWindowMgr
///
/// DESCRIPTION: Char has been pressed
///
/// CREATED:     02/06/2004 12:25:58 PM
///
/// PARAMETERS: 
///             char c
///             DWORD keycode
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWindowMgr::CharPressed(char c, DWORD keycode)
{
   BOOL isHanded = FALSE;
   std::list<CWinBaceObj*>::reverse_iterator pos = m_Childen.rbegin();
   while( pos != m_Childen.rend() )
   {
      CWinBaceObj* pWin = (*pos);
      if(pWin && pWin->IsEnabled())
      {
         isHanded = pWin->CharPressed(c, keycode);
         if(isHanded)
            break;
      }
      pos++;
   }
   return isHanded;
}
