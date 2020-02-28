///////////////////////////////////////////////////////////////////////////////
///  SkyBox.cpp
///
///  PATH: D:\dev2\CommonDev
///
///  CREATED: 02/06/2004 4:46:38 PM by Christiaan Burchell
///
///  PURPOSE:  The Skybox object
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

// SkyBox.cpp: implementation of the CSkyBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkyBox.h"
#include "dxutil.h"
#include "D3DApp.h"
#include "vertextypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CSkyBox
///
/// CLASS: CSkyBox
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 4:46:41 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CSkyBox::CSkyBox()
{
   m_pTexture = NULL;
   m_pVB = NULL;
   m_pIB = NULL;

   m_PlanetRadius = 0.0f;
   m_AtmosphereRadius = 0.0f;
   m_HTileFactor = 1.0f;
   m_VTileFactor = 1.0f;
   m_PlaneSize = 0.0f;
}



/////////////////////////////////////////////////
///
/// NAME: ~CSkyBox
///
/// CLASS: CSkyBox
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 4:46:43 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CSkyBox::~CSkyBox()
{
}



/////////////////////////////////////////////////
///
/// NAME: Render
///
/// CLASS: CSkyBox
///
/// DESCRIPTION: Renders the skybox
///
/// CREATED:     02/06/2004 4:46:51 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE8 pd3dDevice
///             D3DXMATRIX* pMatView
///             DWORD& nTri
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSkyBox::Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXMATRIX& MatView, DWORD& nTri)
{
   D3DXMATRIX matWorld;
   D3DXMatrixIdentity( &matWorld );
   pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld );

   // Center view matrix for skybox and disable zbuffer
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
   pd3dDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
   FLOAT fFogStart =  m_AtmosphereRadius-m_PlanetRadius;
   FLOAT fFogEnd   =  m_PlaneSize/2.0f;
   pd3dDevice->SetRenderState( D3DRS_FOGSTART,       FtoDW(fFogStart) );
   pd3dDevice->SetRenderState( D3DRS_FOGEND,         FtoDW(fFogEnd) );

   D3DXMATRIX matView = MatView;
   matView._41 = 0.0f; 
   matView._42 = 0.0f; 
   matView._43 = 0.0f;
   pd3dDevice->SetTransform( D3DTS_VIEW,      &matView );
   pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
   // Some cards do not disable writing to Z when 
   // D3DRS_ZENABLE is FALSE. So do it explicitly
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

   D3DMATERIAL9 mtrl;
   D3DUtil::InitMaterial( mtrl, 255.0f, 255.0f, 255.0f );
   pd3dDevice->SetMaterial( &mtrl );
   pd3dDevice->SetTexture( 0, m_pTexture );
   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_D3DVERTEX );
   pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(D3DVERTEX) );
   pd3dDevice->SetIndices( m_pIB );

   pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_NumVerts,0, m_NumIndices/3);
   nTri += m_NumIndices/3;

   // Restore the render states
   pd3dDevice->SetTransform( D3DTS_VIEW, &MatView);
   pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DeleteDeviceObjects
