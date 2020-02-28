///////////////////////////////////////////////////////////////////////////////
///  Surface.cpp
///
///  PATH: D:\dev2\CommonDev
///
///  CREATED: 02/06/2004 12:10:34 PM by Christiaan Burchell
///
///  PURPOSE:  implementation of the CSurface class.
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

// Surface.cpp: implementation of the CSurface class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

//#include <D3dx8math.h>
#include "dxutil.h"
#include "Surface.h"
#include "D3DUtil.h"
#include "2DDraw.h"

//#include "TerrafighterApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The custom FVF, which describes the custom vertex structure.
#define D3DFVF_OBJ3DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct OBJ3DVERTEX
{
   D3DXVECTOR4 position; // The position.
   D3DCOLOR    color;    // The color.
   FLOAT       tu, tv;   // The texture coordinates.
};



/////////////////////////////////////////////////
///
/// NAME: CSurface
///
/// CLASS: CSurface
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:06:35 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CSurface::CSurface()
{
   m_VertexBuffer = NULL;
   m_Texture = NULL;
   m_bColorKey = FALSE;
   m_pRenderToSurface = NULL;
   m_pTexSurf  = NULL;
}



/////////////////////////////////////////////////
///
/// NAME: ~CSurface
///
/// CLASS: CSurface
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:06:37 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CSurface::~CSurface()
{
   DeleteDeviceObjects();
}



