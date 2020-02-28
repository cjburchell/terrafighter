// Animation.h: interface for the CAnimation class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ANIMATION_H__9A46DDA3_439F_4AA8_90DF_93CBCD28312E__INCLUDED_)
#define AFX_ANIMATION_H__9A46DDA3_439F_4AA8_90DF_93CBCD28312E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include "../CommonDev/Include/vertextypes.h"
#include "DXUtil.h"
#include <vector>
#include "d3dutil.h"

class CAnimationFrame
{
public:
   CAnimationFrame(): m_fTime(0) {m_pTexture = nullptr;}

	virtual ~CAnimationFrame(){D3DUtil::ReleaseTexture( m_pTexture );};
   CComPtr<IDirect3DTexture9>	m_pTexture;
   FLOAT m_fTime;
   std::string m_strTexture;
};

class CAnimation  
{
public:
   void Update(FLOAT fTime);
   CAnimation();
   virtual ~CAnimation();
   BOOL Create(BOOL bRepeat = FALSE);
   BOOL Create(D3DXVECTOR3 p1,D3DXVECTOR3 p2,D3DXVECTOR3 p3,D3DXVECTOR3 p4, BOOL bRepeat= FALSE);

   BOOL AddFrame(char* strFilename,FLOAT fTime);
   void ClearAllFrames();
   void Render(LPDIRECT3DDEVICE9 pd3dDevice,CD3DCamera * pCamera, D3DXVECTOR3 vPos,FLOAT fSize, FLOAT TimeOffset);
   int GetCurFrame(FLOAT fTime);
   HRESULT DeleteDeviceObjects();
   HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
private:
   std::vector<CAnimationFrame*> m_Frames;
   //D3DXVECTOR3 m_vPos;
   FLOAT m_fCurrentTime;
   FLOAT m_fMaxTime;
   BOOL  m_bRepeat;
   //FLOAT m_fSize;

   D3DXVECTOR3 m_p[4];


   CComPtr<IDirect3DVertexBuffer9> m_pVB;
};

#endif // !defined(AFX_ANIMATION_H__9A46DDA3_439F_4AA8_90DF_93CBCD28312E__INCLUDED_)
