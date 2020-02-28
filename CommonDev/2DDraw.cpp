///////////////////////////////////////////////////////////////////////////////
///  2DDraw.cpp
///
///  PATH: D:\dev2\CommonDev
///
///  CREATED: 02/06/2004 10:45:27 AM by Christiaan Burchell
///
///  PURPOSE:   drawing helper class
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

// 2DDraw.cpp: implementation of the C2DDraw class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "2DDraw.h"
#include "dxutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define ConvertColor(col) D3DCOLOR_XRGB(GetRValue(col),GetGValue(col),GetBValue(col))



/////////////////////////////////////////////////
///
/// NAME: C2DDraw
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 10:45:32 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
C2DDraw::C2DDraw()
{
   m_pFont = NULL;
   m_pdTemp3dDevice = NULL;
   m_pVB = NULL;
}



/////////////////////////////////////////////////
///
/// NAME: ~C2DDraw
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 10:45:34 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
C2DDraw::~C2DDraw()
{

}



/////////////////////////////////////////////////
///
/// NAME: PointToScreen
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Converts a POINT to D3DXVECTOR4
///
/// CREATED:     02/06/2004 10:46:17 AM
///
/// PARAMETERS: 
///             POINT p
///
/// RETURN: D3DXVECTOR4 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
D3DXVECTOR4 C2DDraw::PointToScreen(const POINT& p)
{
   D3DXVECTOR4 newp;
   newp.x = ((float)p.x);
   newp.y = ((float)p.y);
   newp.z = 0.0f;
   newp.w = 1.0f;
   return newp;
}



/////////////////////////////////////////////////
///
/// NAME: PointToScreen
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Converts a D3DXVECTOR2 to D3DXVECTOR4
///
/// CREATED:     02/06/2004 10:46:22 AM
///
/// PARAMETERS: 
///             D3DXVECTOR2 p
///
/// RETURN: D3DXVECTOR4 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
D3DXVECTOR4 C2DDraw::PointToScreen(D3DXVECTOR2 p)
{
   D3DXVECTOR4 newp;
   newp.x = ((float)p.x);
   newp.y = ((float)p.y);
   newp.z = 0.0f;
   newp.w = 1.0f;
   return newp;
}




/////////////////////////////////////////////////
///
/// NAME: OutTextc
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws text to the screen
///
/// CREATED:     02/06/2004 10:47:06 AM
///
/// PARAMETERS: 
///             D3DCOLOR crBackground
///             D3DCOLOR crForeground
///             int x
///             int y
///             BOOL bTrasparent
///             char* format
///             ...
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void C2DDraw::OutTextc(D3DCOLOR crBackground, D3DCOLOR crForeground,int x, int y, BOOL bTrasparent, char* format, ...)
{

   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return;
   }

   FLOAT newy = (FLOAT)y;//-((((float)y/(float)m_dwHeight)*2.0f) - 1.0f);
   FLOAT newx = (FLOAT)x;//((((float)x/(float)m_dwWidth)*2.0f) - 1.0f);


   crBackground = crBackground;
   crForeground = crForeground;
   va_list ap;
   char s[500];

   if(format==NULL)
      return;
   va_start(ap,format);
   vsprintf(s,format,ap);
   va_end(ap);

   char* token;
   token=strtok(s,"\n");
   if(token)
   {
      FLOAT ysize = FONT_SIZE_Y;//((((float)FONT_SIZE_Y/(float)m_dwHeight)*2.0f));

      m_pFont->DrawText(m_pdTemp3dDevice,newx,newy,crForeground,token);
      while((token=strtok(NULL,"\n"))!=NULL)
      {
         newy+=ysize;
         m_pFont->DrawText(m_pdTemp3dDevice,newx,newy,crForeground,token);
      }
   }
}



/////////////////////////////////////////////////
///
/// NAME: SetScreenSize
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Sets the screen size
///
/// CREATED:     02/06/2004 10:47:20 AM
///
/// PARAMETERS: 
///             DWORD dwHeight
///             DWORD dwWidth
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void C2DDraw::SetScreenSize(DWORD dwHeight,DWORD dwWidth)
{
   m_dwWidth = dwWidth;
   m_dwHeight = dwHeight;
}




