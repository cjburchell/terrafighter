#ifndef __INPUT_H
#define __INPUT_H

#pragma warning (disable : 4244)

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#define DIRECTINPUT_VERSION 0x0800
#define UINT_PTR UINT*
#include <dinput.h>
#include <vfw.h>

//////////////////////////////////////////////////////////////////////////////////
// CDXInput Class
//////////////////////////////////////////////////////////////////////////////////
class CInput
{
public:
	CInput(void);
	~CInput(void);

	BOOL Create(HWND hWnd);
	void ReAcquire(void);
	void UnAcquire(void);
	void Update(void);

	void SetActiveDevices(BOOL bMouse, BOOL bKeyboard, BOOL bJoystick);
	void SetMouseAbs(void);
	void SetJoystickAbs(void);
	void RunMouseControlPanel(HWND hWnd = NULL);
	void RunJoystickControlPanel(HWND hWnd = NULL);

public:
	char ConvertKeyToChar(DWORD key);
	BOOL GetKeyPressed(DWORD& key, BOOL& isDown);
	BOOL ClearKeybordBuffer();
	LPDIRECTINPUT8 m_lpDI;
	LPDIRECTINPUTDEVICE8 m_lpDIDKeyboard;
	LPDIRECTINPUTDEVICE8 m_lpDIDMouse;
	LPDIRECTINPUTDEVICE8 m_lpDIDJoystick;


	DIMOUSESTATE m_MouseState;
	DIJOYSTATE m_JoyState;
	POINT Mouse;
	POINT Joystick;
	BYTE Keys[256];

	BOOL m_bMouse;
	BOOL m_bKeyboard;
	BOOL m_bJoystick;
};

#define RELEASE(x) if(x != NULL) { x->Release(); x = NULL; }
#define ODS(a) OutputDebugString(a)

#endif