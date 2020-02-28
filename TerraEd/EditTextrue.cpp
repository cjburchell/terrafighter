///////////////////////////////////////////////////////////////////////////////
///  EditTextrue.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:26:48 PM by Christiaan Burchell
///
///  PURPOSE:  Edit Texture Dialog
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

// EditTextrue.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "EditTextrue.h"
#include "TerraEdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditTexture dialog




/////////////////////////////////////////////////
///
/// NAME: CEditTexture
///
/// CLASS: CEditTexture
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:26:51 PM
///
/// PARAMETERS: 
///             CView* pView
///             CTextureData* pTexture
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CEditTexture::CEditTexture(CView* pView,CTextureData* pTexture,CWnd* pParent /*=NULL*/)
: CDialog(CEditTexture::IDD, pParent)
{
   //{{AFX_DATA_INIT(CEditTexture)
   //}}AFX_DATA_INIT
   m_pTexture=pTexture;
   m_pView = pView;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CEditTexture
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:26:52 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditTexture::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CEditTexture)
   //}}AFX_DATA_MAP

   DDX_Text(pDX, IDC_AL_FILE, m_pTexture->m_AlphaFile);
   DDX_Text(pDX, IDC_AL_SCALE, m_pTexture->m_fAlphaScale);
   DDX_Check(pDX, IDC_BASE_TEXTURE, m_pTexture->m_bBaseTextrue);
   DDX_Text(pDX, IDC_TC_FILE, m_pTexture->m_File);
   DDX_Text(pDX, IDC_TC_SCALE, m_pTexture->m_fScale);
}


BEGIN_MESSAGE_MAP(CEditTexture, CDialog)
   //{{AFX_MSG_MAP(CEditTexture)
   ON_BN_CLICKED(IDC_TC_FILE_BUT, OnTcFileBut)
   ON_BN_CLICKED(IDC_AL_FILE_BUT, OnAlFileBut)
   ON_BN_CLICKED(IDC_GENERATE_LIGHT, OnGenerateLight)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditTexture message handlers





/////////////////////////////////////////////////
///
/// NAME: OnTcFileBut
///
/// CLASS: CEditTexture
///
/// DESCRIPTION: File button pressed
///
/// CREATED:     02/06/2004 5:26:59 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditTexture::OnTcFileBut() 
{
   static TCHAR BASED_CODE szFilter[] = IMAGE_FILE_FILTER;
   CString FileName;
   GetDlgItemText(IDC_TC_FILE,FileName);

   CFileDialog dlg(TRUE,_T(".jpg"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      SetDlgItemText(IDC_TC_FILE,dlg.GetFileName());
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnAlFileBut
///
/// CLASS: CEditTexture
///
/// DESCRIPTION: File button pressed
///
/// CREATED:     02/06/2004 5:27:03 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditTexture::OnAlFileBut() 
{
   static TCHAR BASED_CODE szFilter[] = IMAGE_FILE_FILTER;
   CString FileName;
   GetDlgItemText(IDC_AL_FILE,FileName);

   CFileDialog dlg(TRUE,_T(".jpg"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      SetDlgItemText(IDC_AL_FILE,dlg.GetFileName());
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnGenerateLight
///
/// CLASS: CEditTexture
///
/// DESCRIPTION: Generates the Light map
///
/// CREATED:     02/06/2004 5:27:11 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditTexture::OnGenerateLight() 
{
   if( m_pView->IsKindOf(RUNTIME_CLASS(CTerraEdView)) )
   {
      CString FileName;
      GetDlgItemText(IDC_AL_FILE,FileName);

      BOOL bBaseTextrue = GetDlgItemInt(IDC_BASE_TEXTURE);
      static_cast<CTerraEdView*>(m_pView)->GenLightMap(FileName,!bBaseTextrue);
   }
}
