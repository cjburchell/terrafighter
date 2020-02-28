// ParticleSystem.cpp: implementation of the CParticleSystem class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ParticleSystem.h"
#include "DXUtil.h"
#include "TerraFighterApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
CParticleSystem::CParticleSystem( DWORD dwFlush, DWORD dwDiscard, float fRadius , FLOAT fFadeTime,BOOL bOneSize)
{
   m_fRadius        = fRadius;

   //m_dwBase         = dwDiscard;
   //m_dwFlush        = dwFlush;
   //m_dwDiscard      = dwDiscard;
   m_fFadeTime = fFadeTime;

   m_dwParticles    = 0;
   m_dwParticlesLim = 2048;

   m_pParticles     = NULL;
   m_pParticlesFree = NULL;
   m_pParticlesTemp = NULL;
   //m_pVB            = NULL;
   m_fTime = 0.0f;
   m_pTexture     = NULL;
   m_bOneSize = bOneSize;

   m_bAlpha = FALSE;
   m_bZEnable = TRUE;

}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
CParticleSystem::~CParticleSystem()
{
   DeleteDeviceObjects();

   while( m_pParticles )
   {
      CParticle* pSpark = m_pParticles;
      m_pParticles = pSpark->m_pNext;
      delete pSpark;
   }
   m_pParticles = NULL;

   while( m_pParticlesFree )
   {
      CParticle *pSpark = m_pParticlesFree;
      m_pParticlesFree = pSpark->m_pNext;
      delete pSpark;
   }
   m_pParticlesFree = NULL;

   while( m_pParticlesTemp )
   {
      CParticle *pSpark = m_pParticlesTemp;
      m_pParticlesTemp = pSpark->m_pNext;
      delete pSpark;
   }

   m_pParticlesTemp = NULL;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CParticleSystem::RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
{

   if( FAILED( D3DUtil::CreateTexture( pd3dDevice, (char*)((LPCSTR)m_TextrueName),
      m_pTexture ) ) )
      return E_FAIL;

   return S_OK;
}



//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CParticleSystem::Update(FLOAT fSecsPerFrame,CTerrain* pTerrain, CWater* pWater)
{
   CParticle *pParticle, **ppParticle;
   m_fTime += fSecsPerFrame;

   ppParticle = &m_pParticles;

   while( *ppParticle )
   {
      pParticle = *ppParticle;

      // Calculate new position
      float fT = m_fTime - pParticle->m_fTime0;

      pParticle->m_fFade -= (fSecsPerFrame/m_fFadeTime) * 1.0f;

      pParticle->m_vLastPos = pParticle->m_vPos;
      pParticle->m_vPos    = pParticle->m_vVel0 * fT + pParticle->m_vPos0;
      pParticle->m_vPos.y += (0.5f * pParticle->m_fGravity) * (fT * fT);
      pParticle->m_vVel.y  = pParticle->m_vVel0.y + pParticle->m_fGravity * fT;

      FLOAT TerrainY = pTerrain->GetHightAtPosR(pParticle->m_vPos.x,pParticle->m_vPos.z);
      if( TerrainY> pParticle->m_vPos.y)
      {
         pParticle->m_fFade = 0.0f;
         pParticle->m_bHit = TRUE;
         pParticle->m_vPos = pParticle->m_vLastPos; 
      }
      else if(pTerrain->GetWaterLevel() > pParticle->m_vPos.y)
      {
         pParticle->m_fFade = 0.0f;
         pParticle->m_bHit = TRUE;
         pParticle->m_vPos = pParticle->m_vLastPos; 
      }

      if( pParticle->m_fFade < 0.0f )
         pParticle->m_fFade = 0.0f;

      // Kill old particles
      if( pParticle->m_vPos.y < m_fRadius || pParticle->m_fFade <= 0.0f )
      {
         if(pParticle->m_bHit)
         {
            Hit(pParticle);
         }
         // Kill particle
         *ppParticle = pParticle->m_pNext;
         pParticle->m_pNext = m_pParticlesFree;
         m_pParticlesFree = pParticle;
         m_dwParticles--;
      }
      else
      {
         ppParticle = &pParticle->m_pNext;
      }
   }
   return S_OK;
}


HRESULT CParticleSystem::EmitNewParticles(DWORD dwNumParticlesToEmit,
                                          const D3DXCOLOR &dwEmitColor, 
                                          const D3DXCOLOR &dwFadeColor,
                                          D3DXVECTOR3 vPosition,
                                          D3DXVECTOR3 vEmitVel,
                                          FLOAT	fGravity,
                                          FLOAT	fSize)
{
   // Emit new particles
   DWORD dwParticlesEmit = m_dwParticles + dwNumParticlesToEmit;
   while( m_dwParticles < m_dwParticlesLim && m_dwParticles < dwParticlesEmit )
   {
      CParticle *pParticle;
      if( m_pParticlesFree )
      {
         pParticle = m_pParticlesFree;
         m_pParticlesFree = pParticle->m_pNext;
      }
      else
      {
         if( NULL == ( pParticle = new CParticle ) )
            return E_OUTOFMEMORY;
      }

      pParticle->m_pNext = m_pParticles;
      m_pParticles = pParticle;
      m_dwParticles++;

      // Emit new particle
      //FLOAT fRand1 = ((FLOAT)rand()/(FLOAT)RAND_MAX) * D3DX_PI * 2.0f;
      //FLOAT fRand2 = ((FLOAT)rand()/(FLOAT)RAND_MAX) * D3DX_PI * 0.25f;

      pParticle->m_vPos0 = vPosition + D3DXVECTOR3( 0.0f, m_fRadius, 0.0f );

      pParticle->m_vVel0 = vEmitVel;

      pParticle->m_vPos = pParticle->m_vPos0;
      pParticle->m_vVel = pParticle->m_vVel0;

      pParticle->m_clrDiffuse = dwEmitColor;
      pParticle->m_clrFade    = dwFadeColor;
      pParticle->m_fFade      = 1.0f;
      pParticle->m_fTime0     = m_fTime;
      pParticle->m_bHit       = FALSE;
      pParticle->m_vLastPos = pParticle->m_vPos;
      pParticle->m_fGravity = fGravity;
      pParticle->m_fSize	  =	fSize;
   }

   return S_OK;
}


bool Compare(CParticle* pParta,CParticle* pPartb)
{
   return pParta->m_Distance > pPartb->m_Distance;
}

struct ParticleRenderData
{
   BOOL m_bOneSize;
   //POINTVERTEX* m_pVertices;
   LPDIRECT3DDEVICE9 m_pd3dDevice;
   DWORD m_dwNumParticlesToRender;

   //DWORD m_dwFlush;
   //DWORD m_dwBase;
   //DWORD m_dwDiscard;
   //LPDIRECT3DVERTEXBUFFER8 m_pVB;
};

BOOL AFXAPI CParticleSystem::ProcessElement( void* pvObj, void* pData )
{
   ParticleRenderData* pPartData = (ParticleRenderData*)pData;
   CParticle* pParticle  = (CParticle*)pvObj;

   D3DXVECTOR3 vPos(pParticle->m_vPos);
   D3DXVECTOR3 vVel(pParticle->m_vVel);
   /*FLOAT       fLengthSq = D3DXVec3LengthSq(&vVel);
   UINT        dwSteps;

   if( fLengthSq < 1.0f )        dwSteps = 2;
   else if( fLengthSq <  4.00f ) dwSteps = 3;
   else if( fLengthSq <  9.00f ) dwSteps = 4;
   else if( fLengthSq < 12.25f ) dwSteps = 5;
   else if( fLengthSq < 16.00f ) dwSteps = 6;
   else if( fLengthSq < 20.25f ) dwSteps = 7;
   else                          dwSteps = 8;

   vVel *= -0.015f / (FLOAT)dwSteps;*/

   D3DXCOLOR clrDiffuse;
   D3DXColorLerp(&clrDiffuse, &pParticle->m_clrFade, &pParticle->m_clrDiffuse, pParticle->m_fFade);		
   DWORD dwDiffuse= (DWORD) clrDiffuse;

   pPartData->m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(pParticle->m_fSize) );

   // Render each particle a bunch of times to get a blurring effect

   //for( DWORD i = 0; i < dwSteps; i++ )
   {
      POINTVERTEX Point;
      Point.v     = vPos;
      //pVertices->size  = pParticle->m_fSize;
      Point.color = dwDiffuse;

      if(FAILED(pPartData->m_pd3dDevice->DrawPrimitiveUP( D3DPT_POINTLIST, 1, &Point,sizeof(POINTVERTEX))))
         return FALSE;

      vPos += vVel;
   }
   return TRUE;
}



