///////////////////////////////////////////////////////////////////////////////
///  TerraEdView.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:15:57 AM by Christiaan Burchell
///
///  PURPOSE:  View Object
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

// TerraEdView.h : interface of the CTerraEdView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAEDVIEW_H__A358CA22_0A56_44E7_8468_A8FB78E58310__INCLUDED_)
#define AFX_TERRAEDVIEW_H__A358CA22_0A56_44E7_8468_A8FB78E58310__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3dapp.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "FOVClipper.h"
#include "Terrain.h"
#include "Water.h"
#include "SkyBox.h"
#include "TerraEdDoc.h"

enum ViewMode_e
{
   VM_SELECT_OBJ = 0,
   VM_PAN_VIEW = 1,
   VM_ROTATE = 2,
   VM_ROTATE_OBJ = 3,
   VM_PAINT = 4,
   VM_EDIT_TERRAIN = 5
};


class CTerraEdView : public CView , public CD3DApplication
{
protected: // create from serialization only
   CTerraEdView();
   DECLARE_DYNCREATE(CTerraEdView)

public:

   CLevelData* GetCurrentLevel(){return m_pCurrentLevel;}
   void SetCurrentLevel(CLevelData* pLevel);

protected:
   CLevelData*  m_pCurrentLevel;


   // Attributes
public:
   D3DXVECTOR3 CTerraEdView::PickPoint(CPoint point);

   CTerraEdDoc* GetDocument();

   CImageList m_Icons;
   int m_SelBrush;
   int m_SelTexture;

   BOOL m_bFog;

   BOOL m_bChangedTextures;
   BOOL m_bChangedMap;



   CD3DFont*  m_pFont;              // Font for drawing text
   CFOVClipper m_Clipper;
   D3DXVECTOR3 m_EyePt;
   D3DXVECTOR3 m_LookatPt;

   void SetViewMode(ViewMode_e NewMode);
   FLOAT m_Zoom;

   BOOL SelectItem(CPoint point);
   void GenLightMap(CString CurrentFile,BOOL bInvertAlpha);

   CComPtr<IDirect3DTexture9> m_pBrushs[4];


   D3DXVECTOR3 m_PickPos;
   BOOL m_bShowPick;

   CTerrain* m_pTerrain;
   CSkyBox* m_pSkyBox;
   CWater* m_pWater;
   D3DXMATRIX m_matView;
   D3DXMATRIX m_matProj;

   CDataObjBase* m_pSelObject;
   D3DXVECTOR3   m_SelObjPos;
   FLOAT		  m_fSelObjRad;


   D3DXVECTOR3 m_Rot;

   D3DXVECTOR3 m_CamPos;

   LPD3DXMESH	m_pBoxMesh;

   BOOL m_bMouseDown;
   BOOL m_bRMouseDown;
   CPoint m_LastPos;
   BOOL UpdateMouseCursor();
   HCURSOR m_ZoomCursor;
   HCURSOR m_PanCursor;
   HCURSOR m_PanReadyCursor;
   HCURSOR m_RotateCursor;
   HCURSOR m_OldCursor;

   HRESULT ConfirmDevice( D3DCAPS9*,DWORD,D3DFORMAT );
   HRESULT OneTimeSceneInit();
   HRESULT InitDeviceObjects();
   HRESULT RestoreDeviceObjects();
   HRESULT FrameMove();
   HRESULT Render();
   HRESULT InvalidateDeviceObjects();
   HRESULT DeleteDeviceObjects();
   HRESULT FinalCleanup();
   HRESULT Resize3DEnvironment();

   // Operations
public:

   HTREEITEM AddSpriteToTree(CTreeCtrl* pTree,HTREEITEM hParentItem,CLevelDataSprite* pSprite);
   HTREEITEM AddObjectToTree(CTreeCtrl* pTree,HTREEITEM hParentItem,CLevelDataObject* pObjects);
   void AddTextureToTree(CTreeCtrl* pTree,HTREEITEM hParentItem,CTextureData* pTexture);
   void AddEventToTree(CTreeCtrl* pTree,HTREEITEM hParentItem,CGameEvent* pEvent);
   void AddLevelToTree(CTreeCtrl* pTree,CLevelData* pLevel);

