//-----------------------------------------------------------------------------
// File: ddutil.cpp
//
// Desc: DirectDraw framewark classes. Feel free to use this class as a 
//       starting point for adding extra functionality.
//
//
// Copyright (c) 1995-1999 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include <afx.h>
#include <afxwin.h>
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include "ddutil.h"
#include "dxutil.h"





#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//-----------------------------------------------------------------------------
// Name: CDisplay()
// Desc:
//-----------------------------------------------------------------------------
CDisplay::CDisplay()
{
    
}




//-----------------------------------------------------------------------------
// Name: ~CDisplay()
// Desc:
//-----------------------------------------------------------------------------
CDisplay::~CDisplay()
{

}




HRESULT CDisplay::DrawRect(int x, int y,int w, int h,COLORREF color)
{
   CRect rec(x,y,w,h);
   return DrawRect(rec,color);
}





/*//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CDisplay::SetPalette( LPDIRECTDRAWPALETTE pPalette )
{
    if( NULL == m_pddsFrontBuffer )
        return E_POINTER;

    return m_pddsFrontBuffer->SetPalette( pPalette );
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CDisplay::CreatePaletteFromBitmap( LPDIRECTDRAWPALETTE* ppPalette,
                                           const TCHAR* strBMP )
{
    HRSRC             hResource      = NULL;
    RGBQUAD*          pRGB           = NULL;
    BITMAPINFOHEADER* pbi = NULL;
    PALETTEENTRY      aPalette[256];
    HANDLE            hFile = NULL;
    DWORD             iColor;
    DWORD             dwColors;
    BITMAPFILEHEADER  bf;
    BITMAPINFOHEADER  bi;
    DWORD             dwBytesRead;

    if( m_pDD == NULL || strBMP == NULL || ppPalette == NULL )
        return E_INVALIDARG;

    *ppPalette = NULL;

    //  Try to load the bitmap as a resource, if that fails, try it as a file
    hResource = FindResource( NULL, strBMP, RT_BITMAP );
    if( hResource )
    {
        pbi = (LPBITMAPINFOHEADER) LockResource( LoadResource( NULL, hResource ) );       
        if( NULL == pbi )
            return E_FAIL;

        pRGB = (RGBQUAD*) ( (BYTE*) pbi + pbi->biSize );

        // Figure out how many colors there are
        if( pbi == NULL || pbi->biSize < sizeof(BITMAPINFOHEADER) )
            dwColors = 0;
        else if( pbi->biBitCount > 8 )
            dwColors = 0;
        else if( pbi->biClrUsed == 0 )
            dwColors = 1 << pbi->biBitCount;
        else
            dwColors = pbi->biClrUsed;

        //  A DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        for( iColor = 0; iColor < dwColors; iColor++ )
        {
            aPalette[iColor].peRed   = pRGB[iColor].rgbRed;
            aPalette[iColor].peGreen = pRGB[iColor].rgbGreen;
            aPalette[iColor].peBlue  = pRGB[iColor].rgbBlue;
            aPalette[iColor].peFlags = 0;
        }

        return m_pDD->CreatePalette( DDPCAPS_8BIT, aPalette, ppPalette, NULL );
    }

    // Attempt to load bitmap as a file
    hFile = CreateFile( strBMP, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
    if( NULL == hFile )
        return E_FAIL;

    // Read the BITMAPFILEHEADER
    ReadFile( hFile, &bf, sizeof(bf), &dwBytesRead, NULL );
    if( dwBytesRead != sizeof(bf) )
    {
        CloseHandle( hFile );
        return E_FAIL;
    }

    // Read the BITMAPINFOHEADER
    ReadFile( hFile, &bi, sizeof(bi), &dwBytesRead, NULL );
    if( dwBytesRead != sizeof(bi) )
    {
        CloseHandle( hFile );
        return E_FAIL;
    }

    // Read the PALETTEENTRY 
    ReadFile( hFile, aPalette, sizeof(aPalette), &dwBytesRead, NULL );
    if( dwBytesRead != sizeof(aPalette) )
    {
        CloseHandle( hFile );
        return E_FAIL;
    }

    CloseHandle( hFile );

    // Figure out how many colors there are
    if( bi.biSize != sizeof(BITMAPINFOHEADER) )
        dwColors = 0;
    else if (bi.biBitCount > 8)
        dwColors = 0;
    else if (bi.biClrUsed == 0)
        dwColors = 1 << bi.biBitCount;
    else
        dwColors = bi.biClrUsed;

    //  A DIB color table has its colors stored BGR not RGB
    //  so flip them around since DirectDraw uses RGB
    for( iColor = 0; iColor < dwColors; iColor++ )
    {
        BYTE r = aPalette[iColor].peRed;
        aPalette[iColor].peRed  = aPalette[iColor].peBlue;
        aPalette[iColor].peBlue = r;
    }

    return m_pDD->CreatePalette( DDPCAPS_8BIT, aPalette, ppPalette, NULL );
}*/




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CDisplay::UpdateBounds()
{
    if( m_bWindowed )
    {
        GetClientRect( m_hWnd, &m_rcWindow );
        ClientToScreen( m_hWnd, (POINT*)&m_rcWindow );
        ClientToScreen( m_hWnd, (POINT*)&m_rcWindow+1 );
    }
    else
    {
        SetRect( &m_rcWindow, 0, 0, GetSystemMetrics(SM_CXSCREEN),
                 GetSystemMetrics(SM_CYSCREEN) );
    }

    return S_OK;
}











//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
CSurface::CSurface()
{
    m_bColorKeyed = FALSE;
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
CSurface::~CSurface()
{
    
}









HRESULT CDisplay::FlipToGDISurface()
{
	//m_pDD->FlipToGDISurface();
	return S_OK;	
}





// outtext outputs text to the back buffer
void CDisplay::outtexts(
			 COLORREF crBackground, 
			 COLORREF crForeground,
			 int x, // IN: x position of the text 
			 int y, // IN: y position of the text 
			 char* s // IN: the string to be outputed
			 )
{
	OutTextc(crBackground,crForeground,x*FONT_SIZE_X,y*FONT_SIZE_Y,FALSE,s);
}


void CDisplay::outtext(int x, int y, char* format, ...)
{
	va_list ap;
	char s[500];

	if(format==NULL)
		return;
	va_start(ap,format);
	vsprintf(s,format,ap);
	va_end(ap);
	int i = 0;
	while(s[i] != '\0')
	{
		if(s[i] == '_')
			s[i] = ' ';
		i++;
	}
	outtexts(RGB(195,195,195),RGB(0,0,0),x,y,s);
}

HRESULT CDisplay::DrawRectAroundText(int x, int y,int w, int h,COLORREF color)
{
   CRect rec(x*FONT_SIZE_X,y*FONT_SIZE_Y,w*FONT_SIZE_X,h*FONT_SIZE_Y);
   return DrawRect(rec,color);
}

RECT CDisplay::GetTextRect(CString str)
{
	int pos = str.Find('\n');
	int lastpos = 0;
	int ysize = 0;
	int xsize = 0;
	while(pos != -1 )
	{
		xsize = max(pos - lastpos,xsize);
		ysize++;
		lastpos = pos;
		pos = str.Find('\n',pos+1);
	}
	xsize = max(str.GetLength() - lastpos,xsize);
	ysize++;
	CSize s(FONT_SIZE_X*xsize ,FONT_SIZE_Y*ysize);
	CRect r(CPoint(0,0),s);
	return r;
}


