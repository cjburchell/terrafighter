///////////////////////////////////////////////////////////////////////////////
///  TerraEdView.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:16:47 AM by Christiaan Burchell
///
///  PURPOSE:  The main view object
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

// TerraEdView.cpp : implementation of the CTerraEdView class
//

#include "stdafx.h"
#include "TerraEd.h"

#include "TerraEdDoc.h"
#include "TerraEdView.h"
#include "ChildFrm.h"
#include "EditSprite.h"
#include "EditObject.h"
#include "EditTextrue.h"
#include "EditEvent.h"
#include "EditLevel.h"
#include "EditSpriteData.h"
#include "EditObjectData.h"
#include "D3DUtil.h"
#include "dxutil.h"
#include "RandomObjDlg.h"
#include "FillDialog.h"

#define WORLD_RAD 400.0f
#define WORLD_ATM_THICK 50.0f


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerraEdView

IMPLEMENT_DYNCREATE(CTerraEdView, CView)

BEGIN_MESSAGE_MAP(CTerraEdView, CView)
   //{{AFX_MSG_MAP(CTerraEdView)
   ON_CBN_SELCHANGE(IDC_BRUSH, OnSelchangeBrush)
   ON_CBN_SELCHANGE(IDC_BRUSH_TEXTURE, OnSelchangeBrushTextrue)
   ON_COMMAND(ID_EDIT, OnEdit)
   ON_COMMAND(ID_DELETE, OnDelete)
   ON_COMMAND(ID_EVENT_ADD_CREATESPRITE, OnEventAddCreatesprite)
   ON_COMMAND(ID_LEVEL_ADD_EVENT, OnLevelAddEvent)
   ON_COMMAND(ID_LEVEL_ADD_OBJECT, OnLevelAddObject)
   ON_COMMAND(ID_LEVEL_ADD_SPRITE, OnLevelAddSprite)
   ON_COMMAND(ID_LEVEL_ADD_TEXTURE, OnLevelAddTexture)
   ON_UPDATE_COMMAND_UI(ID_EVENT_ADD_CREATESPRITE, OnUpdateEvent)
   ON_UPDATE_COMMAND_UI(ID_LEVEL_ADD_EVENT, OnUpdateLevel)
   ON_COMMAND(ID_TOOLS_NEW_LEVEL, OnToolsNewLevel)
   ON_COMMAND(ID_LEVEL_ACTIVE, OnLevelActive)
   ON_UPDATE_COMMAND_UI(ID_LEVEL_ACTIVE, OnUpdateLevelActive)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_NEW_LEVEL, OnUpdateToolsNewLevel)
   ON_COMMAND(ID_TOOLS_NEW_OBJECTDATA, OnToolsNewObjectdata)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_NEW_OBJECTDATA, OnUpdateToolsNewObjectdata)
   ON_COMMAND(ID_TOOLS_NEW_SPRITEDATA, OnToolsNewSpritedata)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_NEW_SPRITEDATA, OnUpdateToolsNewSpritedata)
   ON_WM_SIZE()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_COMMAND(ID_SELECT_MODE, OnSelectMode)
   ON_UPDATE_COMMAND_UI(ID_SELECT_MODE, OnUpdateSelectMode)
   ON_COMMAND(ID_PAN_MODE, OnPanMode)
   ON_UPDATE_COMMAND_UI(ID_PAN_MODE, OnUpdatePanMode)
   ON_COMMAND(ID_ROTATE_SELECTED, OnRotateSelected)
   ON_UPDATE_COMMAND_UI(ID_ROTATE_SELECTED, OnUpdateRotateSelected)
   ON_COMMAND(ID_ROTATE_WORLD, OnRotateWorld)
   ON_UPDATE_COMMAND_UI(ID_ROTATE_WORLD, OnUpdateRotateWorld)
   ON_WM_SETCURSOR()
   ON_COMMAND(ID_TOOLS_CREATERANDOMOBJECTS, OnToolsCreaterandomobjects)
   ON_WM_LBUTTONDBLCLK()
   ON_COMMAND(ID_EDIT_TERRAIN, OnEditTerrain)
   ON_UPDATE_COMMAND_UI(ID_EDIT_TERRAIN, OnUpdateEditTerrain)
   ON_COMMAND(ID_PAINT_MODE, OnPaintMode)
   ON_UPDATE_COMMAND_UI(ID_PAINT_MODE, OnUpdatePaintMode)
   ON_WM_RBUTTONDOWN()
   ON_WM_RBUTTONUP()
   ON_COMMAND(ID_FILE_SAVECURRENTTERRAIN, OnFileSavecurrentterrain)
   ON_COMMAND(ID_FILE_SAVETEXTURES, OnFileSavetextures)
   ON_COMMAND(ID_VIEW_FOG, OnViewFog)
   ON_UPDATE_COMMAND_UI(ID_VIEW_FOG, OnUpdateViewFog)
   ON_COMMAND(ID_TOOLS_FILL, OnToolsFill)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_FILL, OnUpdateToolsFill)
   ON_WM_DESTROY()
   ON_COMMAND(ID_EVENT_ADD_CHANGESPRITE, OnEventAddChangesprite)
   ON_COMMAND(ID_EVENT_ADD_CREATEOBJECT, OnEventAddCreateobject)
   ON_COMMAND(ID_EVENT_DISPLAY, OnEventDisplay)
   ON_UPDATE_COMMAND_UI(ID_EVENT_DISPLAY, OnUpdateEventDisplay)
   ON_COMMAND(ID_SPRITE_VIEW, OnSpriteView)
   ON_UPDATE_COMMAND_UI(ID_LEVEL_ADD_OBJECT, OnUpdateLevel)
   ON_UPDATE_COMMAND_UI(ID_LEVEL_ADD_SPRITE, OnUpdateLevel)
   ON_UPDATE_COMMAND_UI(ID_LEVEL_ADD_TEXTURE, OnUpdateLevel)
   ON_UPDATE_COMMAND_UI(ID_EVENT_ADD_CHANGESPRITE, OnUpdateEvent)
   ON_UPDATE_COMMAND_UI(ID_EVENT_ADD_CREATEOBJECT, OnUpdateEvent)
   ON_UPDATE_COMMAND_UI(ID_SPRITE_VIEW, OnUpdateSpriteView)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerraEdView construction/destruction



/////////////////////////////////////////////////
///
/// NAME: CTerraEdView
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 9:16:53 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CTerraEdView::CTerraEdView()
{
   // TODO: add construction code here

   m_Icons.Create(IDB_EDIT_ICONS,16,0,RGB(255,0,255));
   m_pCurrentLevel = NULL;

   m_bActive           = FALSE;
   m_bReady            = FALSE;
   m_bWindowed         = TRUE;

   m_dwAdapter         = 0L;
   m_pD3D              = NULL;
   m_pd3dDevice        = NULL;
   m_bUseDepthBuffer   = TRUE;

   m_pSkyBox      = NULL;
   m_pTerrain = NULL;
   m_pWater = NULL;
   m_pBoxMesh = NULL;
   m_ViewMode = VM_ROTATE;
   m_bMouseDown= FALSE;
   m_bRMouseDown = FALSE;

   m_ZoomCursor = NULL;
   m_PanCursor = NULL;
   m_PanReadyCursor = NULL;
   m_RotateCursor = NULL;
   m_OldCursor = NULL;
   m_Rot = D3DXVECTOR3(0,0,0);
   m_CamPos = D3DXVECTOR3(0,5,0);
   m_SelObjPos = D3DXVECTOR3(0,5,0);
   m_Zoom = -4;
   m_pSelObject = NULL;

   m_pBrushs[0] = NULL;
   m_pBrushs[1] = NULL;
   m_pBrushs[2] = NULL;
   m_pBrushs[3] = NULL;

   m_SelBrush = 0;
   m_SelTexture = -1;

   m_bFog = TRUE;

   m_pFont = NULL;

   m_bChangedTextures = FALSE;
   m_bChangedMap = FALSE;

}



/////////////////////////////////////////////////
///
/// NAME: ~CTerraEdView
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     03/06/2004 9:16:59 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CTerraEdView::~CTerraEdView()
{
   Cleanup3DEnvironment();
   if(m_ZoomCursor)
      DestroyCursor(m_ZoomCursor);
   if(m_PanCursor)
      DestroyCursor(m_PanCursor);
   if(m_PanReadyCursor)
      DestroyCursor(m_PanReadyCursor);
   if(m_RotateCursor)
      DestroyCursor(m_RotateCursor);
   if(m_OldCursor)
      DestroyCursor(m_OldCursor);
}



/////////////////////////////////////////////////
///
/// NAME: PreCreateWindow
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets up window Creation
///
/// CREATED:     03/06/2004 9:17:01 AM
///
/// PARAMETERS: 
///             CREATESTRUCT& cs
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdView::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTerraEdView drawing



/////////////////////////////////////////////////
///
/// NAME: OnDraw
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Drawing Code
///
/// CREATED:     03/06/2004 9:17:04 AM
///
/// PARAMETERS: 
///             CDC* pDC
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnDraw(CDC* pDC)
{
   CTerraEdDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   Render3DEnvironment();
   // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTerraEdView diagnostics

#ifdef _DEBUG


/////////////////////////////////////////////////
///
/// NAME: AssertValid
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Debug Function
///
/// CREATED:     03/06/2004 9:17:06 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::AssertValid() const
{
   CView::AssertValid();
}



/////////////////////////////////////////////////
///
/// NAME: Dump
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Debug Function
///
/// CREATED:     03/06/2004 9:17:08 AM
///
/// PARAMETERS: 
///             CDumpContext& dc
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::Dump(CDumpContext& dc) const
{
   CView::Dump(dc);
}



/////////////////////////////////////////////////
///
/// NAME: GetDocument
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Gets the views document
///
/// CREATED:     03/06/2004 9:17:10 AM
///
/// RETURN: CTerraEdDoc* 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CTerraEdDoc* CTerraEdView::GetDocument() // non-debug version is inline
{
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTerraEdDoc)));
   return (CTerraEdDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTerraEdView message handlers




/////////////////////////////////////////////////
///
/// NAME: AddSpriteToTree
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds a sprite to the tree
///
/// CREATED:     03/06/2004 9:17:27 AM
///
/// PARAMETERS: 
///             CTreeCtrl* pTree
///             HTREEITEM hParentItem
///             CLevelDataSprite* pSprite
///
/// RETURN: HTREEITEM 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HTREEITEM CTerraEdView::AddSpriteToTree(CTreeCtrl* pTree,HTREEITEM hParentItem,CLevelDataSprite* pSprite)
{
   HTREEITEM hItem = NULL;
   switch(pSprite->GetSpriteObjType())
   {

   case 0:
      hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,"Sprite - "+pSprite->m_Name,27,27,0,0,(DWORD)pSprite,hParentItem,NULL);
      break;
   case 1:
      hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,"Change Sprite - "+pSprite->m_Name,15,15,0,0,(DWORD)pSprite,hParentItem,NULL);
      break;
   case 2:
      hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,"Create Sprite - "+pSprite->m_Name,16,16,0,0,(DWORD)pSprite,hParentItem,NULL);
      break;
   }

   pSprite->m_hItem = hItem;
   return hItem;
}



/////////////////////////////////////////////////
///
/// NAME: AddObjectToTree
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds an object to the tree
///
/// CREATED:     03/06/2004 9:17:37 AM
///
/// PARAMETERS: 
///             CTreeCtrl* pTree
///             HTREEITEM hParentItem
///             CLevelDataObject* pObjects
///
/// RETURN: HTREEITEM 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HTREEITEM CTerraEdView::AddObjectToTree(CTreeCtrl* pTree,HTREEITEM hParentItem,CLevelDataObject* pObjects)
{
   HTREEITEM hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,"Object - "+ pObjects->m_Name,24,24,0,0,(DWORD)pObjects,hParentItem,NULL);
   pObjects->m_hItem = hItem;
   return hItem;
}



/////////////////////////////////////////////////
///
/// NAME: AddTextureToTree
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds a texture to the tree
///
/// CREATED:     03/06/2004 9:17:47 AM
///
/// PARAMETERS: 
///             CTreeCtrl* pTree
///             HTREEITEM hParentItem
///             CTextureData* pTexture
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::AddTextureToTree(CTreeCtrl* pTree,HTREEITEM hParentItem,CTextureData* pTexture)
{
   HTREEITEM hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,"Texture - " + pTexture->m_File,3,3,0,0,(DWORD)pTexture,hParentItem,NULL);
   pTexture->m_hItem = hItem;
}



/////////////////////////////////////////////////
///
/// NAME: AddEventToTree
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds an event to the tree
///
/// CREATED:     03/06/2004 9:17:57 AM
///
/// PARAMETERS: 
///             CTreeCtrl* pTree
///             HTREEITEM hParentItem
///             CGameEvent* pEvent
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::AddEventToTree(CTreeCtrl* pTree,HTREEITEM hParentItem,CGameEvent* pEvent)
{
   HTREEITEM hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,"Event - " + pEvent->m_Name,20,20,0,0,(DWORD)pEvent,hParentItem,NULL);

   for(UINT i=0;i<pEvent->m_CreateSprites.size();i++)
   {
      CLevelDataSprite* pSprite = pEvent->m_CreateSprites[i];
      AddSpriteToTree(pTree,hItem,pSprite);
   }

   for(i=0;i<pEvent->m_CreateObjects.size();i++)
   {
      CLevelDataObject* pObjects = pEvent->m_CreateObjects[i];
      AddObjectToTree(pTree,hItem,pObjects);
   }

   for(i=0;i<pEvent->m_SpriteChange.size();i++)
   {
      CLevelDataSprite* pSprite = pEvent->m_SpriteChange[i];
      AddSpriteToTree(pTree,hItem,pSprite);
   }
   pEvent->m_hItem = hItem;
}