/////////////////////////////////////////////////
///
/// NAME: SetColorKey
///
/// CLASS: CSurface
///
/// DESCRIPTION: Sets the color key for the surface
///
/// CREATED:     02/06/2004 12:06:49 PM
///
/// PARAMETERS: 
///             DWORD dwColorKey
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSurface::SetColorKey(DWORD dwColorKey)
{
   m_bColorKey = TRUE;
   D3DCOLOR c = D3DCOLOR_XRGB(GetRValue(dwColorKey),GetGValue(dwColorKey),GetBValue(dwColorKey));
   D3DSURFACE_DESC pDesc;
   m_Texture->GetLevelDesc(0, &pDesc );

   D3DLOCKED_RECT d3dlr;
   m_Texture->LockRect( 0, &d3dlr, 0, 0 );
   DWORD * pDst = (DWORD *)d3dlr.pBits;
   int DPitch = d3dlr.Pitch/4;

   for (DWORD y=0; y<pDesc.Height; ++y)
   {
      for (DWORD x=0; x<pDesc.Width; ++x)
      {
         int index = y*DPitch + x;
         if(pDst[index] == c)
         {
            pDst[index] = 0x00ffffff & pDst[index];
         }
         else
         {
            pDst[index] = 0xff000000 | pDst[index];
         }
      }
   }

   m_Texture->UnlockRect (0);
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: Clear
///
/// CLASS: CSurface
///
/// DESCRIPTION: Clears the surface
///
/// CREATED:     02/06/2004 12:06:58 PM
///
/// PARAMETERS: 
///             D3DCOLOR dwColor
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSurface::Clear(D3DCOLOR dwColor)
{
   D3DSURFACE_DESC pDesc;
   m_Texture->GetLevelDesc(0, &pDesc );

   D3DLOCKED_RECT d3dlr;
   m_Texture->LockRect( 0, &d3dlr, 0, 0 );
   DWORD * pDst = (DWORD *)d3dlr.pBits;
   int DPitch = d3dlr.Pitch/4;

   //memset(pDst,c,pDesc.Height*DPitch);

   for (DWORD y=0; y<pDesc.Height; ++y)
   {
      for (DWORD x=0; x<pDesc.Width; ++x)
      {
         int index = y*DPitch + x;
         pDst[index] = dwColor;	
      }
   }

   m_Texture->UnlockRect (0);
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: Init
///
/// CLASS: CSurface
///
/// DESCRIPTION: Creates the surface given a textrure file
///
/// CREATED:     02/06/2004 12:09:57 PM
///
/// PARAMETERS: 
///             LPCSTR strBMP
///             DWORD dwDesiredWidth
///             DWORD dwDesiredHeight
///             LPDIRECT3DDEVICE9 pd3dDevice
///             CZipArchive* pZip
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSurface::Init(LPCSTR strBMP,DWORD dwDesiredWidth,DWORD dwDesiredHeight,LPDIRECT3DDEVICE9 pd3dDevice, CZipArchive* pZip)
{
   m_dwWidth = dwDesiredWidth;
   m_dwHeight = dwDesiredHeight;
   sys_verify_hr_return_hr(D3DUtil::CreateTexture(pd3dDevice,(char*)strBMP,m_Texture,pZip,D3DFMT_A8R8G8B8));
   sys_verify_hr_return_hr(pd3dDevice->CreateVertexBuffer( 4*sizeof(OBJ3DVERTEX), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_OBJ3DVERTEX, D3DPOOL_DEFAULT, &m_VertexBuffer , NULL) );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: CreateRenderSurface
///
/// CLASS: CSurface
///
/// DESCRIPTION: Creates the render surface
///
/// CREATED:     02/06/2004 12:09:25 PM
///
/// PARAMETERS: 
///             DWORD dwDesiredWidth
///             DWORD dwDesiredHeight
///             LPDIRECT3DDEVICE9 pd3dDevice
///             D3DFORMAT Stencil
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSurface::CreateRenderSurface(DWORD dwDesiredWidth,DWORD dwDesiredHeight,LPDIRECT3DDEVICE9 pd3dDevice, D3DFORMAT Stencil)
{
   m_dwWidth = dwDesiredWidth;
   m_dwHeight = dwDesiredHeight;
   HRESULT hr;

   if(FAILED(hr = D3DXCreateTexture(pd3dDevice, m_dwWidth, m_dwHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_Texture)) &&
      FAILED(hr = D3DXCreateTexture(pd3dDevice, m_dwWidth, m_dwHeight, 1, 0,                     D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_Texture)))
   {
      sys_msg_hr("D3DXCreateTexture",hr);
      return hr;
   }

   sys_verify_hr_return_hr(pd3dDevice->CreateVertexBuffer( 4*sizeof(OBJ3DVERTEX), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_OBJ3DVERTEX, D3DPOOL_DEFAULT, &m_VertexBuffer, NULL ) );

   sys_verify_hr(m_Texture->GetSurfaceLevel(0, &m_pTexSurf));
   D3DSURFACE_DESC desc;
   sys_verify_hr(m_pTexSurf->GetDesc(&desc));

   //GetApp()->GetModeInfo()->DepthStencilFormat

   sys_verify_hr_return_hr(D3DXCreateRenderToSurface(pd3dDevice, desc.Width, desc.Height, 
      desc.Format, TRUE, Stencil, &m_pRenderToSurface));
   
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: BeginScene
///
/// CLASS: CSurface
///
/// DESCRIPTION: Starts rendering to the surface
///
/// CREATED:     02/06/2004 12:07:30 PM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSurface::BeginScene()
{
   if(m_pRenderToSurface)
      return m_pRenderToSurface->BeginScene(m_pTexSurf, NULL);
   else
      return E_FAIL;
}



/////////////////////////////////////////////////
///
/// NAME: EndScene
///
/// CLASS: CSurface
///
/// DESCRIPTION: Ends rendering to the surface
///
/// CREATED:     02/06/2004 12:07:46 PM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSurface::EndScene()
{
   if(m_pRenderToSurface)
      return m_pRenderToSurface->EndScene(D3DX_FILTER_NONE);
   else
      return E_FAIL;
}



/////////////////////////////////////////////////
///
/// NAME: Draw
///
/// CLASS: CSurface
///
/// DESCRIPTION: Draws the surface
///
/// CREATED:     02/06/2004 12:08:33 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             DWORD x
///             DWORD y
///             RECT *prc
///             C2DDraw* pDisplay
///             BOOL bBlend
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSurface::Draw(LPDIRECT3DDEVICE9 pd3dDevice,DWORD x,DWORD y,RECT *prc, BOOL bBlend)
{
   DWORD SurfaceColour = 0xffffffff;
   if(m_bColorKey)
   {

      // Set diffuse blending for alpha set in vertices.
      pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );

      if(bBlend)
      {
         pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
         pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
      }
      else
      {
         pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
         pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
         // Enable alpha testing (skips pixels with less than a certain alpha.)
         //if( m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
         {
            pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
            pd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
            pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
         }
      }
      DWORD SurfaceColour = 0x00ffffff;
   }


   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_OBJ3DVERTEX );
   pd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(OBJ3DVERTEX));

   OBJ3DVERTEX* pVertices;

   CRect r(CPoint(x,y),CSize(m_dwWidth,m_dwHeight));

   FLOAT uoffest = 0.0f;
   FLOAT voffest = 0.0f;

   FLOAT uoffest2 = 1.0f;
   FLOAT voffest2 = 1.0f;


   if(prc)
   {
      CRect rect(*prc);

      uoffest =  (float)rect.left/(float)m_dwWidth;
      voffest =  (float)rect.top/(float)m_dwHeight;

      uoffest2 =  ((float)(rect.right)/(float)m_dwWidth);
      voffest2 =  ((float)(rect.bottom)/(float)m_dwHeight);

      rect.OffsetRect(-rect.TopLeft().x,-rect.TopLeft().y);
      rect.OffsetRect(CPoint(x,y));

      r = rect;
   }

   m_VertexBuffer->Lock( 0, 4*sizeof(OBJ3DVERTEX), (void**)&pVertices, D3DLOCK_DISCARD );
   pVertices[0].position = C2DDraw::PointToScreen(r.TopLeft());
   pVertices[0].color = SurfaceColour;
   pVertices[0].tu=uoffest;
   pVertices[0].tv=voffest;

   pVertices[1].position = C2DDraw::PointToScreen(CPoint(r.right,r.top));
   pVertices[1].color = SurfaceColour;
   pVertices[1].tu=uoffest2;
   pVertices[1].tv=voffest;

   pVertices[2].position = C2DDraw::PointToScreen(CPoint(r.left,r.bottom));
   pVertices[2].color = SurfaceColour;
   pVertices[2].tu=uoffest;
   pVertices[2].tv=voffest2;

   pVertices[3].position = C2DDraw::PointToScreen(r.BottomRight());
   pVertices[3].color = SurfaceColour;
   pVertices[3].tu=uoffest2;
   pVertices[3].tv=voffest2;

   m_VertexBuffer->Unlock();

   pd3dDevice->SetTexture( 0, m_Texture );
   pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
   pd3dDevice->SetTexture( 0, NULL );
   pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );

   return S_OK;
}




