// 2DFont.cpp: implementation of the C2DFont class.
//
//////////////////////////////////////////////////////////////////////
#include <D3DX8.h>
#include "2DFont.h"
#include "dxutil.h"
#include <dxerr8.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct FONT2DVERTEX
{
    D3DXVECTOR3 position; // The position.
    D3DCOLOR    color;    // The color.
    FLOAT       tu, tv;   // The texture coordinates.
};



C2DFont::C2DFont( TCHAR* strFontName, FLOAT dwHeight, FLOAT dwWidth)
{
	_tcscpy( m_strFontName, strFontName );
    m_fFontHeight         = dwHeight;
    m_fFontWidth          = dwWidth;

    m_pd3dDevice           = NULL;
    m_pTexture             = NULL;
    m_VertexBuffer         = NULL;
}

C2DFont::~C2DFont()
{
	SAFE_RELEASE( m_VertexBuffer);
	SAFE_RELEASE( m_pTexture );
    m_pd3dDevice = NULL;
}

HRESULT C2DFont::Init(LPDIRECT3DDEVICE8 pd3dDevice)
{
	HRESULT hr;
	m_pd3dDevice = pd3dDevice;
	DWORD dwTexWidth, dwTexHeight;
	dwTexHeight = dwTexWidth = 256;
	hr = m_pd3dDevice->CreateTexture( dwTexWidth, dwTexHeight, 1,
                                      0, D3DFMT_A4R4G4B4,
                                      D3DPOOL_MANAGED, &m_pTexture );
	if( FAILED(hr) )
	{
		DXTRACE_ERR_NOMSGBOX("CreateTexture",hr);
        return hr;
	}

	// Prepare to create a bitmap
    DWORD*      pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       =  (int)dwTexWidth;
    bmi.bmiHeader.biHeight      = -(int)dwTexHeight;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    // Create a DC and a bitmap for the font
    HDC     hDC       = CreateCompatibleDC( NULL );
    HBITMAP hbmBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,
                                          (VOID**)&pBitmapBits, NULL, 0 );
    SetMapMode( hDC, MM_TEXT );

    // Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
    // antialiased font, but this is not guaranteed.
    INT nHeight    = 12;
    DWORD dwBold   = FW_NORMAL;
    DWORD dwItalic = FALSE;
    HFONT hFont    = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, m_strFontName );
    if( NULL==hFont )
        return E_FAIL;

    SelectObject( hDC, hbmBitmap );
    SelectObject( hDC, hFont );

    // Set text properties
    SetTextColor( hDC, RGB(255,255,255) );
    SetBkColor(   hDC, 0x00000000 );
    SetTextAlign( hDC, TA_TOP );

    // Loop through all printable character and output them to the bitmap..
    // Meanwhile, keep track of the corresponding tex coords for each character.
    DWORD x = 0;
    DWORD y = 0;
    TCHAR str[2] = _T("x");
    SIZE size;

    for( TCHAR c=32; c<127; c++ )
    {
        str[0] = c;
        GetTextExtentPoint32( hDC, str, 1, &size );

        if( (DWORD)(x+size.cx+1) > dwTexWidth )
        {
            x  = 0;
            y += size.cy+1;
        }

        ExtTextOut( hDC, x+0, y+0, ETO_OPAQUE, NULL, str, 1, NULL );

        m_fTexCoords[c-32][0] = ((FLOAT)(x+0))/dwTexWidth;
        m_fTexCoords[c-32][1] = ((FLOAT)(y+0))/dwTexHeight;
        m_fTexCoords[c-32][2] = ((FLOAT)(x+0+size.cx))/dwTexWidth;
        m_fTexCoords[c-32][3] = ((FLOAT)(y+0+size.cy))/dwTexHeight;

        x += size.cx+1;
    }

    // Lock the surface and write the alpha values for the set pixels
    D3DLOCKED_RECT d3dlr;
    m_pTexture->LockRect( 0, &d3dlr, 0, 0 );
    WORD* pDst16 = (WORD*)d3dlr.pBits;
    BYTE bAlpha; // 4-bit measure of pixel intensity

    for( y=0; y < dwTexHeight; y++ )
    {
        for( x=0; x < dwTexWidth; x++ )
        {
            bAlpha = (BYTE)((pBitmapBits[dwTexWidth*y + x] & 0xff) >> 4);
            if (bAlpha > 0)
            {
                *pDst16++ = (bAlpha << 12) | 0x0fff;
            }
            else
            {
                *pDst16++ = 0x0000;
            }
        }
    }

    // Done updating texture, so clean up used objects
    m_pTexture->UnlockRect(0);
    DeleteObject( hbmBitmap );
    DeleteDC( hDC );
    DeleteObject( hFont );

	if( FAILED( hr = m_pd3dDevice->CreateVertexBuffer( 4*sizeof(FONT2DVERTEX), 0, D3DFVF_FONT2DVERTEX, D3DPOOL_DEFAULT, &m_VertexBuffer ) ) )
    {
		SAFE_RELEASE( m_pTexture );
        return hr;
    }



	return S_OK;
}

HRESULT C2DFont::DrawText( FLOAT sx, FLOAT sy, DWORD dwColor, TCHAR* strText)
{
	// Set diffuse blending for alpha set in vertices.
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// Enable alpha testing (skips pixels with less than a certain alpha.)
	//if( m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
	{
		m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
		m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
		m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	}

	m_pd3dDevice->SetVertexShader( D3DFVF_FONT2DVERTEX );
	m_pd3dDevice->SetStreamSource(0, m_VertexBuffer, sizeof(FONT2DVERTEX));
	m_pd3dDevice->SetTexture( 0, m_pTexture );
	FLOAT w = m_fFontWidth;
	FLOAT h = m_fFontHeight;

	FONT2DVERTEX* pVertices;
	while( *strText )
    {
		TCHAR c = *strText++;

		if( c < _T(' ') )
		{
			sx += w;
            continue;
		}

		FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT ty1 = m_fTexCoords[c-32][1];
        FLOAT tx2 = m_fTexCoords[c-32][2];
        FLOAT ty2 = m_fTexCoords[c-32][3];

		m_VertexBuffer->Lock( 0, 4*sizeof(FONT2DVERTEX), (BYTE**)&pVertices, D3DLOCK_DISCARD );
		pVertices[0].position = D3DXVECTOR3(sx,sy,0.5f); //topleft
		pVertices[0].color = dwColor;
		pVertices[0].tu=tx1;
		pVertices[0].tv=ty1;

		pVertices[1].position = D3DXVECTOR3(sx+w,sy,0.5f);; //topright
		pVertices[1].color = dwColor;
		pVertices[1].tu=tx2;
		pVertices[1].tv=ty1;

		pVertices[2].position = D3DXVECTOR3(sx,sy-h,0.5f);; //bottom left
		pVertices[2].color = dwColor;
		pVertices[2].tu=tx1;
		pVertices[2].tv=ty2;

		pVertices[3].position = D3DXVECTOR3(sx+w,sy-h,0.5f); //bottom right
		pVertices[3].color = dwColor;
		pVertices[3].tu=tx2;
		pVertices[3].tv=ty2;
		m_VertexBuffer->Unlock();
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		sx += w;
	}

	m_pd3dDevice->SetTexture( 0, NULL );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
	return S_OK;
}


