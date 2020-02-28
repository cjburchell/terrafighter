///////////////////////////////////////////////////////////////////////////////
///  LevelTreeBar.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 11:22:24 PM by Christiaan Burchell
///
///  PURPOSE:  The Level Tree Bar
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

// LevelTreeBar.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "LevelTreeBar.h"
#include "LevelData.h"
#include "EditEvent.h"
#include "EditLevel.h"
#include "EditObject.h"
#include "EditSprite.h"
#include "EditTextrue.h"
#include "TerraEdDoc.h"
#include "ChildFrm.h"
#include "EditSpriteData.h"
#include "EditObjectData.h"
#include "TerraEdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CLevelTreeBar dialog






/////////////////////////////////////////////////
///
/// NAME: CLevelTreeBar
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 11:22:28 PM
///
/// PARAMETERS: 
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CLevelTreeBar::CLevelTreeBar(CWnd* pParent /*=NULL*/)
: CDialog(CLevelTreeBar::IDD, pParent)
{
   //{{AFX_DATA_INIT(CLevelTreeBar)
   // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT

   m_pDragImage = NULL;
   m_bLDragging = FALSE;
   m_hitemDrag = NULL;

   m_Icons.Create(IDB_EDIT_ICONS,16,0,RGB(255,0,255));
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 11:22:30 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CLevelTreeBar)
   DDX_Control(pDX, IDC_OBJECTS_TREE, m_ObjectTree);
   DDX_Control(pDX, IDC_SPRITE_TREE, m_SpriteTree);
   DDX_Control(pDX, IDC_WEAPONS_TREE, m_WeaponTree);
   DDX_Control(pDX, IDC_TAB1, m_Tab);
   DDX_Control(pDX, IDC_LEVEL_TREE, m_LevelTree);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLevelTreeBar, CDialog)
   //{{AFX_MSG_MAP(CLevelTreeBar)
   ON_WM_SIZE()
   ON_NOTIFY(NM_RCLICK, IDC_LEVEL_TREE, OnRclickTree1)
   ON_NOTIFY(NM_CLICK, IDC_LEVEL_TREE, OnClickTree1)
   ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
   ON_NOTIFY(NM_CLICK, IDC_SPRITE_TREE, OnClickTree2)
   ON_NOTIFY(NM_CLICK, IDC_OBJECTS_TREE, OnClickTree3)
   ON_NOTIFY(NM_CLICK, IDC_WEAPONS_TREE, OnClickTree4)
   ON_NOTIFY(NM_RCLICK, IDC_SPRITE_TREE, OnRclickTree)
   ON_NOTIFY(NM_DBLCLK, IDC_LEVEL_TREE, OnDblclkTree)
   ON_NOTIFY(TVN_BEGINDRAG, IDC_OBJECTS_TREE, OnBegindragTree3)
   ON_WM_MOUSEMOVE()
   ON_WM_LBUTTONUP()
   ON_NOTIFY(NM_DBLCLK, IDC_SPRITE_TREE, OnDblclkTree)
   ON_NOTIFY(NM_DBLCLK, IDC_OBJECTS_TREE, OnDblclkTree)
   ON_NOTIFY(NM_RCLICK, IDC_OBJECTS_TREE, OnRclickTree)
   ON_NOTIFY(NM_DBLCLK, IDC_WEAPONS_TREE, OnDblclkTree)
   ON_NOTIFY(NM_RCLICK, IDC_WEAPONS_TREE, OnRclickTree)
   ON_NOTIFY(TVN_BEGINDRAG, IDC_SPRITE_TREE, OnBegindragTree2)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLevelTreeBar message handlers



/////////////////////////////////////////////////
///
/// NAME: OnSize
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Resizes the window
///
/// CREATED:     02/06/2004 11:22:33 PM
///
/// PARAMETERS: 
///             UINT nType
///             int cx
///             int cy
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnSize(UINT nType, int cx, int cy) 
{
   CDialog::OnSize(nType, cx, cy);

   CRect r;

   if(IsWindow(m_Tab))
   {
      m_Tab.MoveWindow(0,0,cx,cy-1);
      m_Tab.GetClientRect(r);
      r.DeflateRect(3,3,3,25);
   }

   if(IsWindow(m_LevelTree))
      m_LevelTree.MoveWindow(r);

   if(IsWindow(m_SpriteTree))
      m_SpriteTree.MoveWindow(r);

   if(IsWindow(m_ObjectTree))
      m_ObjectTree.MoveWindow(r);

   if(IsWindow(m_WeaponTree))
      m_WeaponTree.MoveWindow(r);

}



