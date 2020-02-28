// ShotSystem.h: interface for the CShotSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOTSYSTEM_H__C98A0CCE_6381_4C9E_B1D7_4BF0E173EFA1__INCLUDED_)
#define AFX_SHOTSYSTEM_H__C98A0CCE_6381_4C9E_B1D7_4BF0E173EFA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParticleSystem.h"
#include "BigExplosion.h"
#include <mmsystem.h>
#include <dsound.h>
#include "dsutil.h"
#include "Sprite.h"

struct CShot : public CParticle
{
   CSprite* m_pOwner;
   FLOAT m_fPower;
};

class CShotSystem : public CParticleSystem  
{
public:
   CShotSystem();
   virtual ~CShotSystem();
   virtual void Hit(CParticle *pPartical);
   virtual HRESULT Create(CSoundManager* pSoundManager);
   virtual HRESULT DeleteDeviceObjects();

   virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );

   virtual HRESULT Update( FLOAT fSecsPerFrame, CTerrain* pTerrain, CWater* pWater);

   virtual HRESULT Render( LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper,CD3DCamera * pCamera );
   virtual void EmitBigExplosion(D3DXVECTOR3 vPosition,LPDIRECT3DDEVICE9 pd3dDevice);

   HRESULT EmitNewParticles(DWORD dwNumParticlesToEmit,
      const D3DXCOLOR &dwEmitColor, 
      const D3DXCOLOR &dwFadeColor,
      D3DXVECTOR3 vPosition,
      D3DXVECTOR3 vEmitVel,
      CSprite* pOwner,
      FLOAT fPower,
      FLOAT fGravity,
      FLOAT fSize);

private:


   CParticleSystem* m_pExplosions;

   std::list <CBigExplosion*> m_pBigExplosions;

   CSound* m_SoundExpl;

   CAnimation m_Animation; // holds the animation sequence used for cashing

};

#endif // !defined(AFX_SHOTSYSTEM_H__C98A0CCE_6381_4C9E_B1D7_4BF0E173EFA1__INCLUDED_)