/////////////////////////////////////////////////
///
/// NAME: AddObjectDataToTree
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds an object data to the tree
///
/// CREATED:     03/06/2004 9:18:18 AM
///
/// PARAMETERS: 
///             CTreeCtrl* pTree
///             CPlanetObjectData* pObj
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::AddObjectDataToTree(CTreeCtrl* pTree,CPlanetObjectData* pObj)
{
   HTREEITEM hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,pObj->m_Name,24,24,0,0,(DWORD)pObj,TVI_ROOT,NULL);
   pTree->SetItemData(hItem,(DWORD)pObj);
}



/////////////////////////////////////////////////
///
/// NAME: AddSpriteDataToTree
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds sprite data to the tree
///
/// CREATED:     03/06/2004 9:18:36 AM
///
/// PARAMETERS: 
///             CTreeCtrl* pTree
///             CSpriteData* pSprite
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::AddSpriteDataToTree(CTreeCtrl* pTree,CSpriteData* pSprite)
{
   HTREEITEM hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,pSprite->m_Name,27,27,0,0,(DWORD)pSprite,TVI_ROOT,NULL);
   pTree->SetItemData(hItem,(DWORD)pSprite);
}



/////////////////////////////////////////////////
///
/// NAME: AddWeaponDataToTree
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds weapon data to the tree
///
/// CREATED:     03/06/2004 9:18:57 AM
///
/// PARAMETERS: 
///             CTreeCtrl* pTree
///             CWeaponData* pObj
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::AddWeaponDataToTree(CTreeCtrl* pTree,CWeaponData* pObj)
{
   HTREEITEM hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,pObj->m_Name,23,23,0,0,(DWORD)pObj,TVI_ROOT,NULL);
   pTree->SetItemData(hItem,(DWORD)pObj);
}



/////////////////////////////////////////////////
///
/// NAME: AddLevelToTree
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds Level to the tree
///
/// CREATED:     03/06/2004 9:19:10 AM
///
/// PARAMETERS: 
///             CTreeCtrl* pTree
///             CLevelData* pLevel
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::AddLevelToTree(CTreeCtrl* pTree,CLevelData* pLevel)
{
   HTREEITEM hItem = pTree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,pLevel->m_Name,8,8,0,0,(DWORD)pLevel,TVI_ROOT,NULL);

   pTree->SetItemData(hItem,(DWORD)pLevel);
   pLevel->m_hItem = hItem;

   for(UINT i=0;i<pLevel->m_Events.size();i++)
   {
      CGameEvent* pEvent = pLevel->m_Events[i];
      AddEventToTree(pTree,hItem,pEvent);
   }

   for(i=0;i<pLevel->m_Textures.size();i++)
   {
      CTextureData* pTexture = pLevel->m_Textures[i];
      AddTextureToTree(pTree,hItem,pTexture);
   }

   for(i=0;i<pLevel->m_Sprites.size();i++)
   {
      CLevelDataSprite* pSprite = pLevel->m_Sprites[i];
      AddSpriteToTree(pTree,hItem,pSprite);
   }

   for(i=0;i<pLevel->m_Objects.size();i++)
   {
      CLevelDataObject* pObjects = pLevel->m_Objects[i];
      AddObjectToTree(pTree,hItem,pObjects);
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnInitialUpdate
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Initializes the window
///
/// CREATED:     03/06/2004 9:19:14 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnInitialUpdate() 
{
   CView::OnInitialUpdate();

   {
      CTreeCtrl* pTree = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette.m_LevelTree;

      pTree->SetImageList(&m_Icons,TVSIL_NORMAL);

      SetCurrentLevel(GetDocument()->m_Levels[0]);

      for(UINT i = 0; i < GetDocument()->m_Levels.size();i++)
      {
         CLevelData* pLevel = GetDocument()->m_Levels[i];
         AddLevelToTree(pTree,pLevel);
      }
   }


   {
      CTreeCtrl* pTree = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette.m_SpriteTree;

      pTree->SetImageList(&m_Icons,TVSIL_NORMAL);

      for(UINT i = 0; i < GetDocument()->m_SpriteData.size();i++)
      {
         CSpriteData* pLevel = GetDocument()->m_SpriteData[i];
         AddSpriteDataToTree(pTree,pLevel);
      }
   }

   {
      CTreeCtrl* pTree = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette.m_ObjectTree;

      pTree->SetImageList(&m_Icons,TVSIL_NORMAL);

      for(UINT i = 0; i < GetDocument()->m_PlanetObjectData.size();i++)
      {
         CPlanetObjectData* pLevel = GetDocument()->m_PlanetObjectData[i];
         AddObjectDataToTree(pTree,pLevel);
      }
   }

   {
      CTreeCtrl* pTree = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette.m_WeaponTree;

      pTree->SetImageList(&m_Icons,TVSIL_NORMAL);

      for(UINT i = 0; i < GetDocument()->m_WeaponData.size();i++)
      {
         CWeaponData* pLevel = GetDocument()->m_WeaponData[i];
         AddWeaponDataToTree(pTree,pLevel);
      }
   }

   CD3DApplication::m_hWnd = GetSafeHwnd();
   CD3DApplication::m_hWndFocus = GetSafeHwnd();
   CD3DApplication::Create( AfxGetInstanceHandle() );

   CDialogBar* pDialogBar = &static_cast<CChildFrame*>(AfxGetApp()->GetMainWnd())->m_wndPaintBar;

   pDialogBar->GetDlgItem(IDC_BRUSH)->EnableWindow(m_ViewMode==VM_PAINT||m_ViewMode==VM_EDIT_TERRAIN);
   pDialogBar->GetDlgItem(IDC_BRUSH_TEXTURE)->EnableWindow(m_ViewMode==VM_PAINT);


   UpdateMap();
}





/////////////////////////////////////////////////
///
/// NAME: OnEdit
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Displays the edit dialog
///
/// CREATED:     03/06/2004 9:19:25 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnEdit() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   pLevelBar->ShowEditDialog();
}



/////////////////////////////////////////////////
///
/// NAME: OnDelete
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Deletes the selected object
///
/// CREATED:     03/06/2004 9:19:38 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnDelete() 
{
   // TODO: Add your command handler code here
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;


   if(pLevelBar->m_Tab.GetCurSel() == LEVEL_TAB)
   {
      HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hItem));

      switch(pObj->GetObjType())
      {
      case DATA_OBJ_TYPE_LEVEL:
         {
            for(std::vector<CLevelData*>::iterator i = GetDocument()->m_Levels.begin();
               i != GetDocument()->m_Levels.end();
               i++)
            {
               if((*i)->GetIndex() == pObj->GetIndex())
               {
                  if(m_pCurrentLevel->GetIndex() == pObj->GetIndex())
                  {
                     GetDocument()->m_Levels.erase(i);
                     SetCurrentLevel(GetDocument()->m_Levels[0]);
                  }
                  else
                     GetDocument()->m_Levels.erase(i);

                  CLevelData* pLevel = static_cast<CLevelData*>(pObj);
                  pLevelBar->m_LevelTree.DeleteItem(pLevel->m_hItem);
                  delete pLevel;
                  break;
               }
            }
         }
         break;
      case DATA_OBJ_TYPE_TEXTURE: 
         {
            HTREEITEM hParentItem = pLevelBar->m_LevelTree.GetParentItem(hItem);
            CLevelData* pParentObj = reinterpret_cast<CLevelData*>(pLevelBar->m_LevelTree.GetItemData(hParentItem));
            for(std::vector<CTextureData*>::iterator i = pParentObj->m_Textures.begin();
               i != pParentObj->m_Textures.end();
               i++)
            {
               if((*i)->GetIndex() == pObj->GetIndex())
               {
                  pParentObj->m_Textures.erase(i);
                  CTextureData* pTexture = static_cast<CTextureData*>(pObj);
                  pLevelBar->m_LevelTree.DeleteItem(pTexture->m_hItem);
                  delete pTexture;
                  break;
               }
            }
         }
         break;
      case DATA_OBJ_TYPE_SPRITE:
         {
            HTREEITEM hParentItem = pLevelBar->m_LevelTree.GetParentItem(hItem);
            CDataObjBase* pParentObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hParentItem));
            std::vector<CLevelDataSprite*>* pObjs;

            static_cast<CLevelDataSprite*>(pObj)->GetSpriteObjType();

            if(static_cast<CLevelDataSprite*>(pObj)->GetSpriteObjType() == 1)
            {
               pObjs = &(static_cast<CGameEvent*>(pParentObj)->m_SpriteChange);
            }
            else if(static_cast<CLevelDataSprite*>(pObj)->GetSpriteObjType() == 2)
            {
               pObjs = &(static_cast<CGameEvent*>(pParentObj)->m_CreateSprites);
            }
            else if(static_cast<CLevelDataSprite*>(pObj)->GetSpriteObjType() == 0)
            {
               pObjs = &(static_cast<CLevelData*>(pParentObj)->m_Sprites);
            }
            else
               break;

            for(std::vector<CLevelDataSprite*>::iterator i = pObjs->begin();
               i != pObjs->end();
               i++)
            {
               if((*i)->GetIndex() == pObj->GetIndex())
               {
                  pObjs->erase(i);
                  CLevelDataSprite* pSprite = static_cast<CLevelDataSprite*>(pObj);
                  pLevelBar->m_LevelTree.DeleteItem(pSprite->m_hItem);
                  delete pSprite;
                  break;
               }
            }
         }
         break;
      case DATA_OBJ_TYPE_OBJECT:
         {
            HTREEITEM hParentItem = pLevelBar->m_LevelTree.GetParentItem(hItem);
            CDataObjBase* pParentObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hParentItem));
            std::vector<CLevelDataObject*>* pObjs;

            if(pParentObj->GetObjType() ==DATA_OBJ_TYPE_EVENT)
            {
               pObjs = &(static_cast<CGameEvent*>(pParentObj)->m_CreateObjects);
            }
            else if(pParentObj->GetObjType() ==DATA_OBJ_TYPE_LEVEL)
            {
               pObjs = &(static_cast<CLevelData*>(pParentObj)->m_Objects);
            }
            else
               break;

            for(std::vector<CLevelDataObject*>::iterator i = pObjs->begin();
               i != pObjs->end();
               i++)
            {
               if((*i)->GetIndex() == pObj->GetIndex())
               {
                  pObjs->erase(i);
                  CLevelDataObject* pObject = static_cast<CLevelDataObject*>(pObj);
                  pLevelBar->m_LevelTree.DeleteItem(pObject->m_hItem);
                  delete pObject;
                  break;
               }
            }

         }
         break;
      case DATA_OBJ_TYPE_EVENT:
         {
            HTREEITEM hParentItem = pLevelBar->m_LevelTree.GetParentItem(hItem);
            CLevelData* pParentObj = reinterpret_cast<CLevelData*>(pLevelBar->m_LevelTree.GetItemData(hParentItem));
            for(std::vector<CGameEvent*>::iterator i = pParentObj->m_Events.begin();
               i != pParentObj->m_Events.end();
               i++)
            {
               if((*i)->GetIndex() == pObj->GetIndex())
               {
                  pParentObj->m_Events.erase(i);
                  CGameEvent* pEvent = static_cast<CGameEvent*>(pObj);
                  pLevelBar->m_LevelTree.DeleteItem(pEvent->m_hItem);
                  delete pEvent;
                  break;
               }
            }
         }
         break;
      }	

      //pLevelBar->m_LevelTree.DeleteAllItems();

      //m_pCurrentLevel = GetDocument()->m_Levels[0];
      //UpdateMap();

      //for(UINT i = 0; i < GetDocument()->m_Levels.size();i++)
      //{
      //	CLevelData* pLevel = GetDocument()->m_Levels[i];
      //	AddLevelToTree(&pLevelBar->m_LevelTree,pLevel);
      //}
   }
   else if(pLevelBar->m_Tab.GetCurSel() == SPRITE_TAB)
   {
      //pLevelBar->m_SpriteTree.DeleteAllItems();

      //for(UINT i = 0; i < GetDocument()->m_SpriteData.size();i++)
      //{
      //	CSpriteData* pLevel = &GetDocument()->m_SpriteData[i];
      //	AddSpriteDataToTree(&pLevelBar->m_SpriteTree,pLevel);
      //}
   }
   else if(pLevelBar->m_Tab.GetCurSel() == OBJECT_TAB)
   {
      //pLevelBar->m_ObjectTree.DeleteAllItems();

      //for(UINT i = 0; i < GetDocument()->m_PlanetObjectData.size();i++)
      //{
      //	CPlanetObjectData* pLevel = &GetDocument()->m_PlanetObjectData[i];
      //	AddObjectDataToTree(&pLevelBar->m_ObjectTree,pLevel);
      //}
   }
   else if(pLevelBar->m_Tab.GetCurSel() == WEAPONS_TAB)
   {
      //pLevelBar->m_WeaponTree.DeleteAllItems();

      //for(UINT i = 0; i < GetDocument()->m_WeaponData.size();i++)
      //{
      //	CPlanetObjectData* pLevel = &GetDocument()->m_PlanetObjectData[i];
      //	AddObjectDataToTree(&pLevelBar->m_ObjectTree,pLevel);
      //}
   }

   UpdateMap();
   Render3DEnvironment();
}




