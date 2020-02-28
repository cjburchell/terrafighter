//-----------------------------------------------------------------------------
// File: ddutil.cpp
//
// Desc: Routines for loading bitmap and palettes from resources
//
// Copyright (C) 1998-1999 Microsoft Corporation. All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef DDUTIL_H
#define DDUTIL_H

#include <ddraw.h>
#include <d3d.h>




//-----------------------------------------------------------------------------
// Classes defined in this header file 
//-----------------------------------------------------------------------------
class CDisplay;
class CSurface;




//-----------------------------------------------------------------------------
// Flags for the CDisplay and CSurface methods
//-----------------------------------------------------------------------------
#define DSURFACELOCK_READ
#define DSURFACELOCK_WRITE

#define FONT_SIZE_X 8
#define FONT_SIZE_Y 13

#define GR_TL 0x01
#define GR_TR 0x02
#define GR_BR 0x04
#define GR_BL 0x08



//-----------------------------------------------------------------------------
// Name: class CDisplay
// Desc: Class to handle all DDraw aspects of a display, including creation of
//       front and back buffers, creating offscreen surfaces and palettes,
//       and blitting surface and displaying bitmaps.
//-----------------------------------------------------------------------------
class CDisplay
{
protected:
    RECT                 m_rcWindow;
    BOOL                 m_bWindowed;

public:
	virtual HRESULT DrawGradientRect(RECT rect, COLORREF color1,COLORREF color2, BYTE flags) = 0;
	virtual HRESULT Create(HWND hWnd) = 0;
	virtual HRESULT GetVideoMode(HINSTANCE hInstance) =0;
	virtual HRESULT DrawBlendRect(RECT rect, COLORREF color) = 0;
	RECT GetTextRect(CString str);
	virtual void OutTextc(COLORREF crBackground, COLORREF crForeground,int x, int y, BOOL bTrasparent,char* format, ...) = 0;
	virtual HRESULT DrawFilledRect(RECT rect, COLORREF color) =0;
	virtual HRESULT DrawLine(POINT p1, POINT p2, COLORREF color) = 0;
	virtual void StartRender() = 0;
	HWND                 m_hWnd;
	DWORD m_dwWidth;
	DWORD m_dwHeight;

	void outtexts(
			 COLORREF crBackground, 
			 COLORREF crForeground,
			 int x, // IN: x position of the text 
			 int y, // IN: y position of the text 
			 char* s // IN: the string to be outputed
			 );
	void outtext(int x, int y, char* format, ...);
	HRESULT FlipToGDISurface();
    CDisplay();
    virtual ~CDisplay();

    // Access functions
    HWND                 GetHWnd()           { return m_hWnd; }
    
    // Status functions
    BOOL    IsWindowed()                     { return m_bWindowed; }

    // Creation/destruction methods
    virtual HRESULT CreateFullScreenDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight,
		                             DWORD dwBPP ) = 0;
    virtual HRESULT CreateWindowedDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight ) = 0;
    HRESULT UpdateBounds();

    // Methods to create child objects
    
    virtual HRESULT CreateSurfaceFromBitmap( CSurface** ppSurface, LPCSTR strBMP,
		                             DWORD dwDesiredWidth,
									 DWORD dwDesiredHeight ) = 0;
    
    //HRESULT CreatePaletteFromBitmap( LPDIRECTDRAWPALETTE* ppPalette, const TCHAR* strBMP );

    // Display methods
	virtual HRESULT DrawRect(RECT rect,COLORREF color) = 0;
	HRESULT DrawRect(int x, int y,int w, int h,COLORREF color);
	HRESULT DrawRectAroundText(int x, int y,int w, int h,COLORREF color);
    virtual HRESULT Clear( DWORD dwColor = 0L ) = 0;
    
    
    virtual HRESULT Blt( DWORD x, DWORD y, CSurface* pSurface, RECT* prc = NULL ) = 0;
    //HRESULT ShowBitmap( HBITMAP hbm, LPDIRECTDRAWPALETTE pPalette=NULL );
    //HRESULT SetPalette( LPDIRECTDRAWPALETTE pPalette );
    virtual HRESULT Present() = 0;
};




//-----------------------------------------------------------------------------
// Name: class CSurface
// Desc: Class to handle aspects of a DirectDrawSurface.
//-----------------------------------------------------------------------------
class CSurface
{
protected:
    BOOL                 m_bColorKeyed;

public:
	
    
    BOOL                 IsColorKeyed()    { return m_bColorKeyed; }
	virtual HRESULT SetColorKey( DWORD dwColorKey ) = 0;

    
    CSurface();
    virtual~CSurface();
};




#endif // DDUTIL_H

