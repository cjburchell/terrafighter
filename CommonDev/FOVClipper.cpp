///////////////////////////////////////////////////////////////////////////////
///  FOVClipper.cpp
///
///  PATH: D:\dev2\CommonDev
///
///  CREATED: 02/06/2004 11:21:41 PM by Christiaan Burchell
///
///  PURPOSE:  Implementation of the CFOVClipper class.
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

#include "stdafx.h"
#include "FOVClipper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CFOVClipper
///
/// CLASS: CFOVClipper
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 10:27:05 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CFOVClipper::CFOVClipper()
{

}



/////////////////////////////////////////////////
///
/// NAME: ~CFOVClipper
///
/// CLASS: CFOVClipper
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 10:27:07 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CFOVClipper::~CFOVClipper()
{

}



/////////////////////////////////////////////////
///
/// NAME: SetupFOVClipPlanes
///
/// CLASS: CFOVClipper
///
/// DESCRIPTION: Sets up the clip Planes
///
/// CREATED:     02/06/2004 10:27:19 AM
///
/// PARAMETERS: 
///             float Angle : Angle of the view fustrum
///             float Aspect : aspect ratio
///             float FrontClip : the Front clip distance
///             float BackClip : the back clip distance
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CFOVClipper::SetupFOVClipPlanes( float Angle , float Aspect , float FrontClip , float BackClip )
{
   D3DXVECTOR3	P0 , P1 , P2;
   m_FOVFrontClip = FrontClip;
   m_FOVBackClip = BackClip;


   //	Calculate left plane using 3 points
   P0.x = 0.0f;
   P0.y = 0.0f;
   P0.z = 0.0f;
   P1.x = -BackClip * ( (float)tanf( Angle * 0.5f ) / Aspect );
   P1.y = -BackClip * ( (float)tanf( Angle * 0.5f ) );
   P1.z = BackClip;
   P2.x = P1.x;
   P2.y = -P1.y;
   P2.z = P1.z;
   D3DXPlaneFromPoints(&m_FOVLeftPlane, &P0, &P1 , &P2);

   //	Calculate right plane using 3 points
   P0.x = 0.0f;
   P0.y = 0.0f;
   P0.z = 0.0f;
   P1.x = BackClip * ( (float)tanf( Angle * 0.5f ) / Aspect);
   P1.y = BackClip * ( (float)tanf( Angle * 0.5f ) );
   P1.z = BackClip;
   P2.x = P1.x;
   P2.y = -P1.y;
   P2.z = P1.z;
   D3DXPlaneFromPoints(&m_FOVRightPlane, &P0, &P1 , &P2);

   //	Calculate top plane using 3 points
   P0.x = 0.0f;
   P0.y = 0.0f;
   P0.z = 0.0f;
   P1.x = -BackClip * ( (float)tanf( Angle * 0.5f ) / Aspect);
   P1.y = BackClip * ( (float)tanf( Angle * 0.5f ) );
   P1.z = BackClip;
   P2.x = -P1.x;
   P2.y = P1.y;
   P2.z = P1.z;
   D3DXPlaneFromPoints(&m_FOVTopPlane, &P0, &P1 , &P2 );

   //	Calculate bottom plane using 3 points
   P0.x = 0.0f;
   P0.y = 0.0f;
   P0.z = 0.0f;
   P1.x = BackClip * ( (float)tanf( Angle * 0.5f ) / Aspect);
   P1.y = -BackClip * ( (float)tanf( Angle * 0.5f ) );
   P1.z = BackClip;
   P2.x = -P1.x;
   P2.y = P1.y;
   P2.z = P1.z;
   D3DXPlaneFromPoints(&m_FOVBottomPlane, &P0, &P1 , &P2 );
}