/////////////////////////////////////////////////
///
/// NAME: OnEventAddCreatesprite
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Creates a new sprite
///
/// CREATED:     03/06/2004 9:20:05 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnEventAddCreatesprite() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;

   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
      return;

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   CGameEvent* pObj = reinterpret_cast<CGameEvent*>(pLevelBar->m_LevelTree.GetItemData(hItem));

   CLevelDataSprite* SpriteData = new CLevelDataSprite;

   CLevelData* pParentObj = NULL;
   HTREEITEM hParentItem = hItem;

   while(pParentObj == NULL && hParentItem != NULL)
   {
      hParentItem = pLevelBar->m_LevelTree.GetParentItem(hParentItem);
      CDataObjBase* pTempObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hParentItem));
      if(pTempObj->GetObjType() == DATA_OBJ_TYPE_LEVEL)
      {
         pParentObj = static_cast<CLevelData*>(pTempObj);
         break;
      }
   }

   CEditSprite dlg(SpriteData,&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_SpriteData,pParentObj);
   if(dlg.DoModal() == IDOK)
   {
      int Index = pObj->m_CreateSprites.size();
      if(Index != 0)
         Index = pObj->m_CreateSprites[Index-1]->GetIndex() + 1;

      SpriteData->SetIndex(Index);
      SpriteData->SetSpriteObjType(2);
      pObj->m_CreateSprites.push_back(SpriteData);

      AddSpriteToTree(&pLevelBar->m_LevelTree,hItem,pObj->m_CreateSprites[pObj->m_CreateSprites.size()-1]);
   }
   else
   {
      delete SpriteData;
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnLevelAddEvent
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Creates a new event
///
/// CREATED:     03/06/2004 9:20:23 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnLevelAddEvent() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
      return;

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   CLevelData* pObj = reinterpret_cast<CLevelData*>(pLevelBar->m_LevelTree.GetItemData(hItem));

   CGameEvent* ObjectData = new CGameEvent;	

   CEditEvent dlg(ObjectData,pObj);
   if(dlg.DoModal() == IDOK)
   {
      int Index = pObj->m_Events.size();
      if(Index != 0)
         Index = pObj->m_Events[Index-1]->GetIndex() + 1;

      ObjectData->SetIndex(Index);
      pObj->m_Events.push_back(ObjectData);

      AddEventToTree(&pLevelBar->m_LevelTree,hItem,pObj->m_Events[pObj->m_Events.size()-1]);
   }
   else
   {
      delete ObjectData;
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnLevelAddObject
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Creates a new object
///
/// CREATED:     03/06/2004 9:20:34 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnLevelAddObject() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
      return;

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   CLevelData* pObj = reinterpret_cast<CLevelData*>(pLevelBar->m_LevelTree.GetItemData(hItem));

   CLevelDataObject* ObjectData = new CLevelDataObject;

   CEditObject dlg(ObjectData,&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_PlanetObjectData);
   if(dlg.DoModal() == IDOK)
   {
      int Index = pObj->m_Objects.size();
      if(Index != 0)
         Index = pObj->m_Objects[Index-1]->GetIndex() + 1;

      ObjectData->SetIndex(Index);

      pObj->m_Objects.push_back(ObjectData);

      AddObjectToTree(&pLevelBar->m_LevelTree,hItem,pObj->m_Objects[pObj->m_Objects.size()-1]);
   }
   else
   {
      delete ObjectData;
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnLevelAddSprite
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Creates a new sprtie
///
/// CREATED:     03/06/2004 9:20:44 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnLevelAddSprite() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
      return;

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   CLevelData* pObj = reinterpret_cast<CLevelData*>(pLevelBar->m_LevelTree.GetItemData(hItem));

   CLevelDataSprite* SpriteData = new CLevelDataSprite;

   CEditSprite dlg(SpriteData,&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_SpriteData,pObj);
   if(dlg.DoModal() == IDOK)
   {
      int Index = pObj->m_Sprites.size();
      if(Index != 0)
         Index = pObj->m_Sprites[Index-1]->GetIndex() + 1;

      SpriteData->SetIndex(Index);
      SpriteData->SetSpriteObjType(2);
      pObj->m_Sprites.push_back(SpriteData);

      AddSpriteToTree(&pLevelBar->m_LevelTree,hItem,pObj->m_Sprites[pObj->m_Sprites.size()-1]);
   }
   else
   {
      delete SpriteData;
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnLevelAddTexture
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Creates a new Textrue
///
/// CREATED:     03/06/2004 9:20:55 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnLevelAddTexture() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
      return;

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   CLevelData* pObj = reinterpret_cast<CLevelData*>(pLevelBar->m_LevelTree.GetItemData(hItem));

   CTextureData* ObjectData = new CTextureData;

   CEditTexture dlg(this,ObjectData);
   if(dlg.DoModal() == IDOK)
   {
      int Index = pObj->m_Textures.size();
      if(Index != 0)
         Index = pObj->m_Textures[Index-1]->GetIndex() + 1;

      ObjectData->SetIndex(Index);
      pObj->m_Textures.push_back(ObjectData);

      AddTextureToTree(&pLevelBar->m_LevelTree,hItem,pObj->m_Textures[pObj->m_Textures.size()-1]);
      UpdateMap();
   }
   else
   {
      delete ObjectData;
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateEvent
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:21:18 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateEvent(CCmdUI* pCmdUI) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
   {
      pCmdUI->Enable(FALSE);
      return;
   }

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();
   BOOL bEnable = FALSE;

   if(hItem)
   {

      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hItem));
      if(pObj)
      {
         bEnable = pObj->GetObjType()==DATA_OBJ_TYPE_EVENT;
      }
   }


   pCmdUI->Enable(bEnable);

}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateLevel
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:21:22 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateLevel(CCmdUI* pCmdUI) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
   {
      pCmdUI->Enable(FALSE);
      return;
   }

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   BOOL bEnable = FALSE;

   if(hItem)
   {

      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hItem));
      if(pObj)
      {
         bEnable = pObj->GetObjType()==DATA_OBJ_TYPE_LEVEL;
      }
   }


   pCmdUI->Enable(bEnable);
}



/////////////////////////////////////////////////
///
/// NAME: OnToolsNewLevel
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Creates a new level
///
/// CREATED:     03/06/2004 9:22:05 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnToolsNewLevel() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   CLevelData* ObjectData = new CLevelData;

   CEditLevel dlg(ObjectData);
   if(dlg.DoModal() == IDOK)
   {
      int Index = GetDocument()->m_Levels.size();
      if(Index != 0)
         Index = GetDocument()->m_Levels[Index-1]->GetIndex() + 1;

      ObjectData->SetIndex(Index);
      GetDocument()->m_Levels.push_back(ObjectData);

      AddLevelToTree(&pLevelBar->m_LevelTree,GetDocument()->m_Levels[GetDocument()->m_Levels.size()-1]);
   }
   else
      delete ObjectData;

}



/////////////////////////////////////////////////
///
/// NAME: OnLevelActive
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Actvates the selected level
///
/// CREATED:     03/06/2004 9:22:20 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnLevelActive() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
   {
      return;
   }

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   if(hItem)
   {

      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hItem));
      if(pObj && pObj->GetObjType()==DATA_OBJ_TYPE_LEVEL)
      {
         if(m_pCurrentLevel == NULL || m_pCurrentLevel->GetIndex() != pObj->GetIndex())
         {
            SetCurrentLevel(static_cast<CLevelData*>(pObj));
            // update the view display
            UpdateMap();
         }
      }
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateLevelActive
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:22:26 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateLevelActive(CCmdUI* pCmdUI) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
   {
      pCmdUI->Enable(FALSE);
      pCmdUI->SetCheck(FALSE);
      return;
   }

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   BOOL bEnable = FALSE;
   BOOL bCheck = FALSE;

   if(hItem)
   {

      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hItem));
      if(pObj && m_pCurrentLevel)
      {
         bEnable = pObj->GetObjType()==DATA_OBJ_TYPE_LEVEL;
         bCheck = m_pCurrentLevel->GetIndex() == pObj->GetIndex();
      }
   }

   pCmdUI->Enable(bEnable);
   pCmdUI->SetCheck(bEnable?bCheck:FALSE);


}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateToolsNewLevel
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:22:31 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateToolsNewLevel(CCmdUI* pCmdUI) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   pCmdUI->Enable(pLevelBar->m_Tab.GetCurSel() == LEVEL_TAB);
}



/////////////////////////////////////////////////
///
/// NAME: OnToolsNewObjectdata
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Creates an new object data
///
/// CREATED:     03/06/2004 9:22:51 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnToolsNewObjectdata() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   CPlanetObjectData Object;

   CEditObjectData dlg(&Object);
   if(dlg.DoModal() == IDOK)
   {
      CPlanetObjectDataEx* pObjectData;
      switch(Object.m_Type)
      {
      case OBJECTTYPE_TREE:
         pObjectData = new CPlanetObjectTree();
         break;
      case OBJECTTYPE_BUSH:
         pObjectData = new CPlanetObjectBush();
         break;
      case OBJECTTYPE_MESH:
      case OBJECTTYPE_POWERUP:
      case OBJECTTYPE_WAYPOINT:
         pObjectData = new CPlanetObjectMesh();
         break;
      }

      pObjectData->Copy(Object);
      pObjectData->RestoreDeviceObjects(m_pd3dDevice,&GetDocument()->m_Zip);
      pObjectData->m_Index = GetDocument()->m_PlanetObjectData.size();
      GetDocument()->m_PlanetObjectData.push_back(pObjectData);
      AddObjectDataToTree(&pLevelBar->m_ObjectTree,GetDocument()->m_PlanetObjectData[GetDocument()->m_PlanetObjectData.size()-1]);
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateToolsNewObjectdata
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:22:56 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateToolsNewObjectdata(CCmdUI* pCmdUI) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   pCmdUI->Enable(pLevelBar->m_Tab.GetCurSel() == OBJECT_TAB);

}



/////////////////////////////////////////////////
///
/// NAME: OnToolsNewSpritedata
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Creates a new sprite data
///
/// CREATED:     03/06/2004 9:23:12 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnToolsNewSpritedata() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   CSpriteDataEx* ObjectData = new CSpriteDataEx;

   CEditSpriteData dlg(ObjectData,&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_WeaponData);
   if(dlg.DoModal() == IDOK)
   {
      ObjectData->m_Index = GetDocument()->m_SpriteData.size();
      ObjectData->RestoreDeviceObjects(m_pd3dDevice,&GetDocument()->m_Zip);
      GetDocument()->m_SpriteData.push_back(ObjectData);

      ObjectData->RestoreDeviceObjects(m_pd3dDevice,&GetDocument()->m_Zip);

      AddSpriteDataToTree(&pLevelBar->m_SpriteTree,GetDocument()->m_SpriteData[GetDocument()->m_SpriteData.size()-1]);
   }
   else
      delete ObjectData;

}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateToolsNewSpritedata
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:23:02 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateToolsNewSpritedata(CCmdUI* pCmdUI) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   pCmdUI->Enable(pLevelBar->m_Tab.GetCurSel() == SPRITE_TAB);

}



/////////////////////////////////////////////////
///
/// NAME: OnLevelAddObjectAt
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds an object at a givin point
///
/// CREATED:     03/06/2004 9:23:33 AM
///
/// PARAMETERS: 
///             CPoint pt
///             CPlanetObjectData* pDataObj
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnLevelAddObjectAt(CPoint pt, CPlanetObjectData* pDataObj) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;

   CLevelData* pObj = m_pCurrentLevel;

   CLevelDataObject* ObjectData = new CLevelDataObject;

   ObjectData->m_Type = pDataObj->m_Index;
   //Calulate new pos
   ObjectData->m_pos = PickPoint(pt);

   CEditObject dlg(ObjectData,&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_PlanetObjectData);
   if(dlg.DoModal() == IDOK)
   {
      int Index = pObj->m_Objects.size();
      if(Index != 0)
         Index = pObj->m_Objects[Index-1]->GetIndex() + 1;

      ObjectData->SetIndex(Index);

      pObj->m_Objects.push_back(ObjectData);



      AddObjectToTree(&pLevelBar->m_LevelTree,m_pCurrentLevel->m_hItem,pObj->m_Objects[pObj->m_Objects.size()-1]);
   }
   else
   {
      delete ObjectData;
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnLevelAddSpriteAt
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds a sprite at a given point
///
/// CREATED:     03/06/2004 9:23:53 AM
///
/// PARAMETERS: 
///             CPoint pt
///             CSpriteData* pDataObj
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnLevelAddSpriteAt(CPoint pt, CSpriteData* pDataObj) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;

   CLevelData* pObj = m_pCurrentLevel;

   CLevelDataSprite* SpriteData = new CLevelDataSprite();

   SpriteData->m_Type = pDataObj->m_Index;
   SpriteData->m_Name = pDataObj->m_Name;
   SpriteData->m_pos = PickPoint(pt);

   CEditSprite dlg(SpriteData,&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_SpriteData,pObj);
   if(dlg.DoModal() == IDOK)
   {
      int Index = pObj->m_Sprites.size();
      if(Index != 0)
         Index = pObj->m_Sprites[Index-1]->GetIndex() + 1;

      SpriteData->SetIndex(Index);
      SpriteData->SetSpriteObjType(0);
      pObj->m_Sprites.push_back(SpriteData);

      AddSpriteToTree(&pLevelBar->m_LevelTree,m_pCurrentLevel->m_hItem,pObj->m_Sprites[pObj->m_Sprites.size()-1]);
   }
   else
   {
      delete SpriteData;
   }

}