/////////////////////////////////////////////////
///
/// NAME: OnInitDialog
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Initializes the dialog
///
/// CREATED:     02/06/2004 11:22:36 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CLevelTreeBar::OnInitDialog() 
{
   CDialog::OnInitDialog();


   m_Tab.SetImageList(&m_Icons);
   m_Tab.InsertItem(LEVEL_TAB,"Level",8);
   m_Tab.InsertItem(SPRITE_TAB,"Sprites",27);
   m_Tab.InsertItem(OBJECT_TAB,"Objects",24);
   m_Tab.InsertItem(WEAPONS_TAB,"Weapons",23);
   m_Tab.SetCurSel(LEVEL_TAB);

   m_SpriteTree.ShowWindow(SW_HIDE);
   m_ObjectTree.ShowWindow(SW_HIDE);
   m_WeaponTree.ShowWindow(SW_HIDE);

   // TODO: Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}




/////////////////////////////////////////////////
///
/// NAME: OnRclickTree1
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: User has right clicked the tree display the popup menu
///
/// CREATED:     02/06/2004 11:23:46 PM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
   CPoint point;
   GetCursorPos(&point);

   m_LevelTree.ScreenToClient(&point);

   HTREEITEM hItem = m_LevelTree.HitTest(point);
   if(hItem)
   {
      m_LevelTree.SelectItem(hItem);

      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(m_LevelTree.GetItemData(hItem));


      if (point.x == -1 && point.y == -1)
      {
         //keystroke invocation
         CRect rect;
         GetClientRect(rect);
         ClientToScreen(rect);

         point = rect.TopLeft();
         point.Offset(5, 5);
      }

      m_LevelTree.ClientToScreen(&point);

      CMenu menu;
      VERIFY(menu.LoadMenu(IDR_OBJ_POPUP));

      CMenu* pPopup = menu.GetSubMenu(pObj->GetObjType());
      ASSERT(pPopup != NULL);
      CWnd* pWndPopupOwner = this;

      while (pWndPopupOwner->GetStyle() & WS_CHILD)
         pWndPopupOwner = pWndPopupOwner->GetParent();

      pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
         pWndPopupOwner);
   }

   *pResult = 0;
}



