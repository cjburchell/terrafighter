///////////////////////////////////////////////////////////////////////////////
///  EditLevel.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:23:05 PM by Christiaan Burchell
///
///  PURPOSE:  Edit Level Dialog
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

// EditLevel.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "EditLevel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditLevel dialog




/////////////////////////////////////////////////
///
/// NAME: CEditLevel
///
/// CLASS: CEditLevel
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:23:09 PM
///
/// PARAMETERS: 
///             CLevelData* pLevel
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CEditLevel::CEditLevel(CLevelData* pLevel,CWnd* pParent /*=NULL*/)
: CDialog(CEditLevel::IDD, pParent)
{
   //{{AFX_DATA_INIT(CEditLevel)
   // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT

   m_pLevel = pLevel;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CEditLevel
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:23:11 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditLevel::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CEditLevel)
   DDX_Control(pDX, IDC_COLOUR, m_colourbutton);
   //}}AFX_DATA_MAP


   DDX_Text(pDX, IDC_NAME, m_pLevel->m_Name);
   DDX_Text(pDX, IDC_DISCRIPTION, m_pLevel->m_Discription);
   DDX_Text(pDX, IDC_HIGHTMAP, m_pLevel->m_MapFile);
   DDX_Text(pDX, IDC_T_SKY, m_pLevel->m_SkyBoxFile);
   DDX_Text(pDX, IDC_T_GROUND, m_pLevel->m_GroundTexture);
   DDX_Text(pDX, IDC_T_WATER1, m_pLevel->m_WaterTexture);
   DDX_Text(pDX, IDC_T_WATER2, m_pLevel->m_WaterTexture2);

   DDX_Check(pDX, IDC_SHOW_SKY_BOX, m_pLevel->m_bShowSkyBox);
   DDX_Check(pDX, IDC_SHOW_WATER, m_pLevel->m_bWater);

   DDX_Text(pDX, IDC_FOG_START, m_pLevel->m_fFogStart);
   DDX_Text(pDX, IDC_FOG_END, m_pLevel->m_fFogEnd);

   DDX_Text(pDX, IDC_WATER_LEVEL, m_pLevel->m_WaterLevel);
   DDX_Text(pDX, IDC_MAP_HIGHT, m_pLevel->m_MapHight);

   DDX_Text(pDX, IDC_POS_X, m_pLevel->m_StartPos.x);
   DDX_Text(pDX, IDC_POS_Y, m_pLevel->m_StartPos.y);
   DDX_Text(pDX, IDC_POS_Z, m_pLevel->m_StartPos.z);

   DDX_Text(pDX, IDC_SUN_POS_X, m_pLevel->m_SunPos.x);
   DDX_Text(pDX, IDC_SUN_POS_Y, m_pLevel->m_SunPos.y);
   DDX_Text(pDX, IDC_SUN_POS_Z, m_pLevel->m_SunPos.z);

   DDX_Text(pDX, IDC_MAP_SCALE, m_pLevel->m_MapScale);

   if(pDX->m_bSaveAndValidate)
   {
      m_pLevel->m_BGColour = D3DCOLOR_ARGB(0xff,GetRValue(m_colourbutton.GetColour()),GetGValue(m_colourbutton.GetColour()),GetBValue(m_colourbutton.GetColour()));
   }
   else
   {
      m_colourbutton.SetColour(RGB(GetBValue(m_pLevel->m_BGColour),GetGValue(m_pLevel->m_BGColour),GetRValue(m_pLevel->m_BGColour)));
      m_colourbutton.Invalidate();
      CString str;
      str.Format("0x%08x",m_pLevel->m_BGColour);
      SetDlgItemText(IDC_BG_COLOUR,str);
   }
}


