///////////////////////////////////////////////////////////////////////////////
///  ChildFrm.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:12:18 AM by Christiaan Burchell
///
///  PURPOSE:  the main child frame window
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

// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__C1D4BEFE_A391_4D9A_A784_4FD59E660186__INCLUDED_)
#define AFX_CHILDFRM_H__C1D4BEFE_A391_4D9A_A784_4FD59E660186__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CoolDialogBar.h"
#include "LevelTreeBar.h"


class CChildFrame : public CFrameWnd
{
   DECLARE_DYNCREATE(CChildFrame)
public:
   CChildFrame();

   // Attributes
public:
   CLevelTreeBar m_wndPalette;
   CCoolDialogBar m_DialogBar;

   CStatusBar  m_wndStatusBar;
   CToolBar    m_wndToolBar;
   CReBar      m_wndReBar;

   CDialogBar m_wndPaintBar;

   // Operations
public:

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CChildFrame)
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   //}}AFX_VIRTUAL

   // Implementation
public:
   virtual ~CChildFrame();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

   // Generated message map functions
protected:
   //{{AFX_MSG(CChildFrame)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__C1D4BEFE_A391_4D9A_A784_4FD59E660186__INCLUDED_)
