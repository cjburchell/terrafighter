// 3DDisplay.cpp: implementation of the C3DDisplay class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include <d3d8.h>
#include <dxerr8.h>
#include "dxutil.h"
#include "3DDisplay.h"
#include "3DObject.h"
#include "resource.h"
#include <windowsx.h>

#define D3DFVF_SOLIDVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct SOLIDVERTEX
{
    D3DXVECTOR3 position; // The position.
    D3DCOLOR    color;    // The color.
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DDisplay::C3DDisplay() : CDisplay()
{
	m_pFont = NULL;
	m_bUseDepthBuffer = FALSE;
	m_dwNumAdapters     = 0;
	m_pd3dDevice = NULL;
	m_pD3D = NULL;
}

C3DDisplay::~C3DDisplay()
{
	SAFE_RELEASE( m_pD3D );
    SAFE_RELEASE( m_pd3dDevice );
	if(m_pFont)
		delete m_pFont;
}

HRESULT C3DDisplay::CreateFullScreenDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP )
{
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_hWnd      = hWnd;
    m_bWindowed = FALSE;
	m_dwBPP = dwBPP;
	return Create(hWnd);
}



HRESULT C3DDisplay::CreateWindowedDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight)
{
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_hWnd      = hWnd;
    m_bWindowed = TRUE;
	return Create(hWnd);
}

HRESULT C3DDisplay::Create(HWND hWnd)
{
	HRESULT hr;
	m_hWnd = hWnd;
	
	D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
	D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
	D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];
	
	 // Set up the presentation parameters
    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.Windowed               = pDeviceInfo->bWindowed;
    m_d3dpp.BackBufferCount        = 1;
    m_d3dpp.MultiSampleType        = pDeviceInfo->MultiSampleType;
    m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.EnableAutoDepthStencil = m_bUseDepthBuffer;
    m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
    m_d3dpp.hDeviceWindow          = m_hWnd;
    if( m_bWindowed )
    {
        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
        m_d3dpp.BackBufferHeight = pModeInfo->Height;
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
    }
    else
    {
        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
        m_d3dpp.BackBufferHeight = pModeInfo->Height;
        m_d3dpp.BackBufferFormat = pModeInfo->Format;
    }

    // Create the device
    hr = m_pD3D->CreateDevice( m_dwAdapter, pDeviceInfo->DeviceType,
                               m_hWnd, pModeInfo->dwBehavior, &m_d3dpp,
                               &m_pd3dDevice );

	// Create the device
	if( FAILED( hr))
	{
		DXTRACE_ERR_NOMSGBOX("CreateDevice",hr);
        return hr;
    }
	
	m_pd3dDevice->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pd3dDevice->SetRenderState ( D3DRS_LIGHTING, FALSE);
	//m_pd3dDevice->SetRenderState ( D3DRS_ZENABLE, TRUE);
	
	//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);//D3DTOP_DISABLE );
	
    // Set diffuse blending for alpha set in vertices.
    //m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
    //m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
   // m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// Set up the default texture states
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP );
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP );

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
    m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );



    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,      TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );
	
	//m_pd3dDevice->SetVertexShader( D3DFVF_SOLIDVERTEX );

	FLOAT newy = ((((float)FONT_SIZE_Y/(float)m_dwHeight)*2.0f));
	FLOAT newx = ((((float)FONT_SIZE_X/(float)m_dwWidth)*2.0f));

	m_pFont = new C2DFont( _T("Courier"), newy, newx );
	m_pFont->Init( m_pd3dDevice );

	
	if( FAILED( hr = m_pd3dDevice->CreateVertexBuffer( 5*sizeof(SOLIDVERTEX), 0, D3DFVF_SOLIDVERTEX, D3DPOOL_DEFAULT, &m_pVB ) ) )
    {
		DXTRACE_ERR_NOMSGBOX("CreateVertexBuffer",hr);
        return hr;
    }


	return S_OK;
}

