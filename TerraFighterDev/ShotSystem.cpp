// ShotSystem.cpp: implementation of the CShotSystem class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ShotSystem.h"
#include "dxutil.h"
#include "terrafighterApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShotSystem::CShotSystem() : CParticleSystem( 512, 2048, 0.03f,10.0f,FALSE)
{
   m_pExplosions = new CParticleSystem( 512, 2048, 0.05f, 1.0f,TRUE);
   m_SoundExpl = NULL;
   CBigExplosion::Init();

   m_Animation.Create();
   m_Animation.AddFrame("Explosion01.bmp", 0.07f);
   m_Animation.AddFrame("Explosion02.bmp", 0.07f);
   m_Animation.AddFrame("Explosion03.bmp", 0.07f);
   m_Animation.AddFrame("Explosion04.bmp", 0.07f);
   m_Animation.AddFrame("Explosion05.bmp", 0.07f);
   m_Animation.AddFrame("Explosion06.bmp", 0.07f);
   m_Animation.AddFrame("Explosion07.bmp", 0.07f);
   m_Animation.AddFrame("Explosion08.bmp", 0.07f);


}

CShotSystem::~CShotSystem()
{
   std::list<CBigExplosion*>::iterator pos = m_pBigExplosions.begin();
   while( pos != m_pBigExplosions.end() )
   {
      CBigExplosion* pObj = (*pos);
      SAFE_DELETE(pObj);
      pos++;
   }
   SAFE_DELETE( m_pExplosions );
   SAFE_DELETE( m_SoundExpl);
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CShotSystem::RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
{
   std::list<CBigExplosion*>::iterator pos = m_pBigExplosions.begin();
   while( pos != m_pBigExplosions.end() )
   {
      CBigExplosion* pObj = (*pos);
      if(pObj)
      {
         pObj->RestoreDeviceObjects(pd3dDevice);
      }
      pos++;
   }

   m_Animation.RestoreDeviceObjects(pd3dDevice);

   m_pExplosions->RestoreDeviceObjects(pd3dDevice);
   return CParticleSystem::RestoreDeviceObjects(pd3dDevice);
}


//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CShotSystem::Update(FLOAT fSecsPerFrame,CTerrain* pTerrain, CWater* pWater)
{
   std::list<CBigExplosion*>::iterator pos = m_pBigExplosions.begin();
   while( pos != m_pBigExplosions.end() )
   {
      CBigExplosion* pObj = (*pos);
      if(pObj)
      {
         pObj->Update(fSecsPerFrame,pTerrain,pWater);
         if(pObj->GetParticles() == NULL)
         {
            m_pBigExplosions.remove(pObj);
            pObj->DeleteDeviceObjects();
            delete pObj;
            break;
         }
      }
      pos++;
   }
   m_pExplosions->Update(fSecsPerFrame,pTerrain,pWater);
   return CParticleSystem::Update(fSecsPerFrame,pTerrain,pWater); 
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Renders the particle system using either pointsprites (if supported)
//       or using 4 vertices per particle
//-----------------------------------------------------------------------------
HRESULT CShotSystem::Render( LPDIRECT3DDEVICE9 pd3dDevice , CFOVClipper* pClipper,CD3DCamera * pCamera)
{
   std::list<CBigExplosion*>::iterator pos = m_pBigExplosions.begin();
   while( pos != m_pBigExplosions.end() )
   {
      CBigExplosion* pObj = (*pos);
      if(pObj)
      {
         pObj->Render(pd3dDevice,pClipper, TRUE,pCamera);
      }
      pos++;
   }
   m_pExplosions->Render(pd3dDevice,pCamera,pClipper, FALSE);
   return CParticleSystem::Render( pd3dDevice ,pCamera, pClipper,FALSE);
}

HRESULT CShotSystem::DeleteDeviceObjects()
{
   std::list<CBigExplosion*>::iterator pos = m_pBigExplosions.begin();
   while( pos != m_pBigExplosions.end() )
   {
      CBigExplosion* pObj = (*pos);
      if(pObj)
      {
         pObj->DeleteDeviceObjects();
      }
      pos++;
   }
   m_Animation.DeleteDeviceObjects();
   m_pExplosions->DeleteDeviceObjects();
   return CParticleSystem::DeleteDeviceObjects();
}

HRESULT CShotSystem::Create(CSoundManager* pSoundManager)
{
   SAFE_DELETE(m_SoundExpl);

   //m_pBigExplosions->Create(pd3dDevice,"Particle.jpg");
   m_pExplosions->Create("Particle.jpg");
   // Load the wave file into a DirectSound buffer

   pSoundManager->Create( &m_SoundExpl, "p_bang.wav",&GetApp()->m_Zip, DSBCAPS_CTRL3D, DS3DALG_NO_VIRTUALIZATION,5);  


   return CParticleSystem::Create("flare2.JPG");
}

void CShotSystem::Hit(CParticle *pPartical)
{
   m_pExplosions->EmitNewParticles(1,D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,255,0,0),pPartical->m_vPos,D3DXVECTOR3(0.0f,0.0f,0.0f),0.0f,1.0f);
   if(m_SoundExpl)
      m_SoundExpl->Play3D(DS3DMODE_NORMAL,D3DXVECTOR3(0,0,0),pPartical->m_vPos);
}

void CShotSystem::EmitBigExplosion(D3DXVECTOR3 vPosition,LPDIRECT3DDEVICE9 pd3dDevice)
{

   CBigExplosion* pObj = new CBigExplosion();
   pObj->Create(vPosition);
   pObj->RestoreDeviceObjects(pd3dDevice);
   m_pBigExplosions.push_back(pObj);

   if(m_SoundExpl)
      m_SoundExpl->Play3D(DS3DMODE_NORMAL,D3DXVECTOR3(0,0,0),vPosition);
}

HRESULT CShotSystem::EmitNewParticles(DWORD dwNumParticlesToEmit,
                                      const D3DXCOLOR &dwEmitColor, 
                                      const D3DXCOLOR &dwFadeColor,
                                      D3DXVECTOR3 vPosition,
                                      D3DXVECTOR3 vEmitVel,
                                      CSprite* pOwner,
                                      FLOAT fPower,
                                      FLOAT fGravity,
                                      FLOAT fSize)
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
         if( NULL == ( pParticle = new CShot ) )
            return E_OUTOFMEMORY;
      }

      pParticle->m_pNext = m_pParticles;
      m_pParticles = pParticle;
      m_dwParticles++;

      // Emit new particle
      FLOAT fRand1 = ((FLOAT)rand()/(FLOAT)RAND_MAX) * D3DX_PI * 2.0f;
      FLOAT fRand2 = ((FLOAT)rand()/(FLOAT)RAND_MAX) * D3DX_PI * 0.25f;

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
      static_cast<CShot*>(pParticle)->m_pOwner = pOwner;
      static_cast<CShot*>(pParticle)->m_fPower = fPower;
      pParticle->m_fGravity = fGravity;
      pParticle->m_fSize	  =	fSize;
   }

   return S_OK;
}