/////////////////////////////////////////////////
///
/// NAME: GetTextRect
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Gets the rect used to display the text
///
/// CREATED:     02/06/2004 10:47:36 AM
///
/// PARAMETERS: 
///             CString str
///
/// RETURN: RECT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
const RECT& C2DDraw::GetTextRect(const TCHAR* str)
{
   CSize s;
   m_pFont->GetTextExtent(str,&s);
   return CRect(CPoint(0,0),s);
}




/////////////////////////////////////////////////
///
/// NAME: DrawBlendRect
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a blended rect using Alpha of the color
///
/// CREATED:     02/06/2004 10:48:05 AM
///
/// PARAMETERS: 
///             RECT rect
///             D3DCOLOR color
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawBlendRect(const RECT& rect, D3DCOLOR color)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }
   m_pdTemp3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
   m_pdTemp3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
   m_pdTemp3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
   DrawFilledRect(rect,color);
   m_pdTemp3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DrawBlendQuad
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a blended quad using Alpha of the color
///
/// CREATED:     02/06/2004 10:48:15 AM
///
/// PARAMETERS: 
///             CPoint tl
///             CPoint bl
///             CPoint tr
///             CPoint br
///             D3DCOLOR color
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawBlendQuad(const CPoint& tl,const CPoint& bl,const CPoint& tr,const CPoint& br, D3DCOLOR color)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }

   m_pdTemp3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
   m_pdTemp3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
   m_pdTemp3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
   DrawFilledQuad(tl,bl,tr,br,color);
   m_pdTemp3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DrawFilledQuad
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a filled quad
///
/// CREATED:     02/06/2004 10:48:44 AM
///
/// PARAMETERS: 
///             CPoint tl
///             CPoint bl
///             CPoint tr
///             CPoint br
///             D3DCOLOR color
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawFilledQuad(const CPoint& tl,const CPoint& bl,const CPoint& tr,const CPoint& br, D3DCOLOR color)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }
   m_pdTemp3dDevice->SetVertexShader( NULL );
   m_pdTemp3dDevice->SetFVF(D3DFVF_SOLIDVERTEX);
   m_pdTemp3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SOLIDVERTEX));

   SOLIDVERTEX* pVertices;


   m_pVB->Lock( 0, 4*sizeof(SOLIDVERTEX), (void**)&pVertices, D3DLOCK_DISCARD );
   pVertices[0].position = PointToScreen(tl);
   pVertices[0].color = color;

   pVertices[1].position = PointToScreen(tr);
   pVertices[1].color = color;

   pVertices[2].position = PointToScreen(bl);
   pVertices[2].color = color;

   pVertices[3].position = PointToScreen(br);
   pVertices[3].color = color;


   m_pVB->Unlock();

   m_pdTemp3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DrawQuadOutline
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a border of a quad
///
/// CREATED:     02/06/2004 10:49:02 AM
///
/// PARAMETERS: 
///             CPoint tl
///             CPoint bl
///             CPoint tr
///             CPoint br
///             D3DCOLOR color
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawQuadOutline(const CPoint& tl,const CPoint& bl,const CPoint& tr,const CPoint& br, D3DCOLOR color)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }

   m_pdTemp3dDevice->SetVertexShader( NULL );
   m_pdTemp3dDevice->SetFVF(D3DFVF_SOLIDVERTEX);
   m_pdTemp3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SOLIDVERTEX));

   SOLIDVERTEX* pVertices;


   m_pVB->Lock( 0, 5*sizeof(SOLIDVERTEX), (void**)&pVertices, D3DLOCK_DISCARD );


   pVertices[0].position = PointToScreen(tl);
   pVertices[0].color = color;

   pVertices[1].position = PointToScreen(bl);
   pVertices[1].color = color;

   pVertices[2].position = PointToScreen(br);
   pVertices[2].color = color;

   pVertices[3].position = PointToScreen(tr);
   pVertices[3].color = color;

   pVertices[4].position = PointToScreen(tl);
   pVertices[4].color = color;

   m_pVB->Unlock();


   m_pdTemp3dDevice->DrawPrimitive( D3DPT_LINESTRIP, 0, 4 );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DrawFilledRect
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a filed rect
///
/// CREATED:     02/06/2004 10:49:15 AM
///
/// PARAMETERS: 
///             RECT rect
///             D3DCOLOR color
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawFilledRect(const RECT& rect, D3DCOLOR color)
{

   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }

   m_pdTemp3dDevice->SetVertexShader( NULL );
   m_pdTemp3dDevice->SetFVF(D3DFVF_SOLIDVERTEX);
   m_pdTemp3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SOLIDVERTEX));

   SOLIDVERTEX* pVertices;

   CRect r(rect);

   m_pVB->Lock( 0, 4*sizeof(SOLIDVERTEX), (void**)&pVertices, D3DLOCK_DISCARD );
   pVertices[0].position = PointToScreen(r.TopLeft());
   pVertices[0].color = color;

   pVertices[1].position = PointToScreen(CPoint(r.right,r.top));
   pVertices[1].color = color;

   pVertices[2].position = PointToScreen(CPoint(r.left,r.bottom));
   pVertices[2].color = color;

   pVertices[3].position = PointToScreen(r.BottomRight());
   pVertices[3].color = color;


   m_pVB->Unlock();

   m_pdTemp3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DrawLine
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a line
///
/// CREATED:     02/06/2004 10:49:26 AM
///
/// PARAMETERS: 
///             POINT p1
///             POINT p2
///             D3DCOLOR color
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawLine(const POINT& p1, const POINT& p2, D3DCOLOR color)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }
   m_pdTemp3dDevice->SetVertexShader( NULL );
   m_pdTemp3dDevice->SetFVF(D3DFVF_SOLIDVERTEX);
   m_pdTemp3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SOLIDVERTEX));

   SOLIDVERTEX* pVertices;

   m_pVB->Lock( 0, 5*sizeof(SOLIDVERTEX), (void**)&pVertices, D3DLOCK_DISCARD );
   pVertices[0].position = PointToScreen(p1);
   pVertices[0].color = color;

   pVertices[1].position = PointToScreen(p2);
   pVertices[1].color = color;
   m_pVB->Unlock();

   m_pdTemp3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DrawGradientRect
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a Gradient Rect
///
/// CREATED:     02/06/2004 10:49:47 AM
///
/// PARAMETERS: 
///             RECT rect
///             D3DCOLOR color1
///             D3DCOLOR color2
///             BYTE flags
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawGradientRect(const RECT& rect, D3DCOLOR color1, D3DCOLOR color2, BYTE flags)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }
   m_pdTemp3dDevice->SetVertexShader( NULL );
   m_pdTemp3dDevice->SetFVF(D3DFVF_SOLIDVERTEX);
   m_pdTemp3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SOLIDVERTEX));

   SOLIDVERTEX* pVertices;

   D3DCOLOR ColorTL = color1;
   D3DCOLOR ColorTR = color1;
   D3DCOLOR ColorBR = color1;
   D3DCOLOR ColorBL = color1;

   if(flags & GR_TL)
      ColorTL = color2;
   if(flags & GR_TR)
      ColorTR = color2;
   if(flags & GR_BR)
      ColorBR = color2;
   if(flags & GR_BL)
      ColorBL = color2;

   CRect r(rect);

   m_pVB->Lock( 0, 4*sizeof(SOLIDVERTEX), (void**)&pVertices, D3DLOCK_DISCARD );
   pVertices[0].position = PointToScreen(r.TopLeft());
   pVertices[0].color = ColorTL;

   pVertices[1].position = PointToScreen(CPoint(r.right,r.top));
   pVertices[1].color = ColorTR;

   pVertices[2].position = PointToScreen(CPoint(r.left,r.bottom));
   pVertices[2].color = ColorBL;

   pVertices[3].position = PointToScreen(r.BottomRight());
   pVertices[3].color = ColorBR;


   m_pVB->Unlock();

   m_pdTemp3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DrawGradientRectBlend
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a Gradient Rect Blended using colors alpha
///
/// CREATED:     02/06/2004 10:50:32 AM
///
/// PARAMETERS: 
///             RECT rect
///             D3DCOLOR color1
///             D3DCOLOR color2
///             BYTE flags
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawGradientRectBlend(const RECT& rect, D3DCOLOR color1, D3DCOLOR color2, BYTE flags)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }
   m_pdTemp3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
   m_pdTemp3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
   m_pdTemp3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
   DrawGradientRect(rect,  color1,  color2,  flags);
   m_pdTemp3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: DrawRect
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a rect
///
/// CREATED:     02/06/2004 10:50:42 AM
///
/// PARAMETERS: 
///             RECT rect
///             D3DCOLOR color
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawRect(const RECT& rect, D3DCOLOR color)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }

   m_pdTemp3dDevice->SetVertexShader( NULL );
   m_pdTemp3dDevice->SetFVF(D3DFVF_SOLIDVERTEX);
   m_pdTemp3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SOLIDVERTEX));

   SOLIDVERTEX* pVertices;

   CRect r(rect);

   m_pVB->Lock( 0, 5*sizeof(SOLIDVERTEX), (void**)&pVertices, D3DLOCK_DISCARD );


   pVertices[0].position = PointToScreen(r.TopLeft());
   pVertices[0].color = color;

   pVertices[1].position = PointToScreen(CPoint(r.left,r.bottom));
   pVertices[1].color = color;

   pVertices[2].position = PointToScreen(r.BottomRight());
   pVertices[2].color = color;

   pVertices[3].position = PointToScreen(CPoint(r.right,r.top));
   pVertices[3].color = color;

   pVertices[4].position = PointToScreen(r.TopLeft());
   pVertices[4].color = color;

   m_pVB->Unlock();

   m_pdTemp3dDevice->DrawPrimitive( D3DPT_LINESTRIP, 0, 4 );

   return S_OK;
}


