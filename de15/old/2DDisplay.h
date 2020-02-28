// 2DDisplay.h: interface for the C2DDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DDISPLAY_H__32CA1479_936E_434A_A067_57BBEA7239E9__INCLUDED_)
#define AFX_2DDISPLAY_H__32CA1479_936E_434A_A067_57BBEA7239E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2DDraw.h"

class C2DDisplay : public C2DDraw  
{
public:
	C2DDisplay();
	virtual ~C2DDisplay();
protected:
	virtual HRESULT DestroyObjects();
    LPDIRECTDRAW7        m_pDD;
    LPDIRECTDRAWSURFACE7 m_pddsFrontBuffer;
    LPDIRECTDRAWSURFACE7 m_pddsBackBuffer;
    LPDIRECTDRAWSURFACE7 m_pddsBackBufferLeft; // For stereo modes

	BOOL                 m_bStereo;

	HRESULT CreateSurfaceFromText( CSurface** ppSurface, HFONT hFont,
		                           LPCSTR strText, 
								   COLORREF crBackground,
								   COLORREF crForeground );

	HRESULT CreateSurface( CSurface** ppSurface, DWORD dwWidth,
		                   DWORD dwHeight );
	HRESULT ColorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds,
                         RECT* prc = NULL );
	HRESULT Blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds,
		         RECT* prc=NULL, DWORD dwFlags=0 );
	HRESULT InitClipper();

public:
	HRESULT Create(HWND hWnd);
	HRESULT GetVideoMode(HINSTANCE hInstance);
	HRESULT DrawBlendRect(RECT rect, COLORREF color);
	void StartRender();
	LPDIRECTDRAW7        GetDirectDraw()     { return m_pDD; }
    LPDIRECTDRAWSURFACE7 GetFrontBuffer()    { return m_pddsFrontBuffer; }
    LPDIRECTDRAWSURFACE7 GetBackBuffer()     { return m_pddsBackBuffer; }
    LPDIRECTDRAWSURFACE7 GetBackBufferLEft() { return m_pddsBackBufferLeft; }
	BOOL    IsStereo()                       { return m_bStereo; }

	HRESULT CreateFullScreenDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight,
		                             DWORD dwBPP );
	HRESULT CreateWindowedDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight );
	HRESULT Present();

	HRESULT CreateSurfaceFromBitmap( CSurface** ppSurface, LPCSTR strBMP,
		                             DWORD dwDesiredWidth,
									 DWORD dwDesiredHeight );
	HRESULT Blt( DWORD x, DWORD y, CSurface* pSurface, RECT* prc = NULL );
	HRESULT Clear( DWORD dwColor = 0L );
	HRESULT DrawRect(RECT rect,COLORREF color);
	HRESULT DrawLine(POINT p1, POINT p2, COLORREF color);
	void OutTextc(COLORREF crBackground, COLORREF crForeground,int x, int y, BOOL bTrasparent,char* format, ...);
	HRESULT DrawFilledRect(RECT rect, COLORREF color);
};

#endif // !defined(AFX_2DDISPLAY_H__32CA1479_936E_434A_A067_57BBEA7239E9__INCLUDED_)