/////////////////////////////////////////////////
///
/// NAME: ShowEditDialog
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Display the Edit dialog
///
/// CREATED:     02/06/2004 11:24:14 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::ShowEditDialog()
{
   if(m_Tab.GetCurSel() == LEVEL_TAB)
   {
      HTREEITEM hItem = m_LevelTree.GetSelectedItem();
      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(m_LevelTree.GetItemData(hItem));


      switch(pObj->GetObjType())
      {
      case DATA_OBJ_TYPE_LEVEL:
         {
            CEditLevel dlg(static_cast<CLevelData*>(pObj));
            if(dlg.DoModal() == IDOK)
            {
               CView* pView = GetParentFrame()->GetActiveView();
               if( pView->IsKindOf(RUNTIME_CLASS(CTerraEdView)) )
               {
                  if(pObj == static_cast<CTerraEdView*>(pView)->GetCurrentLevel())
                     static_cast<CTerraEdView*>(pView)->UpdateMap();
               }
            }

            m_LevelTree.SetItemText(hItem,static_cast<CLevelData*>(pObj)->m_Name);
         }
         break;
      case DATA_OBJ_TYPE_TEXTURE: 
         {
            CView* pView = GetParentFrame()->GetActiveView();
            CEditTexture dlg(pView,static_cast<CTextureData*>(pObj));
            if(dlg.DoModal() == IDOK)
            {
               if( pView->IsKindOf(RUNTIME_CLASS(CTerraEdView)) )
               {
                  static_cast<CTerraEdView*>(pView)->UpdateMap();
               }
            }

            m_LevelTree.SetItemText(hItem,"Texture - "+ static_cast<CTextureData*>(pObj)->m_File);
         }
         break;
      case DATA_OBJ_TYPE_SPRITE:
         {
            CLevelData* pParentObj = NULL;
            HTREEITEM hParentItem = hItem;

            while(pParentObj == NULL && hParentItem != NULL)
            {
               hParentItem = m_LevelTree.GetParentItem(hParentItem);
               CDataObjBase* pTempObj = reinterpret_cast<CDataObjBase*>(m_LevelTree.GetItemData(hParentItem));
               if(pTempObj->GetObjType() == DATA_OBJ_TYPE_LEVEL)
               {
                  pParentObj = static_cast<CLevelData*>(pTempObj);
                  break;
               }
            }


            CEditSprite dlg(static_cast<CLevelDataSprite*>(pObj),&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_SpriteData,pParentObj);
            dlg.DoModal();

            switch(static_cast<CLevelDataSprite*>(pObj)->GetSpriteObjType())
            {
            case 0:
               m_LevelTree.SetItemText(hItem,"Sprite - "+static_cast<CLevelDataSprite*>(pObj)->m_Name);
               break;
            case 1:
               m_LevelTree.SetItemText(hItem,"Change Sprite - "+static_cast<CLevelDataSprite*>(pObj)->m_Name);
               break;
            case 2:
               m_LevelTree.SetItemText(hItem,"Create Sprite - "+static_cast<CLevelDataSprite*>(pObj)->m_Name);
               break;
            }

         }
         break;
      case DATA_OBJ_TYPE_OBJECT:
         {
            CEditObject dlg(static_cast<CLevelDataObject*>(pObj),&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_PlanetObjectData);
            dlg.DoModal();

            CPlanetObjectData* pData = static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_PlanetObjectData[static_cast<CLevelDataObject*>(pObj)->m_Type];

            m_LevelTree.SetItemText(hItem,"Object - " + static_cast<CLevelDataObject*>(pObj)->m_Name);
         }
         break;
      case DATA_OBJ_TYPE_EVENT:
         {
            CLevelData* pParentObj = NULL;
            HTREEITEM hParentItem = hItem;

            while(pParentObj == NULL && hParentItem != NULL)
            {
               hParentItem = m_LevelTree.GetParentItem(hParentItem);
               CDataObjBase* pTempObj = reinterpret_cast<CDataObjBase*>(m_LevelTree.GetItemData(hParentItem));
               if(pTempObj->GetObjType() == DATA_OBJ_TYPE_LEVEL)
               {
                  pParentObj = static_cast<CLevelData*>(pTempObj);
                  break;
               }
            }

            CEditEvent dlg(static_cast<CGameEvent*>(pObj),pParentObj);
            dlg.DoModal();
            m_LevelTree.SetItemText(hItem,"Event - " + static_cast<CGameEvent*>(pObj)->m_Name);
         }
         break;
      }	
   }
   else if(m_Tab.GetCurSel() == SPRITE_TAB)
   {
      HTREEITEM hItem = m_SpriteTree.GetSelectedItem();
      CSpriteDataEx* pObj = reinterpret_cast<CSpriteDataEx*>(m_SpriteTree.GetItemData(hItem));
      CEditSpriteData dlg(pObj,&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_WeaponData);
      if(dlg.DoModal() == IDOK)
      {
         CView* pView = GetParentFrame()->GetActiveView();
         if( pView->IsKindOf(RUNTIME_CLASS(CTerraEdView)) )
         {
            pObj->DeleteDeviceObjects();
            pObj->RestoreDeviceObjects(static_cast<CTerraEdView*>(pView)->GetDevice(),&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_Zip);
         }
      }
      m_SpriteTree.SetItemText(hItem,pObj->m_Name);
   }
   else if(m_Tab.GetCurSel() == OBJECT_TAB)
   {
      HTREEITEM hItem = m_ObjectTree.GetSelectedItem();
      CPlanetObjectDataEx* pObj = reinterpret_cast<CPlanetObjectDataEx*>(m_ObjectTree.GetItemData(hItem));
      CEditObjectData dlg(pObj);
      if(dlg.DoModal() == IDOK)
      {
         CView* pView = GetParentFrame()->GetActiveView();
         if( pView->IsKindOf(RUNTIME_CLASS(CTerraEdView)) )
         {
            pObj->DeleteDeviceObjects();
            pObj->RestoreDeviceObjects(static_cast<CTerraEdView*>(pView)->GetDevice(),&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_Zip);
         }
      }
      m_ObjectTree.SetItemText(hItem, pObj->m_Name);
   }
   else if(m_Tab.GetCurSel() == WEAPONS_TAB)
   {
      HTREEITEM hItem = m_WeaponTree.GetSelectedItem();
      CWeaponData* pObj = reinterpret_cast<CWeaponData*>(m_WeaponTree.GetItemData(hItem));
      //CEditWeaponData dlg(pObj);
      //dlg.DoModal();
      m_WeaponTree.SetItemText(hItem, pObj->m_Name);
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnClickTree1
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Select the on a single click
///
/// CREATED:     02/06/2004 11:24:46 PM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
   CPoint point;
   GetCursorPos(&point);

   m_LevelTree.ScreenToClient(&point);

   HTREEITEM hItem = m_LevelTree.HitTest(point);
   if(hItem)
   {
      m_LevelTree.SelectItem(hItem);
   }

   *pResult = 0;
}



/////////////////////////////////////////////////
///
/// NAME: OnClickTree4
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION:  Select the on a single click
///
/// CREATED:     02/06/2004 11:25:11 PM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnClickTree4(NMHDR* pNMHDR, LRESULT* pResult) 
{
   CPoint point;
   GetCursorPos(&point);

   m_WeaponTree.ScreenToClient(&point);

   HTREEITEM hItem = m_WeaponTree.HitTest(point);
   if(hItem)
   {
      m_WeaponTree.SelectItem(hItem);
   }

   *pResult = 0;
}



/////////////////////////////////////////////////
///
/// NAME: OnCancel
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Closes the dialog
///
/// CREATED:     02/06/2004 11:23:00 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnCancel() 
{
   // Disable closing of the dialog when ESC is pressed

   //CDialog::OnCancel();
}



/////////////////////////////////////////////////
///
/// NAME: OnOK
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: User Pressed the OK Button
///
/// CREATED:     02/06/2004 11:22:47 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnOK() 
{
   // Disable closing of the dialog when enter is pressed

   //CDialog::OnOK();
}



/////////////////////////////////////////////////
///
/// NAME: SetActiveTab
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Activates a given tab
///
/// CREATED:     02/06/2004 11:25:42 PM
///
/// PARAMETERS: 
///             int tab
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::SetActiveTab(int tab) 
{
   m_Tab.SetCurSel(tab);
   switch(m_Tab.GetCurSel())
   {
   case LEVEL_TAB:
      m_LevelTree.ShowWindow(SW_SHOWNOACTIVATE);	
      m_SpriteTree.ShowWindow(SW_HIDE);
      m_ObjectTree.ShowWindow(SW_HIDE);
      m_WeaponTree.ShowWindow(SW_HIDE);
      m_LevelTree.BringWindowToTop();
      break;
   case SPRITE_TAB:
      m_SpriteTree.ShowWindow(SW_SHOWNOACTIVATE);
      m_SpriteTree.BringWindowToTop();
      m_LevelTree.ShowWindow(SW_HIDE);
      m_ObjectTree.ShowWindow(SW_HIDE);
      m_WeaponTree.ShowWindow(SW_HIDE);
      break;
   case OBJECT_TAB:
      m_ObjectTree.ShowWindow(SW_SHOWNOACTIVATE);
      m_ObjectTree.BringWindowToTop();
      m_SpriteTree.ShowWindow(SW_HIDE);
      m_LevelTree.ShowWindow(SW_HIDE);
      m_WeaponTree.ShowWindow(SW_HIDE);
      break;
   case WEAPONS_TAB:
      m_WeaponTree.ShowWindow(SW_SHOWNOACTIVATE);
      m_WeaponTree.BringWindowToTop();
      m_SpriteTree.ShowWindow(SW_HIDE);
      m_LevelTree.ShowWindow(SW_HIDE);
      m_ObjectTree.ShowWindow(SW_HIDE);
      break;
   }
   Invalidate();
}



/////////////////////////////////////////////////
///
/// NAME: OnSelchangeTab1
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: use has selected a new tab activate that tab
///
/// CREATED:     03/06/2004 8:54:15 AM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
   SetActiveTab(m_Tab.GetCurSel());
   *pResult = 0;
}



