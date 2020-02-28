///////////////////////////////////////////////////////////////////////////////
///  Water.cpp
///
///  PATH: D:\dev2\CommonDev
///
///  CREATED: 02/06/2004 4:51:15 PM by Christiaan Burchell
///
///  PURPOSE:  the water object
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

// Water.cpp: implementation of the CWater class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "D3DUtil.h"
#include "Water.h"
#include "DXUtil.h"
#include "vertextypes.h"
#include "D3DApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWater
///
/// CLASS: CWater
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 4:51:18 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWater::CWater()
{
   m_pVB         = NULL;
   m_Tex2Offset = 0.0f;
   //m_pIB = NULL;
   D3DXMatrixTranslation( &m_Matrix, 0.0f, 0.0f, 0.0f );
   m_pTexture = NULL;
   m_pTexture2 = NULL;

   m_pVBGround = NULL;
   m_pTextureGround = NULL;

   m_bShowWater = TRUE;
   m_GroundTexture = _T("sand.jpg");

   m_WaterLevel = 0;
   m_nTri = 0;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWater
///
/// CLASS: CWater
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 4:51:20 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWater::~CWater()
{
}




/////////////////////////////////////////////////
///
/// NAME: Render
///
/// CLASS: CWater
///
/// DESCRIPTION: Render the water
///
/// CREATED:     02/06/2004 4:51:30 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE8 pd3dDevice
///             CFOVClipper* pClipper
///             DWORD& nTri
///             BOOL bWaterEff
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CWater::Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper, DWORD& nTri,BOOL bWaterEff)
{

   D3DMATERIAL9 Material;
   D3DUtil::InitMaterial( Material, 1.0f, 1.0f, 1.0f , 0.0f);
   pd3dDevice->SetMaterial( &Material );
   pd3dDevice->SetTransform( D3DTS_WORLD, &m_Matrix );


   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_D3DVERTEX );
   pd3dDevice->SetStreamSource( 0, m_pVBGround, 0, sizeof(D3DVERTEX) );

   pd3dDevice->SetTexture( 0, m_pTextureGround );

   pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,0, 2);
   nTri+=2;


   if(m_bShowWater)
   {
      // Draw the mirror
      pd3dDevice->SetTexture( 0, m_pTexture );

      if(bWaterEff)
      {
         pd3dDevice->SetTexture( 1, m_pTexture2 );
         pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );

         pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
         pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
         D3DXMATRIX TexMatrix;
         D3DXMatrixIdentity(&TexMatrix); // declared in d3dutil.h
         TexMatrix._31 = m_Tex2Offset; 
         TexMatrix._32 = m_Tex2Offset;
         pd3dDevice->SetTransform( D3DTS_TEXTURE1 , &TexMatrix );

      }

      pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
      pd3dDevice->SetRenderState( D3DRS_SRCBLEND,     D3DBLEND_DESTCOLOR);//D3DBLEND_DESTCOLOR );
      pd3dDevice->SetRenderState( D3DRS_DESTBLEND,    D3DBLEND_SRCCOLOR);//D3DBLEND_ZERO );




      pd3dDevice->SetVertexShader( NULL );
      pd3dDevice->SetFVF( D3DFVF_MIRRORVERTEX );

      pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(MIRRORVERTEX) );
      //pd3dDevice->SetIndices( m_pIB, 0 );

      pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
      pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);





      //pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      FALSE );
      pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,0, 2);
      nTri+=2;
      m_nTri=2;

      //pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      TRUE );



      pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

      if(bWaterEff)
      {
         pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
         pd3dDevice->SetTexture( 1, NULL );
         pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
      }
      pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
      pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
   }
   pd3dDevice->SetTexture( 0, NULL );


   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: Init
///
/// CLASS: CWater
///
/// DESCRIPTION: init the water object
///
/// CREATED:     02/06/2004 4:51:40 PM
///
/// PARAMETERS: 
///             BOOL bShowWater
///             CString GroundTexture
///             const char* strWater1
///             const char* strWater2
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CWater::Init(BOOL bShowWater, CString GroundTexture,const char* strWater1,const char* strWater2)
{
   m_bShowWater = bShowWater;
   if(!GroundTexture.IsEmpty())
      m_GroundTexture = GroundTexture;
   m_WaterTexture1 = strWater1;
   m_WaterTexture2 = strWater2;

   return S_OK;
}





