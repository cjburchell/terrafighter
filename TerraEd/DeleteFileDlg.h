///////////////////////////////////////////////////////////////////////////////
///  DeleteFileDlg.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:12:57 AM by Christiaan Burchell
///
///  PURPOSE:  the delete file dialog
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

#if !defined(AFX_DELETEFILEDLG_H__C2256007_20F4_423C_92F9_C4B458B5C7E9__INCLUDED_)
#define AFX_DELETEFILEDLG_H__C2256007_20F4_423C_92F9_C4B458B5C7E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeleteFileDlg.h : header file
//
#include "ZipArchive.h"
/////////////////////////////////////////////////////////////////////////////
// CDeleteFileDlg dialog

class CDeleteFileDlg : public CDialog
{
   // Construction
public:
   CDeleteFileDlg(CZipArchive* pZip,CWnd* pParent = NULL);   // standard constructor

   // Dialog Data
   //{{AFX_DATA(CDeleteFileDlg)
   enum { IDD = IDD_DELETE_FILES };
   CListCtrl	m_list;
   //}}AFX_DATA

   CZipArchive* m_pZip;

   CWordArray m_DelList;


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CDeleteFileDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

   // Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CDeleteFileDlg)
   virtual void OnOK();
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELETEFILEDLG_H__C2256007_20F4_423C_92F9_C4B458B5C7E9__INCLUDED_)