HRESULT C3DDisplay::Clear(DWORD dwColor)
{
	dwColor = ConvertColor(dwColor);
	//dwColor = D3DCOLOR_XRGB(0,0,0);
	//g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	return m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, dwColor, 1.0f, 0L );
}

HRESULT C3DDisplay::Present()
{
	HRESULT hr;
	hr = m_pd3dDevice->EndScene();
	if FAILED(hr)
	{
		DXTRACE_ERR_NOMSGBOX("EndScene",hr);
	}

	hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	if FAILED(hr)
	{
		DXTRACE_ERR_NOMSGBOX("Present",hr);
	}
	return hr;
}

void C3DDisplay::StartRender()
{
	HRESULT hr;
	hr = m_pd3dDevice->BeginScene();
	if FAILED(hr)
	{
		DXTRACE_ERR_NOMSGBOX("Present",hr);
	}
}

void C3DDisplay::OutTextc(COLORREF crBackground, COLORREF crForeground,int x, int y, BOOL bTrasparent, char* format, ...)
{
	FLOAT newy = -((((float)y/(float)m_dwHeight)*2.0f) - 1.0f);
	FLOAT newx = ((((float)x/(float)m_dwWidth)*2.0f) - 1.0f);
	crBackground = ConvertColor(crBackground);
	crForeground = ConvertColor(crForeground);
	va_list ap;
	char s[500];

	if(format==NULL)
		return;
	va_start(ap,format);
	vsprintf(s,format,ap);
	va_end(ap);

	char* token;
	token=strtok(s,"\n");
	if(token)
	{
		FLOAT ysize = ((((float)FONT_SIZE_Y/(float)m_dwHeight)*2.0f));

		m_pFont->DrawText(newx,newy,crForeground,token);
		while((token=strtok(NULL,"\n"))!=NULL)
		{
			newy-=ysize;
			m_pFont->DrawText(newx,newy,crForeground,token);
		}
	}
}



HRESULT C3DDisplay::DrawFilledRect(RECT rect, COLORREF color)
{
	color = ConvertColor(color);
	m_pd3dDevice->SetVertexShader( D3DFVF_SOLIDVERTEX );
	m_pd3dDevice->SetStreamSource(0, m_pVB, sizeof(SOLIDVERTEX));

	SOLIDVERTEX* pVertices;

	CRect r(rect);

	m_pVB->Lock( 0, 4*sizeof(SOLIDVERTEX), (BYTE**)&pVertices, D3DLOCK_DISCARD );
	pVertices[0].position = PointToScreen(r.TopLeft());
	pVertices[0].color = color;

	pVertices[1].position = PointToScreen(CPoint(r.right,r.top));
	pVertices[1].color = color;

	pVertices[2].position = PointToScreen(CPoint(r.left,r.bottom));
	pVertices[2].color = color;

	pVertices[3].position = PointToScreen(r.BottomRight());
	pVertices[3].color = color;

	
	m_pVB->Unlock();

	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	return S_OK;
}

HRESULT C3DDisplay::DrawLine(POINT p1, POINT p2, COLORREF color)
{
	color = ConvertColor(color);
	m_pd3dDevice->SetVertexShader( D3DFVF_SOLIDVERTEX );
	m_pd3dDevice->SetStreamSource(0, m_pVB, sizeof(SOLIDVERTEX));

	SOLIDVERTEX* pVertices;

	m_pVB->Lock( 0, 5*sizeof(SOLIDVERTEX), (BYTE**)&pVertices, D3DLOCK_DISCARD );
	pVertices[0].position = PointToScreen(p1);
	pVertices[0].color = color;

	pVertices[1].position = PointToScreen(p2);
	pVertices[1].color = color;
	m_pVB->Unlock();

	m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 );
	return S_OK;
}

