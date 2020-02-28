//////////////////////////////////////////////////////////////////////////////////
// Project Name: [ CDX Class Library - CDX.lib ]
// Source File:  [ CInput Implementation ]
// Author:       [ Danny Farley - danny@jags.co.uk ]
// Revision:     [ 1.5 ]
//////////////////////////////////////////////////////////////////////////////////
#include <afx.h>
#include "Input.h"

static LPDIRECTINPUTDEVICE8 lpDID = NULL;
#define KEY_BUFFER_SIZE 1

//////////////////////////////////////////////////////////////////////////////////
// EnumJoystick
//////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
    HRESULT hr;
	

	LPDIRECTINPUT8 pDI = (LPDIRECTINPUT8)pContext;
    // Obtain an interface to the enumerated joystick.
    hr = pDI->CreateDevice( pdidInstance->guidInstance, &lpDID, NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
}

//////////////////////////////////////////////////////////////////////////////////
// CInput Constructor
//////////////////////////////////////////////////////////////////////////////////
CInput::CInput(void)
{
	m_lpDI = NULL;
	m_lpDIDKeyboard = NULL;
	m_lpDIDMouse = NULL;
	m_lpDIDJoystick = NULL;

	m_bMouse = FALSE;
	m_bKeyboard = FALSE;
	m_bJoystick = FALSE;

	Joystick.x = 0;
	Joystick.y = 0;
	Mouse.x = 0;
	Mouse.y = 0;
}

//////////////////////////////////////////////////////////////////////////////////
// CInput Destructor
//////////////////////////////////////////////////////////////////////////////////
CInput::~CInput(void)
{
	RELEASE(m_lpDIDKeyboard);
	RELEASE(m_lpDIDMouse);
	RELEASE(m_lpDIDJoystick);
	RELEASE(m_lpDI);
}

