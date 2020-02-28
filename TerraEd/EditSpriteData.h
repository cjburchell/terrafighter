///////////////////////////////////////////////////////////////////////////////
///  EditSpriteData.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:13:52 AM by Christiaan Burchell
///
///  PURPOSE:  edit sprite data dialog
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

#if !defined(AFX_EDITSPRITEDATA_H__A4F251D9_1ADB_49E3_8113_7300090B6B52__INCLUDED_)
#define AFX_EDITSPRITEDATA_H__A4F251D9_1ADB_49E3_8113_7300090B6B52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditSpriteData.h : header file
//
#include "SpriteData.h"

/////////////////////////////////////////////////////////////////////////////
// CEditSpriteData dialog

class CEditSpriteData : public CDialog
{
   // Construction
public:
   CEditSpriteData(CSpriteData* pObj,std::vector<CWeaponData*>* pWeaponData,CWnd* pParent = NULL);   // standard constructor

   // Dialog Data
   //{{AFX_DATA(CEditSpriteData)
   enum { IDD = IDD_EDIT_SPRITE_DATA };
   CComboBox	m_WeaponCtrl;
   CListBox m_TexturesCtrl;
   //}}AFX_DATA

   CSpriteData* m_pObj;
   std::vector<CWeaponData*>* m_pWeaponData;


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CEditSpriteData)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

   // Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CEditSpriteData)
   virtual BOOL OnInitDialog();
   afx_msg void OnFileBut();
   afx_msg void OnRemoveTexture();
   afx_msg void OnAddTexture();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSPRITEDATA_H__A4F251D9_1ADB_49E3_8113_7300090B6B52__INCLUDED_)
