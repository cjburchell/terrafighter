// 2DSurface.h: interface for the C2DSurface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DSURFACE_H__CCC2F71D_8A4E_41A4_AFA2_044052F06AA2__INCLUDED_)
#define AFX_2DSURFACE_H__CCC2F71D_8A4E_41A4_AFA2_044052F06AA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2DDraw.h"

class C2DSurface : public CSurface  
{
public:
	HRESULT Clear( DWORD dwColor );
	C2DSurface();
	virtual ~C2DSurface();
	LPDIRECTDRAWSURFACE7 GetDDrawSurface() { return m_pdds; }
	HRESULT Create( LPDIRECTDRAW7 pDD, DDSURFACEDESC2* pddsd );
    HRESULT Create( LPDIRECTDRAWSURFACE7 pdds );
	HRESULT DrawBitmap( HBITMAP hBMP, 
						DWORD dwBMPOriginX = 0, DWORD dwBMPOriginY = 0, 
		                DWORD dwBMPWidth = 0, DWORD dwBMPHeight = 0 );
    HRESULT DrawBitmap( TCHAR* strBMP, DWORD dwDesiredWidth, DWORD dwDesiredHeight );

	HRESULT DrawBitmapAt( TCHAR* strBMP, DWORD dwDestX, DWORD dwDestY,DWORD dwDesiredWidth, DWORD dwDesiredHeight );

    HRESULT DrawText( HFONT hFont, LPCSTR strText, DWORD dwOriginX, DWORD dwOriginY,
		              COLORREF crBackground, COLORREF crForeground );

    HRESULT SetColorKey( DWORD dwColorKey );
    DWORD   ConvertGDIColor( COLORREF dwGDIColor );
    static HRESULT GetBitMaskInfo( DWORD dwBitMask, DWORD* pdwShift, DWORD* pdwBits );

	//HRESULT Blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds, RECT* prc, DWORD dwFlags );
	//HRESULT Blt( DWORD x, DWORD y, CSurface* pSurface, RECT* prc );


private:
	HRESULT Destroy();
	LPDIRECTDRAWSURFACE7 m_pdds;
    DDSURFACEDESC2       m_ddsd;

};

#endif // !defined(AFX_2DSURFACE_H__CCC2F71D_8A4E_41A4_AFA2_044052F06AA2__INCLUDED_)
