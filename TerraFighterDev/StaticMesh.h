// StaticMesh.h: interface for the CStaticMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICMESH_H__BB5A5D4F_6A61_45F6_89F1_7C4F6386F0FF__INCLUDED_)
#define AFX_STATICMESH_H__BB5A5D4F_6A61_45F6_89F1_7C4F6386F0FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StaticObj.h"

class CStaticMesh : public CStaticObj  
{
public:
   CStaticMesh(const char* object);
   virtual ~CStaticMesh();

   virtual void BuildShadow(const D3DXVECTOR3& lightpos);
   //
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);
   virtual HRESULT RenderSimple(LPDIRECT3DDEVICE9 pd3dDevice);

   virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);

   virtual void DeleteDeviceObjects();

   virtual HRESULT Create(const char* object, FLOAT Scale,D3DXVECTOR3 pos);

   virtual HRESULT RenderShadow(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);


   virtual Xfile::File* GetMesh()
   {
      return m_pMesh;
   }
   void ShowShadow(BOOL bShow){m_bShowShadow = bShow;}

   virtual void DoCollision(CDisplayObject* pObj,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg);

private:
   BOOL m_bShowShadow;
   BOOL m_bShadowBuilt;
   // mesh stuff
   CString m_MeshFile;
   Xfile::File* m_pMesh;

};

#endif // !defined(AFX_STATICMESH_H__BB5A5D4F_6A61_45F6_89F1_7C4F6386F0FF__INCLUDED_)
