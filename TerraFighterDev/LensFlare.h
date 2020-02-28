// LensFlare.h: interface for the CLensFlare class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LENSFLARE_H__AC4D0C42_6AB8_400C_85D4_DEEC7E261205__INCLUDED_)
#define AFX_LENSFLARE_H__AC4D0C42_6AB8_400C_85D4_DEEC7E261205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vertextypes.h"
#include "FOVClipper.h"
#include "Terrain.h"

class CFlare 
{

public:
   void Set( INT wType, FLOAT fLocation, FLOAT fScale, FLOAT fRed, FLOAT fGreen, FLOAT fBlue );
   INT                m_wType;       // 0..5, matches flare material indices
   FLOAT              m_fLoc;        // postion on axis
   FLOAT              m_fScale;      // relative size
   FLOAT              m_r,m_g,m_b;   // color
   D3DVECTOR          m_vPositionPt; // 3D position for rendering
   FLOAT              m_fRenderSize; // size for rendering the flare
};

#define     NUM_FLARES 8        // Number of lens flares
#define     NUM_FLARES_WITH_SUN 9
#define		FLARE_TEXTURES 6

class CLensFlare  
{
public:
   HRESULT Create(LPDIRECT3DDEVICE9 pd3dDevice);
   HRESULT DeleteDeviceObjects();
   CLensFlare();
   virtual ~CLensFlare();
   HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);
   HRESULT Update(D3DXVECTOR3 vLightPt,CD3DCamera* pCamera,CTerrain* pTerrain);

private:
   CFlare       m_Flare[NUM_FLARES_WITH_SUN]; // The actual flares
   CComPtr<IDirect3DVertexBuffer9> m_pVB;
   //LPDIRECT3DTEXTURE8 m_pShineTextures[10];
   CComPtr<IDirect3DTexture9> m_pFlareTextures[FLARE_TEXTURES];
   D3DXMATRIX    m_matBillboardMatrix; // Used for billboard orientation
   D3DXVECTOR3    m_vLightPt;
   BOOL m_bDisplay;

private:
   static int SortCB( const VOID* arg1, const VOID* arg2 );
};

#endif // !defined(AFX_LENSFLARE_H__AC4D0C42_6AB8_400C_85D4_DEEC7E261205__INCLUDED_)