   void AddObjectDataToTree(CTreeCtrl* pTree,CPlanetObjectData* pObj);

   void AddSpriteDataToTree(CTreeCtrl* pTree,CSpriteData* pSprite);
   void AddWeaponDataToTree(CTreeCtrl* pTree,CWeaponData* pObj);

   void OnLevelAddObjectAt(CPoint pt, CPlanetObjectData* pObj);
   void OnLevelAddSpriteAt(CPoint pt, CSpriteData* pObj);

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTerraEdView)
public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void OnInitialUpdate();
protected:
   //}}AFX_VIRTUAL

   // Implementation
public:
   void UpdateMap();
   virtual ~CTerraEdView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   ViewMode_e m_ViewMode;

   // Generated message map functions
protected:
   //{{AFX_MSG(CTerraEdView)
   afx_msg void OnSelchangeBrush();
   afx_msg void OnSelchangeBrushTextrue();
   afx_msg void OnEdit();
   afx_msg void OnDelete();
   afx_msg void OnEventAddCreatesprite();
   afx_msg void OnLevelAddEvent();
   afx_msg void OnLevelAddObject();
   afx_msg void OnLevelAddSprite();
   afx_msg void OnLevelAddTexture();
   afx_msg void OnUpdateEvent(CCmdUI* pCmdUI);
   afx_msg void OnUpdateLevel(CCmdUI* pCmdUI);
   afx_msg void OnToolsNewLevel();
   afx_msg void OnLevelActive();
   afx_msg void OnUpdateLevelActive(CCmdUI* pCmdUI);
   afx_msg void OnUpdateToolsNewLevel(CCmdUI* pCmdUI);
   afx_msg void OnToolsNewObjectdata();
   afx_msg void OnUpdateToolsNewObjectdata(CCmdUI* pCmdUI);
   afx_msg void OnToolsNewSpritedata();
   afx_msg void OnUpdateToolsNewSpritedata(CCmdUI* pCmdUI);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnSelectMode();
   afx_msg void OnUpdateSelectMode(CCmdUI* pCmdUI);
   afx_msg void OnPanMode();
   afx_msg void OnUpdatePanMode(CCmdUI* pCmdUI);
   afx_msg void OnRotateSelected();
   afx_msg void OnUpdateRotateSelected(CCmdUI* pCmdUI);
   afx_msg void OnRotateWorld();
   afx_msg void OnUpdateRotateWorld(CCmdUI* pCmdUI);
   afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
   afx_msg void OnToolsCreaterandomobjects();
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg void OnEditTerrain();
   afx_msg void OnUpdateEditTerrain(CCmdUI* pCmdUI);
   afx_msg void OnPaintMode();
   afx_msg void OnUpdatePaintMode(CCmdUI* pCmdUI);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnFileSavecurrentterrain();
   afx_msg void OnFileSavetextures();
   afx_msg void OnViewFog();
   afx_msg void OnUpdateViewFog(CCmdUI* pCmdUI);
   afx_msg void OnToolsFill();
   afx_msg void OnUpdateToolsFill(CCmdUI* pCmdUI);
   afx_msg void OnDestroy();
   afx_msg void OnEventAddChangesprite();
   afx_msg void OnEventAddCreateobject();
   afx_msg void OnEventDisplay();
   afx_msg void OnUpdateEventDisplay(CCmdUI* pCmdUI);
   afx_msg void OnSpriteView();
   afx_msg void OnUpdateSpriteView(CCmdUI* pCmdUI);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TerraEdView.cpp
inline CTerraEdDoc* CTerraEdView::GetDocument()
{ return (CTerraEdDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAEDVIEW_H__A358CA22_0A56_44E7_8468_A8FB78E58310__INCLUDED_)