HRESULT C3DDisplay::DrawRect(RECT rect, COLORREF color)
{
	color = ConvertColor(color);
	HRESULT hr;
	hr = m_pd3dDevice->SetVertexShader( D3DFVF_SOLIDVERTEX );
	if FAILED(hr)
	{
		DXTRACE_ERR_NOMSGBOX("SetVertexShader",hr);
	}

	hr = m_pd3dDevice->SetStreamSource(0, m_pVB, sizeof(SOLIDVERTEX));
	if FAILED(hr)
	{
		DXTRACE_ERR_NOMSGBOX("SetStreamSource",hr);
	}

	SOLIDVERTEX* pVertices;

	CRect r(rect);

	hr = m_pVB->Lock( 0, 5*sizeof(SOLIDVERTEX), (BYTE**)&pVertices, D3DLOCK_DISCARD );
	if FAILED(hr)
	{
		DXTRACE_ERR_NOMSGBOX("Lock",hr);
	}

	pVertices[0].position = PointToScreen(r.TopLeft());
	pVertices[0].color = color;

	pVertices[1].position = PointToScreen(CPoint(r.left,r.bottom));
	pVertices[1].color = color;

	pVertices[2].position = PointToScreen(r.BottomRight());
	pVertices[2].color = color;

	pVertices[3].position = PointToScreen(CPoint(r.right,r.top));
	pVertices[3].color = color;

	pVertices[4].position = PointToScreen(r.TopLeft());
	pVertices[4].color = color;

	hr = m_pVB->Unlock();
	if FAILED(hr)
	{
		DXTRACE_ERR_NOMSGBOX("Unlock",hr);
	}

	hr = m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, 0, 4 );
	if FAILED(hr)
	{
		DXTRACE_ERR_NOMSGBOX("DrawPrimitive",hr);
	}
	return S_OK;
}

HRESULT C3DDisplay::CreateSurfaceFromBitmap( CSurface** ppSurface, LPCSTR strBMP,
		                             DWORD dwDesiredWidth,
									 DWORD dwDesiredHeight )
{
	(*ppSurface) = new C3DObject();
	return static_cast<C3DObject*>(*ppSurface)->Init( strBMP,dwDesiredWidth,dwDesiredHeight,m_pd3dDevice);
}

HRESULT C3DDisplay::Blt(DWORD x, DWORD y, CSurface *pSurface, RECT *prc)
{
	return static_cast<C3DObject*>(pSurface)->Draw(x,y,prc,m_pd3dDevice,this);
}

//-----------------------------------------------------------------------------
// Name: SortModesCallback()
// Desc: Callback function for sorting display modes (used by BuildDeviceList).
//-----------------------------------------------------------------------------
int SortModesCallback( const VOID* arg1, const VOID* arg2 )
{
    D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
    D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

    if( p1->Format > p2->Format )   return -1;
    if( p1->Format < p2->Format )   return +1;
    if( p1->Width  < p2->Width )    return -1;
    if( p1->Width  > p2->Width )    return +1;
    if( p1->Height < p2->Height )   return -1;
    if( p1->Height > p2->Height )   return +1;

    return 0;
}

