// LensFlare.cpp: implementation of the CLensFlare class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "LensFlare.h"
#include "D3DApp.h"
#include "DXUtil.h"
#include "TerraFighterApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLensFlare::CLensFlare()
{
   // Initialize the array of lens flares
   //            type   loc   scale  red  green  blue
   m_Flare[0].Set(  2,  1.30f, 0.06f, 0.6f, 0.3f, 0.15f );
   m_Flare[1].Set(  3,  1.00f, 0.15f, 0.4f, 0.2f, 0.1f );
   m_Flare[2].Set(  1,  0.50f, 0.20f, 0.3f, 0.15f, 0.075f );
   m_Flare[3].Set(  3,  0.20f, 0.07f, 0.3f, 0.15f, 0.075f );
   m_Flare[4].Set(  0,  0.00f, 0.06f, 0.3f, 0.15f, 0.075f );
   m_Flare[5].Set(  4, -0.25f, 0.03f, 0.6f, 0.3f, 0.15f );
   m_Flare[6].Set(  4, -0.40f, 0.06f, 0.5f, 0.25f, 0.125f );
   m_Flare[7].Set(  4, -0.60f, 0.04f, 0.2f, 0.1f, 0.05f );

   m_Flare[8].Set(  5, 1.00f, 0.10f, 1.0f, 1.0f, 1.0f );

   for( DWORD f=0; f<FLARE_TEXTURES; f++ )
      m_pFlareTextures[f] = NULL;
   m_pVB = NULL;
}

CLensFlare::~CLensFlare()
{

}

//-----------------------------------------------------------------------------
// Name: RenderFlares()
// Desc: Draws the set of flares
//-----------------------------------------------------------------------------
HRESULT CLensFlare::Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{

   if(!m_bDisplay)
      return S_OK;

   if(!pClipper->MeshFOVCheck(m_vLightPt,1.0f,FALSE))
      return S_OK;
   D3DMATERIAL9 mtrl;
   D3DUtil::InitMaterial( mtrl );

   // Turn on alpha blending for the flares

   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
   pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
   //pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
   //	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
   //	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );


   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
   pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_ONE );
   pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_ONE );
   pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(D3DVERTEX) );
   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_D3DVERTEX );

   // Do the flares
   for( DWORD i=0; i<NUM_FLARES_WITH_SUN; i++ )
   {
      // Set up the emissive color of the flare
      mtrl.Emissive.r = m_Flare[i].m_r;
      mtrl.Emissive.g = m_Flare[i].m_g;
      mtrl.Emissive.b = m_Flare[i].m_b;
      pd3dDevice->SetMaterial( &mtrl );

      pd3dDevice->SetTexture( 0, m_pFlareTextures[m_Flare[i].m_wType] );

      // Translate the world matrix to the flare position
      D3DXMATRIX matWorld;// = m_matBillboardMatrix;

      D3DXMatrixScaling(&matWorld,m_Flare[i].m_fRenderSize,m_Flare[i].m_fRenderSize,1.0f);
      D3DXMatrixMultiply(&matWorld,&matWorld,&m_matBillboardMatrix);

      matWorld._41 = m_Flare[i].m_vPositionPt.x;
      matWorld._42 = m_Flare[i].m_vPositionPt.y;
      matWorld._43 = m_Flare[i].m_vPositionPt.z;

      // Set the new world transform and render the flare
      pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

      pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
   }


   //D3DXMATRIX  matWorld;
   //D3DXMatrixIdentity( &matWorld );
   //pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
   pd3dDevice->SetTexture( 0, NULL);
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
   //	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
   //	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        TRUE );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
   return S_OK;
}
//D3DXVECTOR3 g_vDirFlare;

