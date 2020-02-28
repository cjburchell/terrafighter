///////////////////////////////////////////////////////////////////////////////
///  RandomObjDlg.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:14:42 AM by Christiaan Burchell
///
///  PURPOSE:  Random object dialog
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

#if !defined(AFX_RANDOMOBJDLG_H__B8686831_9352_4079_AB69_B47E6D6AE428__INCLUDED_)
#define AFX_RANDOMOBJDLG_H__B8686831_9352_4079_AB69_B47E6D6AE428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RandomObjDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRandomObjDlg dialog
#include "PlanetObjectDataEx.h"

class CRandomObjDlg : public CDialog
{
   // Construction
public:
   CRandomObjDlg(std::vector<CPlanetObjectDataEx*>* pPlanetObjectData,CWnd* pParent = NULL);   // standard constructor

   // Dialog Data
   //{{AFX_DATA(CRandomObjDlg)
   enum { IDD = IDD_RANDOM_OBJ };
   CListBox	m_Objects;
   DWORD	m_MaxHight;
   DWORD	m_MinHight;
   DWORD	m_NumbItems;
   float	m_Rad;
   BOOL	m_UsePoint;
   FLOAT	m_x;
   FLOAT	m_y;
   //}}AFX_DATA

   std::vector<int> m_ObjList;


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CRandomObjDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

   // Implementation
protected:

   std::vector<CPlanetObjectDataEx*>* m_pPlanetObjectData;

   // Generated message map functions
   //{{AFX_MSG(CRandomObjDlg)
   virtual void OnOK();
   afx_msg void OnUsePoint();
   afx_msg void OnAdd();
   afx_msg void OnRemove();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANDOMOBJDLG_H__B8686831_9352_4079_AB69_B47E6D6AE428__INCLUDED_)