/////////////////////////////////////////////////
///
/// NAME: MeshFOVCheck
///
/// CLASS: CFOVClipper
///
/// DESCRIPTION: Checks the sphere is in the FOV
///
/// CREATED:     02/06/2004 10:29:18 AM
///
/// PARAMETERS: 
///             D3DXVECTOR3 *pBSpherePos the position of the sphere
///             float BSphereRadius the radius of the sphere
///             BOOL bCheckBack : check the back clip distance.
///             BOOL bCheckY : check the y value, if it is false then FOV only dose a 2D check
///
/// RETURN: e_fov_location 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
e_fov_location CFOVClipper::MeshFOVCheck(const D3DXVECTOR3& BSpherePos , float BSphereRadius,BOOL bCheckBack, BOOL bCheckY)
{
   float	Dist;
   D3DXVECTOR3 ViewPos;
   BOOL bBound = FALSE;

   //	Transform Z into view space
   ViewPos.z = m_ViewTransform._13 * BSpherePos.x +
      m_ViewTransform._23 * BSpherePos.y +
      m_ViewTransform._33 * BSpherePos.z + m_ViewTransform._43;

   //	Behind front clip plane?
   if(ViewPos.z < m_FOVFrontClip )
   {
      if( ( ViewPos.z + BSphereRadius ) < m_FOVFrontClip )
         return FOV_Ouside;
      else
         bBound = TRUE;
   }
   else
   {
      if(( ViewPos.z - BSphereRadius ) < m_FOVFrontClip)
         bBound = TRUE; 
   }

   //	Beyond the back clip plane?
   if(bCheckBack)
   {
      if(ViewPos.z > m_FOVBackClip)
      {
         if( ( ViewPos.z - BSphereRadius ) > m_FOVBackClip )
            return FOV_Ouside;
         else
            bBound = TRUE;
      }
      else
      {
         if(( ViewPos.z + BSphereRadius ) < m_FOVFrontClip)
            bBound = TRUE; 
      }
   }

   //	Transform X into view space
   ViewPos.x = m_ViewTransform._11 * BSpherePos.x +
      m_ViewTransform._21 * BSpherePos.y +
      m_ViewTransform._31 * BSpherePos.z + m_ViewTransform._41;

   //	Test against left clip plane
   Dist = ( ViewPos.x * m_FOVLeftPlane.a ) + ( ViewPos.z * m_FOVLeftPlane.c );

   if(Dist > 0.0f)
   {
      if( Dist > BSphereRadius )
         return FOV_Ouside;
      else
         bBound = TRUE;
   }
   else
   {
      if( Dist > -BSphereRadius )
         bBound = TRUE;
   }


   //	Test against right clip plane
   Dist = ( ViewPos.x * m_FOVRightPlane.a ) + ( ViewPos.z * m_FOVRightPlane.c );
   if(Dist > 0.0f)
   {
      if( Dist > BSphereRadius )
         return FOV_Ouside;
      else
         bBound = TRUE;
   }
   else
   {
      if( Dist > -BSphereRadius )
         bBound = TRUE;
   }

   if(bCheckY)
   {
      //	Transform Y into view space
      ViewPos.y = m_ViewTransform._12 * BSpherePos.x +
         m_ViewTransform._22 * BSpherePos.y +
         m_ViewTransform._32 * BSpherePos.z + m_ViewTransform._42;

      //	Test against top clip plane
      Dist = ( ViewPos.y * m_FOVTopPlane.b ) + ( ViewPos.z * m_FOVTopPlane.c );
      if(Dist > 0.0f)
      {
         if( Dist > BSphereRadius )
            return FOV_Ouside;
         else
            bBound = TRUE;
      }
      else
      {
         if( Dist > -BSphereRadius )
            bBound = TRUE;
      }

      //	Test against bottom plane
      Dist = ( ViewPos.y * m_FOVBottomPlane.b ) + ( ViewPos.z * m_FOVBottomPlane.c);
      if(Dist > 0.0f)
      {
         if( Dist > BSphereRadius )
            return FOV_Ouside;
         else
            bBound = TRUE;
      }
      else
      {
         if( Dist > -BSphereRadius )
            bBound = TRUE;
      }
   }

   //	Mesh is inside the field of view
   return bBound?FOV_Bound:FOV_Inside;
}



/////////////////////////////////////////////////
///
/// NAME: SetViewMatix
///
/// CLASS: CFOVClipper
///
/// DESCRIPTION: Sets the view matrix
///
/// CREATED:     02/06/2004 10:28:24 AM
///
/// PARAMETERS: 
///             D3DXMATRIX* MatView
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CFOVClipper::SetViewMatix(D3DXMATRIX* MatView)
{
   m_ViewTransform = *MatView;
}