/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CWater
///
/// DESCRIPTION: Create the water object
///
/// CREATED:     02/06/2004 4:51:48 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE8 pd3dDevice
///             FLOAT sizex
///             FLOAT sizey
///             FLOAT WaterLevel
///             CTerrain* pTerrain
///             CZipArchive* pZip
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CWater::Create(LPDIRECT3DDEVICE9 pd3dDevice,FLOAT sizex, FLOAT sizey, FLOAT WaterLevel,CTerrain* pTerrain, CZipArchive* pZip)
{
   m_WaterLevel = WaterLevel;
   // Set up a material


   if( FAILED( D3DUtil::CreateTexture( pd3dDevice, (LPSTR)((LPCSTR)m_WaterTexture1),
      m_pTexture ,pZip) ) )
      return D3DAPPERR_MEDIANOTFOUND;

   if( FAILED( D3DUtil::CreateTexture( pd3dDevice, (LPSTR)((LPCSTR)m_WaterTexture2),
      m_pTexture2 ,pZip ) ) )
      return D3DAPPERR_MEDIANOTFOUND;

   if( FAILED( D3DUtil::CreateTexture( pd3dDevice, m_GroundTexture.GetBuffer(m_GroundTexture.GetLength()),
      m_pTextureGround , pZip) ) )
      return D3DAPPERR_MEDIANOTFOUND;

   m_dwNumVertices = g_WaterGridSize*g_WaterGridSize;
   m_dwNumFaces = (((g_WaterGridSize-1)*(g_WaterGridSize-1))*2);

   // Create a big square for rendering the mirror
   if( FAILED( pd3dDevice->CreateVertexBuffer( 4*sizeof(MIRRORVERTEX),
      D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 
      D3DFVF_MIRRORVERTEX,
      D3DPOOL_DEFAULT, 
      &m_pVB , NULL) ) )
      return E_FAIL;

   if( FAILED( pd3dDevice->CreateVertexBuffer( 4*sizeof(D3DVERTEX),
      D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 
      D3DFVF_D3DVERTEX,
      D3DPOOL_DEFAULT, 
      &m_pVBGround, NULL ) ) )
      return E_FAIL;

   //sizey = sizex = g_WaterGridSize;

   FLOAT halfx  = (FLOAT)g_WaterGridSize/2.0f;
   FLOAT halfz  = (FLOAT)g_WaterGridSize/2.0f;
   m_Scale =  sizex/g_WaterGridSize;

   FLOAT xsize = (FLOAT) halfx*m_Scale;
   FLOAT zsize = (FLOAT) halfz*m_Scale;

   FLOAT textfactor = g_WaterGridSize/8;


   MIRRORVERTEX* v = NULL;
   m_pVB->Lock( 0, 4*sizeof(MIRRORVERTEX), (void**)&v, D3DLOCK_DISCARD );
   v[0].p = D3DXVECTOR3(-sizex, m_WaterLevel,-sizey );
   v[0].n = D3DXVECTOR3( 0.0f,  -1.0f,  0.0f );
   v[0].tu = textfactor;
   v[0].tv = textfactor;
   v[0].tu1 = g_WaterGridSize;
   v[0].tv1 = g_WaterGridSize;
   v[1].p = D3DXVECTOR3(-sizex, m_WaterLevel, sizey );
   v[1].n = D3DXVECTOR3( 0.0f,  1.0f,  0.0f );
   v[1].tu = textfactor;
   v[1].tv = 0.0f;
   v[1].tu1 = g_WaterGridSize;
   v[1].tv1 = 0.0f;
   v[2].p = D3DXVECTOR3( sizex, m_WaterLevel,-sizey );
   v[2].n = D3DXVECTOR3( 0.0f,  1.0f,  0.0f );
   v[2].tu = 0.0f;
   v[2].tv = textfactor;
   v[2].tu1 = 0.0f;
   v[2].tv1 = g_WaterGridSize;
   v[3].p = D3DXVECTOR3( sizex, m_WaterLevel, sizey );
   v[3].n = D3DXVECTOR3( 0.0f,  1.0f,  0.0f );
   v[3].tu = 0.0f;
   v[3].tv = 0.0f;
   v[3].tu1 = 0.0f;
   v[3].tv1 = 0.0f;
   m_pVB->Unlock();


   D3DVERTEX* v2 = NULL;
   m_pVBGround->Lock( 0, 4*sizeof(D3DVERTEX), (void**)&v2, D3DLOCK_DISCARD );
   v2[0].p = D3DXVECTOR3(-sizex, -0.1f,-sizey );
   v2[0].n = D3DXVECTOR3( 0.0f,  -1.0f,  0.0f );
   v2[0].tu = textfactor;
   v2[0].tv = textfactor;
   v2[1].p = D3DXVECTOR3(-sizex, -0.1f, sizey );
   v2[1].n = D3DXVECTOR3( 0.0f,  1.0f,  0.0f );
   v2[1].tu = textfactor;
   v2[1].tv = 0.0f;
   v2[2].p = D3DXVECTOR3( sizex, -0.1f,-sizey );
   v2[2].n = D3DXVECTOR3( 0.0f,  1.0f,  0.0f );
   v2[2].tu = 0.0f;
   v2[2].tv = textfactor;
   v2[3].p = D3DXVECTOR3( sizex, -0.1f, sizey );
   v2[3].n = D3DXVECTOR3( 0.0f,  1.0f,  0.0f );
   v2[3].tu = 0.0f;
   v2[3].tv = 0.0f;
   m_pVB->Unlock();
   return S_OK;
}

/////////////////////////////////////////////////
///
/// NAME: DeleteDeviceObjects
///
/// CLASS: CWater
///
/// DESCRIPTION: Destroys all device-dependent objects
///
/// CREATED:     02/06/2004 4:54:57 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWater::DeleteDeviceObjects()
{
   m_pVB = NULL;
   m_pVBGround = NULL;

   D3DUtil::ReleaseTexture( m_pTexture );
   m_pTexture = NULL;
   D3DUtil::ReleaseTexture( m_pTexture2 );
   m_pTexture2 = NULL;
   D3DUtil::ReleaseTexture( m_pTextureGround );
   m_pTextureGround = NULL;
}



/////////////////////////////////////////////////
///
/// NAME: AnimateWater
///
/// CLASS: CWater
///
/// DESCRIPTION: Animate the water
///
/// CREATED:     02/06/2004 4:52:00 PM
///
/// PARAMETERS: 
///             float dt
///             BOOL bWaterEff
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWater::AnimateWater(float dt,BOOL bWaterEff)
{
   if(bWaterEff)
   {
      m_Tex2Offset += dt/20.0f;
      if(m_Tex2Offset>2.0f)
         m_Tex2Offset -= 2.0f;
   }
}