/////////////////////////////////////////////////
///
/// NAME: RestoreDeviceObjects
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Restores device objects
///
/// CREATED:     02/06/2004 10:51:00 AM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{

   FLOAT newy = FONT_SIZE_Y;//((((float)FONT_SIZE_Y/(float)m_dwHeight)*2.0f));
   FLOAT newx = FONT_SIZE_X;//((((float)FONT_SIZE_X/(float)m_dwWidth)*2.0f));

   if(m_pFont)
      delete m_pFont;

   m_pFont = new CD3DFont( _T("Arial"), FONT_SIZE_Y);
   m_pFont->RestoreDeviceObjects(pd3dDevice);

   HRESULT hr;
   if( FAILED( hr = pd3dDevice->CreateVertexBuffer( 5*sizeof(SOLIDVERTEX), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_SOLIDVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
   {
      sys_msg_hr("CreateVertexBuffer",hr);
      return hr;
   }

   // Create the state blocks for rendering text
   for( UINT which=0; which<2; which++ )
   {
      pd3dDevice->BeginStateBlock();
      pd3dDevice->SetTexture( 0, NULL );

      pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
      pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
      pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
      pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
      pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
      pd3dDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
      pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
      pd3dDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
      pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
      pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
      pd3dDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
      pd3dDevice->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE,    FALSE );
      pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
      pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND,      FALSE );
      pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
      pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
      pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
      //pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
      pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
      pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
      pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
      pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
      pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
      pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
      pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
      pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
      pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
      pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

      if( which==0 )
         pd3dDevice->EndStateBlock( &m_dwSavedStateBlock );
      else
         pd3dDevice->EndStateBlock( &m_dwDrawTextStateBlock );
   }

   return S_OK;
}


