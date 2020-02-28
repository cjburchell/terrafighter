///////////////////////////////////////////////////////////////////////////////
///  SpriteDataEx.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:05:17 AM by Christiaan Burchell
///
///  PURPOSE:  Sprte data display class
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

// SpriteDataEx.cpp: implementation of the CSpriteDataEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TerraEd.h"
#include "SpriteDataEx.h"
#include "d3dutil.h"
#include "vertextypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CSpriteDataEx
///
/// CLASS: CSpriteDataEx
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 9:05:21 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CSpriteDataEx::CSpriteDataEx()
{
   m_pMesh = NULL;
   m_Radius =1.0f;
   m_fCurTime = 0.0f;
}



/////////////////////////////////////////////////
///
/// NAME: RestoreDeviceObjects
///
/// CLASS: CSpriteDataEx
///
/// DESCRIPTION: Restores/Creates the device objects
///
/// CREATED:     03/06/2004 9:05:25 AM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             CZipArchive * pZip
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSpriteDataEx::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice,CZipArchive * pZip )
{

   D3DXMATRIX matRotateY,matRotateZ;
   D3DXMatrixScaling( &m_MeshScaled, m_fScale, m_fScale, m_fScale);
   // rotiate it 45
   D3DXMatrixRotationY( &matRotateY, HALFPI+PI );
   D3DXMatrixMultiply( &m_MeshScaled, &m_MeshScaled, &matRotateY );

   if(m_zMeshRot != 0.0f)
   {
      D3DXMATRIX matRotateZ;
      D3DXMatrixRotationZ( &matRotateZ, m_zMeshRot);
      D3DXMatrixMultiply( &m_MeshScaled, &m_MeshScaled, &matRotateZ );
   }

   if(m_yMeshRot != 0.0f)
   {
      D3DXMATRIX matRotateY;
      D3DXMatrixRotationY( &matRotateY, m_yMeshRot);
      D3DXMatrixMultiply( &m_MeshScaled, &m_MeshScaled, &matRotateY );
   }

   m_pMesh = MeshCash::createMesh(pd3dDevice, m_FileName.GetBuffer(m_FileName.GetLength()) ,D3DFVF_D3DVERTEX,pZip);
   if(m_pMesh)
   {
      m_pMesh->computeBoundingSphere(&m_Center,&m_Radius);
      m_Radius *= this->m_fScale;
      D3DXVec3TransformCoord(&m_Center,&m_Center,&m_MeshScaled);
   }

   for(UINT i=0;i<m_Textures.size();i++)
   {
      CComPtr<IDirect3DTexture9> pTempTex;
      D3DUtil::CreateTexture( pd3dDevice, (char*)((const char*)m_Textures[i]),(pTempTex),pZip );
      m_TextureList.push_back(pTempTex);
   }

   return m_pMesh == NULL?E_FAIL:S_OK;
}




/////////////////////////////////////////////////
///
/// NAME: Render
///
/// CLASS: CSpriteDataEx
///
/// DESCRIPTION: Display the object
///
/// CREATED:     03/06/2004 9:05:34 AM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             D3DXVECTOR3* pos
///             CFOVClipper* pClipper
///             CTerrain* pTerrain
///             FLOAT fElapsedTime
///             int CurrentTexture
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSpriteDataEx::Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& pos,CFOVClipper* pClipper,CTerrain* pTerrain,FLOAT fElapsedTime,int CurrentTexture)
{
   D3DXMATRIX matFinal,matTrans;
   matFinal = m_MeshScaled;

   FLOAT y = 0;
   if(pTerrain)
      y = pTerrain->GetHightAtPosR( pos.x, pos.z);	

   y = max(pos.y,y);

   D3DXMatrixTranslation( &matTrans,pos.x, y, pos.z );

   D3DXMatrixMultiply( &matFinal, &matFinal, &matTrans );

   if(pClipper == NULL || pClipper->MeshFOVCheck(pos,m_Radius))
   {
      //pd3dDevice->SetTransform( D3DTS_WORLD, &matFinal );

      m_fCurTime += fElapsedTime * 4800;
      if (m_fCurTime > 1.0e15f)
         m_fCurTime = 0;

      if(m_pMesh)
      {
         m_pMesh->setTime(-1,-1,m_fCurTime);
         if(m_pMesh)
         {
            if(CurrentTexture != 0)
            {
               pd3dDevice->SetTexture(0,m_TextureList[CurrentTexture-1]);
            }

            m_pMesh->render(pd3dDevice,&matFinal,CurrentTexture == 0);
         }
      }
   }
   return S_OK;
}