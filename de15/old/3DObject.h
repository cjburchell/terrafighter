// 3DObject.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DOBJECT_H__22EEFBFD_DFF0_467B_8FBA_05795132B155__INCLUDED_)
#define AFX_3DOBJECT_H__22EEFBFD_DFF0_467B_8FBA_05795132B155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2DDraw.h"


class C3DObject : public CSurface  
{
public:
	HRESULT Draw(DWORD x, DWORD y,RECT *prc,LPDIRECT3DDEVICE8 pd3dDevice,CDisplay* pDisplay);
	HRESULT Init(LPCSTR strBMP,DWORD dwDesiredWidth,DWORD dwDesiredHeight,LPDIRECT3DDEVICE8	pd3dDevice);
	HRESULT SetColorKey( DWORD dwColorKey );
	C3DObject();
	virtual ~C3DObject();
private:
	LPDIRECT3DVERTEXBUFFER8	m_VertexBuffer;
	LPDIRECT3DTEXTURE8	m_Texture;

	BOOL m_bColorKey;

	DWORD m_dwHeight;
	DWORD m_dwWidth;
	DWORD   m_dwSavedStateBlock;

};

#endif // !defined(AFX_3DOBJECT_H__22EEFBFD_DFF0_467B_8FBA_05795132B155__INCLUDED_)
