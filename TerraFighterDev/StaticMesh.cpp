// StaticMesh.cpp: implementation of the CStaticMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StaticMesh.h"
#include "D3DApp.h"
#include "vertextypes.h"
#include "TerraFighterApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticMesh::CStaticMesh(const char* object) : CStaticObj(object)
{
   m_MeshFile = object; 
   m_bShadowBuilt = FALSE;
   D3DXMatrixRotationX( &m_Rotate, HALFPI+PI );
   m_pMesh = NULL;
   m_bShowShadow = FALSE;
}

CStaticMesh::~CStaticMesh()
{
   MeshCash::releaseMesh(m_pMesh);
}


void CStaticMesh::BuildShadow(const D3DXVECTOR3& lightpos)
{
   if(m_bShowShadow && IsVisable())
   {
      if(!m_bShadowBuilt)
      {
         if(m_pShadowVolume == NULL)
         {
            m_pShadowVolume = new CShadowVolume();
         }

         // Build the shadow volume
         m_pShadowVolume->Reset();
         m_pShadowVolume->BuildFromMeshFile( m_pMesh, lightpos, GetMatrix(), TRUE );
      }
      m_bShadowBuilt = TRUE;
   }
}

HRESULT CStaticMesh::RenderShadow(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   if(IsVisable())
   {
      return CDisplayObject::RenderShadow(pd3dDevice,pClipper);
   }
   return S_OK;
}

HRESULT CStaticMesh::Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   if(pClipper == NULL || pClipper->MeshFOVCheck(GetPos(),m_Radius) && IsVisable())
   {
      //		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT  );
      //		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );

      if(!m_bShowShadow)
         pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

      //pd3dDevice->SetTransform( D3DTS_WORLD, GetMatrix() );
      if(m_pMesh)
         m_pMesh->render(pd3dDevice,&GetMatrix());

      if(!m_bShowShadow)
         pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
   }
   return S_OK;
}

HRESULT CStaticMesh::RenderSimple(LPDIRECT3DDEVICE9 pd3dDevice)
{
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      FALSE );

   //D3DXMATRIX matFinal;
   //D3DXMatrixTranslation( &matFinal,0.0f ,-2.5, 0.0f );
   //D3DXMatrixMultiply( &matFinal, GetMatrix(), &matFinal );

   //pd3dDevice->SetTransform( D3DTS_WORLD, &matFinal );

   //pd3dDevice->SetTransform( D3DTS_WORLD, GetMatrix() );

   if(m_pMesh)
      m_pMesh->render(pd3dDevice,&GetMatrix());

   pd3dDevice->SetTexture( 0, NULL );

   pd3dDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
   return S_OK;
}

HRESULT CStaticMesh::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
   m_pMesh = MeshCash_CreateMesh(pd3dDevice, m_MeshFile.GetBuffer(m_MeshFile.GetLength()),D3DFVF_D3DVERTEX);

   if(m_pMesh)
   {
      m_pMesh->computeBoundingSphere(&m_Center,&m_Radius);
      m_Radius *= m_Scale; 
   }

   D3DXMATRIX MeshScaled;
   D3DXMatrixScaling( &MeshScaled, GetScale(), GetScale(), GetScale());
   D3DXMatrixMultiply( &MeshScaled, &MeshScaled, &m_Rotate );
   D3DXVec3TransformCoord(&m_Center,&m_Center,&MeshScaled);
   return S_OK;
}

void CStaticMesh::DeleteDeviceObjects()
{
   MeshCash::releaseMesh(m_pMesh);
   m_bShadowBuilt = FALSE;
}

HRESULT CStaticMesh::Create(const char* object, FLOAT Scale,D3DXVECTOR3 pos)
{
   m_MeshFile = object; 
   return CStaticObj::Create(object,  Scale, pos);
}

void CStaticMesh::DoCollision(CDisplayObject* pObj,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg)
{
   // do nothing right now might put in code to avoid 
}
