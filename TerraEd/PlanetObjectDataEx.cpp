///////////////////////////////////////////////////////////////////////////////
///  PlanetObjectDataEx.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 8:58:06 AM by Christiaan Burchell
///
///  PURPOSE:  Planet Object data class used for displaying the object
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

// PlanetObjectDataEx.cpp: implementation of the CPlanetObjectDataEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TerraEd.h"
#include "PlanetObjectDataEx.h"
#include "vertextypes.h"
#include "dxutil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CPlanetObjectDataEx
///
/// CLASS: CPlanetObjectDataEx
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 8:58:25 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CPlanetObjectDataEx::CPlanetObjectDataEx()
{
   m_Radius = 1.0f;
   m_Center = D3DXVECTOR3(0,0,0);

}



/////////////////////////////////////////////////
///
/// NAME: ~CPlanetObjectDataEx
///
/// CLASS: CPlanetObjectDataEx
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     03/06/2004 8:58:27 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CPlanetObjectDataEx::~CPlanetObjectDataEx()
{
   DeleteDeviceObjects();
}



/////////////////////////////////////////////////
///
/// NAME: CPlanetObjectMesh
///
/// CLASS: CPlanetObjectMesh
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 8:58:29 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CPlanetObjectMesh::CPlanetObjectMesh()
{
   m_pMesh = NULL;
}



/////////////////////////////////////////////////
///
/// NAME: Render
///
/// CLASS: CPlanetObjectMesh
///
/// DESCRIPTION: Renders the Object
///
/// CREATED:     03/06/2004 8:59:04 AM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             D3DXVECTOR3* pos
///             CFOVClipper* pClipper
///             CTerrain* pTerrain
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CPlanetObjectMesh::Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& pos,CFOVClipper* pClipper,CTerrain* pTerrain)
{
   if(pClipper == NULL || pClipper->MeshFOVCheck(pos,m_Radius))
   {
      D3DXMATRIX matWorld;
      FLOAT y = pTerrain->GetHightAtPosR( pos.x, pos.z);
      y = max(pos.y,y);

      ::D3DXMatrixTranslation(&matWorld,pos.x,y,pos.z);	
      ::D3DXMatrixMultiply(&matWorld,&m_MeshScaled,&matWorld);
      //pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
      if(m_pMesh)
         m_pMesh->render(pd3dDevice,&matWorld);
   }
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: RestoreDeviceObjects
///
/// CLASS: CPlanetObjectMesh
///
/// DESCRIPTION: Restores/Creates the device objects
///
/// CREATED:     03/06/2004 9:00:13 AM
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
HRESULT CPlanetObjectMesh::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice,CZipArchive * pZip)
{

   D3DXMATRIX Rotate;
   D3DXMatrixRotationX( &Rotate, HALFPI+PI );
   D3DXMatrixScaling( &m_MeshScaled, m_fScale, m_fScale, m_fScale);
   D3DXMatrixMultiply( &m_MeshScaled, &m_MeshScaled, &Rotate );

   m_pMesh = MeshCash::createMesh(pd3dDevice, m_File.GetBuffer(m_File.GetLength()),D3DFVF_D3DVERTEX,pZip);

   if(m_pMesh)
   {
      m_pMesh->computeBoundingSphere(&m_Center,&m_Radius);
      m_Radius *= m_fScale; 
      D3DXVec3TransformCoord(&m_Center,&m_Center,&m_MeshScaled);
   }
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DeleteDeviceObjects
///
/// CLASS: CPlanetObjectMesh
///
/// DESCRIPTION: Destroys all device-dependent objects
///
/// CREATED:     03/06/2004 8:59:10 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CPlanetObjectMesh::DeleteDeviceObjects()
{
   MeshCash::releaseMesh(m_pMesh);
   m_pMesh = NULL;
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: CPlanetObjectBush
///
/// CLASS: CPlanetObjectBush
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 8:59:13 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CPlanetObjectBush::CPlanetObjectBush()
{
   m_nTri = 4;
   m_NumbSubItems = MAX_SUB_ITEMS;
   for(DWORD j=0;j<m_NumbSubItems;j++)
   {
      FLOAT x = (rand()%20 * 0.1f) - 1.0f;
      FLOAT z = (rand()%20 * 0.1f) - 1.0f;
      FLOAT y = 0;
      m_SubItems[j].m_Offset = D3DXVECTOR3(x,y,z);
      m_SubItems[j].m_Rotation = ToRad(rand()%360);
   }
}



/////////////////////////////////////////////////
///
/// NAME: Render
///
/// CLASS: CPlanetObjectBush
///
/// DESCRIPTION: Diplays the object
///
/// CREATED:     03/06/2004 9:00:43 AM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             D3DXVECTOR3* pos
///             CFOVClipper* pClipper
///             CTerrain* pTerrain
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CPlanetObjectBush::Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& pos,CFOVClipper* pClipper,CTerrain* pTerrain)
{	
   if(pClipper == NULL || pClipper->MeshFOVCheck(pos,m_Radius))
   {
      D3DMATERIAL9 mtrl;
      D3DUtil::InitMaterial( mtrl, 128.0f, 128.0f, 128.0f);
      pd3dDevice->SetMaterial( &mtrl );

      pd3dDevice->SetTexture( 0, m_pTexture );
      pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TREEVERTEX) );
      pd3dDevice->SetVertexShader( NULL );
      pd3dDevice->SetFVF( D3DFVF_TREEVERTEX );

      for(DWORD i=0;i<m_NumbSubItems;i++)
      {
         D3DXMATRIX matWorld,Rotate;
         D3DXVECTOR3 ObjPos = pos + m_SubItems[i].m_Offset;
         FLOAT y = pTerrain->GetHightAtPosR( pos.x, pos.z);

         ::D3DXMatrixTranslation(&matWorld,
            ObjPos.x,
            y,
            ObjPos.z);
         ::D3DXMatrixRotationY(&Rotate,m_SubItems[i].m_Rotation);
         ::D3DXMatrixMultiply(&matWorld,&Rotate,&matWorld);

         //D3DXMATRIX matAnimate;
         //::D3DXMatrixRotationX(&matAnimate,ToRad(25));

         //::D3DXMatrixMultiply(&matWorld,&matAnimate,&matWorld);

         pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
         pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST , 0, m_nTri );
      }
   }
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: CPlanetObjectTree
///
/// CLASS: CPlanetObjectTree
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 8:59:18 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CPlanetObjectTree::CPlanetObjectTree()
{
   m_nTri = 8;
   m_pVB      = NULL;
   m_pTexture = NULL;  
}



