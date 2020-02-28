// BigExplosion.h: interface for the CBigExplosion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIGEXPLOSION_H__29A98413_8D3A_4614_8968_FFA4614695BC__INCLUDED_)
#define AFX_BIGEXPLOSION_H__29A98413_8D3A_4614_8968_FFA4614695BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParticleSystem.h"
#include "Animation.h"

#define EXPLOSION_PARTICLES 100
#define EXPLOSION_PARTICLES2 0
#define TOTAL_EXPLOSION_PARTICLES EXPLOSION_PARTICLES+EXPLOSION_PARTICLES2

#define MAX_EXPLOSION 10

class CExplosion
{
public:
   D3DXVECTOR3 m_vPos;
   FLOAT m_fSize;
   FLOAT m_TimeOffset;
};

class CBigExplosion : public CParticleSystem  
{
public:
   virtual HRESULT Create(D3DXVECTOR3 vPosition);
   CBigExplosion();
   virtual ~CBigExplosion();
   static void Init();
   virtual HRESULT Render( LPDIRECT3DDEVICE9 pd3dDevice , CFOVClipper* pClipper,BOOL bZWrite,CD3DCamera * pCamera);
   virtual HRESULT Update(FLOAT fSecsPerFrame,CTerrain* pTerrain, CWater* pWater);
   HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );

private:
   static D3DXVECTOR3 m_vExplosionVel[TOTAL_EXPLOSION_PARTICLES];
   D3DXVECTOR3 m_vPos;
   CAnimation m_Animation;
   CExplosion m_Explosion[MAX_EXPLOSION];

};

#endif // !defined(AFX_BIGEXPLOSION_H__29A98413_8D3A_4614_8968_FFA4614695BC__INCLUDED_)
