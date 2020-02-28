// 2DFont.h: interface for the C2DFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DFONT_H__3FACAC53_E880_470F_A82A_A38808BD9172__INCLUDED_)
#define AFX_2DFONT_H__3FACAC53_E880_470F_A82A_A38808BD9172__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <tchar.h>
#include <D3D8.h>

#define FONT_SIZE_X 8
#define FONT_SIZE_Y 13

class C2DFont  
{
public:
	void DeleteDeviceObjects();
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	HRESULT DrawText( FLOAT sx, FLOAT sy, DWORD dwColor, TCHAR* strText);
	C2DFont(TCHAR* strFontName, FLOAT fHeight, FLOAT fWidth);
	virtual ~C2DFont();
private:
	TCHAR   m_strFontName[80];            // Font properties

	//LPDIRECT3DDEVICE8       m_pd3dDevice; // A D3DDevice used for rendering
    LPDIRECT3DTEXTURE8      m_pTexture;   // The d3d texture for this font
    LPDIRECT3DVERTEXBUFFER8 m_VertexBuffer; // VertexBuffer for rendering text
	FLOAT   m_fFontHeight;
	FLOAT   m_fFontWidth;
    FLOAT   m_fTexCoords[128-32][4];

};

#endif // !defined(AFX_2DFONT_H__3FACAC53_E880_470F_A82A_A38808BD9172__INCLUDED_)