/////////////////////////////////////////////////
///
/// NAME: OneTimeSceneInit
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: One Time Scene creation
///
/// CREATED:     03/06/2004 9:24:22 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::OneTimeSceneInit()
{
   m_pFont = new CD3DFont( _T("Arial"), 9, D3DFONT_BOLD );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: FrameMove
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: the frame has moved
///
/// CREATED:     03/06/2004 9:24:32 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::FrameMove()
{
   D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f,  0.0f );

   D3DXMATRIX matRot;

   D3DXMATRIX matRotateX;
   D3DXMatrixRotationX( &matRotateX, m_Rot.x);
   D3DXMATRIX matRotateY;
   D3DXMatrixRotationY( &matRotateY, m_Rot.y);

   D3DXMATRIX matView;
   D3DXMatrixMultiply( &matView, &matRotateX,&matRotateY);


   if(m_ViewMode== VM_ROTATE_OBJ)
   {
      m_EyePt    = D3DXVECTOR3( 0.0f, 0.0f, m_Zoom );
      m_LookatPt = D3DXVECTOR3( 0.0f, 0.0f,  0.0f );
      m_LookatPt = m_SelObjPos;

      D3DXMATRIX matTrans;
      D3DXMatrixTranslation(&matTrans,m_LookatPt.x,m_LookatPt.y,m_LookatPt.z);
      D3DXMatrixMultiply( &matView, &matView,&matTrans);
      D3DXVec3TransformCoord(&m_EyePt,&m_EyePt,&matView);

   }
   else
   {
      m_EyePt    = D3DXVECTOR3( 0.0f, 0.0f,  0.0f );
      m_LookatPt = D3DXVECTOR3( 0.0f, 0.0f,  4.0f );

      m_EyePt = m_CamPos;

      D3DXMATRIX matTrans;
      D3DXMatrixTranslation(&matTrans,m_CamPos.x,m_CamPos.y,m_CamPos.z);
      D3DXMatrixMultiply( &matView, &matView,&matTrans);
      D3DXVec3TransformCoord(&m_LookatPt,&m_LookatPt,&matView);
   }


   D3DXMatrixLookAtLH( &m_matView, &m_EyePt, &m_LookatPt, &vUpVec );

   m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_matView );
   m_Clipper.SetViewMatix(&m_matView);


   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: Render
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Draw the current scene
///
/// CREATED:     03/06/2004 9:24:55 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::Render()
{
   //HRESULT hr;

   if(m_pCurrentLevel == NULL)
   {
      m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
         0x000000ff, 1.0f, 0L );
   }
   else
   {
      m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
         m_pCurrentLevel->m_BGColour, 1.0f, 0L );
   }

   // Begin the scene
   if( FAILED( m_pd3dDevice->BeginScene() ) )
      return E_FAIL;

   if(m_pCurrentLevel)
   {
      m_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR, m_pCurrentLevel->m_BGColour);
      DWORD dwTR = 0;
      m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ); // we do not need lights for the terain
      if(m_pTerrain)
      {
         if(m_pCurrentLevel->m_bShowSkyBox)
         {
            m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
            m_pSkyBox->Render(m_pd3dDevice,m_matView,dwTR);
            m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
         }
         m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, m_bFog );
         m_pd3dDevice->SetRenderState( D3DRS_FOGSTART,       FtoDW(m_pCurrentLevel->m_fFogStart) );
         m_pd3dDevice->SetRenderState( D3DRS_FOGEND,         FtoDW(m_pCurrentLevel->m_fFogEnd) );

         if(m_ViewMode ==VM_PAINT)
         {
            // must render the painted selection
            m_pTerrain->Render(m_pd3dDevice,&m_Clipper,dwTR,m_EyePt,TRUE,TRUE);
         }
         else
         {
            m_pTerrain->Render(m_pd3dDevice,&m_Clipper,dwTR,m_EyePt,TRUE,TRUE);
         }

         if(m_pWater)
            m_pWater->Render(m_pd3dDevice,&m_Clipper,dwTR,TRUE);
      }

      // draw start pos
      {
         D3DXMATRIX matWorld;
         //D3DXMatrixIdentity( &matWorld );
         D3DXMatrixTranslation(&matWorld,m_pCurrentLevel->m_StartPos.x,m_pCurrentLevel->m_StartPos.y,m_pCurrentLevel->m_StartPos.z);
         m_pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld );

         D3DMATERIAL9 mtrl;
         D3DUtil::InitMaterial( mtrl, 255.0f, 0.0f, 0.0f );

         m_pd3dDevice->SetMaterial( &mtrl );
         m_pd3dDevice->SetTexture( 0, NULL );
         m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
         m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

         m_pBoxMesh->DrawSubset(0);

         m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, m_bFog );
      }

      m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
      m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
      m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

      for(UINT i=0;i<m_pCurrentLevel->m_Sprites.size();i++)
      {
         CLevelDataSprite* pSprite = m_pCurrentLevel->m_Sprites[i];
         CSpriteDataEx* pSpriteMesh = GetDocument()->m_SpriteData[pSprite->m_Type];
         pSpriteMesh->Render(m_pd3dDevice,pSprite->m_pos,&m_Clipper,m_pTerrain,m_fElapsedTime,pSprite->m_CurrentTexture);
      }

      for(i=0;i<m_pCurrentLevel->m_Objects.size();i++)
      {
         CLevelDataObject* pObjects = m_pCurrentLevel->m_Objects[i];
         CPlanetObjectDataEx* pObjectMesh = GetDocument()->m_PlanetObjectData[pObjects->m_Type];
         pObjectMesh->Render(m_pd3dDevice,pObjects->m_pos,&m_Clipper,m_pTerrain);
      }

      for(i=0;i<m_pCurrentLevel->m_Events.size();i++)
      {
         CGameEvent* pEvent = m_pCurrentLevel->m_Events[i];
         if(pEvent->m_bDisplay)
         {
            for(UINT j=0;j<pEvent->m_CreateSprites.size();j++)
            {
               CLevelDataSprite* pSprite = pEvent->m_CreateSprites[j];
               CSpriteDataEx* pSpriteMesh = GetDocument()->m_SpriteData[pSprite->m_Type];
               pSpriteMesh->Render(m_pd3dDevice,pSprite->m_pos,&m_Clipper,m_pTerrain,m_fElapsedTime,pSprite->m_CurrentTexture);
            }

            for(j=0;j<pEvent->m_CreateObjects.size();j++)
            {
               CLevelDataObject* pObjects = pEvent->m_CreateObjects[j];
               CPlanetObjectDataEx* pObjectMesh = GetDocument()->m_PlanetObjectData[pObjects->m_Type];
               pObjectMesh->Render(m_pd3dDevice,pObjects->m_pos,&m_Clipper,m_pTerrain);
            }
         }
      }

      if(m_ViewMode == VM_SELECT_OBJ)
      {
         CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
         if(pLevelBar->m_Tab.GetCurSel() == SPRITE_TAB)
         {
            HTREEITEM hItem = pLevelBar->m_SpriteTree.GetSelectedItem();
            if(hItem)
            {
               CSpriteDataEx* pObj = reinterpret_cast<CSpriteDataEx*>(pLevelBar->m_SpriteTree.GetItemData(hItem));
               pObj->Render(m_pd3dDevice,m_PickPos,&m_Clipper,m_pTerrain,m_fElapsedTime,0);
            }

         }
         else if(pLevelBar->m_Tab.GetCurSel() == OBJECT_TAB)
         {
            HTREEITEM hItem = pLevelBar->m_ObjectTree.GetSelectedItem();
            if(hItem)
            {
               CPlanetObjectDataEx* pObj = reinterpret_cast<CPlanetObjectDataEx*>(pLevelBar->m_ObjectTree.GetItemData(hItem));
               pObj->Render(m_pd3dDevice,m_PickPos,&m_Clipper,m_pTerrain);
            }
         }
      }

      if(m_pSelObject)
      {
         D3DXMATRIX matWorld;
         D3DXMatrixTranslation(&matWorld,m_SelObjPos.x,m_SelObjPos.y,m_SelObjPos.z);

         D3DXMATRIX matScale;
         D3DXMatrixScaling( &matScale, m_fSelObjRad * 1.25f, m_fSelObjRad* 1.25f, m_fSelObjRad* 1.25f);
         D3DXMatrixMultiply( &matWorld, &matScale, &matWorld );

         m_pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld );

         D3DMATERIAL9 mtrl;
         D3DUtil::InitMaterial( mtrl, 255.0f, 255.0f, 0.0f );

         m_pd3dDevice->SetMaterial( &mtrl );
         m_pd3dDevice->SetTexture( 0, NULL );
         m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
         m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
         m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
         m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);

         m_pBoxMesh->DrawSubset(0);

         m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
         m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
         m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, m_bFog );
      }

   }

   //m_pFont->DrawText(m_pd3dDevice, 2,  0, D3DCOLOR_ARGB(255,255,255,0), "Test" );

   // End the scene.
   m_pd3dDevice->EndScene();

   return S_OK;
}

/////////////////////////////////////////////////
///
/// NAME: InitDeviceObjects
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Initialize scene objects.
///
/// CREATED:     03/06/2004 9:25:17 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::InitDeviceObjects()
{
   // Initialize the font's internal textures
   //m_pFont->InitDeviceObjects( m_pd3dDevice );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: GetSunPos
///
/// DESCRIPTION: Gets the suns vector
///
/// CREATED:     03/06/2004 9:25:36 AM
///
/// PARAMETERS: 
///             D3DXVECTOR3* orgin
///             D3DXVECTOR3* SunPos
///             FLOAT distance
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void GetSunPos(D3DXVECTOR3* orgin,D3DXVECTOR3* SunPos, FLOAT distance)
{
   //time_t ltime;
   //tm *today;
   //time( &ltime );
   //today = localtime( &ltime );

   //FLOAT Sec = today->tm_sec + (today->tm_min*SECINMIN) + (today->tm_hour*SECINHOUR);
   FLOAT ZRot = HALFPI - HALFPI/6.0f;//(Sec/SECINDAY * PI2) + PI;

   D3DXMATRIX matRotZ, matSun;
   D3DXMatrixTranslation( &matSun,0, distance, 0 );
   D3DXMatrixRotationZ( &matRotZ, ZRot );
   D3DXMatrixMultiply( &matSun, &matSun, &matRotZ );
   D3DXVECTOR3 result;
   D3DXVec3TransformCoord( SunPos,orgin,&matSun);
}

/////////////////////////////////////////////////
///
/// NAME: RestoreDeviceObjects
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Initialize scene objects.
///
/// CREATED:     03/06/2004 9:26:52 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::RestoreDeviceObjects()
{
   ::CWaitCursor wait;

   D3DUtil::CreateTexture(m_pd3dDevice,(char*)("Brush1.jpg"),m_pBrushs[0],&GetDocument()->m_Zip);
   D3DUtil::CreateTexture(m_pd3dDevice,(char*)("Brush2.jpg"),m_pBrushs[1],&GetDocument()->m_Zip);
   D3DUtil::CreateTexture(m_pd3dDevice,(char*)("SmallBrush.jpg"),m_pBrushs[2],&GetDocument()->m_Zip);
   D3DUtil::CreateTexture(m_pd3dDevice,(char*)("SmallBrush2.jpg"),m_pBrushs[3],&GetDocument()->m_Zip);

   CDialogBar* pDialogBar = &static_cast<CChildFrame*>(AfxGetApp()->GetMainWnd())->m_wndPaintBar;
   CComboBox* pBrushBox = (CComboBox*)pDialogBar->GetDlgItem(IDC_BRUSH);
   pBrushBox->ResetContent();

   pBrushBox->AddString("Dark");
   pBrushBox->AddString("Light");
   pBrushBox->AddString("Small Dark");
   pBrushBox->AddString("Small Light");

   pBrushBox->SetCurSel(0);
   m_SelBrush = 0;



   m_pFont->RestoreDeviceObjects(m_pd3dDevice);

   D3DXCreateBox(m_pd3dDevice,1,1,1,&m_pBoxMesh,NULL);
   D3DXComputeNormals( m_pBoxMesh, NULL );

   // Create and set up the shine materials w/ textures
   D3DMATERIAL9 mtrl;
   D3DUtil::InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
   m_pd3dDevice->SetMaterial( &mtrl );

   // Set the transform matrices
   D3DXMATRIX matWorld, matView;
   D3DXVECTOR3 vEyePt    = D3DXVECTOR3( 0.0f, 0.0f, -4.0f );
   D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f,  0.0f );
   D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f,  0.0f );

   FLOAT fAspect = m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;
   D3DXMatrixIdentity( &matWorld );
   D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
   D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4.0f, fAspect, 1.0f, 1000.0f );

   m_Clipper.SetupFOVClipPlanes(D3DX_PI*0.4f/* 11.0f/24.0f*/, fAspect, 1.0f, 1000.0f);

   m_pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld );
   m_pd3dDevice->SetTransform( D3DTS_VIEW,       &matView );
   m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );

   // Set any appropiate state
   m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,  TRUE );
   m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

   if(m_pTerrain)
   {
      m_pTerrain->Create(m_pd3dDevice,&GetDocument()->m_Zip);
   }


   if(m_pWater)
   {
      m_pWater->Create(m_pd3dDevice,
         (FLOAT)m_pTerrain->GetXSize(),
         (FLOAT)m_pTerrain->GetZSize(),
         m_pWater->GetWaterLevel(),
         m_pTerrain,
         &GetDocument()->m_Zip);
   }


   if(m_pSkyBox)
   {
      if( FAILED( m_pSkyBox->Create( m_pd3dDevice,20,WORLD_RAD,WORLD_RAD+WORLD_ATM_THICK/*800.0f,850.0f*/,1.0f,1.0f, &GetDocument()->m_Zip) ) )
         return D3DAPPERR_MEDIANOTFOUND;
   }

   {
      for(UINT i = 0; i < GetDocument()->m_SpriteData.size();i++)
      {
         CSpriteDataEx* pLevel = GetDocument()->m_SpriteData[i];
         pLevel->RestoreDeviceObjects(m_pd3dDevice,&GetDocument()->m_Zip);
      }
   }

   {

      for(UINT i = 0; i < GetDocument()->m_PlanetObjectData.size();i++)
      {
         CPlanetObjectDataEx* pLevel = GetDocument()->m_PlanetObjectData[i];
         pLevel->RestoreDeviceObjects(m_pd3dDevice,&GetDocument()->m_Zip);
      }
   }



   //m_SunPos = D3DXVECTOR3(0.0f,-2000.0f,0.0f);
   D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);
   //GetSunPos(&orgin,&m_SunPos,2000.0f);
   //lightpos.y = -lightpos.y;

   //if(m_SunPos.y < 0)
   //	m_SunPos.y = -m_SunPos.y;
   //else


   D3DXVECTOR3 LightVector = orgin - m_pCurrentLevel->m_SunPos;

   D3DLIGHT9 light;
   D3DUtil::InitLight( light, D3DLIGHT_DIRECTIONAL , LightVector.x, LightVector.y, LightVector.z );
   light.Attenuation0 = 0.9f;
   light.Attenuation1 = 0.0f;
   m_pd3dDevice->SetLight( 0, &light );
   m_pd3dDevice->LightEnable( 0, TRUE );
   m_pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0x00303030);

   m_pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_LINEAR );
   m_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
   m_pd3dDevice->SetRenderState( D3DRS_RANGEFOGENABLE, FALSE );

   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: Resize3DEnvironment
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Resizes the window
///
/// CREATED:     03/06/2004 9:28:15 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::Resize3DEnvironment()
{
   /*D3DVIEWPORT8 vp;
   vp.X=0;
   vp.Y=0;
   vp.Width=m_d3dpp.BackBufferWidth;
   vp.Height=m_d3dpp.BackBufferHeight;
   vp.MinZ = 0.0;
   vp.MaxZ = 1.0;
   m_pd3dDevice->SetViewport(&vp);*/

   return CD3DApplication::Resize3DEnvironment();
}

