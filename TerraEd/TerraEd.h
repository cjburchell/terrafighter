///////////////////////////////////////////////////////////////////////////////
///  TerraEd.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:15:25 AM by Christiaan Burchell
///
///  PURPOSE:  Application object
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

// TerraEd.h : main header file for the TERRAED application
//

#if !defined(AFX_TERRAED_H__E6FDA05F_121A_4687_BAFD_810EB3B1AC24__INCLUDED_)
#define AFX_TERRAED_H__E6FDA05F_121A_4687_BAFD_810EB3B1AC24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define IMAGE_FILE_FILTER  _T("Image Files (*.bmp;*.jpg;*.dds;*.png;*.tga)|*.bmp;*.jpg;*.dds;*.png;*.tga|All Files (*.*)|*.*||")

/////////////////////////////////////////////////////////////////////////////
// CTerraEdApp:
// See TerraEd.cpp for the implementation of this class
//

class CTerraEdApp : public CWinApp
{
public:
   CTerraEdApp();

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTerraEdApp)
public:
   virtual BOOL InitInstance();
   //}}AFX_VIRTUAL

   // Implementation
   //{{AFX_MSG(CTerraEdApp)
   afx_msg void OnAppAbout();
   // NOTE - the ClassWizard will add and remove member functions here.
   //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAED_H__E6FDA05F_121A_4687_BAFD_810EB3B1AC24__INCLUDED_)
