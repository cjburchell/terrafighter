// Vegetation.h: interface for the CVegetation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VEGETATION_H__63BA73D3_35F9_4079_BC9E_E1EA4B2917FE__INCLUDED_)
#define AFX_VEGETATION_H__63BA73D3_35F9_4079_BC9E_E1EA4B2917FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StaticObj.h"
#include "vertextypes.h"

#define NUMB_TREE_VERTEX 24


class CVegetation : public CStaticObj  
{
public:
   CVegetation(FLOAT fWidth, FLOAT fHeight, const char* TextureFile);
   virtual ~CVegetation();

   virtual void BuildShadow(const D3DXVECTOR3& lightpos);

   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);
   virtual HRESULT RenderSimple(LPDIRECT3DDEVICE9 pd3dDevice);

   virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);

   virtual void DeleteDeviceObjects();

   virtual void DoCollision(CDisplayObject* pObj,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg) // sprites can pass through vegitation
   {
   }

protected:
   CComPtr<IDirect3DTexture9>      m_pTexture;
   CComPtr<IDirect3DVertexBuffer9> m_pVB;
   TREEVERTEX  m_v[NUMB_TREE_VERTEX];
   CString m_TextureFile;
   DWORD m_nTri;
};

#endif // !defined(AFX_VEGETATION_H__63BA73D3_35F9_4079_BC9E_E1EA4B2917FE__INCLUDED_)
