///////////////////////////////////////////////////////////////////////////////
///  edittextrue.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:14:00 AM by Christiaan Burchell
///
///  PURPOSE:  edit texture dialog
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

#if !defined(AFX_EDITTEXTRUE_H__564E8E3E_A140_4C1D_ADD1_52E9EB326F52__INCLUDED_)
#define AFX_EDITTEXTRUE_H__564E8E3E_A140_4C1D_ADD1_52E9EB326F52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditTextrue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditTexture dialog

#include "LevelData.h"

class CEditTexture : public CDialog
{
   // Construction
public:
   CEditTexture(CView* pView,CTextureData* pTexture,CWnd* pParent = NULL);   // standard constructor

   // Dialog Data
   //{{AFX_DATA(CEditTexture)
   enum { IDD = IDD_EDIT_TEXTURE };
   //}}AFX_DATA


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CEditTexture)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

   // Implementation
protected:
   CTextureData* m_pTexture;
   CView* m_pView;


   // Generated message map functions
   //{{AFX_MSG(CEditTexture)
   afx_msg void OnTcFileBut();
   afx_msg void OnAlFileBut();
   afx_msg void OnGenerateLight();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITTEXTRUE_H__564E8E3E_A140_4C1D_ADD1_52E9EB326F52__INCLUDED_)
