///////////////////////////////////////////////////////////////////////////////
///  WinDialog.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:20:01 PM by Christiaan Burchell
///
///  PURPOSE:  the dialog window
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

// WinDialog.h: interface for the CWinDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDIALOG_H__3D7FDA13_4F13_4759_A94A_63B97242EA3F__INCLUDED_)
#define AFX_WINDIALOG_H__3D7FDA13_4F13_4759_A94A_63B97242EA3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <WinBaceObj.h>
#include <input.h>
#include <d3dapp.h>
#include <WindowMgr.h>

class CWinDialog : public CWinBaceObj  
{
public:
   virtual int DoIdle();
   void ShowTitleBar(BOOL show = TRUE);
   virtual BOOL ValidButtonID(int Id);
   virtual int DoModal(CInput* pInput,CD3DApplication* pApp, CWindowMgr* pMgr, CWinBaceObj* pParent = NULL);
   virtual void Display(C2DDraw *pDisplay);
   CWinDialog();
   virtual ~CWinDialog();
   virtual BOOL MouseOver(const CPoint& p);
   virtual BOOL MouseUp(const CPoint& p,int& Id,BOOL bForceCheck = FALSE);
   virtual BOOL MouseDown(const CPoint& p, int& Id,BOOL bForceCheck = FALSE);
   virtual BOOL Create(C2DDraw *pDisplay, CWinBaceObj *pParent,const CPoint& p, const CSize& s, const TCHAR* str);
   CRect GetBarRect();

   virtual void OnDistroy();

   void EndDialog(int code)
   {
      m_bEndDialog = TRUE;
      m_Endcode = code;
   }


protected:
   BOOL LoadDialog(int id,C2DDraw *pDisplay);
   BOOL m_bTitleBar;
   BOOL m_bDrag;
   CPoint m_DragStart;
   CRect m_BarRec;
   BOOL m_bCanMove;

   BOOL m_bEndDialog;
   int  m_Endcode;
};

#endif // !defined(AFX_WINDIALOG_H__3D7FDA13_4F13_4759_A94A_63B97242EA3F__INCLUDED_)
