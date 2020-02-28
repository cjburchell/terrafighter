///////////////////////////////////////////////////////////////////////////////
///  EditObjectData.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:24:31 PM by Christiaan Burchell
///
///  PURPOSE:  Edit Object Data Dialog
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

// EditObjectData.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "EditObjectData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditObjectData dialog




/////////////////////////////////////////////////
///
/// NAME: CEditObjectData
///
/// CLASS: CEditObjectData
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:24:34 PM
///
/// PARAMETERS: 
///             CPlanetObjectData* pObj
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CEditObjectData::CEditObjectData(CPlanetObjectData* pObj, CWnd* pParent /*=NULL*/)
: CDialog(CEditObjectData::IDD, pParent)
{
   //{{AFX_DATA_INIT(CEditObjectData)
   //}}AFX_DATA_INIT
   m_pObj = pObj;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CEditObjectData
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:24:36 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditObjectData::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CEditObjectData)
   DDX_Control(pDX, IDC_PU_TYPE, m_pu_type);
   DDX_Control(pDX, IDC_TYPE, m_Type_Ctrl);
   //}}AFX_DATA_MAP

   DDX_Text(pDX, IDC_NAME, m_pObj->m_Name);
   DDX_Text(pDX, IDC_FILE, m_pObj->m_File);
   DDX_Check(pDX, IDC_SHADOW, m_pObj->m_bShadow);

   int Type = m_pObj->m_Type;
   DDX_CBIndex(pDX, IDC_TYPE, Type);
   m_pObj->m_Type = (PlanetObjectType_e)Type;

   DDX_Text(pDX, IDC_SCALE, m_pObj->m_fScale);
   DDX_Text(pDX, IDC_X_SIZE, m_pObj->m_xSize);
   DDX_Text(pDX, IDC_Y_SIZE, m_pObj->m_ySize);

   Type = m_pObj->m_PowerUp.m_Type;
   DDX_CBIndex(pDX, IDC_PU_TYPE, Type);
   m_pObj->m_PowerUp.m_Type = (PowerUpType_e)Type;


   DDX_Text(pDX, IDC_STATS_WP, m_pObj->m_PowerUp.m_WeaponPower);
   DDX_Text(pDX, IDC_STATS_H, m_pObj->m_PowerUp.m_Health);
   DDX_Text(pDX, IDC_STATS_S, m_pObj->m_PowerUp.m_Shields);
   DDX_Text(pDX, IDC_STATS_MS, m_pObj->m_PowerUp.m_MaxSpeed);
   DDX_Text(pDX, IDC_PU_DURATION, m_pObj->m_PowerUp.m_Duration);

}


BEGIN_MESSAGE_MAP(CEditObjectData, CDialog)
   //{{AFX_MSG_MAP(CEditObjectData)
   ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
   ON_CBN_SELCHANGE(IDC_PU_TYPE, OnSelchangePuType)
   ON_BN_CLICKED(IDC_FILE_BUT, OnFileBut)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditObjectData message handlers



/////////////////////////////////////////////////
///
/// NAME: OnSelchangeType
///
/// CLASS: CEditObjectData
///
/// DESCRIPTION: User Changed the type
///
/// CREATED:     02/06/2004 5:24:46 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditObjectData::OnSelchangeType() 
{
   switch(m_Type_Ctrl.GetCurSel())
   {
   case OBJECTTYPE_TREE:
   case OBJECTTYPE_BUSH:
      GetDlgItem(IDC_X_SIZE)->EnableWindow();
      GetDlgItem(IDC_Y_SIZE)->EnableWindow();
      GetDlgItem(IDC_STATS_WP)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_H)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_S)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_MS)->EnableWindow(FALSE);
      GetDlgItem(IDC_PU_TYPE)->EnableWindow(FALSE);
      GetDlgItem(IDC_PU_DURATION)->EnableWindow(FALSE);
      break;
   case OBJECTTYPE_MESH:
   case OBJECTTYPE_WAYPOINT:
      GetDlgItem(IDC_X_SIZE)->EnableWindow(FALSE);
      GetDlgItem(IDC_Y_SIZE)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_WP)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_H)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_S)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_MS)->EnableWindow(FALSE);

      GetDlgItem(IDC_PU_TYPE)->EnableWindow(FALSE);
      GetDlgItem(IDC_PU_DURATION)->EnableWindow(FALSE);
      break;
   case OBJECTTYPE_POWERUP:
      GetDlgItem(IDC_X_SIZE)->EnableWindow(FALSE);
      GetDlgItem(IDC_Y_SIZE)->EnableWindow(FALSE);
      GetDlgItem(IDC_PU_TYPE)->EnableWindow();
      GetDlgItem(IDC_PU_DURATION)->EnableWindow();
      OnSelchangePuType();
      break;
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnInitDialog
///
/// CLASS: CEditObjectData
///
/// DESCRIPTION: Initializes the dialog
///
/// CREATED:     02/06/2004 5:24:49 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CEditObjectData::OnInitDialog() 
{
   CDialog::OnInitDialog();

   // TODO: Add extra initialization here

   OnSelchangeType();

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}



/////////////////////////////////////////////////
///
/// NAME: OnSelchangePuType
///
/// CLASS: CEditObjectData
///
/// DESCRIPTION: User changed the power up type
///
/// CREATED:     02/06/2004 5:25:01 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditObjectData::OnSelchangePuType() 
{
   switch(m_pu_type.GetCurSel())
   {
   case PU_INVULNERABILITY:
   case PU_INVISIBILITY:
      GetDlgItem(IDC_STATS_WP)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_H)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_S)->EnableWindow(FALSE);
      GetDlgItem(IDC_STATS_MS)->EnableWindow(FALSE);
      break;
   case PU_STATS:
      GetDlgItem(IDC_STATS_WP)->EnableWindow();
      GetDlgItem(IDC_STATS_H)->EnableWindow();
      GetDlgItem(IDC_STATS_S)->EnableWindow();
      GetDlgItem(IDC_STATS_MS)->EnableWindow();
      break;
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnFileBut
///
/// CLASS: CEditObjectData
///
/// DESCRIPTION: File button pressed
///
/// CREATED:     02/06/2004 5:25:09 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditObjectData::OnFileBut() 
{

   CString FileName;
   GetDlgItemText(IDC_FILE,FileName);

   switch(m_Type_Ctrl.GetCurSel())
   {
   case OBJECTTYPE_TREE:
   case OBJECTTYPE_BUSH:
      {
         static TCHAR BASED_CODE szFilter[] = IMAGE_FILE_FILTER;
         CFileDialog dlg(TRUE,_T(".jpg"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
         if(dlg.DoModal() == IDOK)
         {
            SetDlgItemText(IDC_FILE,dlg.GetFileName());
         }
      }
      break;
   case OBJECTTYPE_MESH:
   case OBJECTTYPE_POWERUP:
   case OBJECTTYPE_WAYPOINT:
      {
         static TCHAR BASED_CODE szFilter[] = _T("Mesh Files (*.x)|*.x|All Files (*.*)|*.*||");
         CFileDialog dlg(TRUE,_T(".x"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
         if(dlg.DoModal() == IDOK)
         {
            SetDlgItemText(IDC_FILE,dlg.GetFileName());
         }
      }
      break;
   }

}
