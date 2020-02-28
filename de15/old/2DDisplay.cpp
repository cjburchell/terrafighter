// 2DDisplay.cpp: implementation of the C2DDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "2DDisplay.h"

#include "dxutil.h"
#include "Surface.h"
#include "dialog.h"

#define SCREEN_BPP      16

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2DDisplay::C2DDisplay() : CDisplay()
{
	m_pDD                = NULL;
    m_pddsFrontBuffer    = NULL;
    m_pddsBackBuffer     = NULL;
    m_pddsBackBufferLeft = NULL;
}

C2DDisplay::~C2DDisplay()
{
	DestroyObjects();
}


//-----------------------------------------------------------------------------
// Name: DestroyObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C2DDisplay::DestroyObjects()
{
    SAFE_RELEASE( m_pddsBackBufferLeft );
    SAFE_RELEASE( m_pddsBackBuffer );
    SAFE_RELEASE( m_pddsFrontBuffer );

    if( m_pDD )
        m_pDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );

    SAFE_RELEASE( m_pDD );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CreateFullScreenDisplay()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C2DDisplay::CreateFullScreenDisplay( HWND hWnd, DWORD dwWidth,
                                           DWORD dwHeight, DWORD dwBPP )
{
    HRESULT hr;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

    // Cleanup anything from a previous call
    DestroyObjects();

    // DDraw stuff begins here
    if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD,
                                         IID_IDirectDraw7, NULL ) ) )
        return E_FAIL;

    // Set cooperative level
    hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN );
    if( FAILED(hr) )
        return E_FAIL;

    // Set the display mode
    if( FAILED( m_pDD->SetDisplayMode( dwWidth, dwHeight, dwBPP, 0, 0 ) ) )
        return E_FAIL;

    // Create primary surface (with backbuffer attached)
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize            = sizeof( ddsd );
    ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
                             DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
    ddsd.dwBackBufferCount = 1;

    if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer,
                                           NULL ) ) )
        return E_FAIL;

    // Get a pointer to the back buffer
    DDSCAPS2 ddscaps;
    ZeroMemory( &ddscaps, sizeof( ddscaps ) );
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

    if( FAILED( hr = m_pddsFrontBuffer->GetAttachedSurface( &ddscaps,
                                                            &m_pddsBackBuffer ) ) )
        return E_FAIL;

    m_pddsBackBuffer->AddRef();

    m_hWnd      = hWnd;
    m_bWindowed = FALSE;
    UpdateBounds();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CreateWindowedDisplay()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C2DDisplay::CreateWindowedDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight )
{
    HRESULT hr;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

    // Cleanup anything from a previous call
    DestroyObjects();

    // DDraw stuff begins here
    if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD,
                                         IID_IDirectDraw7, NULL ) ) )
        return E_FAIL;

    // Set cooperative level
    hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_NORMAL );
    if( FAILED(hr) )
        return E_FAIL;

    RECT  rcWork;
    RECT  rc;
    DWORD dwStyle;

    // If we are still a WS_POPUP window we should convert to a normal app
    // window so we look like a windows app.
    dwStyle  = GetWindowStyle( hWnd );
    dwStyle &= ~WS_POPUP;
    dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX;
    SetWindowLong( hWnd, GWL_STYLE, dwStyle );

    // Aet window size
    SetRect( &rc, 0, 0, dwWidth, dwHeight );

    AdjustWindowRectEx( &rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL,
                        GetWindowExStyle(hWnd) );

    SetWindowPos( hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
                  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );

    SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                  SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

    //  Make sure our window does not hang outside of the work area
    SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
    GetWindowRect( hWnd, &rc );
    if( rc.left < rcWork.left ) rc.left = rcWork.left;
    if( rc.top  < rcWork.top )  rc.top  = rcWork.top;
    SetWindowPos( hWnd, NULL, rc.left, rc.top, 0, 0,
                  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

    LPDIRECTDRAWCLIPPER pcClipper;
    
    // Create the primary surface
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize         = sizeof( ddsd );
    ddsd.dwFlags        = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    if( FAILED( m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
        return E_FAIL;

    // Create the backbuffer surface
    ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;    
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
    ddsd.dwWidth        = dwWidth;
    ddsd.dwHeight       = dwHeight;

    if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBuffer, NULL ) ) )
        return E_FAIL;

    if( FAILED( hr = m_pDD->CreateClipper( 0, &pcClipper, NULL ) ) )
        return E_FAIL;

    if( FAILED( hr = pcClipper->SetHWnd( 0, hWnd ) ) )
    {
        pcClipper->Release();
        return E_FAIL;
    }

    if( FAILED( hr = m_pddsFrontBuffer->SetClipper( pcClipper ) ) )
    {
        pcClipper->Release();
        return E_FAIL;
    }

    // Done with clipper
    pcClipper->Release();

    m_hWnd      = hWnd;
    m_bWindowed = TRUE;
    UpdateBounds();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CDisplay::CreateSurfaceFromText()
