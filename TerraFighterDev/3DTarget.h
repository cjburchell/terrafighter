// 3DTarget.h: interface for the C3DTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DTARGET_H__6701AE69_D325_4521_A1D0_282BB4741E10__INCLUDED_)
#define AFX_3DTARGET_H__6701AE69_D325_4521_A1D0_282BB4741E10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LensFlare.h"
#include "Sprite.h"

#define NUM_TARGET 2
#define NUM_TARGET_TEX NUM_TARGET+1

class C3DTarget  
{
public:
   HRESULT Create(LPDIRECT3DDEVICE9 pd3dDevice);
   HRESULT DeleteDeviceObjects();
   C3DTarget();
   virtual ~C3DTarget();
   HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CD3DCamera* pCamera);
   HRESULT Update(CSprite* pSprite,CWeapon* Weapons);
private:
   CFlare       m_Target[NUM_TARGET+2]; // The actual flares
   CComPtr<IDirect3DVertexBuffer9> m_pVB;
   CComPtr<IDirect3DTexture9> m_pTargetTextures[NUM_TARGET_TEX];
   D3DXMATRIX    m_matBillboardMatrix;

   D3DCOLORVALUE m_TargetColour;

   BOOL m_bTargetBox;

};

#endif // !defined(AFX_3DTARGET_H__6701AE69_D325_4521_A1D0_282BB4741E10__INCLUDED_)