//////////////////////////////////////////////////////////////////////////////////
// CInput Create
//////////////////////////////////////////////////////////////////////////////////
BOOL CInput::Create(HWND hWnd)
{
	HRESULT rval;

	rval = DirectInput8Create(GetModuleHandle(NULL), 
							  DIRECTINPUT_VERSION, 
							  IID_IDirectInput8, 
							  (VOID**)&m_lpDI, 
							  NULL);
	if(rval != DI_OK) return FALSE;

	// Create the mouse device
	rval = m_lpDI->CreateDevice(GUID_SysMouse, &m_lpDIDMouse, NULL);
	if(rval == DI_OK)
	{
		m_lpDIDMouse->SetDataFormat(&c_dfDIMouse);
		m_lpDIDMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		rval = m_lpDIDMouse->Acquire();
		if(rval != DI_OK) return FALSE;

		m_bMouse = TRUE;
	}

	// Create the keyboard device
	rval = m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpDIDKeyboard, NULL);
	if(rval == DI_OK)
	{
		m_lpDIDKeyboard->SetDataFormat(&c_dfDIKeyboard);
		m_lpDIDKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		//DIPROPDWORD dipdw;

        //dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        //dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        //dipdw.diph.dwObj        = 0;
        //dipdw.diph.dwHow        = DIPH_DEVICE;
        //dipdw.dwData            = KEY_BUFFER_SIZE; // Arbitary buffer size

        //if( FAILED( rval = m_lpDIDKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
            //return FALSE;

		rval = m_lpDIDKeyboard->Acquire();
		if(rval != DI_OK) return FALSE;

		m_bKeyboard = TRUE;
	}

	// Enumerate the joystick device
	lpDID = NULL;
	rval = m_lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, (VOID*)m_lpDI, DIEDFL_ATTACHEDONLY);
	if(lpDID != NULL)
	{
		if(lpDID == NULL) return FALSE;

		m_lpDIDJoystick=lpDID;

		m_lpDIDJoystick->SetDataFormat(&c_dfDIJoystick);
		m_lpDIDJoystick->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		// Set the X-axis range (-1000 to +1000)
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -1000;
		diprg.lMax = +1000;

		if(m_lpDIDJoystick->SetProperty(DIPROP_RANGE, &diprg.diph) != DI_OK)
			return FALSE;

		// And again for Y-axis range
		diprg.diph.dwObj = DIJOFS_Y;

		if(m_lpDIDJoystick->SetProperty(DIPROP_RANGE, &diprg.diph) != DI_OK)
			return FALSE;

		// Set X axis dead zone to 10%
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = DIJOFS_X;
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = 1000;

		if(m_lpDIDJoystick->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK)
			return FALSE;

		dipdw.diph.dwObj = DIJOFS_Y;

		// Set Y axis dead zone to 10%
		if(m_lpDIDJoystick->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK)
			return FALSE;

		rval = m_lpDIDJoystick->Acquire();
		if(rval != DI_OK) return FALSE;

		m_bJoystick = TRUE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// CInput ReAcquire
//////////////////////////////////////////////////////////////////////////////////
void CInput::ReAcquire(void)
{
	m_lpDIDMouse->Acquire();
	m_lpDIDKeyboard->Acquire();
	m_lpDIDJoystick->Acquire();
}

//////////////////////////////////////////////////////////////////////////////////
// CInput UnAcquire
//////////////////////////////////////////////////////////////////////////////////
void CInput::UnAcquire(void)
{
	m_lpDIDMouse->Unacquire();
	m_lpDIDKeyboard->Unacquire();
	m_lpDIDJoystick->Unacquire();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// CInput Update
/////////////////////////////////////////////////////////////////////////////////////////////
void CInput::Update(void)
{
	if(m_bMouse)
	{
		if(m_lpDIDMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState) == (DIERR_INPUTLOST | DIERR_NOTACQUIRED))
			m_lpDIDMouse->Acquire();

		Mouse.x = m_MouseState.lX;
		Mouse.y = m_MouseState.lY;

		//MouseLB = m_MouseState.rgbButtons[0];
		//MouseRB = m_MouseState.rgbButtons[1];
		//MouseMB = m_MouseState.rgbButtons[2];
	}

	if(m_bKeyboard)
	{
		ZeroMemory( &Keys, sizeof(Keys) );
		HRESULT hr = m_lpDIDKeyboard->GetDeviceState(sizeof(Keys), &Keys);
		if(FAILED(hr))
		{
			hr = m_lpDIDKeyboard->Acquire();
			//while( hr == DIERR_INPUTLOST ) 
				//hr = m_lpDIDKeyboard->Acquire();
		}
	}

	if(m_bJoystick)
	{
		m_lpDIDJoystick->Poll();

		if(m_lpDIDJoystick->GetDeviceState(sizeof(m_JoyState), &m_JoyState) == (DIERR_INPUTLOST | DIERR_NOTACQUIRED))
			m_lpDIDJoystick->Acquire();

		Joystick.x = m_JoyState.lX;
		Joystick.y = m_JoyState.lY;

		//JoystickB1 = m_JoyState.rgbButtons[0];
		//JoystickB2 = m_JoyState.rgbButtons[1];
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// CInput SetActiveDevices
/////////////////////////////////////////////////////////////////////////////////////////////
void CInput::SetActiveDevices(BOOL bMouse, BOOL bKeyboard, BOOL bJoystick)
{
	m_bMouse = bMouse;
	m_bKeyboard = bKeyboard;
	m_bJoystick = bJoystick;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// CInput SetMouseRelative
/////////////////////////////////////////////////////////////////////////////////////////////
void CInput::SetMouseAbs(void)
{
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_ABS;
	m_lpDIDMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// CInput SetJoystickRelative
/////////////////////////////////////////////////////////////////////////////////////////////
void CInput::SetJoystickAbs(void)
{
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_ABS;
	m_lpDIDJoystick->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// CInput RunMouseControlPanel
/////////////////////////////////////////////////////////////////////////////////////////////
void CInput::RunMouseControlPanel(HWND hWnd)
{
	m_lpDIDMouse->RunControlPanel(hWnd, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// CInput RunJoystickControlPanel
/////////////////////////////////////////////////////////////////////////////////////////////
void CInput::RunJoystickControlPanel(HWND hWnd)
{
	m_lpDIDJoystick->RunControlPanel(hWnd, 0);
}


BOOL CInput::ClearKeybordBuffer()
{
	DWORD dwItems = INFINITE; 
	HRESULT hres = m_lpDIDKeyboard->GetDeviceData( 
				sizeof(DIDEVICEOBJECTDATA), 
				NULL, 
				&dwItems, 
				0); 
	if (SUCCEEDED(hres)) 
	{ 
		return TRUE;
	} 
	else
		return FALSE;
}

BOOL CInput::GetKeyPressed(DWORD &key, BOOL& isDown)
{
	DIDEVICEOBJECTDATA rgdod[KEY_BUFFER_SIZE]; 
	DWORD dwItems = KEY_BUFFER_SIZE; 
	HRESULT hres = m_lpDIDKeyboard->GetDeviceData( 
													sizeof(DIDEVICEOBJECTDATA), 
													rgdod, 
													&dwItems, 
													0); 
	if (SUCCEEDED(hres))
	{ 
		// dwItems = number of elements read (could be zero)
		if(dwItems != 0)
		{
			key = rgdod[1].dwOfs;
			isDown = rgdod[1].dwData & 0x80?TRUE:FALSE;
			return TRUE;
		}
	}
	return FALSE;
}



char CInput::ConvertKeyToChar(DWORD key)
{
	char convertchar = 0;
	switch(key)
	{
	case DIK_0:
		convertchar = '0';
		break;
	case DIK_1:
		convertchar = '1';
		break;
	case DIK_2:
		convertchar = '2';
		break;
	case DIK_3:
		convertchar = '3';
		break;
	case DIK_4:
		convertchar = '4';
		break;
	case DIK_5:
		convertchar = '5';
		break;
	case DIK_6:
		convertchar = '6';
		break;
	case DIK_7:
		convertchar = '7';
		break;
	case DIK_8:
		convertchar = '8';
		break;
	case DIK_9:
		convertchar = '9';
		break;
	case DIK_A:
		convertchar = 'A';
		break;
	case DIK_ADD:
		convertchar = '+';
		break;
	case DIK_APOSTROPHE:
		convertchar = '\'';
		break;
	case DIK_B:
		convertchar = 'B';
		break;
	case DIK_BACKSLASH:
		convertchar = '\\';
		break;
	case DIK_C:
		convertchar = 'C';
		break;
	case DIK_COMMA:
		convertchar = ',';
		break;
	case DIK_D:
		convertchar = 'D';
		break;
	case DIK_DECIMAL:
		convertchar = '.';
		break;
	case DIK_DIVIDE:
		convertchar = '/';
		break;
	case DIK_E:
		convertchar = 'E';
		break;
	case DIK_EQUALS:
		convertchar = '=';
		break;
	case DIK_F:
		convertchar = 'F';
		break;
	case DIK_G:
		convertchar = 'G';
		break;
	case DIK_GRAVE:  //Grave accent (`)
		convertchar = '`';
		break;
	case DIK_H:
		convertchar = 'H';
		break;
	case DIK_I:
		convertchar = 'I';
		break;
	case DIK_J:
		convertchar = 'J';
		break;
	case DIK_K:
		convertchar = 'K';
		break;
	case DIK_L:
		convertchar = 'L';
		break;
	case DIK_LBRACKET:  //Left square bracket [
		convertchar = '[';
		break;
    case DIK_M:
		convertchar = 'M';
		break;
	case DIK_MINUS:
		convertchar = '-';
		break;
	case DIK_MULTIPLY:
		convertchar = '*';
		break;
	case DIK_N:
		convertchar = 'N';
		break;
	case DIK_O:
		convertchar = 'O';
		break;
	case DIK_P:
		convertchar = 'P';
		break;
	case DIK_PERIOD:
		convertchar = '.';
		break;
	case DIK_Q:
		convertchar = 'Q';
		break;
	case DIK_R:
		convertchar = 'R';
		break;
	case DIK_RBRACKET:
		convertchar = 'S';
		break;
	case DIK_S:
		convertchar = 'T';
		break;
	case DIK_SEMICOLON :
		convertchar = ';';
		break;
	case DIK_SLASH:  //Forward slash (/) on main keyboard
		convertchar = '/';
		break;
	case DIK_SPACE:
		convertchar = ' ';
		break;
	case DIK_SUBTRACT:
		convertchar = '-';
		break;
	case DIK_T:
		convertchar = 'T';
		break;
	case DIK_U:
		convertchar = 'U';
		break;
	case DIK_V:
		convertchar = 'V';
		break;
	case DIK_W:
		convertchar = 'W';
		break;
	case DIK_X:
		convertchar = 'X';
		break;
	case DIK_Y:
		convertchar = 'Y';
		break;
	case DIK_Z:
		convertchar = 'Z';
		break;
	}
	return convertchar;
}
