///////////////////////////////////////////////////////////////////////////////
///  Input.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:28:30 PM by Christiaan Burchell
///
///  PURPOSE:  The Input Class
///
///  COPYRIGHT NOTICE:   Copyright (C) 2004 Redpoint Games 
///
///   This software is provided 'as-is', without any express or implied 
///   warranty.  In no event will the author be held liable for any damages 
///   arising from the use of this software. 
/// 
///   Permission is granted to anyone to use this software for any purpose,
///   excluding commercial applications, and to alter it and redistribute it
///   freely, subject to the following restrictions:
/// 
///   1. The origin of this software must not be misrepresented; you must not
///      claim that you wrote the original software. If you use this software
///      in a product, an acknowledgment in the product documentation would be
///      appreciated but is not required.
///   2. Altered source versions must be plainly marked as such, and must not be
///      misrepresented as being the original software.
///   3. This notice may not be removed or altered from any source distribution.
///   4. The author permission is required to use this software in commercial 
///      applications 
///
///  LAST CHANGED: $Date$
///
///  REVISION HISTORY:
///  $Log$
/// 

// Input.cpp: implementation of the CInput class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <Input.h>
#include <DXUtil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CInput
///
/// CLASS: CInput
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:28:35 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CInput::CInput()
{
   m_pKeyboard = NULL;
   m_pJoystick = NULL;
   m_pDI = NULL;
   m_pMouse = NULL;
}



/////////////////////////////////////////////////
///
/// NAME: ~CInput
///
/// CLASS: CInput
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:28:37 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CInput::~CInput()
{
   // Unacquire the device one last time just in case 
   // the app tried to exit while the device is still acquired.
   if( m_pKeyboard ) 
      m_pKeyboard->Unacquire();

   if(m_pMouse)
      m_pMouse->Unacquire();

   if(m_pJoystick)
      m_pJoystick->Unacquire();

   // Release any DirectInput objects.
   SAFE_RELEASE( m_pKeyboard );
   SAFE_RELEASE( m_pJoystick );
   SAFE_RELEASE( m_pMouse );
   SAFE_RELEASE( m_pDI );
}