/////////////////////////////////////////////////
///
/// NAME: Render
///
/// CLASS: CPlanetObjectTree
///
/// DESCRIPTION: Renders the object
///
/// CREATED:     03/06/2004 9:02:21 AM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             D3DXVECTOR3* pos
///             CFOVClipper* pClipper
///             CTerrain* pTerrain
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CPlanetObjectTree::Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& pos,CFOVClipper* pClipper,CTerrain* pTerrain)
{
   if(pClipper == NULL || pClipper->MeshFOVCheck(pos,m_Radius))
   {
      D3DMATERIAL9 mtrl;
      D3DUtil::InitMaterial( mtrl, 128.0f, 128.0f, 128.0f);
      pd3dDevice->SetMaterial( &mtrl );

      pd3dDevice->SetTexture( 0, m_pTexture );
      pd3dDevice->SetStreamSource( 0, m_pVB, 0,sizeof(TREEVERTEX) );
      pd3dDevice->SetVertexShader( NULL );
      pd3dDevice->SetFVF( D3DFVF_TREEVERTEX );

      D3DXMATRIX matWorld;
      FLOAT y = pTerrain->GetHightAtPosR( pos.x, pos.z); 
      y = max(pos.y,y);


      ::D3DXMatrixTranslation(&matWorld,
         pos.x,
         y,
         pos.z);



      pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
      pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST , 0, m_nTri );
   }
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: RestoreDeviceObjects
///
/// CLASS: CPlanetObjectTree
///
/// DESCRIPTION: Restores/Creates the device objects
///
/// CREATED:     03/06/2004 9:00:04 AM
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
HRESULT CPlanetObjectTree::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice, CZipArchive * pZip)
{
   m_v[0].p     = D3DXVECTOR3(-m_xSize, 0.0f, 0.0f );
   m_v[0].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[0].tu    = 0.0f;   
   m_v[0].tv	  = 1.0f;

   m_v[1].p     = D3DXVECTOR3(-m_xSize, 2.0f*m_ySize, 0.0f  );
   m_v[1].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[1].tu    = 0.0f;   
   m_v[1].tv    = 0.0f;

   m_v[2].p     = D3DXVECTOR3(m_xSize, 0.0f, 0.0f  );
   m_v[2].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[2].tu    = 1.0f;
   m_v[2].tv    = 1.0f;

   m_v[3] = m_v[2];
   m_v[4] = m_v[1];
   m_v[5].p     = D3DXVECTOR3(m_xSize, 2.0f*m_ySize, 0.0f  );
   m_v[5].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[5].tu    = 1.0f;   
   m_v[5].tv    = 0.0f;


   m_v[6] = m_v[2];
   m_v[7] = m_v[1];
   m_v[8] = m_v[0];

   m_v[9] = m_v[5];
   m_v[10] = m_v[1];
   m_v[11] = m_v[2];

   m_v[12].p     = D3DXVECTOR3(0.0f, 0.0f,-m_xSize );
   m_v[12].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[12].tu    = 0.0f;   
   m_v[12].tv	  = 1.0f;
   m_v[13].p     = D3DXVECTOR3(0.0f, 2.0f*m_ySize, -m_xSize  );
   m_v[13].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[13].tu    = 0.0f;   
   m_v[13].tv    = 0.0f;
   m_v[14].p     = D3DXVECTOR3(0.0f, 0.0f, m_xSize  );
   m_v[14].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[14].tu    = 1.0f;
   m_v[14].tv    = 1.0f;

   m_v[15] = m_v[14];
   m_v[16] = m_v[13];
   m_v[17].p     = D3DXVECTOR3(0.0f, 2.0f*m_ySize, m_xSize  );
   m_v[17].n     = D3DXVECTOR3(0,1.0f,0);
   m_v[17].tu    = 1.0f;   
   m_v[17].tv    = 0.0f;

   m_v[18] = m_v[14];
   m_v[19] = m_v[13];
   m_v[20] = m_v[12];

   m_v[21] = m_v[17];
   m_v[22] = m_v[13];
   m_v[23] = m_v[14];

   // Create the tree textures
   D3DUtil::CreateTexture( pd3dDevice, (char*)((const char*)m_File),m_pTexture,pZip );

   // Create a quad for rendering each tree
   if( FAILED( pd3dDevice->CreateVertexBuffer( NUMB_TREE_VERTEX*sizeof(TREEVERTEX),
      D3DUSAGE_WRITEONLY, D3DFVF_TREEVERTEX,
      D3DPOOL_MANAGED, &m_pVB , NULL) ) )
   {
      return E_FAIL;
   }

   // Copy tree mesh data into vertexbuffer
   TREEVERTEX* v;
   m_pVB->Lock( 0, 0, (void**)&v, 0 );
   WORD* pI = NULL;

   memcpy( v, m_v, NUMB_TREE_VERTEX*sizeof(TREEVERTEX) );

   m_pVB->Unlock();


   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DeleteDeviceObjects
///
/// CLASS: CPlanetObjectTree
///
/// DESCRIPTION: Destroys all device-dependent objects
///
/// CREATED:     03/06/2004 8:59:24 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CPlanetObjectTree::DeleteDeviceObjects()
{
   D3DUtil::ReleaseTexture( m_pTexture );
   m_pVB  = NULL;
   return S_OK;
}

