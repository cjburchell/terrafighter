//-----------------------------------------------------------------------------
// File: D3DFont.h
//
// Desc: Texture-based font class
//
// Copyright (c) 1999-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef D3DFONT_H
#define D3DFONT_H
#include <tchar.h>
#include <D3D9.h>
#include <atlcomcli.h>



// Font creation flags
#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002
#define D3DFONT_ZENABLE     0x0004

// Font rendering flags
#define D3DFONT_CENTERED    0x0001
#define D3DFONT_TWOSIDED    0x0002
#define D3DFONT_FILTERED    0x0004




//-----------------------------------------------------------------------------
// Name: class CD3DFont
// Desc: Texture-based font class for doing text in a 3D scene.
//-----------------------------------------------------------------------------
class CD3DFont
{
   TCHAR   m_strFontName[80];            // Font properties
   DWORD   m_dwFontHeight;
   DWORD   m_dwFontFlags;

   //LPDIRECT3DDEVICE8       m_pd3dDevice; // A D3DDevice used for rendering
   CComPtr<IDirect3DTexture9>      m_pTexture;   // The d3d texture for this font
   CComPtr<IDirect3DVertexBuffer9> m_pVB;        // VertexBuffer for rendering text
   DWORD   m_dwTexWidth;                 // Texture dimensions
   DWORD   m_dwTexHeight;
   FLOAT   m_fTextScale;
   FLOAT   m_fTexCoords[128-32][4];

   // Stateblocks for setting and restoring render states
   CComPtr<IDirect3DStateBlock9>   m_dwSavedStateBlock;
   CComPtr<IDirect3DStateBlock9>   m_dwDrawTextStateBlock;
   HRESULT InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
public:
   // 2D and 3D text drawing functions
   HRESULT DrawText(LPDIRECT3DDEVICE9 pd3dDevice,  FLOAT x, FLOAT y, DWORD dwColor, 
      const TCHAR* strText, DWORD dwFlags=0L );
   HRESULT DrawTextScaled(LPDIRECT3DDEVICE9 pd3dDevice,  FLOAT x, FLOAT y, FLOAT z, 
      FLOAT fXScale, FLOAT fYScale, DWORD dwColor, 
      TCHAR* strText, DWORD dwFlags=0L );
   HRESULT Render3DText(LPDIRECT3DDEVICE9 pd3dDevice,  TCHAR* strText, DWORD dwFlags=0L );

   // Function to get extent of text
   HRESULT GetTextExtent( const char* strText, SIZE* pSize );

   // Initializing and destroying device-dependent objects
   HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
   HRESULT InvalidateDeviceObjects();
   HRESULT DeleteDeviceObjects();

   // Constructor / destructor
   CD3DFont( TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L );
   ~CD3DFont();
};




#endif