/////////////////////////////////////////////////
///
/// NAME: InvalidateDeviceObjects
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Called when the device-dependent objects are about to be lost.
///
/// CREATED:     03/06/2004 9:29:56 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::InvalidateDeviceObjects()
{
   m_pFont->InvalidateDeviceObjects();
   return S_OK;
}


/////////////////////////////////////////////////
///
/// NAME: DeleteDeviceObjects
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Called when the app is exiting, or the device is being changed,
///       this function deletes any device dependent objects.
///
/// CREATED:     03/06/2004 9:30:03 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::DeleteDeviceObjects()
{
   SAFE_RELEASE(m_pBoxMesh);
   m_pFont->DeleteDeviceObjects();
   D3DUtil::ReleaseTexture( m_pBrushs[0] );
   D3DUtil::ReleaseTexture( m_pBrushs[1] );
   D3DUtil::ReleaseTexture( m_pBrushs[2] );
   D3DUtil::ReleaseTexture( m_pBrushs[3] );
   if(m_pTerrain)
      m_pTerrain->DeleteDeviceObjects();
   if(m_pWater)
      m_pWater->DeleteDeviceObjects();
   if(m_pSkyBox)
      m_pSkyBox->DeleteDeviceObjects();

   {
      for(UINT i = 0; i < GetDocument()->m_SpriteData.size();i++)
      {
         CSpriteDataEx* pLevel = GetDocument()->m_SpriteData[i];
         pLevel->DeleteDeviceObjects();
      }
   }

   {

      for(UINT i = 0; i < GetDocument()->m_PlanetObjectData.size();i++)
      {
         CPlanetObjectDataEx* pLevel = GetDocument()->m_PlanetObjectData[i];
         pLevel->DeleteDeviceObjects();
      }
   }
   return S_OK;
}


/////////////////////////////////////////////////
///
/// NAME: FinalCleanup
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Called before the app exits, this function gives the app the chance
///				 to cleanup after itself.
///
/// CREATED:     03/06/2004 9:30:23 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::FinalCleanup()
{
   SAFE_DELETE( m_pFont );
   SAFE_DELETE( m_pSkyBox );

   SAFE_DELETE(m_pTerrain);
   SAFE_DELETE(m_pWater);
   return S_OK;
}

/////////////////////////////////////////////////
///
/// NAME: ConfirmDevice
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Called during device intialization, this code checks the device
///       for some minimum set of capabilities
///
/// CREATED:     03/06/2004 9:30:57 AM
///
/// PARAMETERS: 
///              D3DCAPS9* pCaps
///             DWORD dwBehavior
///             D3DFORMAT Format
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerraEdView::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format )
{
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: OnSize
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Resizes the window
///
/// CREATED:     03/06/2004 9:31:16 AM
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
void CTerraEdView::OnSize(UINT nType, int cx, int cy) 
{
   CView::OnSize(nType, cx, cy);

   if( m_bActive && m_bWindowed )
   {
      RECT rcClientOld;
      rcClientOld = m_rcWindowClient;

      // Update window properties
      GetWindowRect(&m_rcWindowBounds );
      GetClientRect(&m_rcWindowClient );

      if( rcClientOld.right - rcClientOld.left !=
         m_rcWindowClient.right - m_rcWindowClient.left ||
         rcClientOld.bottom - rcClientOld.top !=
         m_rcWindowClient.bottom - m_rcWindowClient.top)
      {
         // A new window size will require a new backbuffer
         // size, so the 3D structures must be changed accordingly.
         m_bReady = FALSE;
         int oldWidth = m_d3dpp.BackBufferWidth;
         int oldHeight = m_d3dpp.BackBufferHeight;

         m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
         m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

         if( m_d3dpp.BackBufferWidth > 1 &&  m_d3dpp.BackBufferHeight > 1)
         {
            // Resize the 3D environment
            if( FAILED( Resize3DEnvironment() ) )
            {
               DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
            }
         }
         else
         {
            m_d3dpp.BackBufferWidth = oldWidth;
            m_d3dpp.BackBufferHeight = oldHeight;
         }

         m_bReady = TRUE;
      }
   }

}



/////////////////////////////////////////////////
///
/// NAME: UpdateMap
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates all the objects and refreshes the level
///
/// CREATED:     03/06/2004 9:33:58 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::UpdateMap()
{
   CWaitCursor cursor;
   if(m_pCurrentLevel)
   {
      if(m_pTerrain)
      {
         m_pTerrain->DeleteDeviceObjects();
         SAFE_DELETE(m_pTerrain);
      }

      if(m_pWater)
      {
         m_pWater->DeleteDeviceObjects();
         SAFE_DELETE(m_pWater);
      }

      if(m_pSkyBox)
      {
         m_pSkyBox->DeleteDeviceObjects();
         SAFE_DELETE(m_pSkyBox);
      }

      m_pTerrain = new CTerrain();
      m_pTerrain->SetupMap(m_pd3dDevice,m_pCurrentLevel->m_MapFile,
         m_pCurrentLevel->m_MapHight,
         m_pCurrentLevel->m_Textures,
         m_pCurrentLevel->m_MapScale,
         &GetDocument()->m_Zip);

      m_pTerrain->SetWaterLevel(m_pCurrentLevel->m_WaterLevel);

      m_pTerrain->Create(m_pd3dDevice,&GetDocument()->m_Zip);

      m_pWater = new CWater();

      m_pWater->Init(m_pCurrentLevel->m_bWater,m_pCurrentLevel->m_GroundTexture,m_pCurrentLevel->m_WaterTexture,m_pCurrentLevel->m_WaterTexture2);
      m_pWater->Create(m_pd3dDevice,(FLOAT)m_pTerrain->GetXSize(),(FLOAT)m_pTerrain->GetZSize(),m_pCurrentLevel->m_WaterLevel,m_pTerrain,&GetDocument()->m_Zip);
      m_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR, m_pCurrentLevel->m_BGColour);

      if(m_pCurrentLevel->m_bShowSkyBox)
      {
         m_pSkyBox      = new CSkyBox();
         m_pSkyBox->SetTex(m_pCurrentLevel->m_SkyBoxFile);

         m_pSkyBox->Create( m_pd3dDevice,20,WORLD_RAD,WORLD_RAD+WORLD_ATM_THICK/*800.0f,850.0f*/,1.0f,1.0f,&GetDocument()->m_Zip);
      }

      Invalidate(); // force redraw

      CDialogBar* pDialogBar = &static_cast<CChildFrame*>(AfxGetApp()->GetMainWnd())->m_wndPaintBar;
      CComboBox* pBrushBox = (CComboBox*)pDialogBar->GetDlgItem(IDC_BRUSH_TEXTURE);
      pBrushBox->ResetContent();

      for(UINT i=1;i<m_pTerrain->m_Textrues.size();i++)
      {
         int index = pBrushBox->AddString(m_pTerrain->m_Textrues[i].m_TexFile);
         pBrushBox->SetItemData(index,i);
      }

      if(m_pTerrain->m_Textrues.size() > 1)
      {
         pBrushBox->SetCurSel(0);
         m_SelTexture = pBrushBox->GetItemData(0);
      }
      else
      {
         m_SelTexture = -1;
      }
   }
}



/////////////////////////////////////////////////
///
/// NAME: PickPoint
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Takes a Screen point and fineds the 3D point on the terrain
///
/// CREATED:     03/06/2004 9:34:49 AM
///
/// PARAMETERS: 
///             CPoint point
///
/// RETURN: D3DXVECTOR3 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
D3DXVECTOR3 CTerraEdView::PickPoint(CPoint point)
{
   D3DXVECTOR3 m_vPickRayDir;
   D3DXVECTOR3 m_vPickRayOrig;

   //D3DXMATRIX matProj = m_matProj;
   //m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

   // Compute the vector of the pick ray in screen space
   D3DXVECTOR3 v;
   v.x =  ( ( ( 2.0f * point.x ) / m_d3dsdBackBuffer.Width  ) - 1 ) / m_matProj._11;
   v.y = -( ( ( 2.0f * point.y ) / m_d3dsdBackBuffer.Height ) - 1 ) / m_matProj._22;
   v.z =  1.0f;

   // Get the inverse view matrix
   D3DXMATRIX m;
   D3DXMatrixInverse( &m, NULL, &m_matView );

   // Transform the screen space pick ray into 3D space
   m_vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
   m_vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
   m_vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
   m_vPickRayOrig.x = m._41;
   m_vPickRayOrig.y = m._42;
   m_vPickRayOrig.z = m._43;


   return m_pTerrain->IntersectV(m_vPickRayOrig,m_vPickRayDir);
}



/////////////////////////////////////////////////
///
/// NAME: BurshAlpha
///
/// DESCRIPTION: Copys the alpha from one texture to another
///
/// CREATED:     03/06/2004 9:35:25 AM
///
/// PARAMETERS: 
///             CPoint point
///             LPDIRECT3DTEXTURE9 pBrushTexture
///             LPDIRECT3DTEXTURE9 pDestTexture
///             BOOL bInvert
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void BurshAlpha(CPoint point,LPDIRECT3DTEXTURE9 pBrushTexture,LPDIRECT3DTEXTURE9 pDestTexture,BOOL bInvert)
{
   D3DSURFACE_DESC pBrushDesc;
   pBrushTexture->GetLevelDesc(0, &pBrushDesc );

   D3DSURFACE_DESC pDestDesc;
   pDestTexture->GetLevelDesc(0, &pDestDesc );

   D3DLOCKED_RECT d3dlrBrush;
   pBrushTexture->LockRect( 0, &d3dlrBrush, 0, 0 );
   DWORD * pBrush = (DWORD *)d3dlrBrush.pBits;
   int BrushPitch = d3dlrBrush.Pitch/4;

   D3DLOCKED_RECT d3dlrDest;
   pDestTexture->LockRect( 0, &d3dlrDest, 0, 0 );
   DWORD * pDst = (DWORD *)d3dlrDest.pBits;
   int DPitch = d3dlrDest.Pitch/4;

   int xoff = pBrushDesc.Width/2;
   int yoff = pBrushDesc.Height/2;

   point.Offset(-xoff,-yoff);

   //memset(pDst,c,pDesc.Height*DPitch);

   for (DWORD y=0; y<pBrushDesc.Height; ++y)
   {
      for (DWORD x=0; x<pBrushDesc.Width; ++x)
      {
         DWORD Brushindex = y*BrushPitch + x;

         DWORD xDest = point.x+x;
         DWORD yDest = point.y+y;

         if(xDest < pDestDesc.Width && yDest<pDestDesc.Height && yDest>=0 && xDest>=0)
         {
            DWORD Destindex  = yDest*DPitch + xDest;
            D3DXCOLOR SrcColor(pBrush[Brushindex]);
            D3DXCOLOR DestColor(pDst[Destindex]);
            if(bInvert)
               DestColor.a -= SrcColor.r;
            else
               DestColor.a += SrcColor.r;

            pDst[Destindex] = DestColor;
         }
      }
   }

   pDestTexture->UnlockRect(0);
   pBrushTexture->UnlockRect(0);
}




/////////////////////////////////////////////////
///
/// NAME: BurshColors
///
/// DESCRIPTION: Copys the colors from one texture to another
///
/// CREATED:     03/06/2004 9:35:42 AM
///
/// PARAMETERS: 
///             CPoint point
///             LPDIRECT3DTEXTURE9 pBrushTexture
///             LPDIRECT3DTEXTURE9 pDestTexture
///             BOOL bInvert
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void BurshColors(CPoint point,LPDIRECT3DTEXTURE9 pBrushTexture,LPDIRECT3DTEXTURE9 pDestTexture,BOOL bInvert)
{
   D3DSURFACE_DESC pBrushDesc;
   pBrushTexture->GetLevelDesc(0, &pBrushDesc );

   D3DSURFACE_DESC pDestDesc;
   pDestTexture->GetLevelDesc(0, &pDestDesc );

   D3DLOCKED_RECT d3dlrBrush;
   pBrushTexture->LockRect( 0, &d3dlrBrush, 0, 0 );
   DWORD * pBrush = (DWORD *)d3dlrBrush.pBits;
   int BrushPitch = d3dlrBrush.Pitch/4;

   D3DLOCKED_RECT d3dlrDest;
   pDestTexture->LockRect( 0, &d3dlrDest, 0, 0 );
   DWORD * pDst = (DWORD *)d3dlrDest.pBits;
   int DPitch = d3dlrDest.Pitch/4;

   int xoff = pBrushDesc.Width/2;
   int yoff = pBrushDesc.Height/2;

   point.Offset(-xoff,-yoff);

   //memset(pDst,c,pDesc.Height*DPitch);

   for (DWORD y=0; y<pBrushDesc.Height; ++y)
   {
      for (DWORD x=0; x<pBrushDesc.Width; ++x)
      {
         DWORD Brushindex = y*BrushPitch + x;

         DWORD xDest = point.x+x;
         DWORD yDest = point.y+y;

         if(xDest < pDestDesc.Width && yDest<pDestDesc.Height && yDest>=0 && xDest>=0)
         {
            DWORD Destindex  = yDest*DPitch + xDest;
            D3DXCOLOR SrcColor(pBrush[Brushindex]);
            D3DXCOLOR DestColor(pDst[Destindex]);
            if(bInvert)
               DestColor -= SrcColor;
            else
               DestColor += SrcColor;

            pDst[Destindex] = DestColor;
         }
      }
   }

   pDestTexture->UnlockRect(0);
   pBrushTexture->UnlockRect(0);
}



