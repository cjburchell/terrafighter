// Vegetation.cpp: implementation of the CVegetation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vegetation.h"
#include "D3DApp.h"
#include "DXUtil.h"
#include "TerraFighterApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CVegetation::CVegetation(FLOAT fWidth, FLOAT fHeight, const char* TextureFile) : CStaticObj(TextureFile)
{
   m_nTri = 8;
   m_pVB      = NULL;
   m_pTexture = NULL;
   m_TextureFile = TextureFile;

   m_v[0].p     = D3DXVECTOR3(-fWidth, 0.0f, 0.0f );
   m_v[0].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[0].tu    = 0.0f;   
   m_v[0].tv	  = 1.0f;

   m_v[1].p     = D3DXVECTOR3(-fWidth, 2.0f*fHeight, 0.0f  );
   m_v[1].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[1].tu    = 0.0f;   
   m_v[1].tv    = 0.0f;

   m_v[2].p     = D3DXVECTOR3(fWidth, 0.0f, 0.0f  );
   m_v[2].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[2].tu    = 1.0f;
   m_v[2].tv    = 1.0f;

   m_v[3] = m_v[2];
   m_v[4] = m_v[1];
   m_v[5].p     = D3DXVECTOR3(fWidth, 2.0f*fHeight, 0.0f  );
   m_v[5].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[5].tu    = 1.0f;   
   m_v[5].tv    = 0.0f;


   m_v[6] = m_v[2];
   m_v[7] = m_v[1];
   m_v[8] = m_v[0];

   m_v[9] = m_v[5];
   m_v[10] = m_v[1];
   m_v[11] = m_v[2];

   m_v[12].p     = D3DXVECTOR3(0.0f, 0.0f,-fWidth );
   m_v[12].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[12].tu    = 0.0f;   
   m_v[12].tv	  = 1.0f;
   m_v[13].p     = D3DXVECTOR3(0.0f, 2.0f*fHeight, -fWidth  );
   m_v[13].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[13].tu    = 0.0f;   
   m_v[13].tv    = 0.0f;
   m_v[14].p     = D3DXVECTOR3(0.0f, 0.0f, fWidth  );
   m_v[14].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[14].tu    = 1.0f;
   m_v[14].tv    = 1.0f;

   m_v[15] = m_v[14];
   m_v[16] = m_v[13];
   m_v[17].p     = D3DXVECTOR3(0.0f, 2.0f*fHeight, fWidth  );
   m_v[17].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[17].tu    = 1.0f;   
   m_v[17].tv    = 0.0f;

   m_v[18] = m_v[14];
   m_v[19] = m_v[13];
   m_v[20] = m_v[12];

   m_v[21] = m_v[17];
   m_v[22] = m_v[13];
   m_v[23] = m_v[14];
}

CVegetation::~CVegetation()
{

}

HRESULT CVegetation::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
   // Create the tree textures
   if( FAILED( D3DUtil::CreateTexture( pd3dDevice, (char*)((const char*)m_TextureFile),
      m_pTexture ) ) )
      return D3DAPPERR_MEDIANOTFOUND;

   // Create a quad for rendering each tree
   if( FAILED( pd3dDevice->CreateVertexBuffer( NUMB_TREE_VERTEX*sizeof(TREEVERTEX),
      D3DUSAGE_WRITEONLY, D3DFVF_TREEVERTEX,
      D3DPOOL_MANAGED, &m_pVB , NULL) ) )
   {
      return E_FAIL;
   }

   // Copy tree mesh data into vertexbuffer
   TREEVERTEX* v;
   m_pVB->Lock( 0, 0, (void**)&v, 0 );
   WORD* pI = NULL;

   memcpy( v, m_v, NUMB_TREE_VERTEX*sizeof(TREEVERTEX) );

   m_pVB->Unlock();

   return S_OK;
}

void CVegetation::DeleteDeviceObjects()
{
   D3DUtil::ReleaseTexture( m_pTexture );
   m_pVB = NULL;
}

HRESULT CVegetation::Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   if(pClipper == NULL || pClipper->MeshFOVCheck(GetPos(),m_Radius))
   {
      D3DMATERIAL9 mtrl;
      D3DUtil::InitMaterial( mtrl, 128.0f, 128.0f, 128.0f);
      pd3dDevice->SetMaterial( &mtrl );

      pd3dDevice->SetTexture( 0, m_pTexture );
      pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TREEVERTEX) );
      pd3dDevice->SetVertexShader( NULL );
      pd3dDevice->SetFVF( D3DFVF_TREEVERTEX );
      pd3dDevice->SetTransform( D3DTS_WORLD, &GetMatrix() );
      pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST , 0, m_nTri );

   }
   return S_OK;
}

HRESULT CVegetation::RenderSimple(LPDIRECT3DDEVICE9 pd3dDevice)
{
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      FALSE );

   pd3dDevice->SetTexture( 0, m_pTexture );
   pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TREEVERTEX) );
   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_TREEVERTEX );
   pd3dDevice->SetTransform( D3DTS_WORLD, &GetMatrix() );
   pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST , 0, m_nTri );

   pd3dDevice->SetTexture( 0, NULL );

   pd3dDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
   return S_OK;
}

void CVegetation::BuildShadow(const D3DXVECTOR3& lightpos)
{

}