/////////////////////////////////////////////////
///
/// NAME: OnClickTree2
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Single click on the sprite tree
///
/// CREATED:     03/06/2004 8:55:03 AM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnClickTree2(NMHDR* pNMHDR, LRESULT* pResult) 
{
   CPoint point;
   GetCursorPos(&point);

   m_SpriteTree.ScreenToClient(&point);

   HTREEITEM hItem = m_SpriteTree.HitTest(point);
   if(hItem)
   {
      m_SpriteTree.SelectItem(hItem);
   }

   *pResult = 0;
}



/////////////////////////////////////////////////
///
/// NAME: OnClickTree3
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Single click on the object tree
///
/// CREATED:     03/06/2004 8:55:20 AM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnClickTree3(NMHDR* pNMHDR, LRESULT* pResult) 
{
   CPoint point;
   GetCursorPos(&point);

   m_ObjectTree.ScreenToClient(&point);

   HTREEITEM hItem = m_ObjectTree.HitTest(point);
   if(hItem)
   {
      m_ObjectTree.SelectItem(hItem);
   }

   *pResult = 0;
}



/////////////////////////////////////////////////
///
/// NAME: OnDblclkTree
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Double click on the tree
///
/// CREATED:     03/06/2004 8:55:59 AM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
   switch(m_Tab.GetCurSel())
   {
   case LEVEL_TAB:
      {
         HTREEITEM hItem = m_LevelTree.GetSelectedItem();
         ShowEditDialog();
         m_LevelTree.Expand(hItem,TVE_TOGGLE);
      }
      break;
   case SPRITE_TAB:
      {
         HTREEITEM hItem = m_SpriteTree.GetSelectedItem();
         ShowEditDialog();
         m_SpriteTree.Expand(hItem,TVE_TOGGLE);
      }

      break;
   case OBJECT_TAB:
      {
         HTREEITEM hItem = m_ObjectTree.GetSelectedItem();
         ShowEditDialog();
         m_ObjectTree.Expand(hItem,TVE_TOGGLE);
      }
      break;
   case WEAPONS_TAB:
      {
         HTREEITEM hItem = m_WeaponTree.GetSelectedItem();
         ShowEditDialog();
         m_ObjectTree.Expand(hItem,TVE_TOGGLE);
      }
      break;
   }
   *pResult = 0;
}