/////////////////////////////////////////////////
///
/// NAME: OnLButtonDown
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Handes the Left button down
///
/// CREATED:     03/06/2004 9:36:00 AM
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
void CTerraEdView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   if(m_ViewMode == VM_SELECT_OBJ)
   {
      CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
      BOOL bAddedItem = FALSE;
      if(pLevelBar->m_Tab.GetCurSel() == SPRITE_TAB)
      {
         HTREEITEM hItem = pLevelBar->m_SpriteTree.GetSelectedItem();
         if(hItem)
         {
            CSpriteDataEx* pObj = reinterpret_cast<CSpriteDataEx*>(pLevelBar->m_SpriteTree.GetItemData(hItem));
            OnLevelAddSpriteAt(point,pObj);
            bAddedItem = TRUE;
         }

      }
      else if(pLevelBar->m_Tab.GetCurSel() == OBJECT_TAB)
      {
         HTREEITEM hItem = pLevelBar->m_ObjectTree.GetSelectedItem();
         if(hItem)
         {
            CPlanetObjectDataEx* pObj = reinterpret_cast<CPlanetObjectDataEx*>(pLevelBar->m_ObjectTree.GetItemData(hItem));
            OnLevelAddObjectAt(point,pObj);
            bAddedItem = TRUE;
         }
      }


      if(!bAddedItem)
      {
         if(SelectItem(point))
         {
            if(m_pSelObject->m_hItem)
            {
               pLevelBar->SetActiveTab(LEVEL_TAB);
               pLevelBar->m_LevelTree.Select(m_pSelObject->m_hItem,TVGN_CARET);
            }
            SetCapture();
            m_bMouseDown = TRUE;
            m_LastPos = point;
            Render3DEnvironment();
         }

      }
   }
   else if(m_ViewMode == VM_PAINT)
   {
      if(m_SelTexture != -1)
      {
         D3DXVECTOR3 PaintPoint = PickPoint(point);
         BurshAlpha(m_pTerrain->GetPaintPos(PaintPoint),m_pBrushs[m_SelBrush],m_pTerrain->m_Textrues[m_SelTexture].m_pAlphaTexture,FALSE);
         SetCapture();
         m_bMouseDown = TRUE;
         m_LastPos = point;
         m_bChangedTextures = TRUE;
         Render3DEnvironment();
      }
   }
   else if(m_ViewMode == VM_EDIT_TERRAIN)
   {
      D3DXVECTOR3 PaintPoint = PickPoint(point);
      BurshColors(m_pTerrain->GetPaintPos(PaintPoint),m_pBrushs[m_SelBrush],m_pTerrain->GetHightMapTextrue(),FALSE);
      m_bChangedMap = FALSE;
      m_pTerrain->RefreshHightMap();
      Render3DEnvironment();
   }
   else
   {
      SetCapture();
      m_bMouseDown = TRUE;
      m_LastPos = point;
   }

   UpdateMouseCursor();

   CView::OnLButtonDown(nFlags, point);
}



/////////////////////////////////////////////////
///
/// NAME: OnLButtonUp
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Handels the left button up
///
/// CREATED:     03/06/2004 9:36:11 AM
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
void CTerraEdView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   if(m_bMouseDown)
   {
      m_bMouseDown = FALSE;
      ReleaseCapture();
   }
   CView::OnLButtonUp(nFlags, point);
}



/////////////////////////////////////////////////
///
/// NAME: OnMouseMove
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Handels mouse movement
///
/// CREATED:     03/06/2004 9:36:22 AM
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
void CTerraEdView::OnMouseMove(UINT nFlags, CPoint point) 
{
   BOOL bRender = FALSE;
   int xDelta = point.x - m_LastPos.x;
   int yDelta = point.y - m_LastPos.y;
   m_LastPos = point;

   switch(m_ViewMode)
   {
   case VM_PAINT:
      {
         if(m_SelTexture != -1)
         {
            D3DXVECTOR3 PaintPoint = PickPoint(point);
            if(m_bMouseDown)
            {	
               // paint at the current point using selected color and brush
               BurshAlpha(m_pTerrain->GetPaintPos(PaintPoint),m_pBrushs[m_SelBrush],m_pTerrain->m_Textrues[m_SelTexture].m_pAlphaTexture,FALSE);
               bRender = TRUE;
               m_bChangedTextures = TRUE;
            }
            else if(m_bRMouseDown)
            {
               // paint at the current point using selected color and brush inverted
               BurshAlpha(m_pTerrain->GetPaintPos(PaintPoint),m_pBrushs[m_SelBrush],m_pTerrain->m_Textrues[m_SelTexture].m_pAlphaTexture,TRUE);
               bRender = TRUE;
               m_bChangedTextures = TRUE;
            }
         }
      }
      break;
   case VM_SELECT_OBJ:
      m_PickPos = PickPoint(point);
      if(m_bMouseDown)
      {
         D3DXVECTOR3 CurrentPoint;
         switch(m_pSelObject->GetObjType())
         {
         case DATA_OBJ_TYPE_LEVEL:
            CurrentPoint = m_pCurrentLevel->m_StartPos;
            break;
         case DATA_OBJ_TYPE_SPRITE:
         case DATA_OBJ_TYPE_OBJECT:
            CurrentPoint = static_cast<CLevelDataObject*>(m_pSelObject)->m_pos;
            break;
         }

         if(MK_CONTROL & nFlags)
         {
            CurrentPoint.y -= yDelta * 0.5f;
         }
         else
         {
            if(m_PickPos.x != 0 && m_PickPos.z !=0)
            {
               CurrentPoint.x = m_PickPos.x;
               CurrentPoint.z = m_PickPos.z;
            }
         }

         switch(m_pSelObject->GetObjType())
         {
         case DATA_OBJ_TYPE_LEVEL:
            m_pCurrentLevel->m_StartPos = CurrentPoint;
            m_SelObjPos = CurrentPoint;
            break;
         case DATA_OBJ_TYPE_SPRITE:
            {
               CLevelDataSprite* pSprite = static_cast<CLevelDataSprite*>(m_pSelObject);
               CSpriteDataEx* pSpriteMesh = GetDocument()->m_SpriteData[pSprite->m_Type];
               pSprite->m_pos = CurrentPoint;
               FLOAT y = 0;
               if(m_pTerrain)
                  y = m_pTerrain->GetHightAtPosR( pSprite->m_pos.x, pSprite->m_pos.z);	
               y = max(pSprite->m_pos.y,y);
               D3DXVECTOR3 Center = pSprite->m_pos;
               Center.y = y;
               Center = pSpriteMesh->m_Center + Center;
               m_SelObjPos = Center;
            }
            break;
         case DATA_OBJ_TYPE_OBJECT:
            {
               CLevelDataObject* pObjects = static_cast<CLevelDataObject*>(m_pSelObject);
               CPlanetObjectDataEx* pObjectMesh = GetDocument()->m_PlanetObjectData[pObjects->m_Type];
               pObjects->m_pos = CurrentPoint;
               FLOAT y = 0;
               if(m_pTerrain)
                  y = m_pTerrain->GetHightAtPosR( pObjects->m_pos.x, pObjects->m_pos.z);	
               y = max(pObjects->m_pos.y,y);
               D3DXVECTOR3 Center = pObjects->m_pos;
               Center.y = y;
               Center = pObjectMesh->m_Center + Center;
               m_SelObjPos = Center;
            }
            break;
         }
      }
      bRender = TRUE;
      break;
   case VM_ROTATE_OBJ:
      if(MK_CONTROL & nFlags && m_bMouseDown)
      {
         m_Zoom -= yDelta * 0.5f;
         if(m_Zoom > 0.0f)
            m_Zoom = 0.0f;
         bRender = TRUE;
         break;
      }
   case VM_ROTATE:
      if(m_bMouseDown)
      {
         m_Rot.x+= ToRad(yDelta * 0.5f);
         m_Rot.y+= ToRad(xDelta * 0.5f);
         bRender = TRUE;
      }
      break;
   case VM_PAN_VIEW:
      if(m_bMouseDown)
      {
         if(MK_CONTROL & nFlags)
         {
            m_CamPos.y -= yDelta * 0.5f;
         }
         else
         {
            D3DXVECTOR3 vDelta = D3DXVECTOR3(-xDelta* 0.5f,0,yDelta* 0.5f);
            D3DXMATRIX matRotateY;
            D3DXMatrixRotationY( &matRotateY, m_Rot.y);
            D3DXVec3TransformCoord(&vDelta,&vDelta,&matRotateY);
            m_CamPos.x += vDelta.x;
            m_CamPos.z += vDelta.z;
         }
         bRender = TRUE;
      }
      break;
   }

   if(bRender)
   {
      Render3DEnvironment();
      UpdateMouseCursor();
   }

   CView::OnMouseMove(nFlags, point);
}



/////////////////////////////////////////////////
///
/// NAME: OnSelectMode
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets select meode
///
/// CREATED:     03/06/2004 9:36:47 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnSelectMode() 
{
   SetViewMode(VM_SELECT_OBJ);
}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateSelectMode
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:38:35 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateSelectMode(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_ViewMode == VM_SELECT_OBJ);
}



/////////////////////////////////////////////////
///
/// NAME: OnPanMode
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Set Pan mode
///
/// CREATED:     03/06/2004 9:38:49 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnPanMode() 
{
   SetViewMode(VM_PAN_VIEW);
}



/////////////////////////////////////////////////
///
/// NAME: OnUpdatePanMode
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:38:31 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdatePanMode(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_ViewMode == VM_PAN_VIEW);
}



/////////////////////////////////////////////////
///
/// NAME: OnRotateSelected
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Set rotate object mode
///
/// CREATED:     03/06/2004 9:39:12 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnRotateSelected() 
{
   SetViewMode(VM_ROTATE_OBJ);
}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateRotateSelected
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:38:26 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateRotateSelected(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_ViewMode == VM_ROTATE_OBJ);
}



/////////////////////////////////////////////////
///
/// NAME: OnRotateWorld
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets rotate mode
///
/// CREATED:     03/06/2004 9:39:27 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnRotateWorld() 
{
   SetViewMode(VM_ROTATE);
}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateRotateWorld
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:38:23 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateRotateWorld(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_ViewMode == VM_ROTATE);
}



/////////////////////////////////////////////////
///
/// NAME: UpdateMouseCursor
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the mouse cursor
///
/// CREATED:     03/06/2004 9:39:37 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdView::UpdateMouseCursor()
{
   switch(m_ViewMode)
   {
   case VM_ROTATE_OBJ:
      if(GetAsyncKeyState(VK_LCONTROL) || GetAsyncKeyState(VK_RCONTROL))
      {
         if(m_ZoomCursor == NULL)
         {
            m_ZoomCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM);
         }
         SetCursor(m_ZoomCursor);
         return TRUE;
      }
   case VM_ROTATE:
      if(m_RotateCursor == NULL)
      {
         m_RotateCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_ROTATE);
      }
      SetCursor(m_RotateCursor);
      return TRUE;
   case VM_PAN_VIEW:
      if(m_bMouseDown)
      {
         if(m_PanCursor == NULL)
         {
            m_PanCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_PAN_SEL);
         }
         SetCursor(m_PanCursor);
      }
      else
      {
         if(m_PanReadyCursor == NULL)
         {
            m_PanReadyCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_PAN);
         }
         SetCursor(m_PanReadyCursor);
      }
      return TRUE;
   }

   if(m_OldCursor == NULL)
   {
      m_OldCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
   }
   SetCursor(m_OldCursor);
   return FALSE;
}



/////////////////////////////////////////////////
///
/// NAME: OnSetCursor
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets the mouse cursor
///
/// CREATED:     03/06/2004 9:39:46 AM
///
/// PARAMETERS: 
///             CWnd* pWnd
///             UINT nHitTest
///             UINT message
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
   if(UpdateMouseCursor())
      return TRUE;

   return CView::OnSetCursor(pWnd, nHitTest, message);
}



/////////////////////////////////////////////////
///
/// NAME: SetViewMode
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets the view mode
///
/// CREATED:     03/06/2004 9:39:58 AM
///
/// PARAMETERS: 
///             ViewMode_e NewMode
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::SetViewMode(ViewMode_e NewMode)
{
   if(m_ViewMode != NewMode)
   {
      if(m_ViewMode == VM_ROTATE_OBJ)
      {
         m_CamPos = m_EyePt;

      }

      if(NewMode == VM_ROTATE_OBJ)
      {
         // we dont do anything yet
      }

      m_ViewMode = NewMode;
      Render3DEnvironment();
      UpdateMouseCursor();

      CDialogBar* pDialogBar = &static_cast<CChildFrame*>(AfxGetApp()->GetMainWnd())->m_wndPaintBar;

      pDialogBar->GetDlgItem(IDC_BRUSH)->EnableWindow(m_ViewMode==VM_PAINT||m_ViewMode==VM_EDIT_TERRAIN);
      pDialogBar->GetDlgItem(IDC_BRUSH_TEXTURE)->EnableWindow(m_ViewMode==VM_PAINT);

   }
}



