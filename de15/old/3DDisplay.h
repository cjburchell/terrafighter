// 3DDisplay.h: interface for the C3DDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DDISPLAY_H__084EF42A_6C10_4761_9E88_5CA724D040F8__INCLUDED_)
#define AFX_3DDISPLAY_H__084EF42A_6C10_4761_9E88_5CA724D040F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx8math.h>
#include "2DDraw.h"

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESIZEFAILED        0x8200000c




//-----------------------------------------------------------------------------
// Name: struct D3DModeInfo
// Desc: Structure for holding information about a display mode
//-----------------------------------------------------------------------------
struct D3DModeInfo
{
    DWORD      Width;      // Screen width in this mode
    DWORD      Height;     // Screen height in this mode
    D3DFORMAT  Format;     // Pixel format in this mode
    DWORD      dwBehavior; // Hardware / Software / Mixed vertex processing
    D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
};




//-----------------------------------------------------------------------------
// Name: struct D3DDeviceInfo
// Desc: Structure for holding information about a Direct3D device, including
//       a list of modes compatible with this device
//-----------------------------------------------------------------------------
struct D3DDeviceInfo
{
    // Device data
    D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
    D3DCAPS8     d3dCaps;         // Capabilities of this device
    const TCHAR* strDesc;         // Name of this device
    BOOL         bCanDoWindowed;  // Whether this device can work in windowed mode

    // Modes for this device
    DWORD        dwNumModes;
    D3DModeInfo  modes[150];

    // Current state
    DWORD        dwCurrentMode;
    BOOL         bWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleType;
};




//-----------------------------------------------------------------------------
// Name: struct D3DAdapterInfo
// Desc: Structure for holding information about an adapter, including a list
//       of devices available on this adapter
//-----------------------------------------------------------------------------
struct D3DAdapterInfo
{
    // Adapter data
    D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
    D3DDISPLAYMODE d3ddmDesktop;      // Desktop display mode for this adapter

    // Devices for this adapter
    DWORD          dwNumDevices;
    D3DDeviceInfo  devices[5];

    // Current state
    DWORD          dwCurrentDevice;
};


class C3DDisplay : public CDisplay  
{
public:
	void StartRender();
	HRESULT Present();
	HRESULT Clear(DWORD dwColor =0L);
	HRESULT Create(HWND hWnd);
	HRESULT CreateWindowedDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight );
	HRESULT CreateFullScreenDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP);
	C3DDisplay();
	virtual ~C3DDisplay();
protected:	
	C2DFont*					m_pFont;
	LPDIRECT3D8					m_pD3D;
	LPDIRECT3DDEVICE8			m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER8     m_pVB;
	HRESULT BuildDeviceList();
	D3DAdapterInfo m_Adapters[10];
	DWORD             m_dwNumAdapters;
    DWORD             m_dwAdapter;
	BOOL m_bUseDepthBuffer;
	HRESULT ConfirmDevice(D3DCAPS8*,DWORD,D3DFORMAT);
	BOOL    FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
                D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );

	DWORD             m_dwMinDepthBits;    // Minimum number of bits needed in depth buffer
    DWORD             m_dwMinStencilBits;  // Minimum number of bits needed in stencil buffer

	DWORD m_dwBPP;
	D3DPRESENT_PARAMETERS m_d3dpp;

	static INT_PTR CALLBACK SelectDeviceProc( HWND hDlg, UINT msg, 
                WPARAM wParam, LPARAM lParam );

public:
	HRESULT DrawGradientRect(RECT rect, COLORREF color1,COLORREF color2, BYTE flags);
	HRESULT GetVideoMode(HINSTANCE hInstance);
	HRESULT DrawBlendRect(RECT rect, COLORREF color);
	DWORD ConvertColor(DWORD col);
	D3DXVECTOR3 PointToScreen(POINT p);
	HRESULT Blt( DWORD x, DWORD y, CSurface* pSurface, RECT* prc );
	HRESULT DrawRect(RECT rect,COLORREF color);
	HRESULT DrawLine(POINT p1, POINT p2, COLORREF color);
	HRESULT DrawFilledRect(RECT rect, COLORREF color);
	void OutTextc(COLORREF crBackground, COLORREF crForeground,int x, int y, BOOL bTrasparent,char* format, ...);

	HRESULT CreateSurfaceFromBitmap( CSurface** ppSurface, LPCSTR strBMP,
		                             DWORD dwDesiredWidth,
									 DWORD dwDesiredHeight );

};

#endif // !defined(AFX_3DDISPLAY_H__084EF42A_6C10_4761_9E88_5CA724D040F8__INCLUDED_)