// Desc: Creates a DirectDrawSurface from a text string using hFont or the default 
//       GDI font if hFont is NULL.
//-----------------------------------------------------------------------------
HRESULT C2DDisplay::CreateSurfaceFromText( CSurface** ppSurface,
                                         HFONT hFont, LPCSTR strText, 
                                         COLORREF crBackground, COLORREF crForeground )
{
    HDC                  hDC  = NULL;
    HRESULT              hr;
    DDSURFACEDESC2       ddsd;
    SIZE                 sizeText;

    if( m_pDD == NULL || strText == NULL || ppSurface == NULL )
        return E_INVALIDARG;

    *ppSurface = NULL;

    hDC = GetDC( NULL );

    if( hFont )
        SelectObject( hDC, hFont );

    GetTextExtentPoint32( hDC, strText, _tcslen(strText), &sizeText );
    ReleaseDC( NULL, hDC );

    // Create a DirectDrawSurface for this bitmap
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth        = sizeText.cx;
    ddsd.dwHeight       = sizeText.cy;
	
	C2DSurface* p2DSurface = new C2DSurface();
    (*ppSurface) = p2DSurface;
    if( FAILED( hr = p2DSurface->Create( m_pDD, &ddsd ) ) )
    {
        delete p2DSurface;
        return hr;
    }

    if( FAILED( hr = p2DSurface->DrawText( hFont, strText, 0, 0, 
                                             crBackground, crForeground ) ) )
        return hr;

    return S_OK;
}