//-----------------------------------------------------------------------------
// Name: BuildDeviceList()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::BuildDeviceList()
{
    const DWORD dwNumDeviceTypes = 2;
    const TCHAR* strDeviceDescs[] = { _T("HAL"), _T("REF") };
    const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

    BOOL bHALExists = FALSE;
    BOOL bHALIsWindowedCompatible = FALSE;
    BOOL bHALIsDesktopCompatible = FALSE;
    BOOL bHALIsSampleCompatible = FALSE;

    // Loop through all the adapters on the system (usually, there's just one
    // unless more than one graphics card is present).
    for( UINT iAdapter = 0; iAdapter < m_pD3D->GetAdapterCount(); iAdapter++ )
    {
        // Fill in adapter info
        D3DAdapterInfo* pAdapter  = &m_Adapters[m_dwNumAdapters];
        m_pD3D->GetAdapterIdentifier( iAdapter, 0, &pAdapter->d3dAdapterIdentifier );
        m_pD3D->GetAdapterDisplayMode( iAdapter, &pAdapter->d3ddmDesktop );
        pAdapter->dwNumDevices    = 0;
        pAdapter->dwCurrentDevice = 0;

        // Enumerate all display modes on this adapter
        D3DDISPLAYMODE modes[100];
        D3DFORMAT      formats[20];
        DWORD dwNumFormats      = 0;
        DWORD dwNumModes        = 0;
        DWORD dwNumAdapterModes = m_pD3D->GetAdapterModeCount( iAdapter );

        // Add the adapter's current desktop format to the list of formats
        formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

        for( UINT iMode = 0; iMode < dwNumAdapterModes; iMode++ )
        {
            // Get the display mode attributes
            D3DDISPLAYMODE DisplayMode;
            m_pD3D->EnumAdapterModes( iAdapter, iMode, &DisplayMode );

            // Filter out low-resolution modes
            if( DisplayMode.Width  < 640 || DisplayMode.Height < 400 )
                continue;

            // Check if the mode already exists (to filter out refresh rates)
            for( DWORD m=0L; m<dwNumModes; m++ )
            {
                if( ( modes[m].Width  == DisplayMode.Width  ) &&
                    ( modes[m].Height == DisplayMode.Height ) &&
                    ( modes[m].Format == DisplayMode.Format ) )
                    break;
            }

            // If we found a new mode, add it to the list of modes
            if( m == dwNumModes )
            {
                modes[dwNumModes].Width       = DisplayMode.Width;
                modes[dwNumModes].Height      = DisplayMode.Height;
                modes[dwNumModes].Format      = DisplayMode.Format;
                modes[dwNumModes].RefreshRate = 0;
                dwNumModes++;

                // Check if the mode's format already exists
                for( DWORD f=0; f<dwNumFormats; f++ )
                {
                    if( DisplayMode.Format == formats[f] )
                        break;
                }

                // If the format is new, add it to the list
                if( f== dwNumFormats )
                    formats[dwNumFormats++] = DisplayMode.Format;
            }
        }

        // Sort the list of display modes (by format, then width, then height)
        qsort( modes, dwNumModes, sizeof(D3DDISPLAYMODE), SortModesCallback );

        // Add devices to adapter
        for( UINT iDevice = 0; iDevice < dwNumDeviceTypes; iDevice++ )
        {
            // Fill in device info
            D3DDeviceInfo* pDevice;
            pDevice                 = &pAdapter->devices[pAdapter->dwNumDevices];
            pDevice->DeviceType     = DeviceTypes[iDevice];
            m_pD3D->GetDeviceCaps( iAdapter, DeviceTypes[iDevice], &pDevice->d3dCaps );
            pDevice->strDesc        = strDeviceDescs[iDevice];
            pDevice->dwNumModes     = 0;
            pDevice->dwCurrentMode  = 0;
            pDevice->bCanDoWindowed = FALSE;
            pDevice->bWindowed      = FALSE;
            pDevice->MultiSampleType = D3DMULTISAMPLE_NONE;

            // Examine each format supported by the adapter to see if it will
            // work with this device and meets the needs of the application.
            BOOL  bFormatConfirmed[20];
            DWORD dwBehavior[20];
            D3DFORMAT fmtDepthStencil[20];

            for( DWORD f=0; f<dwNumFormats; f++ )
            {
                bFormatConfirmed[f] = FALSE;
                fmtDepthStencil[f] = D3DFMT_UNKNOWN;

                // Skip formats that cannot be used as render targets on this device
                if( FAILED( m_pD3D->CheckDeviceType( iAdapter, pDevice->DeviceType,
                                                     formats[f], formats[f], FALSE ) ) )
                    continue;

                if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                {
                    // This system has a HAL device
                    bHALExists = TRUE;

                    if( pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED )
                    {
                        // HAL can run in a window for some mode
                        bHALIsWindowedCompatible = TRUE;

                        if( f == 0 )
                        {
                            // HAL can run in a window for the current desktop mode
                            bHALIsDesktopCompatible = TRUE;
                        }
                    }
                }

                // Confirm the device/format for HW vertex processing
                if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT )
                {
                    if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_PUREDEVICE )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING |
                                        D3DCREATE_PUREDEVICE;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }
                }

                // Confirm the device/format for SW vertex processing
                if( FALSE == bFormatConfirmed[f] )
                {
                    dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

                    if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                  formats[f] ) ) )
                        bFormatConfirmed[f] = TRUE;
                }

                // Find a suitable depth/stencil buffer format for this device/format
                if( bFormatConfirmed[f] && m_bUseDepthBuffer )
                {
                    if( !FindDepthStencilFormat( iAdapter, pDevice->DeviceType,
                        formats[f], &fmtDepthStencil[f] ) )
                    {
                        bFormatConfirmed[f] = FALSE;
                    }
                }
            }

            // Add all enumerated display modes with confirmed formats to the
            // device's list of valid modes
            for( DWORD m=0L; m<dwNumModes; m++ )
            {
                for( DWORD f=0; f<dwNumFormats; f++ )
                {
                    if( modes[m].Format == formats[f] )
                    {
                        if( bFormatConfirmed[f] == TRUE )
                        {
                            // Add this mode to the device's list of valid modes
                            pDevice->modes[pDevice->dwNumModes].Width      = modes[m].Width;
                            pDevice->modes[pDevice->dwNumModes].Height     = modes[m].Height;
                            pDevice->modes[pDevice->dwNumModes].Format     = modes[m].Format;
                            pDevice->modes[pDevice->dwNumModes].dwBehavior = dwBehavior[f];
                            pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
                            pDevice->dwNumModes++;

                            if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                                bHALIsSampleCompatible = TRUE;
                        }
                    }
                }
            }

            // Select any 640x480 mode for default (but prefer a 16-bit mode)
            for( m=0; m<pDevice->dwNumModes; m++ )
            {
                if( pDevice->modes[m].Width==640 && pDevice->modes[m].Height==480 )
                {
                    pDevice->dwCurrentMode = m;
                    if( pDevice->modes[m].Format == D3DFMT_R5G6B5 ||
                        pDevice->modes[m].Format == D3DFMT_X1R5G5B5 ||
                        pDevice->modes[m].Format == D3DFMT_A1R5G5B5 )
                    {
                        break;
                    }
                }
            }

            // Check if the device is compatible with the desktop display mode
            // (which was added initially as formats[0])
            if( bFormatConfirmed[0] && (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED) )
            {
                pDevice->bCanDoWindowed = TRUE;
                pDevice->bWindowed      = TRUE;
            }

            // If valid modes were found, keep this device
            if( pDevice->dwNumModes > 0 )
                pAdapter->dwNumDevices++;
        }

        // If valid devices were found, keep this adapter
        if( pAdapter->dwNumDevices > 0 )
            m_dwNumAdapters++;
    }

    // Return an error if no compatible devices were found
    if( 0L == m_dwNumAdapters )
        return D3DAPPERR_NOCOMPATIBLEDEVICES;

    // Pick a default device that can render into a window
    // (This code assumes that the HAL device comes before the REF
    // device in the device array).
    for( DWORD a=0; a<m_dwNumAdapters; a++ )
    {
        for( DWORD d=0; d < m_Adapters[a].dwNumDevices; d++ )
        {
            if( m_Adapters[a].devices[d].bWindowed )
            {
                m_Adapters[a].dwCurrentDevice = d;
                m_dwAdapter = a;
                m_bWindowed = TRUE;

                // Display a warning message
                /*if( m_Adapters[a].devices[d].DeviceType == D3DDEVTYPE_REF )
                {
                    if( !bHALExists )
                        DisplayErrorMsg( D3DAPPERR_NOHARDWAREDEVICE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsSampleCompatible )
                        DisplayErrorMsg( D3DAPPERR_HALNOTCOMPATIBLE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsWindowedCompatible )
                        DisplayErrorMsg( D3DAPPERR_NOWINDOWEDHAL, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsDesktopCompatible )
                        DisplayErrorMsg( D3DAPPERR_NODESKTOPHAL, MSGWARN_SWITCHEDTOREF );
                    else // HAL is desktop compatible, but not sample compatible
                        DisplayErrorMsg( D3DAPPERR_NOHALTHISMODE, MSGWARN_SWITCHEDTOREF );
                }*/

                return S_OK;
            }
        }
    }

    return D3DAPPERR_NOWINDOWABLEDEVICES;
}

