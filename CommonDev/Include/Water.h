///////////////////////////////////////////////////////////////////////////////
///  Water.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 4:37:34 PM by Christiaan Burchell
///
///  PURPOSE:  Water Object
///
///  COPYRIGHT NOTICE:   Copyright (C) 2004 Redpoint Games 
///
///   This software is provided 'as-is', without any express or implied 
///   warranty.  In no event will the author be held liable for any damages 
///   arising from the use of this software. 
/// 
///   Permission is granted to anyone to use this software for any purpose,
///   excluding commercial applications, and to alter it and redistribute it
///   freely, subject to the following restrictions:
/// 
///   1. The origin of this software must not be misrepresented; you must not
///      claim that you wrote the original software. If you use this software
///      in a product, an acknowledgment in the product documentation would be
///      appreciated but is not required.
///   2. Altered source versions must be plainly marked as such, and must not be
///      misrepresented as being the original software.
///   3. This notice may not be removed or altered from any source distribution.
///   4. The author permission is required to use this software in commercial 
///      applications 
///
///  LAST CHANGED: $Date$
///
///  REVISION HISTORY:
///  $Log$
/// 

// Water.h: interface for the CWater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATER_H__B1AAD27D_C035_46BA_A15D_3C9B26B94F37__INCLUDED_)
#define AFX_WATER_H__B1AAD27D_C035_46BA_A15D_3C9B26B94F37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OctTree.h"
#include "vertextypes.h"
#include "Terrain.h"

//#define WATER_LEVEL 3.0f
#define g_WaterGridSize 100//number of gridpoints along a side


class CWaterObj
{
public:
   D3DXVECTOR3 m_CenterPoint;
   FLOAT m_Raidus;
   WORD nVertex;
   WORD x;
   WORD z;
   WORD xoffset;
   MIRRORVERTEX v;
};

class CWater
{
public:
   //water stuff
   virtual void DeleteDeviceObjects();
   virtual HRESULT Init(BOOL bShowWater, CString GroundTexture,const char* strWater1,const char* strWater2);

   virtual HRESULT Create(LPDIRECT3DDEVICE9 pd3dDevice,FLOAT sizex, FLOAT sizey,FLOAT WaterLevel,CTerrain* pTerrain,CZipArchive* pZip);
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper,DWORD& nTri,BOOL bWaterEff);
   CWater();
   virtual ~CWater();
   void AnimateWater(const float dt,BOOL bWaterEff);
   FLOAT GetWaterLevel(){return m_WaterLevel;}
   DWORD GetTri(){return m_nTri;};

protected:
   FLOAT m_WaterLevel;
   DWORD m_nTri;

private:
   BOOL m_bShowWater;
   CString m_GroundTexture;
   CString m_WaterTexture1;
   CString m_WaterTexture2;

   DWORD m_dwNumFaces;
   DWORD m_dwNumVertices;

   CComPtr<IDirect3DVertexBuffer9> m_pVB;
   CComPtr<IDirect3DTexture9>      m_pTexture;
   CComPtr<IDirect3DTexture9>      m_pTexture2;
   D3DXMATRIX   m_Matrix;         // Matrix to position mirror

   CComPtr<IDirect3DVertexBuffer9> m_pVBGround;
   CComPtr<IDirect3DTexture9>      m_pTextureGround;

   //FLOAT m_SplashTime;
   FLOAT m_Scale;

   FLOAT m_Tex2Offset;
};

#endif // !defined(AFX_WATER_H__B1AAD27D_C035_46BA_A15D_3C9B26B94F37__INCLUDED_)
