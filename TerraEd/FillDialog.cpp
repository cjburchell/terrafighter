///////////////////////////////////////////////////////////////////////////////
///  FillDialog.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:27:28 PM by Christiaan Burchell
///
///  PURPOSE:  Fill terrain dialog
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

// FillDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "FillDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFillDialog dialog




/////////////////////////////////////////////////
///
/// NAME: CFillDialog
///
/// CLASS: CFillDialog
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:27:30 PM
///
/// PARAMETERS: 
///             std::vector<CPassData>* pTextures
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CFillDialog::CFillDialog(std::vector<CPassData>* pTextures,CWnd* pParent /*=NULL*/)
: CDialog(CFillDialog::IDD, pParent)
{
   //{{AFX_DATA_INIT(CFillDialog)
   m_max = 255;
   m_min = 0;
   m_percent = 100;
   m_texture = 0;
   //}}AFX_DATA_INIT

   m_pTextures = pTextures;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CFillDialog
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:27:32 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CFillDialog::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CFillDialog)
   DDX_Control(pDX, IDC_TEXTURE, m_TexCtrl);
   DDX_Text(pDX, IDC_MAX_HIGHT, m_max);
   DDX_Text(pDX, IDC_MIN_HIGHT, m_min);
   DDX_Text(pDX, IDC_PERCENT, m_percent);
   DDX_CBIndex(pDX, IDC_TEXTURE, m_texture);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFillDialog, CDialog)
   //{{AFX_MSG_MAP(CFillDialog)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFillDialog message handlers



/////////////////////////////////////////////////
///
/// NAME: OnInitDialog
///
/// CLASS: CFillDialog
///
/// DESCRIPTION: Initializes the dialog
///
/// CREATED:     02/06/2004 5:27:34 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CFillDialog::OnInitDialog() 
{
   CDialog::OnInitDialog();

   for(int i=1;i<m_pTextures->size();i++)
   {
      m_TexCtrl.AddString((*m_pTextures)[i].m_TexFile);
   }

   m_TexCtrl.SetCurSel(m_texture);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