HRESULT C2DDisplay::Present()
{
    HRESULT hr;

    if( NULL == m_pddsFrontBuffer && NULL == m_pddsBackBuffer )
        return E_POINTER;

    while( 1 )
    {
        if( m_bWindowed )
            hr = m_pddsFrontBuffer->Blt( &m_rcWindow, m_pddsBackBuffer,
                                         NULL, DDBLT_WAIT, NULL );
        else
            hr = m_pddsFrontBuffer->Flip( NULL, 0 );

        if( hr == DDERR_SURFACELOST )
        {
            m_pddsFrontBuffer->Restore();
            m_pddsBackBuffer->Restore();
        }

        if( hr != DDERR_WASSTILLDRAWING )
            return hr;
    }
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DDisplay::CreateSurface( CSurface** ppSurface,
                                 DWORD dwWidth, DWORD dwHeight )
{
    if( NULL == m_pDD )
        return E_POINTER;
    if( NULL == ppSurface )
        return E_INVALIDARG;

    HRESULT        hr;
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize         = sizeof( ddsd );
    ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT; 
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth        = dwWidth;
    ddsd.dwHeight       = dwHeight;

    C2DSurface* p2DSurface = new C2DSurface();
    (*ppSurface) = p2DSurface;
    if( FAILED( hr = p2DSurface->Create( m_pDD, &ddsd ) ) )
    {
        delete p2DSurface;
        return hr;
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CDisplay::CreateSurfaceFromBitmap()
// Desc: Create a DirectDrawSurface from a bitmap resource or bitmap file.
//       Use MAKEINTRESOURCE() to pass a constant into strBMP.
//-----------------------------------------------------------------------------
HRESULT C2DDisplay::CreateSurfaceFromBitmap( CSurface** ppSurface,
                                           LPCSTR strBMP,                                            
                                           DWORD dwDesiredWidth, 
                                           DWORD dwDesiredHeight )
{
    HRESULT        hr;
    HBITMAP        hBMP = NULL;
    BITMAP         bmp;
    DDSURFACEDESC2 ddsd;

    if( m_pDD == NULL || strBMP == NULL || ppSurface == NULL ) 
        return E_INVALIDARG;

    *ppSurface = NULL;

    //  Try to load the bitmap as a resource, if that fails, try it as a file
    hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), strBMP, 
                                IMAGE_BITMAP, dwDesiredWidth, dwDesiredHeight, 
                                LR_CREATEDIBSECTION );
    if( hBMP == NULL )
    {
        hBMP = (HBITMAP) LoadImage( NULL, strBMP, 
                                    IMAGE_BITMAP, dwDesiredWidth, dwDesiredHeight, 
                                    LR_LOADFROMFILE | LR_CREATEDIBSECTION );
        if( hBMP == NULL )
            return E_FAIL;
    }

    // Get size of the bitmap
    GetObject( hBMP, sizeof(bmp), &bmp );

    // Create a DirectDrawSurface for this bitmap
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth        = bmp.bmWidth;
    ddsd.dwHeight       = bmp.bmHeight;

    C2DSurface* p2DSurface = new C2DSurface();
    (*ppSurface) = p2DSurface;
    if( FAILED( hr = p2DSurface->Create( m_pDD, &ddsd ) ) )
    {
        delete (*ppSurface);
        return hr;
    }

    // Draw the bitmap on this surface
    if( FAILED( hr = p2DSurface->DrawBitmap( hBMP, 0, 0, 0, 0 ) ) )
    {
        DeleteObject( hBMP );
        return hr;
    }

    DeleteObject( hBMP );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DDisplay::ColorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds,
                               RECT* prc )
{
    if( NULL == m_pddsBackBuffer )
        return E_POINTER;

    return m_pddsBackBuffer->BltFast( x, y, pdds, prc, DDBLTFAST_SRCCOLORKEY );
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DDisplay::Blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds, RECT* prc,
                       DWORD dwFlags )
{
    if( NULL == m_pddsBackBuffer )
        return E_POINTER;

    return m_pddsBackBuffer->BltFast( x, y, pdds, prc, dwFlags );
}

HRESULT C2DDisplay::Blt( DWORD x, DWORD y, CSurface* pSurface, RECT* prc )
{
    if( NULL == pSurface )
        return E_INVALIDARG;

    if( pSurface->IsColorKeyed() )
        return Blt( x, y, static_cast<C2DSurface*>(pSurface)->GetDDrawSurface(), prc, DDBLTFAST_SRCCOLORKEY );
    else
        return Blt( x, y, static_cast<C2DSurface*>(pSurface)->GetDDrawSurface(), prc, 0L );
}

HRESULT C2DDisplay::Clear( DWORD dwColor )
{
    if( NULL == m_pddsBackBuffer )
        return E_POINTER;

    // Erase the background
    DDBLTFX ddbltfx;
    ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
    ddbltfx.dwSize      = sizeof(ddbltfx);
    ddbltfx.dwFillColor = dwColor;

    return m_pddsBackBuffer->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
}

HRESULT C2DDisplay::DrawRect(RECT rect,COLORREF color)
{
    // Get a DC for the surface. Then, write out the buffer
    HDC hDC;
    if( SUCCEEDED( m_pddsBackBuffer->GetDC(&hDC) ) )
    {
        HBRUSH  brush= ::CreateSolidBrush(color);
        FrameRect( hDC, &rect, brush);
        m_pddsBackBuffer->ReleaseDC(hDC);
		DeleteObject(brush);
		return S_OK;
	}
	else
		return E_FAIL;
}

//-----------------------------------------------------------------------------
// Name: CDisplay::InitClipper
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C2DDisplay::InitClipper()
{
    LPDIRECTDRAWCLIPPER pClipper;
    HRESULT hr;

    // Create a clipper when using GDI to draw on the primary surface 
    if( FAILED( hr = m_pDD->CreateClipper( 0, &pClipper, NULL ) ) )
        return hr;

    pClipper->SetHWnd( 0, m_hWnd );

    if( FAILED( hr = m_pddsFrontBuffer->SetClipper( pClipper ) ) )
        return hr;

    // We can release the clipper now since g_pDDSPrimary 
    // now maintains a ref count on the clipper
    SAFE_RELEASE( pClipper );

    return S_OK;
}


HRESULT C2DDisplay::DrawLine(POINT p1, POINT p2, COLORREF color)
{
	// Get a DC for the surface. Then, write out the buffer
    HDC hDC;
    if( SUCCEEDED( m_pddsBackBuffer->GetDC(&hDC) ) )
    {
		HPEN pen = CreatePen(PS_SOLID,0,color);
		SelectObject(hDC,pen);
		MoveToEx(hDC, p1.x, p1.y, (LPPOINT) NULL);
        LineTo(hDC,p2.x, p2.y);
		DeleteObject(pen);
        m_pddsBackBuffer->ReleaseDC(hDC);
		return S_OK;
	}
	else
		return E_FAIL;
}

HRESULT C2DDisplay::DrawFilledRect(RECT rect, COLORREF color)
{
	HDC hDC;
    if( SUCCEEDED( m_pddsBackBuffer->GetDC(&hDC) ) )
    {
        HBRUSH  brush= ::CreateSolidBrush(color);
        FillRect( hDC, &rect, brush);
        m_pddsBackBuffer->ReleaseDC(hDC);
		DeleteObject(brush);
		return S_OK;
	}
	else
		return E_FAIL;
}

void C2DDisplay::OutTextc(COLORREF crBackground, COLORREF crForeground,int x, int y, BOOL bTrasparent, char* format, ...)
{
	va_list ap;
	char s[500];

	if(format==NULL)
		return;
	va_start(ap,format);
	vsprintf(s,format,ap);
	va_end(ap);
	CFont font;
	VERIFY(font.CreateFont(
	12,                        // nHeight
	0,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_NORMAL,                 // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	ANSI_CHARSET,              // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	"Courier"));               // lpszFacename
	CSurface* FontSerface = NULL;
	char* token;
	token=strtok(s,"\n");

	if(FAILED(CreateSurfaceFromText(&FontSerface,font,token,crBackground,crForeground)))
	{
		//pDirDraw->FlipToGDISurface();
		//MessageBox(hWindow,"can't open file","ERROR",MB_ICONERROR|MB_OK);
	}
	else
	{
		if(bTrasparent)
			FontSerface->SetColorKey(crBackground);
		Blt(x,y,FontSerface);
		SAFE_DELETE(FontSerface);
	}
	while((token=strtok(NULL,"\n"))!=NULL)
	{
		if(FAILED(CreateSurfaceFromText(&FontSerface,font,token,crBackground,crForeground)))
		{
			//pDirDraw->FlipToGDISurface();
			//MessageBox(hWindow,"can't open file","ERROR",MB_ICONERROR|MB_OK);
		}
		else
		{
			y+=FONT_SIZE_Y;
			if(bTrasparent)
				FontSerface->SetColorKey(crBackground);
			Blt(x,y,FontSerface);
			SAFE_DELETE(FontSerface);
		}
	}
}


// Function name	: C2DDisplay::StartRender
// Description	    : 
// Return type		: void 
void C2DDisplay::StartRender()
{

}


// Function name	: C2DDisplay::DrawBlendRect
// Description	    : 
// Return type		: HRESULT 
// Argument         : RECT rect
// Argument         : COLORREF color
HRESULT C2DDisplay::DrawBlendRect(RECT rect, COLORREF color)
{
	CSurface* pBkgndSurface;
	if(FAILED(CreateSurfaceFromBitmap(&pBkgndSurface,"blend2.bmp",200,100)))
	{
		return E_FAIL;
	}
	pBkgndSurface->SetColorKey(RGB(255,0,255));
	CRect r(rect);
	CPoint p = r.TopLeft();
	r.OffsetRect(-p.x,-p.y);
	Blt(p.x,p.y,pBkgndSurface,&r);
	delete pBkgndSurface;
	return S_OK;
}


// Function name	: C2DDisplay::GetVideoMode
// Description	    : 
// Return type		: HRESULT 
// Argument         : HINSTANCE hInstance
HRESULT C2DDisplay::GetVideoMode(HINSTANCE hInstance)
{
	CVideoDialogBox* pVdlb=NULL;
	if((pVdlb= new CVideoDialogBox(NULL,hInstance))==NULL)
		return E_FAIL;

	if(pVdlb->getvideomode())
	{
		m_dwWidth = 640;
		m_dwHeight = 480;
	}
	else
	{
		m_dwWidth = 800;
		m_dwHeight = 600;
	}

	if(pVdlb!=NULL)
		delete pVdlb;

	m_bWindowed = TRUE;
	return S_OK;
}


// Function name	: C2DDisplay::Create
// Description	    : 
// Return type		: HRESULT 
// Argument         : HWND hWnd
HRESULT C2DDisplay::Create(HWND hWnd)
{
	if(m_bWindowed)
	{
		return CreateWindowedDisplay( hWnd, m_dwWidth, m_dwHeight );
	}
	else
	{
		return CreateFullScreenDisplay( hWnd, m_dwWidth, m_dwHeight, SCREEN_BPP ) ;
	}
}