BEGIN_MESSAGE_MAP(CEditLevel, CDialog)
   //{{AFX_MSG_MAP(CEditLevel)
   ON_BN_CLICKED(IDC_COLOUR, OnColour)
   ON_BN_CLICKED(IDC_HM_FILE, OnHmFile)
   ON_BN_CLICKED(IDC_SKY_FILE, OnSkyFile)
   ON_BN_CLICKED(IDC_GROUND_FILE, OnGroundFile)
   ON_BN_CLICKED(IDC_WATER1_FILE, OnWater1File)
   ON_BN_CLICKED(IDC_WATER2_FILE, OnWater2File)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLevel message handlers



/////////////////////////////////////////////////
///
/// NAME: OnColour
///
/// CLASS: CEditLevel
///
/// DESCRIPTION: Change color
///
/// CREATED:     02/06/2004 5:23:27 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditLevel::OnColour() 
{
   // TODO: Add your control notification handler code here
   CColorDialog dlg(m_colourbutton.GetColour());
   if(dlg.DoModal() == IDOK)
   {
      m_colourbutton.SetColour(dlg.GetColor());
      m_colourbutton.Invalidate();
      CString str;
      str.Format("0x%08x",D3DCOLOR_ARGB(0xff,GetRValue(dlg.GetColor()),GetGValue(dlg.GetColor()),GetBValue(dlg.GetColor())));
      this->SetDlgItemText(IDC_BG_COLOUR,str);
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnHmFile
///
/// CLASS: CEditLevel
///
/// DESCRIPTION: File button pressed
///
/// CREATED:     02/06/2004 5:23:40 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditLevel::OnHmFile() 
{
   static TCHAR BASED_CODE szFilter[] = _T("Bitmap (*.bmp)|*.bmp|All Files (*.*)|*.*||");
   CString FileName;
   GetDlgItemText(IDC_HIGHTMAP,FileName);

   CFileDialog dlg(TRUE,_T(".bmp"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      SetDlgItemText(IDC_HIGHTMAP,dlg.GetFileName());
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnSkyFile
///
/// CLASS: CEditLevel
///
/// DESCRIPTION: File button pressed
///
/// CREATED:     02/06/2004 5:23:44 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditLevel::OnSkyFile() 
{
   static TCHAR BASED_CODE szFilter[] = IMAGE_FILE_FILTER;
   CString FileName;
   GetDlgItemText(IDC_T_SKY,FileName);

   CFileDialog dlg(TRUE,_T(".jpg"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      SetDlgItemText(IDC_T_SKY,dlg.GetFileName());
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnGroundFile
///
/// CLASS: CEditLevel
///
/// DESCRIPTION: File button pressed
///
/// CREATED:     02/06/2004 5:23:48 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditLevel::OnGroundFile() 
{
   static TCHAR BASED_CODE szFilter[] = IMAGE_FILE_FILTER;
   CString FileName;
   GetDlgItemText(IDC_T_GROUND,FileName);

   CFileDialog dlg(TRUE,_T(".jpg"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      SetDlgItemText(IDC_T_GROUND,dlg.GetFileName());
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnWater1File
///
/// CLASS: CEditLevel
///
/// DESCRIPTION: File button pressed
///
/// CREATED:     02/06/2004 5:23:52 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditLevel::OnWater1File() 
{
   static TCHAR BASED_CODE szFilter[] = IMAGE_FILE_FILTER;
   CString FileName;
   GetDlgItemText(IDC_T_WATER1,FileName);

   CFileDialog dlg(TRUE,_T(".jpg"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      SetDlgItemText(IDC_T_WATER1,dlg.GetFileName());
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnWater2File
///
/// CLASS: CEditLevel
///
/// DESCRIPTION: File button pressed
///
/// CREATED:     02/06/2004 5:23:56 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditLevel::OnWater2File() 
{
   static TCHAR BASED_CODE szFilter[] = IMAGE_FILE_FILTER;
   CString FileName;
   GetDlgItemText(IDC_T_WATER2,FileName);

   CFileDialog dlg(TRUE,_T(".jpg"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      SetDlgItemText(IDC_T_WATER2,dlg.GetFileName());
   }

}
