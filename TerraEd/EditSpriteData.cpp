///////////////////////////////////////////////////////////////////////////////
///  EditSpriteData.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:26:04 PM by Christiaan Burchell
///
///  PURPOSE:  Edit sprite data dialog
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

// EditSpriteData.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "EditSpriteData.h"
#include "AddTexture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSpriteData dialog




/////////////////////////////////////////////////
///
/// NAME: CEditSpriteData
///
/// CLASS: CEditSpriteData
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:26:06 PM
///
/// PARAMETERS: 
///             CSpriteData* pObj
///             std::vector<CWeaponData*>* pWeaponData
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CEditSpriteData::CEditSpriteData(CSpriteData* pObj,std::vector<CWeaponData*>* pWeaponData,CWnd* pParent /*=NULL*/)
: CDialog(CEditSpriteData::IDD, pParent)
{
   //{{AFX_DATA_INIT(CEditSpriteData)
   // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   m_pObj = pObj;
   m_pWeaponData = pWeaponData;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CEditSpriteData
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:26:08 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditSpriteData::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CEditSpriteData)
   DDX_Control(pDX, IDC_WEAPON_TYPE, m_WeaponCtrl);
   DDX_Control(pDX, IDC_TEXTURES, m_TexturesCtrl);
   //}}AFX_DATA_MAP

   DDX_CBIndex(pDX, IDC_WEAPON_TYPE, m_pObj->m_DefaultWeapon);
   DDX_Text(pDX, IDC_FILE, m_pObj->m_FileName);
   DDX_Text(pDX, IDC_NAME, m_pObj->m_Name);
   DDX_Text(pDX, IDC_SCALE, m_pObj->m_fScale);
   DDX_Text(pDX, IDC_MIN_SPEED, m_pObj->m_fMinSpeed);
   DDX_Text(pDX, IDC_MAX_SPEED, m_pObj->m_fMaxSpeed);
   DDX_Text(pDX, IDC_ACCEL, m_pObj->m_fAcceleration);
   DDX_Text(pDX, IDC_DRAG, m_pObj->m_Drag);
   DDX_Text(pDX, IDC_TURN, m_pObj->m_fTurningRate);
   DDX_Text(pDX, IDC_SHIELDS, m_pObj->m_fMaxShields);
   DDX_Text(pDX, IDC_HEALTH, m_pObj->m_fMaxDamage);
   DDX_Text(pDX, IDC_REGEN, m_pObj->m_fRegenRate);
   DDX_Text(pDX, IDC_MIN_HIGHT, m_pObj->m_fMinHight);
   DDX_Text(pDX, IDC_MAX_HIGHT, m_pObj->m_fMaxHight);
   DDX_Text(pDX, IDC_ROT_Z, m_pObj->m_zMeshRot);
   DDX_Text(pDX, IDC_ROT_Y, m_pObj->m_yMeshRot);

   DDX_Check(pDX, IDC_REST_ROT, m_pObj->m_bRestrictRot);
   DDX_Check(pDX, IDC_BUILDING, m_pObj->m_IsBuilding);
   DDX_Check(pDX, IDC_USER, m_pObj->m_bCanBeUser);
}


BEGIN_MESSAGE_MAP(CEditSpriteData, CDialog)
   //{{AFX_MSG_MAP(CEditSpriteData)
   ON_BN_CLICKED(IDC_FILE_BUT, OnFileBut)
   ON_BN_CLICKED(IDC_REMOVE_TEXTURE, OnRemoveTexture)
   ON_BN_CLICKED(IDC_ADD_TEXTURE, OnAddTexture)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSpriteData message handlers



/////////////////////////////////////////////////
///
/// NAME: OnInitDialog
///
/// CLASS: CEditSpriteData
///
/// DESCRIPTION: Initializes the dialog
///
/// CREATED:     02/06/2004 5:26:11 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CEditSpriteData::OnInitDialog() 
{
   CDialog::OnInitDialog();

   for(int i=0;i<m_pWeaponData->size();i++)
   {
      m_WeaponCtrl.AddString((*m_pWeaponData)[i]->m_Name);
   }

   m_WeaponCtrl.SetCurSel(m_pObj->m_DefaultWeapon);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}



/////////////////////////////////////////////////
///
/// NAME: OnFileBut
///
/// CLASS: CEditSpriteData
///
/// DESCRIPTION: File button pressed
///
/// CREATED:     02/06/2004 5:26:35 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditSpriteData::OnFileBut() 
{
   CString FileName;
   GetDlgItemText(IDC_FILE,FileName);
   static TCHAR BASED_CODE szFilter[] = _T("Mesh Files (*.x)|*.x|All Files (*.*)|*.*||");
   CFileDialog dlg(TRUE,_T(".x"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      SetDlgItemText(IDC_FILE,dlg.GetFileName());
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnRemoveTexture
///
/// CLASS: CEditSpriteData
///
/// DESCRIPTION: Remove texture button
///
/// CREATED:     02/06/2004 5:26:31 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditSpriteData::OnRemoveTexture() 
{
   int index = m_TexturesCtrl.GetCurSel();
   m_TexturesCtrl.DeleteString(index);

}



/////////////////////////////////////////////////
///
/// NAME: OnAddTexture
///
/// CLASS: CEditSpriteData
///
/// DESCRIPTION: Add Texture Button
///
/// CREATED:     02/06/2004 5:26:24 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditSpriteData::OnAddTexture() 
{
   CAddTexture dlg;
   if(dlg.DoModal() == IDOK)
   {
      int index = m_TexturesCtrl.AddString(dlg.m_File);
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnOK
///
/// CLASS: CEditSpriteData
///
/// DESCRIPTION: User Pressed the OK Button
///
/// CREATED:     02/06/2004 5:26:16 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditSpriteData::OnOK() 
{
   m_pObj->m_Textures.clear();
   for(int i=0;i<m_TexturesCtrl.GetCount();i++)
   {
      CString str;
      m_TexturesCtrl.GetText(i,str);
      m_pObj->m_Textures.push_back(str);
   }
   CDialog::OnOK();
}