HRESULT C3DDisplay::ConfirmDevice(D3DCAPS8* caps,DWORD behve,D3DFORMAT format)   
{ 
	return S_OK; 
}

//-----------------------------------------------------------------------------
// Name: FindDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//       with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
BOOL C3DDisplay::FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
    D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat )
{
    if( m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D16;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D15S1;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24S8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X4S4;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D32;
                return TRUE;
            }
        }
    }

    return FALSE;
}

D3DXVECTOR3 C3DDisplay::PointToScreen(POINT p)
{
	D3DXVECTOR3 newp;
	newp.x = ((((float)p.x/(float)m_dwWidth)*2.0f) - 1.0f);
	newp.y = -((((float)p.y/(float)m_dwHeight)*2.0f) - 1.0f);
	newp.z = 0.5f;
	return newp;
}

DWORD C3DDisplay::ConvertColor(DWORD col)
{
	D3DCOLOR c = D3DCOLOR_XRGB(GetRValue(col),GetGValue(col),GetBValue(col));
	return c;
}

HRESULT C3DDisplay::DrawBlendRect(RECT rect, COLORREF color)
{

	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	m_pd3dDevice->SetVertexShader( D3DFVF_SOLIDVERTEX );
	m_pd3dDevice->SetStreamSource(0, m_pVB, sizeof(SOLIDVERTEX));

	SOLIDVERTEX* pVertices;

	CRect r(rect);

	m_pVB->Lock( 0, 4*sizeof(SOLIDVERTEX), (BYTE**)&pVertices, D3DLOCK_DISCARD );
	pVertices[0].position = PointToScreen(r.TopLeft());
	pVertices[0].color = color;

	pVertices[1].position = PointToScreen(CPoint(r.right,r.top));
	pVertices[1].color = color;

	pVertices[2].position = PointToScreen(CPoint(r.left,r.bottom));
	pVertices[2].color = color;

	pVertices[3].position = PointToScreen(r.BottomRight());
	pVertices[3].color = color;

	
	m_pVB->Unlock();

	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: SelectDeviceProc()
// Desc: Windows message handling function for the device select dialog
//-----------------------------------------------------------------------------
INT_PTR CALLBACK C3DDisplay::SelectDeviceProc( HWND hDlg, UINT msg,
                                                    WPARAM wParam, LPARAM lParam )
{
    // Get access to the UI controls
    HWND hwndAdapterList        = GetDlgItem( hDlg, IDC_ADAPTER_COMBO );
    HWND hwndDeviceList         = GetDlgItem( hDlg, IDC_DEVICE_COMBO );
    HWND hwndFullscreenModeList = GetDlgItem( hDlg, IDC_FULLSCREENMODES_COMBO );
    HWND hwndWindowedRadio      = GetDlgItem( hDlg, IDC_WINDOW );
    HWND hwndFullscreenRadio    = GetDlgItem( hDlg, IDC_FULLSCREEN );
    BOOL bUpdateDlgControls     = FALSE;

    // Static state for adapter/device/mode selection
    static C3DDisplay* pd3dApp;
    static DWORD  dwOldAdapter, dwNewAdapter;
    static DWORD  dwOldDevice,  dwNewDevice;
    static DWORD  dwOldMode,    dwNewMode;
    static BOOL   bOldWindowed, bNewWindowed;

    // Working variables
    D3DAdapterInfo* pAdapter;
    D3DDeviceInfo*  pDevice;

    // Handle the initialization message
    if( WM_INITDIALOG == msg )
    {
        // Old state
        pd3dApp      = (C3DDisplay*)lParam;
        dwOldAdapter = pd3dApp->m_dwAdapter;
        pAdapter     = &pd3dApp->m_Adapters[dwOldAdapter];

        dwOldDevice  = pAdapter->dwCurrentDevice;
        pDevice      = &pAdapter->devices[dwOldDevice];

        dwOldMode    = pDevice->dwCurrentMode;
        bOldWindowed = pDevice->bWindowed;

        // New state is initially the same as the old state
        dwNewAdapter = dwOldAdapter;
        dwNewDevice  = dwOldDevice;
        dwNewMode    = dwOldMode;
        bNewWindowed = bOldWindowed;

        // Set flag to update dialog controls below
        bUpdateDlgControls = TRUE;
    }

    if( WM_COMMAND == msg )
    {
        // Get current UI state
        bNewWindowed  = Button_GetCheck( hwndWindowedRadio );

        if( IDOK == LOWORD(wParam) )
        {
            // Handle the case when the user hits the OK button. Check if any
            // of the options were changed
            if( dwNewAdapter != dwOldAdapter || dwNewDevice  != dwOldDevice  ||
                dwNewMode    != dwOldMode    || bNewWindowed != bOldWindowed )
            {
                pd3dApp->m_dwAdapter = dwNewAdapter;

                pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
                pAdapter->dwCurrentDevice = dwNewDevice;

                pAdapter->devices[dwNewDevice].dwCurrentMode = dwNewMode;
                pAdapter->devices[dwNewDevice].bWindowed     = bNewWindowed;

            }

			EndDialog( hDlg, IDOK );
            return TRUE;
        }
        else if( IDCANCEL == LOWORD(wParam) )
        {
            // Handle the case when the user hits the Cancel button
            EndDialog( hDlg, IDCANCEL );
            return TRUE;
        }
        else if( CBN_SELENDOK == HIWORD(wParam) )
        {
            if( LOWORD(wParam) == IDC_ADAPTER_COMBO )
            {
                dwNewAdapter = ComboBox_GetCurSel( hwndAdapterList );
                pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

                dwNewDevice  = pAdapter->dwCurrentDevice;
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
            else if( LOWORD(wParam) == IDC_DEVICE_COMBO )
            {
                pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

                dwNewDevice  = ComboBox_GetCurSel( hwndDeviceList );
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
            else if( LOWORD(wParam) == IDC_FULLSCREENMODES_COMBO )
            {
                dwNewMode = ComboBox_GetCurSel( hwndFullscreenModeList );
            }

        }
        // Keep the UI current
        bUpdateDlgControls = TRUE;
    }

    // Update the dialog controls
    if( bUpdateDlgControls )
    {
        // Reset the content in each of the combo boxes
        ComboBox_ResetContent( hwndAdapterList );
        ComboBox_ResetContent( hwndDeviceList );
        ComboBox_ResetContent( hwndFullscreenModeList );

        pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
        pDevice  = &pAdapter->devices[dwNewDevice];

        // Add a list of adapters to the adapter combo box
        for( DWORD a=0; a < pd3dApp->m_dwNumAdapters; a++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndAdapterList,
                             pd3dApp->m_Adapters[a].d3dAdapterIdentifier.Description );

            // Set the item data to identify this adapter
            ComboBox_SetItemData( hwndAdapterList, dwItem, a );

            // Set the combobox selection on the current adapater
            if( a == dwNewAdapter )
                ComboBox_SetCurSel( hwndAdapterList, dwItem );
        }

        // Add a list of devices to the device combo box
        for( DWORD d=0; d < pAdapter->dwNumDevices; d++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndDeviceList,
                                               pAdapter->devices[d].strDesc );

            // Set the item data to identify this device
            ComboBox_SetItemData( hwndDeviceList, dwItem, d );

            // Set the combobox selection on the current device
            if( d == dwNewDevice )
                ComboBox_SetCurSel( hwndDeviceList, dwItem );
        }

        // Add a list of modes to the mode combo box
        for( DWORD m=0; m < pDevice->dwNumModes; m++ )
        {
            DWORD BitDepth = 16;
            if( pDevice->modes[m].Format == D3DFMT_X8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_R8G8B8 )
            {
                BitDepth = 32;
            }

            // Add mode desc to the combo box
            TCHAR strMode[80];
            _stprintf( strMode, _T("%ld x %ld x %ld"), pDevice->modes[m].Width,
                                                       pDevice->modes[m].Height,
                                                       BitDepth );
            DWORD dwItem = ComboBox_AddString( hwndFullscreenModeList, strMode );

            // Set the item data to identify this mode
            ComboBox_SetItemData( hwndFullscreenModeList, dwItem, m );

            // Set the combobox selection on the current mode
            if( m == dwNewMode )
                ComboBox_SetCurSel( hwndFullscreenModeList, dwItem );
        }

        
        EnableWindow( hwndWindowedRadio, pDevice->bCanDoWindowed );

        if( bNewWindowed )
        {
            Button_SetCheck( hwndWindowedRadio,   TRUE );
            Button_SetCheck( hwndFullscreenRadio, FALSE );
            EnableWindow( hwndFullscreenModeList, FALSE );
        }
        else
        {
            Button_SetCheck( hwndWindowedRadio,   FALSE );
            Button_SetCheck( hwndFullscreenRadio, TRUE );
            EnableWindow( hwndFullscreenModeList, TRUE );
        }
        return TRUE;
    }

    return FALSE;
}

HRESULT C3DDisplay::GetVideoMode(HINSTANCE hInstance)
{
	HRESULT hr;
	m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );

	if( FAILED( hr = BuildDeviceList()))
	{
		DXTRACE_ERR_NOMSGBOX("BuildDeviceList",hr);
        return hr;
    }

	// Prompt the user to change the mode
    if( IDOK != DialogBoxParam( hInstance,
                                MAKEINTRESOURCE(IDD_SELECTDEVICE), NULL,
                                SelectDeviceProc, (LPARAM)this ) )
	return E_FAIL;

	D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
	D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
	D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];
	
	m_dwWidth = pModeInfo->Width;
	m_dwHeight = pModeInfo->Height;
	m_bWindowed = pDeviceInfo->bWindowed;

	return S_OK;
}