///
/// CLASS: CSkyBox
///
/// DESCRIPTION: Deletes the device objects
///
/// CREATED:     02/06/2004 4:47:06 PM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSkyBox::DeleteDeviceObjects()
{
   D3DUtil::ReleaseTexture( m_pTexture );
   m_pTexture = NULL;
   m_pVB = NULL;
   m_pIB = NULL;
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CSkyBox
///
/// DESCRIPTION: Creates the Skybox
///
/// CREATED:     02/06/2004 4:47:58 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE8 pd3dDevice
///             WORD divisions
///             FLOAT planetRadius
///             FLOAT atmosphereRadius
///             FLOAT hTilefactor
///             FLOAT vTilefactor
///             CZipArchive* pZip
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSkyBox::Create(LPDIRECT3DDEVICE9 pd3dDevice, 
                        WORD divisions, 
                        FLOAT planetRadius, 
                        FLOAT atmosphereRadius, 
                        FLOAT hTilefactor, 
                        FLOAT vTilefactor, 
                        CZipArchive* pZip)
{
   if( FAILED( D3DUtil::CreateTexture( pd3dDevice, m_TexFile.GetBuffer(m_TexFile.GetLength()),
      m_pTexture, pZip ) ) )
      return D3DAPPERR_MEDIANOTFOUND;

   if (divisions > 256) 
      divisions = 256; //64k verts is the max size for a VB 


   m_PlanetRadius = planetRadius;
   m_AtmosphereRadius = atmosphereRadius;
   m_HTileFactor = hTilefactor;
   m_VTileFactor = vTilefactor;

   m_NumVerts = (divisions + 1) * (divisions + 1);   // 1 division would give 4 verts
   m_NumIndices = divisions * divisions * 2 * 3;   // 1 division would give 6 indices for 2 tris



   // Create a big square for rendering the mirror
   sys_verify_hr_return_hr(pd3dDevice->CreateVertexBuffer( m_NumVerts * sizeof(D3DVERTEX),
      D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 
      D3DFVF_D3DVERTEX,
      D3DPOOL_DEFAULT, 
      &m_pVB, NULL ) );

   sys_verify_hr_return_hr(pd3dDevice->CreateIndexBuffer( m_NumIndices * sizeof(WORD),
      D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,
      D3DFMT_INDEX16 , D3DPOOL_DEFAULT,
      &m_pIB , NULL));


   D3DVERTEX* v = NULL;
   WORD* pI = NULL;
   sys_verify_hr(m_pVB->Lock( 0, m_NumVerts * sizeof(MIRRORVERTEX), (void**)&v, D3DLOCK_DISCARD ));
   sys_verify_hr(m_pIB->Lock( 0, m_NumIndices * sizeof(WORD), (void**)&pI, D3DLOCK_DISCARD ));

   WORD num_divisions = (WORD)sqrt(m_NumIndices/6.0f);

   m_PlaneSize = 2.0f * (float)sqrt((m_AtmosphereRadius*m_AtmosphereRadius) - 
      (m_PlanetRadius*m_PlanetRadius));

   float delta = m_PlaneSize/(float)num_divisions;
   float tex_delta = 2.0f/(float)num_divisions;
   float x_dist   = 0.0f;
   float z_dist   = 0.0f;
   float x_height = 0.0f;
   float z_height = 0.0f;
   float height = 0.0f;

   int vertex = 0;
   for (WORD i=0;i<=num_divisions;i++)
   {
      for (WORD j=0;j<=num_divisions;j++)
      {
         x_dist = (-0.5f * m_PlaneSize) + ((float)j*delta);
         z_dist = (-0.5f * m_PlaneSize) + ((float)i*delta);

         x_height = (x_dist*x_dist) / m_AtmosphereRadius;
         z_height = (z_dist*z_dist) / m_AtmosphereRadius;
         height = x_height + z_height;

#define SKYBOX_OFFSET_Y 1.0f

         v[vertex].p = D3DXVECTOR3(x_dist, atmosphereRadius - planetRadius - height, z_dist );
         v[vertex].n = D3DXVECTOR3( 0.0f,  -1.0f,  0.0f );

         v[vertex].tu = m_HTileFactor*((float)j * tex_delta*0.5f);
         v[vertex].tv = m_VTileFactor*(1.0f - (float)i * tex_delta*0.5f);

         vertex++;
      }
   }

   num_divisions = (WORD)sqrt(m_NumIndices/6.0f);
   int index = 0;

   for (i=0;i<num_divisions;i++)
   {
      for (WORD j=0;j<num_divisions;j++)
      {
         WORD startvert = (i*(num_divisions+1) + j);
         // tri 1
         pI[index++] = startvert;
         pI[index++] = startvert+1;
         pI[index++] = startvert+num_divisions+1;

         // tri 2
         pI[index++] = startvert+1;
         pI[index++] = startvert+num_divisions+2;
         pI[index++] = startvert+num_divisions+1;
      }
   }

   sys_verify_hr(m_pIB->Unlock());
   sys_verify_hr(m_pVB->Unlock());
   return S_OK;
}
