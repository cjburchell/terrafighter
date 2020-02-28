///////////////////////////////////////////////////////////////////////////////
///  SelObjDlg.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:04:24 AM by Christiaan Burchell
///
///  PURPOSE:  Select object dialog
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

// SelObjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "SelObjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelObjDlg dialog




/////////////////////////////////////////////////
///
/// NAME: CSelObjDlg
///
/// CLASS: CSelObjDlg
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 9:04:27 AM
///
/// PARAMETERS: 
///             std::vector<CPlanetObjectDataEx*>* pPlanetObjectData
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CSelObjDlg::CSelObjDlg(std::vector<CPlanetObjectDataEx*>* pPlanetObjectData,CWnd* pParent /*=NULL*/)
: CDialog(CSelObjDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CSelObjDlg)
   m_Index = 0;
   //}}AFX_DATA_INIT
   m_pPlanetObjectData = pPlanetObjectData; 
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CSelObjDlg
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     03/06/2004 9:04:29 AM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CSelObjDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CSelObjDlg)
   DDX_Control(pDX, IDC_OBJECTS, m_ObjectCtrl);
   DDX_CBIndex(pDX, IDC_OBJECTS, m_Index);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelObjDlg, CDialog)
   //{{AFX_MSG_MAP(CSelObjDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelObjDlg message handlers



/////////////////////////////////////////////////
///
/// NAME: OnInitDialog
///
/// CLASS: CSelObjDlg
///
/// DESCRIPTION: Initializes the dialog
///
/// CREATED:     03/06/2004 9:04:31 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CSelObjDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();

   for(UINT i=0;i<m_pPlanetObjectData->size();i++)
   {
      m_ObjectCtrl.AddString((*m_pPlanetObjectData)[i]->m_Name);
   }
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