//-----------------------------------------------------------------------------
// Name: OnCreateDevice()
// Desc: Setups a the keyboard device using the flags from the dialog.
//-----------------------------------------------------------------------------
HRESULT CInput::CreateDevice(HWND hWnd)
{
   HRESULT hr;
   DWORD   dwCoopFlags;

#ifdef _WIN64
   HINSTANCE hInst = (HINSTANCE) GetWindowLongPtr( hWnd, GWLP_HINSTANCE );
#else
   HINSTANCE hInst = (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE );
#endif

   dwCoopFlags = DISCL_NONEXCLUSIVE;
   dwCoopFlags |= DISCL_FOREGROUND;

   // Create a DInput object
   if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
      IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
      return hr;

   // Obtain an interface to the system keyboard device.
   if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
      return hr;

   // Set the data format to "keyboard format" - a predefined data format 
   //
   // A data format specifies which controls on a device we
   // are interested in, and how they should be reported.
   //
   // This tells DirectInput that we will be passing an array
   // of 256 bytes to IDirectInputDevice::GetDeviceState.
   if( FAILED( hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
      return hr;

   // Set the cooperativity level to let DirectInput know how
   // this device should interact with the system and with other
   // DirectInput applications.
   hr = m_pKeyboard->SetCooperativeLevel( hWnd, dwCoopFlags );
   if( FAILED(hr) )
      return hr;

   // Acquire the newly created device
   m_pKeyboard->Acquire();

   // create mouse
   // Obtain an interface to the system mouse device.
   if( FAILED( hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL ) ) )
      return hr;


   // Set the data format to "mouse format" - a predefined data format 
   //
   // A data format specifies which controls on a device we
   // are interested in, and how they should be reported.
   //
   // This tells DirectInput that we will be passing a
   // DIMOUSESTATE2 structure to IDirectInputDevice::GetDeviceState.
   if( FAILED( hr = m_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
      return hr;

   // Set the cooperativity level to let DirectInput know how
   // this device should interact with the system and with other
   // DirectInput applications.
   hr = m_pMouse->SetCooperativeLevel( hWnd, dwCoopFlags );
   if( FAILED(hr) )
      return hr;

   m_pMouse->Acquire();

   //create joystick
   // Look for a simple joystick we can use for this sample program.
   if( FAILED( hr = m_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
      CInput::EnumJoysticksCallback,
      (VOID*)this, DIEDFL_ATTACHEDONLY ) ) )
      return hr;

   // Make sure we got a joystick
   if( NULL == m_pJoystick )
   {
      return S_OK;
   }


   // Set the data format to "simple joystick" - a predefined data format 
   //
   // A data format specifies which controls on a device we are interested in,
   // and how they should be reported. This tells DInput that we will be
   // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
   if( FAILED( hr = m_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
      return hr;

   // Set the cooperative level to let DInput know how this device should
   // interact with the system and with other DInput applications.
   if( FAILED( hr = m_pJoystick->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | 
      DISCL_FOREGROUND ) ) )
      return hr;

   // Enumerate the joystick objects. The callback function enabled user
   // interface elements for objects that are found, and sets the min/max
   // values property for discovered axes.
   if( FAILED( hr = m_pJoystick->EnumObjects( CInput::EnumObjectsCallback, 
      (VOID*)this, DIDFT_ALL ) ) )
      return hr;

   m_pJoystick->Acquire();


   return S_OK;
}


//-----------------------------------------------------------------------------
// Name: ReadImmediateData()
// Desc: Read the input device's state when in immediate mode and display it.
//-----------------------------------------------------------------------------
HRESULT CInput::Update()
{
   HRESULT hr;  

   if( NULL == m_pKeyboard ) 
      return S_OK;

   // Get the input's device state, and put the state in dims
   ZeroMemory( &m_diks, sizeof(m_diks) );
   hr = m_pKeyboard->GetDeviceState( sizeof(m_diks), &m_diks );
   if( FAILED(hr) ) 
   {
      // DirectInput may be telling us that the input stream has been
      // interrupted.  We aren't tracking any state between polls, so
      // we don't have any special reset that needs to be done.
      // We just re-acquire and try again.

      // If input is lost then acquire and keep trying 
      hr = m_pKeyboard->Acquire();
      while( hr == DIERR_INPUTLOST ) 
         hr = m_pKeyboard->Acquire();

      // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
      // may occur when the app is minimized or in the process of 
      // switching, so just try again later 
      return S_OK; 
   } 


   if( NULL == m_pMouse ) 
      return S_OK;

   // Get the input's device state, and put the state in dims
   ZeroMemory( &m_dims2, sizeof(m_dims2) );
   hr = m_pMouse->GetDeviceState( sizeof(m_dims2), &m_dims2 );
   if( FAILED(hr) ) 
   {
      // DirectInput may be telling us that the input stream has been
      // interrupted.  We aren't tracking any state between polls, so
      // we don't have any special reset that needs to be done.
      // We just re-acquire and try again.

      // If input is lost then acquire and keep trying 
      hr = m_pMouse->Acquire();
      while( hr == DIERR_INPUTLOST ) 
         hr = m_pMouse->Acquire();

      // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
      // may occur when the app is minimized or in the process of 
      // switching, so just try again later 
      return S_OK; 
   }

   if( NULL == m_pJoystick ) 
      return S_OK;

   // Poll the device to read the current state
   hr = m_pJoystick->Poll(); 
   if( FAILED(hr) )  
   {
      // DInput is telling us that the input stream has been
      // interrupted. We aren't tracking any state between polls, so
      // we don't have any special reset that needs to be done. We
      // just re-acquire and try again.
      hr = m_pJoystick->Acquire();
      while( hr == DIERR_INPUTLOST ) 
         hr = m_pJoystick->Acquire();

      // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
      // may occur when the app is minimized or in the process of 
      // switching, so just try again later 
      return S_OK; 
   }

   // Get the input's device state
   if( FAILED( hr = m_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &m_js ) ) )
      return hr; // The device should have been acquired during the Poll()

   return S_OK;
}

//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK CInput::EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                            VOID* pContext )
{
   HRESULT hr;

   CInput* pInput = (CInput*)pContext;
   // Obtain an interface to the enumerated joystick.
   hr = pInput->m_pDI->CreateDevice( pdidInstance->guidInstance, &(pInput->m_pJoystick), NULL );

   // If it failed, then we can't use this joystick. (Maybe the user unplugged
   // it while we were in the middle of enumerating it.)
   if( FAILED(hr) ) 
      return DIENUM_CONTINUE;

   // Stop enumeration. Note: we're just taking the first joystick we get. You
   // could store all the enumerated joysticks and let the user pick.
   return DIENUM_STOP;
}




//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK CInput::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                          VOID* pContext )
{
   CInput* pInput = (CInput*)pContext;
   // For axes that are returned, set the DIPROP_RANGE property for the
   // enumerated axis in order to scale min/max values.
   if( pdidoi->dwType & DIDFT_AXIS )
   {
      DIPROPRANGE diprg; 
      diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
      diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
      diprg.diph.dwHow        = DIPH_BYID; 
      diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
      diprg.lMin              = -1000; 
      diprg.lMax              = +1000; 

      // Set the range for the axis
      if( FAILED( pInput->m_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
         return DIENUM_STOP;

   }
   return DIENUM_CONTINUE;
}



/////////////////////////////////////////////////
///
/// NAME: AddButtonPress
///
/// CLASS: CInput
///
/// DESCRIPTION: Adds a button press
///
/// CREATED:     02/06/2004 12:28:55 PM
///
/// PARAMETERS: 
///             int button
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CInput::AddButtonPress(int button)
{
   m_list.push_back(button);
}



/////////////////////////////////////////////////
///
/// NAME: RemoveButtonPress
///
/// CLASS: CInput
///
/// DESCRIPTION: Removes the button press
///
/// CREATED:     02/06/2004 12:29:08 PM
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CInput::RemoveButtonPress()
{
   int var  = m_list.front();
   m_list.pop_front();
   return var;
}



/////////////////////////////////////////////////
///
/// NAME: IsButtonPressEmpty
///
/// CLASS: CInput
///
/// DESCRIPTION: Checks if the press is empty
///
/// CREATED:     02/06/2004 12:29:20 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CInput::IsButtonPressEmpty()
{
   return m_list.empty();
}



/////////////////////////////////////////////////
///
/// NAME: ClearButtonPressBuffer
///
/// CLASS: CInput
///
/// DESCRIPTION: Clear the buffer
///
/// CREATED:     02/06/2004 12:29:30 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CInput::ClearButtonPressBuffer()
{
   m_list.clear();
}



/////////////////////////////////////////////////
///
/// NAME: FoundJoystick
///
/// CLASS: CInput
///
/// DESCRIPTION: Found joystick input
///
/// CREATED:     02/06/2004 12:29:42 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CInput::FoundJoystick()
{
   return NULL != m_pJoystick;
}