/////////////////////////////////////////////////
///
/// NAME: InvalidateDeviceObjects
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Destroys all device-dependent objects
///
/// CREATED:     02/06/2004 10:51:16 AM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::InvalidateDeviceObjects()
{
   m_dwSavedStateBlock    = NULL;
   m_dwDrawTextStateBlock = NULL;

   if(m_pFont)
      m_pFont->InvalidateDeviceObjects();

   m_pdTemp3dDevice = NULL;

   return S_OK;
}

/////////////////////////////////////////////////
///
/// NAME: DeleteDeviceObjects
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Destroys all device-dependent objects
///
/// CREATED:     02/06/2004 10:51:40 AM
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DeleteDeviceObjects()
{
   m_pVB = NULL;

   if(m_pFont)
      m_pFont->DeleteDeviceObjects();

   SAFE_DELETE(m_pFont);
   m_pdTemp3dDevice = NULL;
   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: Start
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Starts the 2D Draw
///
/// CREATED:     02/06/2004 10:52:28 AM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void C2DDraw::Start(LPDIRECT3DDEVICE9 pd3dDevice)
{
   //if(m_pdTemp3dDevice != NULL)
   //{
   //sys_msg("End Not Called");
   //}

   // Setup renderstate
   m_pdTemp3dDevice = pd3dDevice;
   m_dwSavedStateBlock->Capture( );
   m_dwDrawTextStateBlock->Apply( );
}



/////////////////////////////////////////////////
///
/// NAME: End
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Ends the 2D Draw
///
/// CREATED:     02/06/2004 10:52:38 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void C2DDraw::End()
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return;
   }

   // Restore the modified renderstates
   m_dwSavedStateBlock->Apply(  );
   //m_pdTemp3dDevice = NULL; // cash the last used device
}