//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CLensFlare::Update(D3DXVECTOR3 vLightPt,CD3DCamera* pCamera,CTerrain* pTerrain)
{
   // Compute the vectors between the from, lookat, and light positions. 
   m_vLightPt = vLightPt;
   m_matBillboardMatrix  = pCamera->GetBillboardMatrix();
   D3DXVECTOR3 vLookatPt = pCamera->GetLookatPt();
   D3DXVECTOR3 vFromPt   = pCamera->GetEyePt();
   D3DXVECTOR3 vViewVec  = vLookatPt - vFromPt;
   D3DXVec3Normalize(&vViewVec,&vViewVec);
   D3DXVECTOR3 vLightVec = vLightPt - vFromPt;
   D3DXVec3Normalize(&vLightVec,&vLightVec );

   if(pTerrain->Intersect(vFromPt,vLightPt))
      m_bDisplay = FALSE;
   else
      m_bDisplay = TRUE;


   // Compute the vector and center point for the lens flare axis
   FLOAT     fDotProduct = D3DXVec3Dot( &vLightVec, &vViewVec );
   D3DXVECTOR3 vNewLightPt = vFromPt + 1.0f/fDotProduct * vLightVec * 1.01f;
   D3DXVECTOR3 vCenterPt   = vFromPt + vViewVec*1.01f;
   D3DXVECTOR3 vAxisVec    = vNewLightPt - vCenterPt;

   // Compute the offset of the lens flare along the flare axis

   D3DXVECTOR3 dx;
   D3DXVec3Normalize(&dx,&vAxisVec);
   FLOAT     fDeltaX    = ( dx.x - dx.y );
   FLOAT     fDeltaY    = ( dx.y + dx.x );
   FLOAT     fViewScale = (FLOAT)sqrt( fDeltaX*fDeltaX + fDeltaY*fDeltaY );

   // Store the lens flares positions for each flare
   for( DWORD i=0; i<NUM_FLARES_WITH_SUN; i++ )
   {
      // Store the position of the flare along the axis
      m_Flare[i].m_vPositionPt = vCenterPt + vAxisVec * m_Flare[i].m_fLoc;

      // Store the render size of the flare. This is the lens flare size
      // corrected for the orientation of the flaring axis.
      m_Flare[i].m_fRenderSize = fViewScale * m_Flare[i].m_fScale;

   }
   m_Flare[NUM_FLARES].m_fRenderSize = 0.5f;

   //g_vDirFlare = pCamera->GetViewDir();
   //qsort( m_Flare, NUM_FLARES, sizeof(CFlare), CLensFlare::SortCB );

   return S_OK;
}

/*int CLensFlare::SortCB( const VOID* arg1, const VOID* arg2 )
{
CFlare* p1 = (CFlare*)arg1;
CFlare* p2 = (CFlare*)arg2;

FLOAT d1 = p1->m_vPositionPt.x * g_vDirFlare.x + p1->m_vPositionPt.z * g_vDirFlare.z;
FLOAT d2 = p2->m_vPositionPt.x * g_vDirFlare.x + p2->m_vPositionPt.z * g_vDirFlare.z;
if (d1 < d2)
return +1;

return -1;
}*/


//-----------------------------------------------------------------------------
// Name: SetFlare()
// Desc: Helper function to initialize a flare
//-----------------------------------------------------------------------------
void CFlare::Set( INT wType, FLOAT fLocation, FLOAT fScale, FLOAT fRed,
                 FLOAT fGreen, FLOAT fBlue )
{
   m_wType  = wType;
   m_fLoc   = fLocation;
   m_fScale = fScale;
   m_r      = fRed;
   m_g      = fGreen;
   m_b      = fBlue;
}

HRESULT CLensFlare::Create(LPDIRECT3DDEVICE9 pd3dDevice)
{
   CHAR strTexName[40];
   for( int i=0; i<FLARE_TEXTURES; i++ )
   {
      sprintf( strTexName, "flare%d.jpg", i );
      if( FAILED( D3DUtil::CreateTexture( pd3dDevice, strTexName,
         m_pFlareTextures[i] ) ) )
         return D3DAPPERR_MEDIANOTFOUND;

   }

   // Create a quad for rendering each tree
   if( FAILED( pd3dDevice->CreateVertexBuffer(   4*sizeof(D3DVERTEX),
      D3DUSAGE_WRITEONLY, D3DFVF_D3DVERTEX,
      D3DPOOL_MANAGED, &m_pVB, NULL ) ) )
   {
      return E_FAIL;
   }

   // Copy tree mesh data into vertexbuffer
   D3DVERTEX* v;
   m_pVB->Lock( 0, 0, (void**)&v, 0 );

   // Initializes vertices used to render the flares
   D3DXVECTOR3 vNorm = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
   v[0].p = D3DXVECTOR3(-1.0f,-1.0f, 0.0f );
   v[0].n = vNorm;
   v[0].tu = 0.0f;
   v[0].tv = 1.0f;
   v[1].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f );
   v[1].n = vNorm;
   v[1].tu = 0.0f;
   v[1].tv = 0.0f;
   v[2].p = D3DXVECTOR3( 1.0f,-1.0f, 0.0f );
   v[2].n = vNorm;
   v[2].tu = 1.0f;
   v[2].tv = 1.0f;
   v[3].p = D3DXVECTOR3( 1.0f, 1.0f, 0.0f );
   v[3].n = vNorm;
   v[3].tu = 1.0f;
   v[3].tv = 0.0f;

   m_pVB->Unlock();
   return S_OK;
}

HRESULT CLensFlare::DeleteDeviceObjects()
{
   for(int i=0; i<FLARE_TEXTURES; i++ )
   {
      D3DUtil::ReleaseTexture( m_pFlareTextures[i] );
      m_pFlareTextures[i] = NULL;
   }

   m_pVB = NULL;
   return S_OK;
}
