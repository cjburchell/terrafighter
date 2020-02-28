///////////////////////////////////////////////////////////////////////////////
///  ChildFrm.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:19:31 PM by Christiaan Burchell
///
///  PURPOSE:  The Child Frame
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

// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "TerraEd.h"

#include "ChildFrm.h"
#include <dxutil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT indicators[] =
{
   ID_SEPARATOR,           // status line indicator
      ID_INDICATOR_CAPS,
      ID_INDICATOR_NUM,
      ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CFrameWnd)
   ON_COMMAND_EX(CG_ID_VIEW_PAINTBAR, OnBarCheck)
   ON_UPDATE_COMMAND_UI(CG_ID_VIEW_PAINTBAR, OnUpdateControlBarMenu)
   //{{AFX_MSG_MAP(CChildFrame)
   ON_WM_CREATE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction



/////////////////////////////////////////////////
///
/// NAME: CChildFrame
///
/// CLASS: CChildFrame
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:19:03 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CChildFrame::CChildFrame()
{
   // TODO: add member initialization code here

}



/////////////////////////////////////////////////
///
/// NAME: ~CChildFrame
///
/// CLASS: CChildFrame
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 5:19:04 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CChildFrame::~CChildFrame()
{
}



/////////////////////////////////////////////////
///
/// NAME: PreCreateWindow
///
/// CLASS: CChildFrame
///
/// DESCRIPTION: Sets up window Creation
///
/// CREATED:     02/06/2004 5:19:06 PM
///
/// PARAMETERS: 
///             CREATESTRUCT& cs
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   if( !CFrameWnd::PreCreateWindow(cs) )
      return FALSE;

   return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG


/////////////////////////////////////////////////
///
/// NAME: AssertValid
///
/// CLASS: CChildFrame
///
/// DESCRIPTION: Debug Function
///
/// CREATED:     02/06/2004 5:19:09 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CChildFrame::AssertValid() const
{
   CFrameWnd::AssertValid();
}



/////////////////////////////////////////////////
///
/// NAME: Dump
///
/// CLASS: CChildFrame
///
/// DESCRIPTION: Debug Function
///
/// CREATED:     02/06/2004 5:19:11 PM
///
/// PARAMETERS: 
///             CDumpContext& dc
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CChildFrame::Dump(CDumpContext& dc) const
{
   CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers



/////////////////////////////////////////////////
///
/// NAME: OnCreate
///
/// CLASS: CChildFrame
///
/// DESCRIPTION: Creates the window
///
/// CREATED:     02/06/2004 5:19:14 PM
///
/// PARAMETERS: 
///             LPCREATESTRUCT lpCreateStruct
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
   if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
      return -1;

   if (!m_wndToolBar.CreateEx(this) ||
      !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
   {
      sys_msg("Failed to create toolbar\n");
      return -1;      // fail to create
   }

   if (!m_wndPaintBar.Create(this, CG_IDD_PAINTBAR, 
      CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
   {
      sys_msg("Failed to create dialogbar\n");
      return -1;		// fail to create
   }

   if (!m_wndReBar.Create(this) ||
      !m_wndReBar.AddBar(&m_wndToolBar) ||
      !m_wndReBar.AddBar(&m_wndPaintBar))
   {
      sys_msg("Failed to create rebar\n");
      return -1;      // fail to create
   }

   if (!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators,
      sizeof(indicators)/sizeof(UINT)))
   {
      sys_msg("Failed to create status bar\n");
      return -1;      // fail to create
   }

   // TODO: Remove this if you don't want tool tips
   m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
      CBRS_TOOLTIPS | CBRS_FLYBY);

   // TODO: Add a menu item that will toggle the visibility of the
   // dialog bar named "Paint Bar":
   //   1. In ResourceView, open the menu resource that is used by
   //      the CMainFrame class
   //   2. Select the View submenu
   //   3. Double-click on the blank item at the bottom of the submenu
   //   4. Assign the new item an ID: CG_ID_VIEW_PAINTBAR
   //   5. Assign the item a Caption: Paint Bar

   // TODO: Change the value of CG_ID_VIEW_PAINTBAR to an appropriate value:
   //   1. Open the file resource.h
   // CG: The following block was inserted by the 'Dialog Bar' component

   EnableDocking(CBRS_ALIGN_ANY);


   CString title("CoolBar Demo");
   if (!m_DialogBar.Create(this, &m_wndPalette,title, CG_IDD_PALETTE))
   {
      sys_msg("Failed to create dialogbar\n");
      return -1;      // fail to create
   }

   // TODO: Add your specialized creation code here

   return 0;
}