/////////////////////////////////////////////////
///
/// NAME: DrawAngle
///
/// CLASS: CSurface
///
/// DESCRIPTION: Draws at an angel
///
/// CREATED:     02/06/2004 12:08:16 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             DWORD x
///             DWORD y
///             RECT *prc
///             C2DDraw* pDisplay
///             FLOAT Angel
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CSurface::DrawAngle(LPDIRECT3DDEVICE9 pd3dDevice,DWORD x,DWORD y,RECT *prc, FLOAT Angel)
{
   if(m_bColorKey)
   {
      // Set diffuse blending for alpha set in vertices.
      pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
      pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
      pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

      // Enable alpha testing (skips pixels with less than a certain alpha.)
      //if( m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
      {
         pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
         pd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
         pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
      }
   }


   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF(D3DFVF_OBJ3DVERTEX);
   pd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(OBJ3DVERTEX));

   OBJ3DVERTEX* pVertices;

   CRect r(CPoint(0,0),CSize(m_dwWidth,m_dwHeight));

   FLOAT uoffest = 0.0f;
   FLOAT voffest = 0.0f;

   FLOAT uoffest2 = 1.0f;
   FLOAT voffest2 = 1.0f;


   if(prc)
   {
      CRect rect(*prc);

      uoffest =  (float)rect.left/(float)m_dwWidth;
      voffest =  (float)rect.top/(float)m_dwHeight;

      uoffest2 =  ((float)(rect.right)/(float)m_dwWidth);
      voffest2 =  ((float)(rect.bottom)/(float)m_dwHeight);

      rect.OffsetRect(-rect.TopLeft().x,-rect.TopLeft().y);
      rect.OffsetRect(CPoint(0,0));

      r = rect;
   }

   // center the rect around the point
   r.OffsetRect(-r.Size().cx/2 ,-r.Size().cy/2);

   m_VertexBuffer->Lock( 0, 4*sizeof(OBJ3DVERTEX), (void**)&pVertices, D3DLOCK_DISCARD );

   D3DXVECTOR2 CenterPoint(x,y);
   D3DXMATRIX matRotate2;
   D3DXMatrixRotationZ( &matRotate2, Angel);

   D3DXVECTOR2 TopLeft(r.left,r.top);
   D3DXVec2TransformCoord(&TopLeft,&TopLeft,&matRotate2);
   TopLeft+=CenterPoint;

   pVertices[0].position = C2DDraw::PointToScreen(TopLeft);
   pVertices[0].color = 0xffffffff;
   pVertices[0].tu=uoffest;
   pVertices[0].tv=voffest;

   D3DXVECTOR2 TopRight(r.right,r.top);
   D3DXVec2TransformCoord(&TopRight,&TopRight,&matRotate2);
   TopRight+=CenterPoint;

   pVertices[1].position = C2DDraw::PointToScreen(TopRight);
   pVertices[1].color = 0xffffffff;
   pVertices[1].tu=uoffest2;
   pVertices[1].tv=voffest;

   D3DXVECTOR2 BottomLeft(r.left,r.bottom);
   D3DXVec2TransformCoord(&BottomLeft,&BottomLeft,&matRotate2);
   BottomLeft+=CenterPoint;

   pVertices[2].position = C2DDraw::PointToScreen(BottomLeft);
   pVertices[2].color = 0xffffffff;
   pVertices[2].tu=uoffest;
   pVertices[2].tv=voffest2;

   D3DXVECTOR2 BottomRight(r.right,r.bottom);
   D3DXVec2TransformCoord(&BottomRight,&BottomRight,&matRotate2);
   BottomRight+=CenterPoint;

   pVertices[3].position = C2DDraw::PointToScreen(BottomRight);
   pVertices[3].color = 0xffffffff;
   pVertices[3].tu=uoffest2;
   pVertices[3].tv=voffest2;

   m_VertexBuffer->Unlock();

   pd3dDevice->SetTexture( 0, m_Texture );
   pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
   pd3dDevice->SetTexture( 0, NULL );
   pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );

   return S_OK;
}

/////////////////////////////////////////////////
///
/// NAME: DeleteDeviceObjects
///
/// CLASS: CSurface
///
/// DESCRIPTION: Destroys all device-dependent objects
///
/// CREATED:     02/06/2004 4:56:35 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CSurface::DeleteDeviceObjects()
{
   m_VertexBuffer = NULL;
   D3DUtil::ReleaseTexture( m_Texture );
   m_Texture = NULL;
   m_pRenderToSurface = NULL;
   m_pTexSurf = NULL;
}
