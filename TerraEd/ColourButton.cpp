///////////////////////////////////////////////////////////////////////////////
///  ColourButton.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:21:26 PM by Christiaan Burchell
///
///  PURPOSE:  the color button
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

// ColourButton.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "ColourButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CColourButton



/////////////////////////////////////////////////
///
/// NAME: CColourButton
///
/// CLASS: CColourButton
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:21:09 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CColourButton::CColourButton()
{
   m_colour = RGB(0,0,0);
}



/////////////////////////////////////////////////
///
/// NAME: ~CColourButton
///
/// CLASS: CColourButton
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 5:21:11 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CColourButton::~CColourButton()
{
}


BEGIN_MESSAGE_MAP(CColourButton, CButton)
   //{{AFX_MSG_MAP(CColourButton)
   // NOTE - the ClassWizard will add and remove mapping macros here.
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColourButton message handlers



/////////////////////////////////////////////////
///
/// NAME: DrawItem
///
/// CLASS: CColourButton
///
/// DESCRIPTION: Draws the control
///
/// CREATED:     02/06/2004 5:21:18 PM
///
/// PARAMETERS: 
///             LPDRAWITEMSTRUCT lpDrawItemStruct
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CColourButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
   // TODO: Add your code to draw the specified item
   CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
   CBrush brush(m_colour);
   pDC->FillRect(&lpDrawItemStruct->rcItem,&brush);
}
