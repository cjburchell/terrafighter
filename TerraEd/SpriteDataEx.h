///////////////////////////////////////////////////////////////////////////////
///  SpriteDataEx.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:15:10 AM by Christiaan Burchell
///
///  PURPOSE:  Sprite Data Object
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

// SpriteDataEx.h: interface for the CSpriteDataEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITEDATAEX_H__E2FB4283_4C00_45C6_A8F3_84502DEC73A7__INCLUDED_)
#define AFX_SPRITEDATAEX_H__E2FB4283_4C00_45C6_A8F3_84502DEC73A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LevelData.h"
#include "SpriteData.h"
#include "d3dfile.h"
#include "FOVClipper.h"
#include "Terrain.h"

class CSpriteDataEx : public CSpriteData  
{
public:
   CSpriteDataEx();
   HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& pos, CFOVClipper* pClipper,CTerrain* pTerrain,FLOAT fElapsedTime,int CurrentTexture);
private:
   Xfile::File*     m_pMesh;
   D3DXMATRIX m_MeshScaled;
   FLOAT m_fCurTime;
   std::vector< CComPtr<IDirect3DTexture9> > m_TextureList;
public:
   FLOAT m_Radius;
   D3DXVECTOR3 m_Center;

   virtual ~CSpriteDataEx()
   {
      DeleteDeviceObjects();
   }


   HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice,CZipArchive * pZip);


   HRESULT DeleteDeviceObjects()
   {
      MeshCash::releaseMesh(m_pMesh);
      for(UINT i=0;i<m_TextureList.size();i++)
      {
         D3DUtil::ReleaseTexture( m_TextureList[i] );
      }
      m_TextureList.clear();
      m_pMesh = NULL;
      return S_OK;
   }

};

#endif // !defined(AFX_SPRITEDATAEX_H__E2FB4283_4C00_45C6_A8F3_84502DEC73A7__INCLUDED_)
