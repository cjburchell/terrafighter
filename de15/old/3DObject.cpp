// 3DObject.cpp: implementation of the C3DObject class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <d3d8.h>
#include <dxerr8.h>
#include <D3dx8math.h>
#include "dxutil.h"
#include "3DObject.h"
#include "D3DUtil.h"
#include "3DDisplay.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The custom FVF, which describes the custom vertex structure.
#define D3DFVF_OBJ3DVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct OBJ3DVERTEX
{
    D3DXVECTOR3 position; // The position.
    D3DCOLOR    color;    // The color.
    FLOAT       tu, tv;   // The texture coordinates.
};

C3DObject::C3DObject() : CSurface()
{
	m_VertexBuffer = NULL;
	m_Texture = NULL;
	m_bColorKey = FALSE;
}

C3DObject::~C3DObject()
{
	SAFE_RELEASE( m_VertexBuffer );
	SAFE_RELEASE( m_Texture);
	
}

HRESULT C3DObject::SetColorKey(DWORD dwColorKey)
{
	m_bColorKey = TRUE;
	D3DCOLOR c = D3DCOLOR_XRGB(GetRValue(dwColorKey),GetGValue(dwColorKey),GetBValue(dwColorKey));
	D3DSURFACE_DESC pDesc;
	m_Texture->GetLevelDesc(0, &pDesc );
	
    D3DLOCKED_RECT d3dlr;
    m_Texture->LockRect( 0, &d3dlr, 0, 0 );
	DWORD * pDst = (DWORD *)d3dlr.pBits;
	int DPitch = d3dlr.Pitch/4;

	for (int y=0; y<pDesc.Height; ++y)
	{
		for (int x=0; x<pDesc.Width; ++x)
		{
			int index = y*DPitch + x;
			if(pDst[index] != c)
			{
			   pDst[index] = 0xff000000 | pDst[index];
			}
			else
			{
			   pDst[index] = 0x00ffffff & pDst[index];
			}
		}
	}

	m_Texture->UnlockRect (0);
	return S_OK;
}

HRESULT C3DObject::Init(LPCSTR strBMP,DWORD dwDesiredWidth,DWORD dwDesiredHeight,LPDIRECT3DDEVICE8	pd3dDevice)
{
	m_dwWidth = dwDesiredWidth;
	m_dwHeight = dwDesiredHeight;
	HRESULT hr;
	hr = D3DUtil_CreateTexture(pd3dDevice,(char*)strBMP,&m_Texture,D3DFMT_A8R8G8B8);
	if FAILED(hr)
	{
		DXTRACE_ERR_NOMSGBOX("D3DUtil_CreateTexture",hr);
		return hr;
	}

	if( FAILED( hr = pd3dDevice->CreateVertexBuffer( 4*sizeof(OBJ3DVERTEX), 0, D3DFVF_OBJ3DVERTEX, D3DPOOL_DEFAULT, &m_VertexBuffer ) ) )
    {
		DXTRACE_ERR_NOMSGBOX("CreateVertexBuffer",hr);
		SAFE_RELEASE( m_Texture);
        return hr;
    }
	return S_OK;
}

HRESULT C3DObject::Draw(DWORD x,DWORD y,RECT *prc,LPDIRECT3DDEVICE8 pd3dDevice, CDisplay* pDisplay)
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


	pd3dDevice->SetVertexShader( D3DFVF_OBJ3DVERTEX );
	pd3dDevice->SetStreamSource(0, m_VertexBuffer, sizeof(OBJ3DVERTEX));

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

	m_VertexBuffer->Lock( 0, 4*sizeof(OBJ3DVERTEX), (BYTE**)&pVertices, D3DLOCK_DISCARD );
	pVertices[0].position = static_cast<C3DDisplay*>(pDisplay)->PointToScreen(r.TopLeft());
	pVertices[0].color = 0xffffffff;
	pVertices[0].tu=uoffest;
	pVertices[0].tv=voffest;

	pVertices[1].position = static_cast<C3DDisplay*>(pDisplay)->PointToScreen(CPoint(r.right,r.top));
	pVertices[1].color = 0xffffffff;
	pVertices[1].tu=uoffest2;
	pVertices[1].tv=voffest;

	pVertices[2].position = static_cast<C3DDisplay*>(pDisplay)->PointToScreen(CPoint(r.left,r.bottom));
	pVertices[2].color = 0xffffffff;
	pVertices[2].tu=uoffest;
	pVertices[2].tv=voffest2;

	pVertices[3].position = static_cast<C3DDisplay*>(pDisplay)->PointToScreen(r.BottomRight());
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
