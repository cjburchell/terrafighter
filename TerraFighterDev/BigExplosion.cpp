// BigExplosion.cpp: implementation of the CBigExplosion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BigExplosion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define EXPLOSION_SPEED 8.0f
#define EXPLOSION_SPEED2 12.0f

D3DXVECTOR3 CBigExplosion::m_vExplosionVel[TOTAL_EXPLOSION_PARTICLES];

CBigExplosion::CBigExplosion() : CParticleSystem( 512, 2048, 0.03f, 1.5f,TRUE)
{

}

CBigExplosion::~CBigExplosion()
{

}

void CBigExplosion::Init()
{
   for(int i=0;i<EXPLOSION_PARTICLES;i++)
   {
      D3DXMATRIX matDirect, matRotate, matRotate2;

      D3DXMatrixRotationZ( &matRotate2, PI2 * (FLOAT)rand()/(FLOAT)RAND_MAX );
      D3DXMatrixRotationY( &matRotate, PI2 * (FLOAT)rand()/(FLOAT)RAND_MAX );
      D3DXMatrixMultiply( &matRotate, &matRotate, &matRotate2 );
      D3DXMatrixTranslation( &matDirect,EXPLOSION_SPEED,0.0f, 0.0f );
      D3DXMatrixMultiply( &matDirect, &matDirect, &matRotate );
      D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);
      D3DXVECTOR3 vVelocity;
      D3DXVec3TransformCoord( &vVelocity,&orgin,&matDirect);
      m_vExplosionVel[i] = vVelocity; 
   }

   for(int j=0;j<EXPLOSION_PARTICLES2;j++)
   {
      D3DXMATRIX matDirect, matRotate;
      D3DXMatrixRotationY( &matRotate, PI2 * (FLOAT)rand()/(FLOAT)RAND_MAX );
      D3DXMatrixTranslation( &matDirect,EXPLOSION_SPEED2,0.0f, 0.0f );
      D3DXMatrixMultiply( &matDirect, &matDirect, &matRotate );
      D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);
      D3DXVECTOR3 vVelocity;
      D3DXVec3TransformCoord( &vVelocity,&orgin,&matDirect);
      m_vExplosionVel[i+j] = vVelocity; 
   }
}

float RandomNumber(float fMin, float fMax)
{
   if (fMin == fMax) return(fMin);
   float fRandom = (float)rand() / (float)RAND_MAX;
   return((fRandom * (float)fabs(fMax-fMin))+fMin);
}

inline D3DXVECTOR3 RandomNumber(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
{
   float x = RandomNumber(vMin.x, vMax.x);
   float y = RandomNumber(vMin.y, vMax.y);
   float z = RandomNumber(vMin.z, vMax.z);
   return(D3DXVECTOR3(x,y,z));
}

HRESULT CBigExplosion::Create(D3DXVECTOR3 vPosition)
{
   m_vPos = vPosition;
   CParticleSystem::Create("Particle.jpg");
   for(int i=0;i<EXPLOSION_PARTICLES;i++)
   {
      EmitNewParticles(1,D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,255,0,0),vPosition,m_vExplosionVel[i],-9.8f,0.15f);
   }

   m_Animation.Create();
   m_Animation.AddFrame("Explosion01.bmp", 0.07f);
   m_Animation.AddFrame("Explosion02.bmp", 0.07f);
   m_Animation.AddFrame("Explosion03.bmp", 0.07f);
   m_Animation.AddFrame("Explosion04.bmp", 0.07f);
   m_Animation.AddFrame("Explosion05.bmp", 0.07f);
   m_Animation.AddFrame("Explosion06.bmp", 0.07f);
   m_Animation.AddFrame("Explosion07.bmp", 0.07f);
   m_Animation.AddFrame("Explosion08.bmp", 0.07f);


   m_Explosion[0].m_vPos = m_vPos;
   m_Explosion[0].m_fSize = 5.0f;
   m_Explosion[0].m_TimeOffset = 0.0f;

   for(i=1;i<MAX_EXPLOSION;i++)
   {
      m_Explosion[i].m_vPos = m_vPos + 
         RandomNumber(
         D3DXVECTOR3(-2.5f, -2.5f, 0.0f),
         D3DXVECTOR3(2.5f, 2.5f, 0.0f));

      m_Explosion[i].m_fSize = 2.0f + RandomNumber(-1.5f, 2.5f);

      m_Explosion[i].m_TimeOffset = RandomNumber(-0.14f, 0.14f);
   }

   return S_OK;
}

HRESULT CBigExplosion::Render( LPDIRECT3DDEVICE9 pd3dDevice , CFOVClipper* pClipper,BOOL bZWrite,CD3DCamera * pCamera)
{
   if(pClipper->MeshFOVCheck(m_vPos,10.0f))
   {
      for(int i=0;i<MAX_EXPLOSION;i++)
      {
         m_Animation.Render(pd3dDevice,pCamera,m_Explosion[i].m_vPos,m_Explosion[i].m_fSize,m_Explosion[i].m_TimeOffset);
      }
      return CParticleSystem::Render(pd3dDevice,pCamera,NULL,bZWrite);
   }
   return S_OK;
}

HRESULT CBigExplosion::Update(FLOAT fSecsPerFrame,CTerrain* pTerrain, CWater* pWater)
{
   m_Animation.Update(fSecsPerFrame);
   return CParticleSystem::Update(fSecsPerFrame,pTerrain,pWater); 
}


HRESULT CBigExplosion::RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
{
   m_Animation.RestoreDeviceObjects(pd3dDevice);
   return CParticleSystem::RestoreDeviceObjects(pd3dDevice); 
}
