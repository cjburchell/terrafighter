///////////////////////////////////////////////////////////////////////////////
///  WinArrowButton.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 10:10:11 AM by Christiaan Burchell
///
///  PURPOSE:  Implementation of the CWinArrowButton class.
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
#include "WinArrowButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinArrowButton
///
/// CLASS: CWinArrowButton
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 10:09:56 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinArrowButton::CWinArrowButton() : CWinButton()
{
   m_Type = WINARROW_RIGHT;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinArrowButton
///
/// CLASS: CWinArrowButton
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 10:09:58 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinArrowButton::~CWinArrowButton()
{

}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinArrowButton
///
/// DESCRIPTION: Displays the control
///
/// CREATED:     02/06/2004 10:10:35 AM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinArrowButton::Display(C2DDraw *pDisplay)
{
   CRect rect = GetDrawRect();
   switch(m_Type)
   {
   case WINARROW_RIGHT:
      {

         int CenterPoint = rect.top + ((rect.bottom - rect.top) / 2);

         if(m_bPressed)
         {
            // |
            pDisplay->DrawLine(CPoint(rect.left+1,rect.top+1),CPoint(rect.left+1,rect.bottom),m_BorderColor3);
            pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.left,rect.bottom+1),m_BorderColor);

            // \ 
            pDisplay->DrawLine(CPoint(rect.left+1,rect.top+1),CPoint(rect.right,CenterPoint+1),m_BorderColor3);
            pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.right+1,CenterPoint),m_BorderColor);

            // /
            pDisplay->DrawLine(CPoint(rect.right,CenterPoint),CPoint(rect.left-1,rect.bottom),m_BorderColor2);
         }
         else
         {
            // |
            pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.left,rect.bottom),m_BorderColor2);

            // /
            pDisplay->DrawLine(CPoint(rect.right-1,CenterPoint-1),CPoint(rect.left,rect.bottom-1),m_BorderColor3);
            pDisplay->DrawLine(CPoint(rect.right,CenterPoint),CPoint(rect.left-1,rect.bottom),m_BorderColor);

            // \ 
            pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.right,CenterPoint),m_BorderColor2);
         }
      }
      break;
   case WINARROW_LEFT:
      {
         int CenterPoint = rect.top + ((rect.bottom - rect.top) / 2);

         if(m_bPressed)
         {
            // |
            pDisplay->DrawLine(CPoint(rect.right,rect.top),CPoint(rect.right,rect.bottom),m_BorderColor2);

            // /
            pDisplay->DrawLine(CPoint(rect.left,CenterPoint),CPoint(rect.right,rect.bottom),m_BorderColor2);

            // \ 
            pDisplay->DrawLine(CPoint(rect.left,CenterPoint),CPoint(rect.right,rect.top),m_BorderColor);
            pDisplay->DrawLine(CPoint(rect.left,CenterPoint+1),CPoint(rect.right,rect.top+1),m_BorderColor3);
         }
         else
         {
            // |
            pDisplay->DrawLine(CPoint(rect.right,rect.top),CPoint(rect.right,rect.bottom),m_BorderColor);
            pDisplay->DrawLine(CPoint(rect.right-1,rect.top),CPoint(rect.right-1,rect.bottom),m_BorderColor3);


            // /
            pDisplay->DrawLine(CPoint(rect.left+1,CenterPoint-1),CPoint(rect.right,rect.bottom-1),m_BorderColor3);
            pDisplay->DrawLine(CPoint(rect.left,CenterPoint),CPoint(rect.right,rect.bottom),m_BorderColor);

            // \ 
            pDisplay->DrawLine(CPoint(rect.left,CenterPoint),CPoint(rect.right,rect.top),m_BorderColor2);

         }
      }
      break;
   case WINARROW_UP:
      {

         int CenterPoint = rect.left + ((rect.right - rect.left) / 2);

         if(m_bPressed)
         {
            // _
            pDisplay->DrawLine(CPoint(rect.left, rect.bottom),CPoint(rect.right,rect.bottom),m_BorderColor2);


            // \ 
            pDisplay->DrawLine(CPoint(rect.right,rect.bottom),CPoint(CenterPoint,rect.top),m_BorderColor2);


            // /
            pDisplay->DrawLine(CPoint(rect.left,rect.bottom),CPoint(CenterPoint,rect.top),m_BorderColor);
         }
         else
         {
            // _
            pDisplay->DrawLine(CPoint(rect.left, rect.bottom),CPoint(rect.right,rect.bottom),m_BorderColor);


            // \ 
            pDisplay->DrawLine(CPoint(rect.right,rect.bottom),CPoint(CenterPoint,rect.top),m_BorderColor);


            // /
            pDisplay->DrawLine(CPoint(rect.left,rect.bottom),CPoint(CenterPoint,rect.top),m_BorderColor2);

         }
      }

      break;
   case WINARROW_DOWN:
      {
         int CenterPoint = rect.left + ((rect.right - rect.left) / 2);

         if(m_bPressed)
         {
            // _
            pDisplay->DrawLine(CPoint(rect.left, rect.top),CPoint(rect.right,rect.top),m_BorderColor);


            // \ 
            pDisplay->DrawLine(CPoint(rect.right,rect.top),CPoint(CenterPoint,rect.bottom),m_BorderColor2);


            // /
            pDisplay->DrawLine(CPoint(rect.left,rect.top),CPoint(CenterPoint,rect.bottom),m_BorderColor);
         }
         else
         {
            // _
            pDisplay->DrawLine(CPoint(rect.left, rect.top),CPoint(rect.right,rect.top),m_BorderColor2);


            // \ 
            pDisplay->DrawLine(CPoint(rect.right,rect.top),CPoint(CenterPoint,rect.bottom),m_BorderColor);


            // /
            pDisplay->DrawLine(CPoint(rect.left,rect.top),CPoint(CenterPoint,rect.bottom),m_BorderColor2);

         }
      }
      break;

   }
   CWinBaceObj::Display(pDisplay);
}
