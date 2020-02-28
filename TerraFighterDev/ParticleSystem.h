// ParticleSystem.h: interface for the CParticleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTICLESYSTEM_H__5E5E153B_F510_4B6F_9117_2B8421436A4E__INCLUDED_)
#define AFX_PARTICLESYSTEM_H__5E5E153B_F510_4B6F_9117_2B8421436A4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vertextypes.h"
#include "FOVClipper.h"
#include "Terrain.h"
#include "Water.h"
#include "SortedList.h"
//-----------------------------------------------------------------------------
// Global data for the particles
//-----------------------------------------------------------------------------
struct CParticle
{
   D3DXVECTOR3 m_vPos;       // Current position
   D3DXVECTOR3 m_vVel;       // Current velocity
   D3DXVECTOR3 m_vLastPos;

   D3DXVECTOR3 m_vPos0;      // Initial position
   D3DXVECTOR3 m_vVel0;      // Initial velocity
   FLOAT       m_fTime0;     // Time of creation

   D3DXCOLOR   m_clrDiffuse; // Initial diffuse color
   D3DXCOLOR   m_clrFade;    // Faded diffuse color
   FLOAT       m_fFade;      // Fade progression

   BOOL		m_bHit;

   CParticle*   m_pNext;      // Next particle in list
   FLOAT		 m_fGravity;
   FLOAT		 m_fSize;
   FLOAT		 m_Distance;
};


//enum PARTICLE_COLORS { COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_BLUE, NUM_COLORS };


/*const D3DXCOLOR g_clrColor[NUM_COLORS] =
{
D3DXCOLOR( 255.0f,   255.0f,   255.0f,   255.0f ),
D3DXCOLOR( 255.0f,   0.5f,   0.5f,   1.0f ),
D3DXCOLOR( 0.5f,   1.0f,   0.5f,   1.0f ),
D3DXCOLOR( 0.125f, 0.5f,   1.0f,   1.0f )
};


const DWORD g_clrColorFade[NUM_COLORS] =
{
D3DXCOLOR( 1.0f,   0.25f,   0.25f,   1.0f ),
D3DXCOLOR( 1.0f,   0.25f,   0.25f,   1.0f ),
D3DXCOLOR( 0.25f,  0.75f,   0.25f,   1.0f ),
D3DXCOLOR( 0.125f, 0.25f,   0.75f,   1.0f )
};*/

class CParticleSystem
{
protected:
   CSortedList<CParticle*> m_DisplayList;
   static BOOL AFXAPI ProcessElement( void* pvObj, void* pData );

   FLOAT     m_fRadius;
   BOOL	  m_bOneSize;
   BOOL	  m_bAlpha;

   //DWORD     m_dwBase;
   //DWORD     m_dwFlush;
   //DWORD     m_dwDiscard;

   DWORD     m_dwParticles;
   DWORD     m_dwParticlesLim;
   CParticle* m_pParticles;
   CParticle* m_pParticlesFree;

   CParticle* m_pParticlesTemp;
   FLOAT m_fFadeTime;

   // Geometry
   //LPDIRECT3DVERTEXBUFFER8 m_pVB;
   FLOAT m_fTime;
   CComPtr<IDirect3DTexture9> m_pTexture;

   BOOL m_bZEnable;

public:
   CString m_TextrueName;

   virtual void Hit(CParticle *pPartical);
   CParticleSystem( DWORD dwFlush, DWORD dwDiscard, FLOAT fRadius , FLOAT fFadeTime,BOOL bOneSize);
   virtual ~CParticleSystem();
   CParticle* GetParticles(){return m_pParticles;}
   virtual HRESULT Create(TCHAR* strTex, BOOL  bAlpha = FALSE);
   virtual HRESULT DeleteDeviceObjects();
   virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );

   HRESULT EmitNewParticles(DWORD dwNumParticlesToEmit,
      const D3DXCOLOR &dwEmitColor, 
      const D3DXCOLOR &dwFadeColor,
      D3DXVECTOR3 vPosition,
      D3DXVECTOR3 vEmitVel,
      FLOAT	fGravity,
      FLOAT	fSize);

   virtual HRESULT Update( FLOAT fSecsPerFrame, CTerrain* pTerrain, CWater* pWater);

   virtual HRESULT Render( LPDIRECT3DDEVICE9 pd3dDevice,CD3DCamera* pCamera,CFOVClipper* pClipper,BOOL bZWrite);
};

#endif // !defined(AFX_PARTICLESYSTEM_H__5E5E153B_F510_4B6F_9117_2B8421436A4E__INCLUDED_)
