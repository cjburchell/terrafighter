///////////////////////////////////////////////////////////////////////////////
///  TerraEdDoc.h
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:15:45 AM by Christiaan Burchell
///
///  PURPOSE:  Document Object
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

// TerraEdDoc.h : interface of the CTerraEdDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAEDDOC_H__8630AC75_4CC6_4461_BD4C_AB8F37D32566__INCLUDED_)
#define AFX_TERRAEDDOC_H__8630AC75_4CC6_4461_BD4C_AB8F37D32566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "LevelData.h"
#include "SpriteDataEx.h"
#include "PlanetObjectDataEx.h"


class CTerraEdDoc : public CDocument
{
protected: // create from serialization only
   CTerraEdDoc();
   DECLARE_DYNCREATE(CTerraEdDoc)

   // Attributes
public:

   CZipArchive m_Zip;

   std::vector<CLevelData*> m_Levels;
   std::vector<CPlanetObjectDataEx*> m_PlanetObjectData;
   std::vector<CSpriteDataEx*> m_SpriteData;
   std::vector<CWeaponData*> m_WeaponData;

   // Operations
public:

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTerraEdDoc)
public:
   virtual BOOL OnNewDocument();
   virtual void Serialize(CArchive& ar);
   virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
   virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
   //}}AFX_VIRTUAL

   // Implementation
public:
   BOOL SaveLevelData();
   BOOL LoadLevelData();
   virtual ~CTerraEdDoc();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   void ClearLevelData();
   void ClearPlanetObjectData();
   void ClearSpriteData();
   BOOL LoadPlanetObjectData();
   BOOL LoadSpriteData();
   BOOL LoadWeaponData();

   BOOL SavePlanetObjectData();
   BOOL SaveSpriteData();
   BOOL SaveWeaponData();

   // Generated message map functions
protected:
   //{{AFX_MSG(CTerraEdDoc)
   afx_msg void OnToolsAddfile();
   afx_msg void OnToolsRemovefile();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAEDDOC_H__8630AC75_4CC6_4461_BD4C_AB8F37D32566__INCLUDED_)
