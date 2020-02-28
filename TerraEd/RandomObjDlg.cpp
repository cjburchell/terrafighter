///////////////////////////////////////////////////////////////////////////////
///  RandomObjDlg.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:02:59 AM by Christiaan Burchell
///
///  PURPOSE:  The random object dialog
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

// RandomObjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "RandomObjDlg.h"
#include "SelObjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CRandomObjDlg dialog




/////////////////////////////////////////////////
///
/// NAME: CRandomObjDlg
///
/// CLASS: CRandomObjDlg
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 9:03:02 AM
///
/// PARAMETERS: 
///             std::vector<CPlanetObjectDataEx*>* pPlanetObjectData
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CRandomObjDlg::CRandomObjDlg(std::vector<CPlanetObjectDataEx*>* pPlanetObjectData,CWnd* pParent /*=NULL*/)
: CDialog(CRandomObjDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CRandomObjDlg)
   m_MaxHight = 20;
   m_MinHight = 0;
   m_NumbItems = 500;
   m_Rad = 40.0f;
   m_UsePoint = FALSE;
   m_x = 0.0f;
   m_y = 0.0f;
   //}}AFX_DATA_INIT
   m_pPlanetObjectData = pPlanetObjectData;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CRandomObjDlg
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     03/06/2004 9:03:05 AM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CRandomObjDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CRandomObjDlg)
   DDX_Control(pDX, IDC_OBJECTS, m_Objects);
   DDX_Text(pDX, IDC_MAX_HIGHT, m_MaxHight);
   DDX_Text(pDX, IDC_MIN_HIGHT, m_MinHight);
   DDX_Text(pDX, IDC_NUMB_ITEMS, m_NumbItems);
   DDX_Text(pDX, IDC_RAD, m_Rad);
   DDX_Check(pDX, IDC_USE_POINT, m_UsePoint);
   DDX_Text(pDX, IDC_X, m_x);
   DDX_Text(pDX, IDC_Y, m_y);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRandomObjDlg, CDialog)
   //{{AFX_MSG_MAP(CRandomObjDlg)
   ON_BN_CLICKED(IDC_USE_POINT, OnUsePoint)
   ON_BN_CLICKED(IDC_ADD, OnAdd)
   ON_BN_CLICKED(IDC_REMOVE, OnRemove)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRandomObjDlg message handlers



/////////////////////////////////////////////////
///
/// NAME: OnOK
///
/// CLASS: CRandomObjDlg
///
/// DESCRIPTION: User Pressed the OK Button
///
/// CREATED:     03/06/2004 9:03:07 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CRandomObjDlg::OnOK() 
{
   // TODO: Add extra validation here
   for(int i=0;i<m_Objects.GetCount();i++)
   {
      m_ObjList.push_back(m_Objects.GetItemData(i));
   }

   CDialog::OnOK();
}



/////////////////////////////////////////////////
///
/// NAME: OnUsePoint
///
/// CLASS: CRandomObjDlg
///
/// DESCRIPTION: User selected the Use point check box
///
/// CREATED:     03/06/2004 9:03:35 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CRandomObjDlg::OnUsePoint() 
{
   UpdateData(TRUE);
   GetDlgItem(IDC_X)->EnableWindow(m_UsePoint);
   GetDlgItem(IDC_Y)->EnableWindow(m_UsePoint);
   GetDlgItem(IDC_RAD)->EnableWindow(m_UsePoint);
}



/////////////////////////////////////////////////
///
/// NAME: OnAdd
///
/// CLASS: CRandomObjDlg
///
/// DESCRIPTION: Adds a object
///
/// CREATED:     03/06/2004 9:03:59 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CRandomObjDlg::OnAdd() 
{
   CSelObjDlg dlg(m_pPlanetObjectData);
   if(dlg.DoModal() == IDOK)
   {
      int index = m_Objects.AddString((*m_pPlanetObjectData)[dlg.m_Index]->m_Name);
      m_Objects.SetItemData(index,dlg.m_Index);
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnRemove
///
/// CLASS: CRandomObjDlg
///
/// DESCRIPTION: Removes an object
///
/// CREATED:     03/06/2004 9:04:07 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CRandomObjDlg::OnRemove() 
{
   int index = m_Objects.GetCurSel();
   m_Objects.DeleteString(index);
}