//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Renders the particle system using either pointsprites (if supported)
//       or using 4 vertices per particle
//-----------------------------------------------------------------------------
HRESULT CParticleSystem::Render( LPDIRECT3DDEVICE9 pd3dDevice ,CD3DCamera* pCamera, CFOVClipper* pClipper,BOOL bZWrite)
{

   D3DMATERIAL9 mtrl;
   D3DUtil::InitMaterial( mtrl );

   D3DXMATRIX matWorld;
   D3DXMatrixIdentity( &matWorld );
   pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld );


   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
   pd3dDevice->SetRenderState( D3DRS_ZENABLE, m_bZEnable );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, m_bZEnable);

   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
   pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_ONE );
   pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_ONE );

   pd3dDevice->SetTexture(0, m_pTexture);//m_pTexture );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

   //HRESULT hr;

   // Set the render states for using point sprites
   pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );
   pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(0.00f) );
   pd3dDevice->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.00f) );
   pd3dDevice->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.00f) );
   pd3dDevice->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.00f) );


   // Set up the vertex buffer to be rendered
   //pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(POINTVERTEX) );
   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_POINTVERTEX );

   CParticle*    pParticle = m_pParticles;

   CParticle*    pTempParticle = m_pParticlesTemp;
   ParticleRenderData RenderData;

   // Render each particle
   if(pParticle)
      pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(pParticle->m_fSize) );

   m_DisplayList.Clear();

   while( pParticle )
   {
      D3DXVECTOR3 vPos(pParticle->m_vPos);

      BOOL vis = TRUE;
      if(pClipper)
         vis = pClipper->MeshFOVCheck(vPos,0.1f);

      if(vis)
      {
         D3DXVECTOR3 vdist = pCamera->GetEyePt() - vPos;
         FLOAT d = D3DXVec3LengthSq(&vdist);
         pParticle->m_Distance = d;
         m_DisplayList.Add(pParticle);

         D3DXVECTOR3 vVel(pParticle->m_vVel);
         FLOAT       fLengthSq = D3DXVec3LengthSq(&vVel);
         UINT        dwSteps;

         if( fLengthSq < 1.0f )        dwSteps = 2;
         else if( fLengthSq <  4.00f ) dwSteps = 3;
         else if( fLengthSq <  9.00f ) dwSteps = 4;
         else if( fLengthSq < 12.25f ) dwSteps = 5;
         else if( fLengthSq < 16.00f ) dwSteps = 6;
         else if( fLengthSq < 20.25f ) dwSteps = 7;
         else                          dwSteps = 8;

         vVel *= -0.015f / (FLOAT)dwSteps;
         vPos += vVel;

         for( DWORD i = 1; i < dwSteps; i++ )
         {
            vdist = pCamera->GetEyePt() - vPos;
            d = D3DXVec3LengthSq(&vdist);

            CParticle* pNewTempParticle = NULL;
            // get a temp partical
            if(pTempParticle)
            {
               CParticle* pNext = pTempParticle->m_pNext;
               *pTempParticle = *pParticle;
               pNewTempParticle = pTempParticle;
               pTempParticle->m_pNext = pNext;
               pTempParticle = pNext;
            }
            else
            {
               // we ran out of particles add some more
               if( NULL == ( pNewTempParticle = new CParticle ) )
                  return E_OUTOFMEMORY;
               *pNewTempParticle = *pParticle;
               pNewTempParticle->m_pNext = m_pParticlesTemp;
               m_pParticlesTemp = pNewTempParticle;
            }


            if(pNewTempParticle)
            {
               pNewTempParticle->m_Distance = d;
               pNewTempParticle->m_vPos = vPos;
               m_DisplayList.Add(pNewTempParticle);
            }
            vPos += vVel;
         }
      }
      pParticle = pParticle->m_pNext;
   }


   RenderData.m_bOneSize = m_bOneSize;
   RenderData.m_pd3dDevice= pd3dDevice;
   RenderData.m_dwNumParticlesToRender = 0;

   m_DisplayList.ProcessElements(&RenderData,CParticleSystem::ProcessElement);

   // Reset render states
   pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
   pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );
   pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        TRUE );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

   return S_OK;
}

HRESULT CParticleSystem::DeleteDeviceObjects()
{
   D3DUtil::ReleaseTexture( m_pTexture );
   m_pTexture = NULL;
   return S_OK;
}

HRESULT CParticleSystem::Create(TCHAR* strTex,BOOL  bAlpha)
{
   m_bAlpha = bAlpha;
   m_TextrueName = strTex;
   return S_OK;
}

void CParticleSystem::Hit(CParticle *pPartical)
{

}
