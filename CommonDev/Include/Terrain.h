///////////////////////////////////////////////////////////////////////////////
///  Terrain.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 4:40:40 PM by Christiaan Burchell
///
///  PURPOSE:  Terrain display class
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

// SeaFloor.h: interface for the CSeaFloor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEAFLOOR_H__50EB1DE4_65D5_467B_9FC1_24BEDDD48CE3__INCLUDED_)
#define AFX_SEAFLOOR_H__50EB1DE4_65D5_467B_9FC1_24BEDDD48CE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FOVClipper.h"
#include "Surface.h"
#include "dxutil.h"
#include "OctTree.h"
#include <vector>
#include "LevelData.h"


#define TERRAIN_LOD1 1024.0f
#define TERRAIN_LOD2 4096.0f
#define TERRAIN_LOD3 16384.0f


class CPassData
{
public:
   CPassData()
   {
      m_pTexture = NULL;
      m_pAlphaTexture = NULL;
   }
   // reg texture
   CString m_TexFile;
   CComPtr<IDirect3DTexture9> m_pTexture;
   D3DXMATRIX m_TexMatrix;
   FLOAT m_TexSize;

   // alpha textrue
   CString m_AlphaFile;
   CComPtr<IDirect3DTexture9> m_pAlphaTexture;
   D3DXMATRIX m_TexAlphaMatrix;
   FLOAT m_AlphaSize;
};


class CTerrainObj 
{
public:
   D3DXVECTOR3 m_CenterPoint;
   FLOAT m_Raidus;
   WORD nVertex;
   D3DXVECTOR3 m_Nabpos[4];
   BOOL m_bNab[4];
   BOOL m_bFlatPlainAll;
   BOOL m_bFlatPlain[4];
};

#define MINI_MAP_SIZE 64.0f

//typedef list<CTerrainObj*> TerrainObjList;
//typedef TerrainObjList::iterator TerrainObjItor;

class CTerrain
{
public:
   static BOOL AFXAPI ProcessElement( void* pvObj, void* pData );
   virtual HRESULT Create(LPDIRECT3DDEVICE9 pd3dDevice,CZipArchive* pZip);
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper, DWORD& nTri,const D3DXVECTOR3& vEyePt,BOOL bMulTex,BOOL bLOD);
   void DeleteDeviceObjects();
   void LoadDEM();
   void RefreshHightMap();
   void RefreshOctTree();
   CTerrain();
   virtual ~CTerrain();

   FLOAT GetHightAtPosR(FLOAT x,FLOAT z);
   DWORD GetXSize(){return (DWORD)m_RealSizeX;}
   DWORD GetZSize(){return (DWORD)m_RealSizeZ;}

   CPoint GetPaintPos(const D3DXVECTOR3& point);

   void GenerateLightMap(const TCHAR* filename,LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& vSunDir, D3DXCOLOR AmbientColor, D3DXCOLOR SunColor,LPDIRECT3DTEXTURE9 pAlphaTexture,BOOL bInvertAlpha);

   void SetupMap(LPDIRECT3DDEVICE9 pd3dDevice,const TCHAR* mapname, FLOAT MapHight, std::vector<CTextureData*>& Textures,FLOAT MapScale,CZipArchive* pZip);

   HRESULT RenderMiniMap(LPDIRECT3DDEVICE9 pd3dDevice,POINT p, FLOAT angle);
   void ConvertToMiniMapPoint(FLOAT& x,FLOAT& y, FLOAT angle);

   BOOL Intersect(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2);
   D3DXVECTOR3 IntersectV(const D3DXVECTOR3& Origin,const D3DXVECTOR3& Dir);

   FLOAT GetWaterLevel(){return m_fWaterLevel;}
   void SetWaterLevel(FLOAT wl){m_fWaterLevel = wl;}

   //void SetLowDetail(BOOL d){m_bLowDetail = d;};
   //BOOL IsLowDetail(){return m_bLowDetail;};

   DWORD GetTri(){return m_nTri;};

   FLOAT GetMapHight(){return m_MapHight;}

   LPDIRECT3DTEXTURE9 GetHightMapTextrue(){return m_pHightMapTex;}

   void SaveHightMap(CZipArchive* pZip);
   void SaveTextures(CZipArchive* pZip);

   std::vector<CPassData> m_Textrues;

protected:

   FLOAT m_fWaterLevel;

   FLOAT GetHightAtPos(FLOAT x,FLOAT z);
   COctTree<CTerrainObj>* m_pRoot;

   CString m_HightMapFile;

   CSurface   m_MiniMap;

   CComPtr<IDirect3DTexture9> m_pHightMapTex;
   BYTE* m_pHightMap;
   WORD m_sizex;
   WORD m_sizez;
   long m_halfx;
   long m_halfz;
   FLOAT m_RealSizeX;
   FLOAT m_RealSizeZ;



   CComPtr<IDirect3DVertexBuffer9> m_pVB;
   CComPtr<IDirect3DIndexBuffer9>  m_pIB;
   DWORD m_dwNumVertices;
   DWORD m_dwNumFaces;

   FLOAT m_MapScale;



   DWORD m_nTri;
   FLOAT m_MapHight;
   FLOAT m_MapHightDev256;
};

#endif // !defined(AFX_SEAFLOOR_H__50EB1DE4_65D5_467B_9FC1_24BEDDD48CE3__INCLUDED_)
