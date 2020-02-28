///////////////////////////////////////////////////////////////////////////////
///  AddTexture.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:18:48 PM by Christiaan Burchell
///
///  PURPOSE:  The Add Texture dialog
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

// AddTexture.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "AddTexture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddTexture dialog




/////////////////////////////////////////////////
///
/// NAME: CAddTexture
///
/// CLASS: CAddTexture
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:17:31 PM
///
/// PARAMETERS: 
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CAddTexture::CAddTexture(CWnd* pParent /*=NULL*/)
: CDialog(CAddTexture::IDD, pParent)
{
   //{{AFX_DATA_INIT(CAddTexture)
   m_File = _T("");
   //}}AFX_DATA_INIT
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CAddTexture
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:17:33 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CAddTexture::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CAddTexture)
   DDX_Text(pDX, IDC_TEXTURE_FILE, m_File);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddTexture, CDialog)
   //{{AFX_MSG_MAP(CAddTexture)
   ON_BN_CLICKED(IDC_TC_FILE_BUT, OnTcFileBut)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddTexture message handlers



/////////////////////////////////////////////////
///
/// NAME: OnTcFileBut
///
/// CLASS: CAddTexture
///
/// DESCRIPTION: File button hit
///
/// CREATED:     02/06/2004 5:18:29 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CAddTexture::OnTcFileBut() 
{
   static TCHAR BASED_CODE szFilter[] = IMAGE_FILE_FILTER;
   CString FileName;
   GetDlgItemText(IDC_TEXTURE_FILE,FileName);

   CFileDialog dlg(TRUE,_T(".jpg"),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      SetDlgItemText(IDC_TEXTURE_FILE,dlg.GetFileName());
   }
}