/////////////////////////////////////////////////
///
/// NAME: OnRclickTree
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Display menu on the given tree
///
/// CREATED:     03/06/2004 8:56:16 AM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
   CPoint point;
   GetCursorPos(&point);

   CTreeCtrl* pTree = NULL;

   switch(m_Tab.GetCurSel())
   {
   case SPRITE_TAB:
      pTree = &m_SpriteTree;

      break;
   case OBJECT_TAB:
      pTree = &m_ObjectTree;
      break;

   case WEAPONS_TAB:
      pTree = &m_WeaponTree;
      break;
   }

   if(pTree == NULL)
      return;

   pTree->ScreenToClient(&point);

   HTREEITEM hItem = pTree->HitTest(point);
   if(hItem)
   {
      pTree->SelectItem(hItem);		

      if (point.x == -1 && point.y == -1)
      {
         //keystroke invocation
         CRect rect;
         GetClientRect(rect);
         ClientToScreen(rect);

         point = rect.TopLeft();
         point.Offset(5, 5);
      }

      pTree->ClientToScreen(&point);

      CMenu menu;
      VERIFY(menu.LoadMenu(IDR_OBJ_POPUP));

      CMenu* pPopup = menu.GetSubMenu(5);
      ASSERT(pPopup != NULL);
      CWnd* pWndPopupOwner = this;

      while (pWndPopupOwner->GetStyle() & WS_CHILD)
         pWndPopupOwner = pWndPopupOwner->GetParent();

      pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
         pWndPopupOwner);
   }

   *pResult = 0;
}



/////////////////////////////////////////////////
///
/// NAME: OnBegindragTree3
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Start the drag on the Object tree
///
/// CREATED:     03/06/2004 8:56:43 AM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnBegindragTree3(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
   *pResult = 0;

   m_hitemDrag = pNMTreeView->itemNew.hItem;
   m_ObjectTree.SelectItem(m_hitemDrag);

   m_pDragImage = m_ObjectTree.CreateDragImage(m_hitemDrag);  // get the image list for dragging
   // CreateDragImage() returns NULL if no image list
   // associated with the tree view control
   if( !m_pDragImage )
      return;

   m_bLDragging = TRUE;
   m_pDragImage->BeginDrag(0, CPoint(0,0));
   POINT pt = pNMTreeView->ptDrag;
   ClientToScreen( &pt );
   m_pDragImage->DragEnter(GetDesktopWindow(), pt);
   SetCapture();
}



