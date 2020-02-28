///////////////////////////////////////////////////////////////////////////////
///  FOVClipper.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 10:25:48 AM by Christiaan Burchell
///
///  PURPOSE:  Interface for the CFOVClipper class.
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

#if !defined(AFX_FOVCLIPPER_H__9A5483E6_B27E_4B62_B485_E7372C17C015__INCLUDED_)
#define AFX_FOVCLIPPER_H__9A5483E6_B27E_4B62_B485_E7372C17C015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D3DUtil.h"

#define FOV_BACK_DISTACNE 128.0f

enum e_fov_location{FOV_Ouside=0,FOV_Inside=1,FOV_Bound=2};

class CFOVClipper  
{
public:
   void SetViewMatix(D3DXMATRIX* MatView);
   void SetupFOVClipPlanes( float Angle , float Aspect , float FrontClip , float BackClip );
   e_fov_location MeshFOVCheck(const D3DXVECTOR3& BSpherePos , float BSphereRadius, BOOL bCheckBack = TRUE, BOOL bCheckY = TRUE);
   CFOVClipper();
   virtual ~CFOVClipper();
private:
   D3DXPLANE	m_FOVLeftPlane;
   D3DXPLANE	m_FOVRightPlane;
   D3DXPLANE	m_FOVTopPlane;
   D3DXPLANE	m_FOVBottomPlane;
   float	m_FOVFrontClip;
   float	m_FOVBackClip;
   D3DXMATRIX	m_ViewTransform;
};

#endif // !defined(AFX_FOVCLIPPER_H__9A5483E6_B27E_4B62_B485_E7372C17C015__INCLUDED_)
