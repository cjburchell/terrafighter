///////////////////////////////////////////////////////////////////////////////
///  SkyBox.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 4:37:05 PM by Christiaan Burchell
///
///  PURPOSE:  Skybox Object
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

// SkyBox.h: interface for the CSkyBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKYBOX_H__27BA2150_7098_4CCD_8106_000D7E23E6B3__INCLUDED_)
#define AFX_SKYBOX_H__27BA2150_7098_4CCD_8106_000D7E23E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3dutil.h"
#include "D3DFile.h"

class CSkyBox  
{
private:
   CComPtr<IDirect3DVertexBuffer9> m_pVB;
   CComPtr<IDirect3DIndexBuffer9>  m_pIB;
   CComPtr<IDirect3DTexture9> m_pTexture;

   FLOAT m_PlanetRadius;
   FLOAT m_AtmosphereRadius;
   FLOAT m_HTileFactor;
   FLOAT m_VTileFactor;
   DWORD m_NumVerts;
   DWORD m_NumIndices;
   CString m_TexFile;
   FLOAT m_PlaneSize;

public:
   void SetTex(CString tex)
   {
      m_TexFile = tex;
   }
   HRESULT Create(LPDIRECT3DDEVICE9 pd3dDevice,
      WORD divisions,
      FLOAT planetRadius,
      FLOAT atmosphereRadius,
      FLOAT hTilefactor,
      FLOAT vTilefactor,
      CZipArchive* pZip);
   HRESULT DeleteDeviceObjects();
   HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,const D3DXMATRIX& MatView,DWORD& nTri);
   CSkyBox();
   virtual ~CSkyBox();

};

#endif // !defined(AFX_SKYBOX_H__27BA2150_7098_4CCD_8106_000D7E23E6B3__INCLUDED_)