/////////////////////////////////////////////////
///
/// NAME: SelectItem
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Selects a 3D object given screen points
///
/// CREATED:     03/06/2004 9:40:38 AM
///
/// PARAMETERS: 
///             CPoint point
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdView::SelectItem(CPoint point)
{
   D3DXVECTOR3 m_vPickRayDir;
   D3DXVECTOR3 m_vPickRayOrig;

   //D3DXMATRIX matProj = m_matProj;
   //m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

   // Compute the vector of the pick ray in screen space
   D3DXVECTOR3 v;
   v.x =  ( ( ( 2.0f * point.x ) / m_d3dsdBackBuffer.Width  ) - 1 ) / m_matProj._11;
   v.y = -( ( ( 2.0f * point.y ) / m_d3dsdBackBuffer.Height ) - 1 ) / m_matProj._22;
   v.z =  1.0f;

   // Get the inverse view matrix
   D3DXMATRIX m;
   D3DXMatrixInverse( &m, NULL, &m_matView );

   // Transform the screen space pick ray into 3D space
   m_vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
   m_vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
   m_vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
   m_vPickRayOrig.x = m._41;
   m_vPickRayOrig.y = m._42;
   m_vPickRayOrig.z = m._43;
   m_pSelObject = NULL;
   FLOAT Seldist = 5000.0f;



   if(D3DXSphereBoundProbe(&m_pCurrentLevel->m_StartPos,1.0f,&m_vPickRayOrig,&m_vPickRayDir))
   {
      D3DXVECTOR3 v = m_EyePt-m_pCurrentLevel->m_StartPos;
      FLOAT dist = D3DXVec3Length(&v);

      if(Seldist > dist)
      {
         Seldist = dist;
         m_pSelObject = m_pCurrentLevel;
         m_SelObjPos = m_pCurrentLevel->m_StartPos;
         m_fSelObjRad = 1.0f;
      }
   }



   for(UINT i=0;i<m_pCurrentLevel->m_Sprites.size();i++)
   {
      CLevelDataSprite* pSprite = m_pCurrentLevel->m_Sprites[i];
      CSpriteDataEx* pSpriteMesh = GetDocument()->m_SpriteData[pSprite->m_Type];

      FLOAT y = 0;
      if(m_pTerrain)
         y = m_pTerrain->GetHightAtPosR( pSprite->m_pos.x, pSprite->m_pos.z);	
      y = max(pSprite->m_pos.y,y);
      D3DXVECTOR3 Center = pSprite->m_pos;
      Center.y = y;
      Center = pSpriteMesh->m_Center + Center;

      if(D3DXSphereBoundProbe(&Center,pSpriteMesh->m_Radius,&m_vPickRayOrig,&m_vPickRayDir))
      {
         D3DXVECTOR3 v = m_EyePt-Center;
         FLOAT dist = D3DXVec3Length(&v);

         if(Seldist > dist)
         {
            Seldist = dist;
            m_pSelObject = pSprite;
            m_SelObjPos = Center;
            m_fSelObjRad = pSpriteMesh->m_Radius;
         }
      }
   }

   for(i=0;i<m_pCurrentLevel->m_Objects.size();i++)
   {
      CLevelDataObject* pObjects = m_pCurrentLevel->m_Objects[i];
      CPlanetObjectDataEx* pObjectMesh = GetDocument()->m_PlanetObjectData[pObjects->m_Type];
      FLOAT y = 0;
      if(m_pTerrain)
         y = m_pTerrain->GetHightAtPosR( pObjects->m_pos.x, pObjects->m_pos.z);	
      y = max(pObjects->m_pos.y,y);
      D3DXVECTOR3 Center = pObjects->m_pos;
      Center.y = y;
      Center = pObjectMesh->m_Center + Center;

      if(D3DXSphereBoundProbe(&Center,pObjectMesh->m_Radius,&m_vPickRayOrig,&m_vPickRayDir))
      {
         D3DXVECTOR3 v = m_EyePt-Center;
         FLOAT dist = D3DXVec3Length(&v);

         if(Seldist > dist)
         {
            Seldist = dist;
            m_pSelObject = pObjects;
            m_SelObjPos = Center;
            m_fSelObjRad = pObjectMesh->m_Radius;
         }
      }
   }

   for(i=0;i<m_pCurrentLevel->m_Events.size();i++)
   {
      CGameEvent* pEvent = m_pCurrentLevel->m_Events[i];
      if(pEvent->m_bDisplay)
      {
         for(UINT j=0;j<pEvent->m_CreateSprites.size();j++)
         {
            CLevelDataSprite* pSprite = pEvent->m_CreateSprites[j];
            CSpriteDataEx* pSpriteMesh = GetDocument()->m_SpriteData[pSprite->m_Type];

            FLOAT y = 0;
            if(m_pTerrain)
               y = m_pTerrain->GetHightAtPosR( pSprite->m_pos.x, pSprite->m_pos.z);	
            y = max(pSprite->m_pos.y,y);
            D3DXVECTOR3 Center = pSprite->m_pos;
            Center.y = y;
            Center = pSpriteMesh->m_Center + Center;

            if(D3DXSphereBoundProbe(&Center,pSpriteMesh->m_Radius,&m_vPickRayOrig,&m_vPickRayDir))
            {
               D3DXVECTOR3 v = m_EyePt-Center;
               FLOAT dist = D3DXVec3Length(&v);

               if(Seldist > dist)
               {
                  Seldist = dist;
                  m_pSelObject = pSprite;
                  m_SelObjPos = Center;
                  m_fSelObjRad = pSpriteMesh->m_Radius;
               }
            }
         }

         for(j=0;j<pEvent->m_CreateObjects.size();j++)
         {
            CLevelDataObject* pObjects = pEvent->m_CreateObjects[j];
            CPlanetObjectDataEx* pObjectMesh = GetDocument()->m_PlanetObjectData[pObjects->m_Type];
            FLOAT y = 0;
            if(m_pTerrain)
               y = m_pTerrain->GetHightAtPosR( pObjects->m_pos.x, pObjects->m_pos.z);	
            y = max(pObjects->m_pos.y,y);
            D3DXVECTOR3 Center = pObjects->m_pos;
            Center.y = y;
            Center = pObjectMesh->m_Center + Center;

            if(D3DXSphereBoundProbe(&Center,pObjectMesh->m_Radius,&m_vPickRayOrig,&m_vPickRayDir))
            {
               D3DXVECTOR3 v = m_EyePt-Center;
               FLOAT dist = D3DXVec3Length(&v);

               if(Seldist > dist)
               {
                  Seldist = dist;
                  m_pSelObject = pObjects;
                  m_SelObjPos = Center;
                  m_fSelObjRad = pObjectMesh->m_Radius;
               }
            }
         }
      }
   }

   return m_pSelObject != NULL;
}



/////////////////////////////////////////////////
///
/// NAME: OnToolsCreaterandomobjects
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Creates Random objects
///
/// CREATED:     03/06/2004 9:40:57 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnToolsCreaterandomobjects() 
{
   CRandomObjDlg dlg(&GetDocument()->m_PlanetObjectData);
   if(m_pSelObject)
   {
      dlg.m_x = m_SelObjPos.x;
      dlg.m_y = m_SelObjPos.z;
   }

   if(dlg.DoModal() == IDOK)
   {
      if(dlg.m_ObjList.size() == 0)
         return;

      CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;

      CLevelData* pObj = m_pCurrentLevel;

#define TREE_DENC 1000
      // Initialize the tree data
      for(UINT i=0; i<dlg.m_NumbItems; i++ )
      {
         CLevelDataObject* ObjectData = new CLevelDataObject;

         // Position the trees randomly
         FLOAT MinHight = 0.0f;

         if(m_pWater)
            MinHight = m_pWater->GetWaterLevel();

         MinHight = max(MinHight,dlg.m_MinHight);

         FLOAT TreeLine = (FLOAT)dlg.m_MaxHight;

         do
         {
            if(dlg.m_UsePoint)
            {
               FLOAT fTheta  = 2.0f*D3DX_PI*(FLOAT)rand()/RAND_MAX;
               FLOAT fRadius = dlg.m_Rad * (FLOAT)rand()/RAND_MAX;
               ObjectData->m_pos.x  = (fRadius * sinf(fTheta)) + dlg.m_x;
               ObjectData->m_pos.z  = (fRadius * cosf(fTheta)) + dlg.m_y;
            }
            else
            {
               ObjectData->m_pos.x  = (FLOAT(rand()%TREE_DENC) * (float)m_pTerrain->GetXSize()/(float)TREE_DENC) - (float)m_pTerrain->GetXSize()/2.0f;
               ObjectData->m_pos.z  = (FLOAT(rand()%TREE_DENC) * (float)m_pTerrain->GetZSize()/(float)TREE_DENC) - (float)m_pTerrain->GetZSize()/2.0f;
            }
            ObjectData->m_pos.y  = m_pTerrain->GetHightAtPosR( ObjectData->m_pos.x, ObjectData->m_pos.z );
         }while(ObjectData->m_pos.y < MinHight || ObjectData->m_pos.y>TreeLine);

         ObjectData->m_Type = dlg.m_ObjList[rand()%(dlg.m_ObjList.size())];




         int Index = pObj->m_Objects.size();
         if(Index != 0)
            Index = pObj->m_Objects[Index-1]->GetIndex() + 1;

         CPlanetObjectData* pData = GetDocument()->m_PlanetObjectData[ObjectData->m_Type];
         ObjectData->m_Name.Format("%s-%d",pData->m_Name,Index);

         ObjectData->SetIndex(Index);
         pObj->m_Objects.push_back(ObjectData);

         HTREEITEM hItem = AddObjectToTree(&pLevelBar->m_LevelTree,m_pCurrentLevel->m_hItem,pObj->m_Objects[pObj->m_Objects.size()-1]);
      }
   }
}



/////////////////////////////////////////////////
///
/// NAME: GenLightMap
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Generates the Light/Shadow map
///
/// CREATED:     03/06/2004 9:41:09 AM
///
/// PARAMETERS: 
///             CString CurrentFile
///             BOOL bInvertAlpha
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::GenLightMap(CString CurrentFile, BOOL bInvertAlpha) 
{
   if(m_pTerrain)
   {
      CWaitCursor wait;
      CComPtr<IDirect3DTexture9> pTexture = NULL;
      D3DUtil::CreateTexture(m_pd3dDevice,(char*)(LPCTSTR)CurrentFile,pTexture,&GetDocument()->m_Zip);
      m_pTerrain->GenerateLightMap(CurrentFile,m_pd3dDevice,m_pCurrentLevel->m_SunPos,D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),D3DXCOLOR(0.3f,0.3f,0.3f,0.0f),pTexture,bInvertAlpha);
      D3DUtil::ReleaseTexture(pTexture);

      int index = GetDocument()->m_Zip.FindFile(CurrentFile);
      if( index != -1)
      {
         GetDocument()->m_Zip.DeleteFile((WORD)index);
      }
      GetDocument()->m_Zip.AddNewFile(CurrentFile);
      DeleteFile(CurrentFile);
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnLButtonDblClk
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Displays the edit dialog of a selected item when it is double clicked
///
/// CREATED:     03/06/2004 9:42:32 AM
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
void CTerraEdView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   if(m_ViewMode == VM_SELECT_OBJ)
   {
      if(SelectItem(point) && m_pSelObject)
      {
         CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
         if(m_pSelObject->m_hItem)
         {
            pLevelBar->SetActiveTab(LEVEL_TAB);
            pLevelBar->m_LevelTree.Select(m_pSelObject->m_hItem,TVGN_CARET);
            pLevelBar->ShowEditDialog();
            Render3DEnvironment();
         }
      }
   }

   CView::OnLButtonDblClk(nFlags, point);
}





/////////////////////////////////////////////////
///
/// NAME: OnEditTerrain
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets the Edit terrain mode
///
/// CREATED:     03/06/2004 9:42:53 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnEditTerrain() 
{
   SetViewMode(VM_EDIT_TERRAIN);

}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateEditTerrain
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:38:02 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateEditTerrain(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_ViewMode == VM_EDIT_TERRAIN);
}



/////////////////////////////////////////////////
///
/// NAME: OnPaintMode
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets the Paint mode
///
/// CREATED:     03/06/2004 9:43:08 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnPaintMode() 
{
   SetViewMode(VM_PAINT);

}



/////////////////////////////////////////////////
///
/// NAME: OnUpdatePaintMode
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:37:57 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdatePaintMode(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_ViewMode == VM_PAINT);
}



/////////////////////////////////////////////////
///
/// NAME: OnRButtonDown
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Handels the right mouse button down
///
/// CREATED:     03/06/2004 9:43:29 AM
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
void CTerraEdView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   if(m_ViewMode == VM_PAINT)
   {
      if(m_SelTexture != -1)
      {
         D3DXVECTOR3 PaintPoint = PickPoint(point);
         // paint at the current point using selected color and brush inverted
         BurshAlpha(m_pTerrain->GetPaintPos(PaintPoint),m_pBrushs[m_SelBrush],m_pTerrain->m_Textrues[m_SelTexture].m_pAlphaTexture,TRUE);
         m_bChangedTextures = TRUE;
         SetCapture();
         m_bRMouseDown = TRUE;
         m_LastPos = point;
         Render3DEnvironment();
      }
   }
   else if(m_ViewMode == VM_EDIT_TERRAIN)
   {
      D3DXVECTOR3 PaintPoint = PickPoint(point);
      BurshColors(m_pTerrain->GetPaintPos(PaintPoint),m_pBrushs[m_SelBrush],m_pTerrain->GetHightMapTextrue(),TRUE);
      m_bChangedMap = FALSE;
      m_pTerrain->RefreshHightMap();
      Render3DEnvironment();
   }

   CView::OnRButtonDown(nFlags, point);
}



/////////////////////////////////////////////////
///
/// NAME: OnRButtonUp
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Handels the right mouse button up
///
/// CREATED:     03/06/2004 9:43:42 AM
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
void CTerraEdView::OnRButtonUp(UINT nFlags, CPoint point) 
{
   if(m_bRMouseDown)
   {
      m_bRMouseDown = FALSE;
      ReleaseCapture();
   }
   CView::OnRButtonUp(nFlags, point);
}