/////////////////////////////////////////////////
///
/// NAME: DrawDottedRect
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a doted rect
///
/// CREATED:     02/06/2004 10:52:53 AM
///
/// PARAMETERS: 
///             RECT rect
///             D3DCOLOR color
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawDottedRect(const RECT& rect, D3DCOLOR color)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }

   //D3DLINEPATTERN pattern;

   //pattern.wLinePattern = 0xCCCC;
   //pattern.wRepeatFactor = 1;
   HRESULT hr;
   //hr = m_pdTemp3dDevice->SetRenderState(D3DRS_LINEPATTERN,(DWORD)&pattern);


   hr = DrawRect(rect, color);

   //pattern.wLinePattern = 0;
   //pattern.wRepeatFactor = 0;
   //m_pdTemp3dDevice->SetRenderState(D3DRS_LINEPATTERN,(DWORD)&pattern);
   return hr;
}



/////////////////////////////////////////////////
///
/// NAME: DrawDottedLine
///
/// CLASS: C2DDraw
///
/// DESCRIPTION: Draws a dotted line
///
/// CREATED:     02/06/2004 10:53:04 AM
///
/// PARAMETERS: 
///             POINT p1
///             POINT p2
///             D3DCOLOR color
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT C2DDraw::DrawDottedLine(const POINT& p1, const POINT& p2, D3DCOLOR color)
{
   if(m_pdTemp3dDevice == NULL)
   {
      sys_msg("Start Not Called");
      return E_FAIL;
   }

   //D3DLINEPATTERN pattern;

   //pattern.wLinePattern = 0xCCCC;
   //pattern.wRepeatFactor = 1;
   //m_pdTemp3dDevice->SetRenderState(D3DRS_LINEPATTERN,(DWORD)&pattern);

   HRESULT hr;
   hr = DrawLine( p1,  p2,  color);


   //pattern.wLinePattern = 0;
   //pattern.wRepeatFactor = 0;
   //m_pdTemp3dDevice->SetRenderState(D3DRS_LINEPATTERN,(DWORD)&pattern);
   return hr;
}


/*HRESULT C2DDraw::CreateSurfaceFromBitmap(CSurface** surface, const TCHAR* filename, DWORD width,DWORD height, CZipArchive* pZip)
{
   *surface = new CSurface();
   return (*surface)->Init(filename,width,height,GetDevice(),pZip);
}*/

HRESULT C2DDraw::Blt(DWORD x, DWORD y, CSurface* surface, RECT *prc)
{
   return surface->Draw(GetTempDevice(),x,y,prc);
}
