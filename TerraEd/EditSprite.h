///////////////////////////////////////////////////////////////////////////////
///  EditSprite.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:13:43 AM by Christiaan Burchell
///
///  PURPOSE:  edit sprite dialog
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

#if !defined(AFX_EDITSPRITE_H__F5246055_68C0_4329_B31F_0C05079A92CC__INCLUDED_)
#define AFX_EDITSPRITE_H__F5246055_68C0_4329_B31F_0C05079A92CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditSprite.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditSprite dialog

#include "LevelData.h"
#include "SpriteDataEx.h"



class CEditSprite : public CDialog
{
   // Construction
public:
   CEditSprite(CLevelDataSprite* pSprite,std::vector<CSpriteDataEx*>* pSpriteData,CLevelData* pLevel,CWnd* pParent = NULL);   // standard constructor

   // Dialog Data
   //{{AFX_DATA(CEditSprite)
   enum { IDD = IDD_EDIT_SPRITE };
   CComboBox	m_TypeCtrl;
   CComboBox m_GoalCtrl;
   CComboBox m_TextureCtrl;
   //}}AFX_DATA


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CEditSprite)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

   // Implementation
protected:
   CLevelDataSprite* m_pSprite;
   std::vector<CSpriteDataEx*>* m_pSpriteData;
   CLevelData* m_pLevel;

   // Generated message map functions
   //{{AFX_MSG(CEditSprite)
   virtual BOOL OnInitDialog();
   afx_msg void OnSelchangeType();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSPRITE_H__F5246055_68C0_4329_B31F_0C05079A92CC__INCLUDED_)
