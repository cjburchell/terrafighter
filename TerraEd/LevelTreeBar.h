///////////////////////////////////////////////////////////////////////////////
///  LevelTreeBar.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:14:20 AM by Christiaan Burchell
///
///  PURPOSE:  Tree dialog bar
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

#if !defined(AFX_LEVELTREEBAR_H__29D940EC_ED41_4A5B_9CE9_854C8C705A04__INCLUDED_)
#define AFX_LEVELTREEBAR_H__29D940EC_ED41_4A5B_9CE9_854C8C705A04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LevelTreeBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLevelTreeBar dialog

#define LEVEL_TAB 0
#define SPRITE_TAB 1
#define OBJECT_TAB 2
#define WEAPONS_TAB 3

class CLevelTreeBar : public CDialog
{
   // Construction
public:
   void ShowEditDialog();
   CLevelTreeBar(CWnd* pParent = NULL);   // standard constructor

   void SetActiveTab(int tab);

   //HTREEITEM m_hCurrentItem;

   // Dialog Data
   //{{AFX_DATA(CLevelTreeBar)
   enum { IDD = CG_IDD_PALETTE };
   CTreeCtrl	m_WeaponTree;
   CTreeCtrl	m_ObjectTree;
   CTreeCtrl	m_SpriteTree;
   CTabCtrl	m_Tab;
   CTreeCtrl	m_LevelTree;
   //}}AFX_DATA

   CImageList m_Icons;

   CImageList*	m_pDragImage;
   BOOL		m_bLDragging;
   HTREEITEM	m_hitemDrag;


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CLevelTreeBar)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

   // Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CLevelTreeBar)
   afx_msg void OnSize(UINT nType, int cx, int cy);
   virtual BOOL OnInitDialog();
   afx_msg void OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
   virtual void OnCancel();
   virtual void OnOK();
   afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnClickTree2(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnClickTree3(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnClickTree4(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnBegindragTree3(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnBegindragTree2(NMHDR* pNMHDR, LRESULT* pResult);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEVELTREEBAR_H__29D940EC_ED41_4A5B_9CE9_854C8C705A04__INCLUDED_)
