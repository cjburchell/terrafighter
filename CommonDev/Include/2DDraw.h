///////////////////////////////////////////////////////////////////////////////
///  2DDraw.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 10:42:01 AM by Christiaan Burchell
///
///  PURPOSE:  The 2D drawing helper class
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

// 2DDraw.h: interface for the C2DDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DDRAW_H__BBB65081_5B7E_4551_902E_2CB02612E99A__INCLUDED_)
#define AFX_2DDRAW_H__BBB65081_5B7E_4551_902E_2CB02612E99A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afx.h>
#include <d3d9.h>
#include <D3dx9math.h>
#include "D3DFont.h"
#include "surface.h"

#define GR_TL 0x01
#define GR_TR 0x02
#define GR_BR 0x04
#define GR_BL 0x08

#define FONT_SIZE_X 8
#define FONT_SIZE_Y 13

#define D3DFVF_SOLIDVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

struct SOLIDVERTEX
{
   D3DXVECTOR4 position; // The position.
   D3DCOLOR    color;    // The color.
};

class C2DDraw  
{
public:
   void End();
   void Start(LPDIRECT3DDEVICE9 pd3dDevice);
   void SetScreenSize(DWORD dwHeight,DWORD dwWidth);
   C2DDraw();
   virtual ~C2DDraw();
   DWORD GetScreenHight() {return m_dwHeight;}
   DWORD GetScreenWidth() {return m_dwWidth;}
   static D3DXVECTOR4 PointToScreen(const POINT& p);
   static D3DXVECTOR4 PointToScreen(D3DXVECTOR2 p);
   void OutTextc(D3DCOLOR crBackground, D3DCOLOR crForeground,int x, int y, BOOL bTrasparent, char* format, ...);
   const RECT& GetTextRect(const TCHAR* str);
   HRESULT DrawBlendRect(const RECT& rect, D3DCOLOR color);
   HRESULT DrawFilledRect(const RECT& rect, D3DCOLOR color);
   HRESULT DrawLine(const POINT& p1, const POINT& p2, D3DCOLOR color);
   HRESULT DrawGradientRect(const RECT& rect, D3DCOLOR color1, D3DCOLOR color2, BYTE flags);
   HRESULT DrawGradientRectBlend(const RECT& rect, D3DCOLOR color1, D3DCOLOR color2, BYTE flags);
   HRESULT DrawRect(const RECT& rect, D3DCOLOR color);
   HRESULT DrawFilledQuad(const CPoint& tl,const CPoint& bl,const CPoint& tr,const CPoint& br, D3DCOLOR color);
   HRESULT DrawBlendQuad (const CPoint& tl,const CPoint& bl,const CPoint& tr,const CPoint& br, D3DCOLOR color);
   HRESULT DrawQuadOutline(const CPoint& tl,const CPoint& bl,const CPoint& tr,const CPoint& br, D3DCOLOR color);

   HRESULT DrawDottedLine(const POINT& p1, const POINT& p2, D3DCOLOR color);
   HRESULT DrawDottedRect(const RECT& rect, D3DCOLOR color);



   HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
   HRESULT InvalidateDeviceObjects();
   HRESULT DeleteDeviceObjects();

   CComPtr<IDirect3DDevice9> GetTempDevice(){return m_pdTemp3dDevice;}

   HRESULT Blt( DWORD x, DWORD y, CSurface* surface, RECT *prc);

private:
   DWORD m_dwWidth; // screen width
   DWORD m_dwHeight; // screen hight
   CD3DFont* m_pFont;
   CComPtr<IDirect3DDevice9> m_pdTemp3dDevice; // temp pointer to the device object 
   CComPtr<IDirect3DVertexBuffer9>    m_pVB;
   // Stateblocks for setting and restoring render states
   CComPtr<IDirect3DStateBlock9>   m_dwSavedStateBlock;
   CComPtr<IDirect3DStateBlock9>   m_dwDrawTextStateBlock;

};

#endif // !defined(AFX_2DDRAW_H__BBB65081_5B7E_4551_902E_2CB02612E99A__INCLUDED_)
