///////////////////////////////////////////////////////////////////////////////
///  ColourButton.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:12:33 AM by Christiaan Burchell
///
///  PURPOSE:  the colour button control
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

#if !defined(AFX_COLOURBUTTON_H__F15584C8_5292_4030_89FB_1C1B3C46282F__INCLUDED_)
#define AFX_COLOURBUTTON_H__F15584C8_5292_4030_89FB_1C1B3C46282F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColourButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColourButton window

class CColourButton : public CButton
{
   // Construction
public:
   CColourButton();

   // Attributes
public:

   // Operations
public:
   void SetColour(COLORREF c){m_colour = c;}
   COLORREF GetColour(){return m_colour;};

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CColourButton)
public:
   virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
   //}}AFX_VIRTUAL

   // Implementation
public:
   virtual ~CColourButton();

   // Generated message map functions
protected:
   COLORREF m_colour;
   //{{AFX_MSG(CColourButton)
   // NOTE - the ClassWizard will add and remove member functions here.
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOURBUTTON_H__F15584C8_5292_4030_89FB_1C1B3C46282F__INCLUDED_)