/////////////////////////////////////////////////
///
/// NAME: OnFileSavecurrentterrain
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Saves the hight map
///
/// CREATED:     03/06/2004 9:44:51 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnFileSavecurrentterrain() 
{
   m_pTerrain->SaveHightMap(&GetDocument()->m_Zip);
   m_bChangedMap = FALSE;	
}



/////////////////////////////////////////////////
///
/// NAME: OnFileSavetextures
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Saves textures
///
/// CREATED:     03/06/2004 9:45:06 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnFileSavetextures() 
{
   m_pTerrain->SaveTextures(&GetDocument()->m_Zip);
   m_bChangedTextures = FALSE;
}



/////////////////////////////////////////////////
///
/// NAME: OnSelchangeBrush
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Changes the current brush
///
/// CREATED:     03/06/2004 9:45:26 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnSelchangeBrush()
{
   CDialogBar* pDialogBar = &static_cast<CChildFrame*>(AfxGetApp()->GetMainWnd())->m_wndPaintBar;
   CComboBox* pBrushBox = (CComboBox*)pDialogBar->GetDlgItem(IDC_BRUSH);
   m_SelBrush = pBrushBox->GetCurSel();
}



/////////////////////////////////////////////////
///
/// NAME: OnSelchangeBrushTextrue
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Changes the current texture bursh
///
/// CREATED:     03/06/2004 9:45:40 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnSelchangeBrushTextrue()
{
   CDialogBar* pDialogBar = &static_cast<CChildFrame*>(AfxGetApp()->GetMainWnd())->m_wndPaintBar;
   CComboBox* pBrushBox = (CComboBox*)pDialogBar->GetDlgItem(IDC_BRUSH_TEXTURE);
   int index = pBrushBox->GetCurSel();
   m_SelTexture = pBrushBox->GetItemData(index);
}



/////////////////////////////////////////////////
///
/// NAME: SetCurrentLevel
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets the level as current
///
/// CREATED:     03/06/2004 9:46:03 AM
///
/// PARAMETERS: 
///             CLevelData* pLevel
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::SetCurrentLevel(CLevelData* pLevel)
{
   if(m_pCurrentLevel != pLevel)
   {
      if(m_bChangedTextures)
      {
         if(AfxMessageBox("You have changed textures, would you like to save them?",MB_YESNO)==IDYES)
         {
            OnFileSavetextures();
         }
         m_bChangedTextures = FALSE;
      }

      if(m_bChangedMap)
      {
         if(AfxMessageBox("You have changed the terrain, would you like to save it?",MB_YESNO)==IDYES)
         {
            OnFileSavecurrentterrain();
         }
         m_bChangedMap = FALSE;
      }
      m_pCurrentLevel = pLevel;

   }
}



/////////////////////////////////////////////////
///
/// NAME: OnViewFog
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Toggels the fog on/off
///
/// CREATED:     03/06/2004 9:46:16 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnViewFog() 
{
   m_bFog = !m_bFog;
   Render3DEnvironment();
}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateViewFog
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:37:46 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateViewFog(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_bFog);	
}



/////////////////////////////////////////////////
///
/// NAME: FillAlpha
///
/// DESCRIPTION: Fills the alpha given a range
///
/// CREATED:     03/06/2004 9:46:53 AM
///
/// PARAMETERS: 
///             LPDIRECT3DTEXTURE9 pTerrainTexture
///             LPDIRECT3DTEXTURE9 pDestTexture
///             DWORD max
///             DWORD min
///             DWORD percent
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void FillAlpha(LPDIRECT3DTEXTURE9 pTerrainTexture,LPDIRECT3DTEXTURE9 pDestTexture,DWORD max, DWORD min, DWORD percent)
{
   D3DSURFACE_DESC pTerrainDesc;
   pTerrainTexture->GetLevelDesc(0, &pTerrainDesc );

   D3DSURFACE_DESC pDestDesc;
   pDestTexture->GetLevelDesc(0, &pDestDesc );

   D3DLOCKED_RECT d3dlrTerrain;
   pTerrainTexture->LockRect( 0, &d3dlrTerrain, 0, 0 );
   DWORD * pTerrain = (DWORD *)d3dlrTerrain.pBits;
   int TerrainPitch = d3dlrTerrain.Pitch/4;

   D3DLOCKED_RECT d3dlrDest;
   pDestTexture->LockRect( 0, &d3dlrDest, 0, 0 );
   DWORD * pDst = (DWORD *)d3dlrDest.pBits;
   int DPitch = d3dlrDest.Pitch/4;

   FLOAT fmax = (float)max / 255.0f;
   FLOAT fmin = (float)min / 255.0f;
   FLOAT alpha = (float)percent / 100.0f;


   for (DWORD y=0; y<pTerrainDesc.Height; ++y)
   {
      for (DWORD x=0; x<pTerrainDesc.Width; ++x)
      {
         int Terrainindex = y*TerrainPitch + x;
         D3DXCOLOR SrcColor(pTerrain[Terrainindex]);

         if(SrcColor.r >= fmin && SrcColor.r < fmax)
         {
            int Destindex  = y*DPitch + x;
            D3DXCOLOR DestColor(pDst[Destindex]);
            DestColor.a = alpha;
            pDst[Destindex] = DestColor;
         }
      }
   }

   pDestTexture->UnlockRect(0);
   pTerrainTexture->UnlockRect(0);
}



/////////////////////////////////////////////////
///
/// NAME: OnToolsFill
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Displays the fill dialog
///
/// CREATED:     03/06/2004 9:47:21 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnToolsFill() 
{
   CFillDialog dlg(&m_pTerrain->m_Textrues);
   dlg.m_texture = m_SelTexture-1;
   if(dlg.DoModal() == IDOK)
   {
      if(dlg.m_texture >= 0 && m_pTerrain->m_Textrues.size() > 1)
      {
         FillAlpha(m_pTerrain->GetHightMapTextrue(),m_pTerrain->m_Textrues[dlg.m_texture+1].m_pAlphaTexture,dlg.m_max,dlg.m_min,dlg.m_percent);
         m_bChangedTextures = TRUE;
         Render3DEnvironment();
      }
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateToolsFill
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:37:36 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateToolsFill(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_ViewMode == VM_PAINT);
}



/////////////////////////////////////////////////
///
/// NAME: OnDestroy
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Destroys the window
///
/// CREATED:     03/06/2004 9:47:25 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnDestroy() 
{
   if(m_bChangedTextures)
   {
      if(AfxMessageBox("You have changed textures, would you like to save them?",MB_YESNO)==IDYES)
      {
         OnFileSavetextures();
      }
      m_bChangedTextures = FALSE;
   }

   if(m_bChangedMap)
   {
      if(AfxMessageBox("You have changed the terrain, would you like to save it?",MB_YESNO)==IDYES)
      {
         OnFileSavecurrentterrain();
      }
      m_bChangedMap = FALSE;
   }

   CView::OnDestroy();	
}



/////////////////////////////////////////////////
///
/// NAME: OnEventAddChangesprite
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds an Change Sprite
///
/// CREATED:     03/06/2004 9:47:47 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnEventAddChangesprite() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;

   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
      return;

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   CGameEvent* pObj = reinterpret_cast<CGameEvent*>(pLevelBar->m_LevelTree.GetItemData(hItem));

   CLevelDataSprite* SpriteData = new CLevelDataSprite;

   CLevelData* pParentObj = NULL;
   HTREEITEM hParentItem = hItem;

   while(pParentObj == NULL && hParentItem != NULL)
   {
      hParentItem = pLevelBar->m_LevelTree.GetParentItem(hParentItem);
      CDataObjBase* pTempObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hParentItem));
      if(pTempObj->GetObjType() == DATA_OBJ_TYPE_LEVEL)
      {
         pParentObj = static_cast<CLevelData*>(pTempObj);
         break;
      }
   }

   CEditSprite dlg(SpriteData,&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_SpriteData,pParentObj);
   if(dlg.DoModal() == IDOK)
   {
      int Index = pObj->m_SpriteChange.size();
      if(Index != 0)
         Index = pObj->m_SpriteChange[Index-1]->GetIndex() + 1;

      SpriteData->SetIndex(Index);
      SpriteData->SetSpriteObjType(1);
      pObj->m_SpriteChange.push_back(SpriteData);

      AddSpriteToTree(&pLevelBar->m_LevelTree,hItem,pObj->m_SpriteChange[pObj->m_SpriteChange.size()-1]);
   }
   else
   {
      delete SpriteData;
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnEventAddCreateobject
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Adds an Event Create object
///
/// CREATED:     03/06/2004 9:48:08 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnEventAddCreateobject() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;

   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
      return;

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   CGameEvent* pObj = reinterpret_cast<CGameEvent*>(pLevelBar->m_LevelTree.GetItemData(hItem));


   CLevelDataObject* ObjectData = new CLevelDataObject;

   CEditObject dlg(ObjectData,&static_cast<CTerraEdDoc*>(static_cast<CChildFrame*>(GetParentFrame())->GetActiveDocument())->m_PlanetObjectData);
   if(dlg.DoModal() == IDOK)
   {
      int Index = pObj->m_CreateObjects.size();
      if(Index != 0)
         Index = pObj->m_CreateObjects[Index-1]->GetIndex() + 1;

      ObjectData->SetIndex(Index);

      pObj->m_CreateObjects.push_back(ObjectData);

      AddObjectToTree(&pLevelBar->m_LevelTree,hItem,pObj->m_CreateObjects[pObj->m_CreateObjects.size()-1]);
   }
   else
   {
      delete ObjectData;
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnEventDisplay
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets the event as displayed
///
/// CREATED:     03/06/2004 9:48:24 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnEventDisplay() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;

   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
      return;

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   CGameEvent* pObj = reinterpret_cast<CGameEvent*>(pLevelBar->m_LevelTree.GetItemData(hItem));
   pObj->m_bDisplay = !pObj->m_bDisplay;
   Render3DEnvironment();
}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateEventDisplay
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:37:28 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateEventDisplay(CCmdUI* pCmdUI) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
   {
      pCmdUI->Enable(FALSE);
      return;
   }

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();
   BOOL bEnable = FALSE;
   BOOL bCheck = FALSE;

   if(hItem)
   {

      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hItem));
      if(pObj)
      {
         bEnable = pObj->GetObjType()==DATA_OBJ_TYPE_EVENT;
         if(bEnable)
         {
            bCheck = static_cast<CGameEvent*>(pObj)->m_bDisplay;
         }
      }
   }

   pCmdUI->Enable(bEnable);
   pCmdUI->SetCheck(bCheck);

}



/////////////////////////////////////////////////
///
/// NAME: OnSpriteView
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Sets the selected sprite/Object as visable and selected
///
/// CREATED:     03/06/2004 9:50:35 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnSpriteView() 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
   {
      return;
   }

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   BOOL bEnable = FALSE;

   if(hItem)
   {

      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hItem));
      if(pObj)
      {
         if(pObj->GetObjType()==DATA_OBJ_TYPE_SPRITE)
         {
            CLevelDataSprite* pSprite = (CLevelDataSprite*)pObj;
            CSpriteDataEx* pSpriteMesh = GetDocument()->m_SpriteData[pSprite->m_Type];

            FLOAT y = 0;
            if(m_pTerrain)
               y = m_pTerrain->GetHightAtPosR( pSprite->m_pos.x, pSprite->m_pos.z);	
            y = max(pSprite->m_pos.y,y);
            D3DXVECTOR3 Center = pSprite->m_pos;
            Center.y = y;
            Center = pSpriteMesh->m_Center + Center;
            m_pSelObject = pSprite;
            m_SelObjPos = Center;
            m_fSelObjRad = pSpriteMesh->m_Radius;
         }
         else if(pObj->GetObjType()==DATA_OBJ_TYPE_OBJECT)
         {
            CLevelDataObject* pObjects = (CLevelDataObject*)pObj;
            CPlanetObjectDataEx* pObjectMesh = GetDocument()->m_PlanetObjectData[pObjects->m_Type];
            FLOAT y = 0;
            if(m_pTerrain)
               y = m_pTerrain->GetHightAtPosR( pObjects->m_pos.x, pObjects->m_pos.z);	
            y = max(pObjects->m_pos.y,y);
            D3DXVECTOR3 Center = pObjects->m_pos;
            Center.y = y;
            Center = pObjectMesh->m_Center + Center;
            m_pSelObject = pObjects;
            m_SelObjPos = Center;
            m_fSelObjRad = pObjectMesh->m_Radius;
         }	
         SetViewMode(VM_ROTATE_OBJ);
      }
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnUpdateSpriteView
///
/// CLASS: CTerraEdView
///
/// DESCRIPTION: Updates the UI
///
/// CREATED:     03/06/2004 9:37:22 AM
///
/// PARAMETERS: 
///             CCmdUI* pCmdUI
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdView::OnUpdateSpriteView(CCmdUI* pCmdUI) 
{
   CLevelTreeBar* pLevelBar = &static_cast<CChildFrame*>(GetParentFrame())->m_wndPalette;
   if(pLevelBar->m_Tab.GetCurSel() != LEVEL_TAB)
   {
      pCmdUI->Enable(FALSE);
      return;
   }

   HTREEITEM hItem = pLevelBar->m_LevelTree.GetSelectedItem();

   BOOL bEnable = FALSE;

   if(hItem)
   {

      CDataObjBase* pObj = reinterpret_cast<CDataObjBase*>(pLevelBar->m_LevelTree.GetItemData(hItem));
      if(pObj)
      {
         bEnable = pObj->GetObjType()==DATA_OBJ_TYPE_SPRITE || pObj->GetObjType()==DATA_OBJ_TYPE_OBJECT;
      }
   }


   pCmdUI->Enable(bEnable);	
}
