///////////////////////////////////////////////////////////////////////////////
///  PlanetObjectDataEx.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:14:32 AM by Christiaan Burchell
///
///  PURPOSE:  Planet objects
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

// PlanetObjectDataEx.h: interface for the CPlanetObjectDataEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANETOBJECTDATAEX_H__0C2D7061_5D15_4520_88BC_41053F8C27A4__INCLUDED_)
#define AFX_PLANETOBJECTDATAEX_H__0C2D7061_5D15_4520_88BC_41053F8C27A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LevelData.h"
#include "FOVClipper.h"
#include "D3DFile.h"
#include "vertextypes.h"
#include "Terrain.h"

class CPlanetObjectDataEx : public CPlanetObjectData  
{
public:
   CPlanetObjectDataEx();
   virtual ~CPlanetObjectDataEx();
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& pos,CFOVClipper* pClipper,CTerrain* pTerrain) = 0;
public:

   virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice,CZipArchive * pZip) = 0;
   virtual HRESULT DeleteDeviceObjects(){return S_OK;};
   FLOAT m_Radius;
   D3DXVECTOR3 m_Center;
};


class CPlanetObjectMesh : public CPlanetObjectDataEx  
{
public:
   CPlanetObjectMesh();
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& pos,CFOVClipper* pClipper,CTerrain* pTerrain);
public:

   virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice,CZipArchive * pZip);
   virtual HRESULT DeleteDeviceObjects();

private:
   Xfile::File* m_pMesh;
   D3DXMATRIX m_MeshScaled;

};

#define NUMB_TREE_VERTEX 24


class CPlanetObjectTree : public CPlanetObjectDataEx  
{
public:
   CPlanetObjectTree();
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& pos,CFOVClipper* pClipper,CTerrain* pTerrain);
public:

   virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice,CZipArchive * pZip);
   virtual HRESULT DeleteDeviceObjects();
protected:
   CComPtr<IDirect3DTexture9>      m_pTexture;
   CComPtr<IDirect3DVertexBuffer9> m_pVB;
   TREEVERTEX  m_v[NUMB_TREE_VERTEX];
   DWORD m_nTri;
};

struct CSubObj
{
   D3DXVECTOR3 m_Offset;
   FLOAT m_Rotation;
};


#define MAX_SUB_ITEMS 10


class CPlanetObjectBush : public CPlanetObjectTree  
{
public:
   CPlanetObjectBush();
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& pos,CFOVClipper* pClipper,CTerrain* pTerrain);
private:
   DWORD m_NumbSubItems;
   CSubObj m_SubItems[MAX_SUB_ITEMS];
};

#endif // !defined(AFX_PLANETOBJECTDATAEX_H__0C2D7061_5D15_4520_88BC_41053F8C27A4__INCLUDED_)