/////////////////////////////////////////////////
///
/// NAME: OnMouseMove
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Mouse has moved
///
/// CREATED:     02/06/2004 11:23:19 PM
///
/// PARAMETERS: 
///             UINT nFlags
///             CPoint point
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnMouseMove(UINT nFlags, CPoint point) 
{
   if (m_bLDragging)
   {
      POINT pt = point;
      ClientToScreen( &pt );
      m_pDragImage->DragMove(pt);
      m_pDragImage->DragShowNolock(FALSE);
      CWnd* pDropWnd = WindowFromPoint(pt);
      if( pDropWnd->IsKindOf(RUNTIME_CLASS(CTerraEdView)) )
      {
         CPoint tmppt = pt;
         CTerraEdView* pView = (CTerraEdView*)pDropWnd; 
         pView->ScreenToClient(&tmppt);
         pView->m_PickPos = pView->PickPoint(tmppt);
         pView->SetViewMode(VM_SELECT_OBJ);
         pView->Render3DEnvironment();
      }	
      pDropWnd->ScreenToClient(&pt);

      m_pDragImage->DragShowNolock(TRUE);
   }

   CDialog::OnMouseMove(nFlags, point);
}



/////////////////////////////////////////////////
///
/// NAME: OnLButtonUp
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Handel the drop end 
///
/// CREATED:     03/06/2004 8:57:05 AM
///
/// PARAMETERS: 
///             UINT nFlags
///             CPoint point
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
   if (m_bLDragging)
   {
      m_bLDragging = FALSE;
      m_pDragImage->DragLeave(GetDesktopWindow());
      m_pDragImage->EndDrag();
      ReleaseCapture();

      delete m_pDragImage;
      CPoint ptDropPoint = point;

      ClientToScreen(&ptDropPoint);


      CWnd* pDropWnd = WindowFromPoint(ptDropPoint);
      if( pDropWnd->IsKindOf(RUNTIME_CLASS(CTerraEdView)) )
      {
         CTerraEdView* pView = (CTerraEdView*)pDropWnd; 
         pView->ScreenToClient(&ptDropPoint);
         HTREEITEM hItem = m_hitemDrag;
         if(m_Tab.GetCurSel() == SPRITE_TAB)
         {
            CSpriteData* pObj = reinterpret_cast<CSpriteData*>(m_SpriteTree.GetItemData(hItem));
            pView->OnLevelAddSpriteAt(ptDropPoint, pObj);
         }
         else if(m_Tab.GetCurSel() == OBJECT_TAB)
         {
            CPlanetObjectData* pObj = reinterpret_cast<CPlanetObjectData*>(m_ObjectTree.GetItemData(hItem));
            pView->OnLevelAddObjectAt(ptDropPoint,pObj);
         }
         return;
      }
   }

   CDialog::OnLButtonUp(nFlags, point);
}



/////////////////////////////////////////////////
///
/// NAME: OnBegindragTree2
///
/// CLASS: CLevelTreeBar
///
/// DESCRIPTION: Start dragging on the Sprite tree
///
/// CREATED:     03/06/2004 8:57:34 AM
///
/// PARAMETERS: 
///             NMHDR* pNMHDR
///             LRESULT* pResult
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelTreeBar::OnBegindragTree2(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
   *pResult = 0;

   m_hitemDrag = pNMTreeView->itemNew.hItem;
   m_SpriteTree.SelectItem(m_hitemDrag);

   m_pDragImage = m_SpriteTree.CreateDragImage(m_hitemDrag);  // get the image list for dragging
   // CreateDragImage() returns NULL if no image list
   // associated with the tree view control
   if( !m_pDragImage )
      return;

   m_bLDragging = TRUE;
   m_pDragImage->BeginDrag(0, CPoint(0,0));
   POINT pt = pNMTreeView->ptDrag;
   ClientToScreen( &pt );
   m_pDragImage->DragEnter(GetDesktopWindow(), pt);
   SetCapture();
}