HRESULT C3DDisplay::DrawGradientRect(RECT rect, COLORREF color1, COLORREF color2, BYTE flags)
{
	color1 = ConvertColor(color1);
	color2 = ConvertColor(color2);
	m_pd3dDevice->SetVertexShader( D3DFVF_SOLIDVERTEX );
	m_pd3dDevice->SetStreamSource(0, m_pVB, sizeof(SOLIDVERTEX));

	SOLIDVERTEX* pVertices;

	COLORREF ColorTL = color1;
	COLORREF ColorTR = color1;
	COLORREF ColorBR = color1;
	COLORREF ColorBL = color1;

	if(flags & GR_TL)
		ColorTL = color2;
	if(flags & GR_TR)
		ColorTR = color2;
	if(flags & GR_BR)
		ColorBR = color2;
	if(flags & GR_BL)
		ColorBL = color2;

	CRect r(rect);

	m_pVB->Lock( 0, 4*sizeof(SOLIDVERTEX), (BYTE**)&pVertices, D3DLOCK_DISCARD );
	pVertices[0].position = PointToScreen(r.TopLeft());
	pVertices[0].color = ColorTL;

	pVertices[1].position = PointToScreen(CPoint(r.right,r.top));
	pVertices[1].color = ColorTR;

	pVertices[2].position = PointToScreen(CPoint(r.left,r.bottom));
	pVertices[2].color = ColorBL;

	pVertices[3].position = PointToScreen(r.BottomRight());
	pVertices[3].color = ColorBR;

	
	m_pVB->Unlock();

	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	return S_OK;
}
