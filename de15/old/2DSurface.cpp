// 2DSurface.cpp: implementation of the C2DSurface class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "2DSurface.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2DSurface::C2DSurface() : CSurface()
{
	m_pdds = NULL;
}

C2DSurface::~C2DSurface()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DSurface::Create( LPDIRECTDRAWSURFACE7 pdds )
{
    m_pdds = pdds;

    if( m_pdds )
    {
        m_pdds->AddRef();

        // Get the DDSURFACEDESC structure for this surface
        m_ddsd.dwSize = sizeof(m_ddsd);
        m_pdds->GetSurfaceDesc( &m_ddsd );
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DSurface::Create( LPDIRECTDRAW7 pDD, DDSURFACEDESC2* pddsd )
{
    HRESULT hr;

    // Create the DDraw surface
    if( FAILED( hr = pDD->CreateSurface( pddsd, &m_pdds, NULL ) ) )
        return hr;

    // Prepare the DDSURFACEDESC structure
    m_ddsd.dwSize = sizeof(m_ddsd);

    // Get the DDSURFACEDESC structure for this surface
    m_pdds->GetSurfaceDesc( &m_ddsd );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DSurface::Destroy()
{
    SAFE_RELEASE( m_pdds );
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSurface::DrawBitmap()
// Desc: Draws a bitmap over an entire DirectDrawSurface, stretching the 
//       bitmap if nessasary
//-----------------------------------------------------------------------------
HRESULT C2DSurface::DrawBitmap( HBITMAP hBMP, 
                              DWORD dwBMPOriginX, DWORD dwBMPOriginY, 
                              DWORD dwBMPWidth, DWORD dwBMPHeight )
{
    HDC            hDCImage;
    HDC            hDC;
    BITMAP         bmp;
    DDSURFACEDESC2 ddsd;
    HRESULT        hr;

    if( hBMP == NULL || m_pdds == NULL )
        return E_INVALIDARG;

    // Make sure this surface is restored.
    if( FAILED( hr = m_pdds->Restore() ) )
        return hr;

    // Get the surface.description
    ddsd.dwSize  = sizeof(ddsd);
    m_pdds->GetSurfaceDesc( &ddsd );

    if( ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC )
        return E_NOTIMPL;

    // Select bitmap into a memoryDC so we can use it.
    hDCImage = CreateCompatibleDC( NULL );
    if( NULL == hDCImage )
        return E_FAIL;

    SelectObject( hDCImage, hBMP );

    // Get size of the bitmap
    GetObject( hBMP, sizeof(bmp), &bmp );

    // Use the passed size, unless zero
    dwBMPWidth  = ( dwBMPWidth  == 0 ) ? bmp.bmWidth  : dwBMPWidth;     
    dwBMPHeight = ( dwBMPHeight == 0 ) ? bmp.bmHeight : dwBMPHeight;

    // Stretch the bitmap to cover this surface
    if( FAILED( hr = m_pdds->GetDC( &hDC ) ) )
        return hr;

    StretchBlt( hDC, 0, 0, 
                ddsd.dwWidth, ddsd.dwHeight, 
                hDCImage, dwBMPOriginX, dwBMPOriginY,
                dwBMPWidth, dwBMPHeight, SRCCOPY );

    if( FAILED( hr = m_pdds->ReleaseDC( hDC ) ) )
        return hr;

    DeleteDC( hDCImage );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C2DSurface::DrawText()
// Desc: Draws a text string on a DirectDraw surface using hFont or the default
//       GDI font if hFont is NULL.  
//-----------------------------------------------------------------------------
HRESULT C2DSurface::DrawText( HFONT hFont, LPCSTR strText, 
                            DWORD dwOriginX, DWORD dwOriginY,
                            COLORREF crBackground, COLORREF crForeground )
{
    HDC     hDC = NULL;
    HRESULT hr;

    if( m_pdds == NULL || strText == NULL )
        return E_INVALIDARG;

    // Make sure this surface is restored.
    if( FAILED( hr = m_pdds->Restore() ) )
        return hr;

    if( FAILED( hr = m_pdds->GetDC( &hDC ) ) )
        return hr;

    // Set the background and foreground color
    SetBkColor( hDC, crBackground );
    SetTextColor( hDC, crForeground );

    if( hFont )
        SelectObject( hDC, hFont );

    // Use GDI to draw the text on the surface
    TextOut( hDC, dwOriginX, dwOriginY, strText, _tcslen(strText) );

    if( FAILED( hr = m_pdds->ReleaseDC( hDC ) ) )
        return hr;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C2DSurface::ReDrawBitmapOnSurface()
// Desc: Load a bitmap from a file or resource into a DirectDraw surface.
//       normaly used to re-load a surface after a restore.
//-----------------------------------------------------------------------------

HRESULT C2DSurface::DrawBitmap( TCHAR* strBMP, 
                              DWORD dwDesiredWidth, DWORD dwDesiredHeight  )
{
    HBITMAP hBMP;
    HRESULT hr;

    if( m_pdds == NULL || strBMP == NULL )
        return E_INVALIDARG;

    //  Try to load the bitmap as a resource, if that fails, try it as a file
    hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), strBMP, 
                                IMAGE_BITMAP, dwDesiredWidth, dwDesiredHeight, 
                                LR_CREATEDIBSECTION );
    if( hBMP == NULL )
    {
        hBMP = (HBITMAP) LoadImage( NULL, strBMP, IMAGE_BITMAP, 
                                    dwDesiredWidth, dwDesiredHeight, 
                                    LR_LOADFROMFILE | LR_CREATEDIBSECTION );
        if( hBMP == NULL )
            return E_FAIL;
    }

    // Draw the bitmap on this surface
    if( FAILED( hr = DrawBitmap( hBMP, 0, 0, 0, 0 ) ) )
    {
        DeleteObject( hBMP );
        return hr;
    }

    DeleteObject( hBMP );

    return S_OK;
}

HRESULT C2DSurface::DrawBitmapAt( TCHAR* strBMP, DWORD dwDestX, DWORD dwDestY,DWORD dwDesiredWidth, DWORD dwDesiredHeight )
{
	HBITMAP hBMP;
    HRESULT hr;

    if( m_pdds == NULL || strBMP == NULL )
        return E_INVALIDARG;

    //  Try to load the bitmap as a resource, if that fails, try it as a file
    hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), strBMP, 
                                IMAGE_BITMAP, dwDesiredWidth, dwDesiredHeight, 
                                LR_CREATEDIBSECTION );
    if( hBMP == NULL )
    {
        hBMP = (HBITMAP) LoadImage( NULL, strBMP, IMAGE_BITMAP, 
                                    dwDesiredWidth, dwDesiredHeight, 
                                    LR_LOADFROMFILE | LR_CREATEDIBSECTION );
        if( hBMP == NULL )
            return E_FAIL;
    }

    // Draw the bitmap on this surface
	HDC            hDCImage;
    HDC            hDC;
    BITMAP         bmp;
    DDSURFACEDESC2 ddsd;
	DWORD dwBMPOriginX = 0;
    DWORD dwBMPOriginY = 0; 
    DWORD dwBMPWidth = 0;
    DWORD dwBMPHeight = 0;

    if( hBMP == NULL || m_pdds == NULL )
        return E_INVALIDARG;

    // Make sure this surface is restored.
    if( FAILED( hr = m_pdds->Restore() ) )
        return hr;

    // Get the surface.description
    ddsd.dwSize  = sizeof(ddsd);
    m_pdds->GetSurfaceDesc( &ddsd );

    if( ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC )
        return E_NOTIMPL;

    // Select bitmap into a memoryDC so we can use it.
    hDCImage = CreateCompatibleDC( NULL );
    if( NULL == hDCImage )
        return E_FAIL;

    SelectObject( hDCImage, hBMP );

    // Get size of the bitmap
    GetObject( hBMP, sizeof(bmp), &bmp );

    // Use the passed size, unless zero
    dwBMPWidth  = ( dwBMPWidth  == 0 ) ? bmp.bmWidth  : dwBMPWidth;     
    dwBMPHeight = ( dwBMPHeight == 0 ) ? bmp.bmHeight : dwBMPHeight;

    // Stretch the bitmap to cover this surface
    if( FAILED( hr = m_pdds->GetDC( &hDC ) ) )
        return hr;

    StretchBlt( hDC, dwDestX, dwDestY, 
                bmp.bmWidth, bmp.bmHeight, 
                hDCImage, dwBMPOriginX, dwBMPOriginY,
                dwBMPWidth, dwBMPHeight, SRCCOPY );

    if( FAILED( hr = m_pdds->ReleaseDC( hDC ) ) )
        return hr;

    DeleteDC( hDCImage );
    DeleteObject( hBMP );

    return S_OK;
}



//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DSurface::SetColorKey( DWORD dwColorKey )
{
    if( NULL == m_pdds )
        return E_POINTER;

    m_bColorKeyed = TRUE;

    DDCOLORKEY ddck;
    ddck.dwColorSpaceLowValue  = ConvertGDIColor( dwColorKey );
    ddck.dwColorSpaceHighValue = ConvertGDIColor( dwColorKey );
    
    return m_pdds->SetColorKey( DDCKEY_SRCBLT, &ddck );
}





//-----------------------------------------------------------------------------
// Name: C2DSurface::ConvertGDIColor()
// Desc: Converts a GDI color (0x00bbggrr) into the equivalent color on a 
//       DirectDrawSurface using its pixel format.  
//-----------------------------------------------------------------------------
DWORD C2DSurface::ConvertGDIColor( COLORREF dwGDIColor )
{
    if( m_pdds == NULL )
	    return 0x00000000;

    COLORREF       rgbT;
    HDC            hdc;
    DWORD          dw = CLR_INVALID;
    DDSURFACEDESC2 ddsd;
    HRESULT        hr;

    //  Use GDI SetPixel to color match for us
    if( dwGDIColor != CLR_INVALID && m_pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
        SetPixel(hdc, 0, 0, dwGDIColor);       // Set our value
        m_pdds->ReleaseDC(hdc);
    }

    // Now lock the surface so we can read back the converted color
    ddsd.dwSize = sizeof(ddsd);
    hr = m_pdds->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );
    if( hr == DD_OK)
    {
        dw = *(DWORD *) ddsd.lpSurface; 
        if( ddsd.ddpfPixelFormat.dwRGBBitCount < 32 ) // Mask it to bpp
            dw &= ( 1 << ddsd.ddpfPixelFormat.dwRGBBitCount ) - 1;  
        m_pdds->Unlock(NULL);
    }

    //  Now put the color that was there back.
    if( dwGDIColor != CLR_INVALID && m_pdds->GetDC(&hdc) == DD_OK )
    {
        SetPixel( hdc, 0, 0, rgbT );
        m_pdds->ReleaseDC(hdc);
    }
    
    return dw;    
}




//-----------------------------------------------------------------------------
// Name: C2DSurface::GetBitMaskInfo()
// Desc: Returns the number of bits and the shift in the bit mask
//-----------------------------------------------------------------------------
HRESULT C2DSurface::GetBitMaskInfo( DWORD dwBitMask, DWORD* pdwShift, DWORD* pdwBits )
{
    DWORD dwShift = 0;
    DWORD dwBits  = 0; 

    if( pdwShift == NULL || pdwBits == NULL )
        return E_INVALIDARG;

    if( dwBitMask )
    {
        while( (dwBitMask & 1) == 0 )
        {
            dwShift++;
            dwBitMask >>= 1;
        }
    }

    while( (dwBitMask & 1) != 0 )
    {
        dwBits++;
        dwBitMask >>= 1;
    }

    *pdwShift = dwShift;
    *pdwBits  = dwBits;

    return S_OK;
}

/*//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DSurface::Blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds, RECT* prc,
                       DWORD dwFlags )
{
    if( NULL == GetDDrawSurface() )
        return E_POINTER;

    return GetDDrawSurface()->BltFast( x, y, pdds, prc, dwFlags );
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DSurface::Blt( DWORD x, DWORD y, C2DSurface* pSurface, RECT* prc )
{
    if( NULL == pSurface )
        return E_INVALIDARG;

    if( pSurface->IsColorKeyed() )
        return Blt( x, y, pSurface->GetDDrawSurface(), prc, DDBLTFAST_SRCCOLORKEY );
    else
        return Blt( x, y, pSurface->GetDDrawSurface(), prc, 0L );
}*/

HRESULT C2DSurface::Clear(DWORD dwColor)
{
	if( NULL == m_pdds )
		return E_POINTER;

    // Erase the background
    DDBLTFX ddbltfx;
    ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
    ddbltfx.dwSize      = sizeof(ddbltfx);
    ddbltfx.dwFillColor = dwColor;

    return m_pdds->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
}