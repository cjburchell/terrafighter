///////////////////////////////////////////////////////////////////////////////
///  Input.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:27:47 PM by Christiaan Burchell
///
///  PURPOSE:  Input class
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

// Input.h: interface for the CInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUT_H__17C30A22_3263_4C69_87F4_99CB7A68AB86__INCLUDED_)
#define AFX_INPUT_H__17C30A22_3263_4C69_87F4_99CB7A68AB86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma warning( disable : 4786 )
#include <list>

class CInput  
{
   static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext );
   static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance,VOID* pContext);
public:
   BOOL FoundJoystick();

   BOOL FoundMouse()
   {
      return  NULL != m_pMouse;
   }
   BOOL IsKeyDown(BYTE key)
   {
      return m_diks[key] & 0x80;
   }

   BOOL IsMouseButtonDown(BYTE button)
   {
      if(FoundMouse())
         return m_dims2.rgbButtons[button] & 0x80;
      return FALSE;
   }

   LONG GetMouseXPos()
   {
      return m_dims2.lX;
   }
   LONG GetMouseYPos()
   {
      return m_dims2.lY;
   }
   LONG GetMouseZPos()
   {
      return m_dims2.lZ;
   }

   BOOL IsJoyButtonDown(BYTE button)
   {
      if(FoundJoystick())
         return m_js.rgbButtons[button] & 0x80;
      return FALSE;
   }

   LONG GetJoyXPos()
   {
      return m_js.lX;
   }
   LONG GetJoyYPos()
   {
      return m_js.lY;
   }
   LONG GetJoyZPos()
   {
      return m_js.lZ;
   }

   LONG GetJoyZRot()
   {
      return m_js.lRz;
   }

   LONG GetJoyXRot()
   {
      return m_js.lRx;
   }

   LONG GetJoyYRot()
   {
      return m_js.lRy;
   }

   LONG GetJoySlider(BYTE slider)
   {
      return m_js.rglSlider[slider];
   }

   LONG GetJoyPOV(BYTE slider)
   {
      return m_js.rgdwPOV[slider];
   }

   CInput();
   virtual ~CInput();
   HRESULT CreateDevice(HWND hWnd);
   HRESULT Update();


   // UI button Presses
   void ClearButtonPressBuffer();
   BOOL IsButtonPressEmpty();
   int RemoveButtonPress();
   void AddButtonPress(int button);

private:
   LPDIRECTINPUT8       m_pDI;         
   LPDIRECTINPUTDEVICE8 m_pKeyboard; 
   LPDIRECTINPUTDEVICE8 m_pMouse;
   LPDIRECTINPUTDEVICE8 m_pJoystick;

   DIMOUSESTATE2 m_dims2;
   BYTE m_diks[256];
   DIJOYSTATE2 m_js;

   std::list<int> m_list;
};

#endif // !defined(AFX_INPUT_H__17C30A22_3263_4C69_87F4_99CB7A68AB86__INCLUDED_)
