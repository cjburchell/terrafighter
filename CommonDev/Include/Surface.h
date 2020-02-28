///////////////////////////////////////////////////////////////////////////////
///  Surface.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 11:43:29 AM by Christiaan Burchell
///
///  PURPOSE:  interface for the CSurface class.
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

// 3DObject.h: 
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DOBJECT_H__22EEFBFD_DFF0_467B_8FBA_05795132B155__INCLUDED_)
#define AFX_3DOBJECT_H__22EEFBFD_DFF0_467B_8FBA_05795132B155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZipArchive.h"

class CSurface  
{
public:

   HRESULT BeginScene();
   HRESULT EndScene();

   void DeleteDeviceObjects();
   HRESULT Draw(LPDIRECT3DDEVICE9 pd3dDevice,DWORD x, DWORD y,RECT *prc, BOOL bBlend = FALSE);
   HRESULT DrawAngle(LPDIRECT3DDEVICE9 pd3dDevice,DWORD x,DWORD y,RECT *prc, FLOAT Angel);
   HRESULT Init(LPCSTR strBMP,DWORD dwDesiredWidth,DWORD dwDesiredHeight,LPDIRECT3DDEVICE9	pd3dDevice,CZipArchive* pZip);

   HRESULT CreateRenderSurface(DWORD dwDesiredWidth,DWORD dwDesiredHeight,LPDIRECT3DDEVICE9 pd3dDevice,D3DFORMAT Stencil);

   HRESULT SetColorKey( DWORD dwColorKey );
   HRESULT Clear(DWORD dwColor);
   CSurface();
   virtual ~CSurface();

   BOOL m_bColorKey;
private:
   CComPtr<IDirect3DVertexBuffer9>	m_VertexBuffer;
   CComPtr<IDirect3DTexture9>	m_Texture;

   CComPtr<IDirect3DSurface9> m_pTexSurf;
   CComPtr<ID3DXRenderToSurface> m_pRenderToSurface;

   DWORD m_dwHeight;
   DWORD m_dwWidth;

};

#endif // !defined(AFX_3DOBJECT_H__22EEFBFD_DFF0_467B_8FBA_05795132B155__INCLUDED_)
