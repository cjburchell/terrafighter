///////////////////////////////////////////////////////////////////////////////
///  DeleteFileDlg.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:22:17 PM by Christiaan Burchell
///
///  PURPOSE:  Delete file dialog
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

// DeleteFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "DeleteFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeleteFileDlg dialog




/////////////////////////////////////////////////
///
/// NAME: CDeleteFileDlg
///
/// CLASS: CDeleteFileDlg
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:21:53 PM
///
/// PARAMETERS: 
///             CZipArchive* pZip
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CDeleteFileDlg::CDeleteFileDlg(CZipArchive* pZip,CWnd* pParent /*=NULL*/)
: CDialog(CDeleteFileDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CDeleteFileDlg)
   // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   m_pZip = pZip;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CDeleteFileDlg
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:21:55 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CDeleteFileDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CDeleteFileDlg)
   DDX_Control(pDX, IDC_LIST1, m_list);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeleteFileDlg, CDialog)
   //{{AFX_MSG_MAP(CDeleteFileDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteFileDlg message handlers



/////////////////////////////////////////////////
///
/// NAME: OnOK
///
/// CLASS: CDeleteFileDlg
///
/// DESCRIPTION: User Pressed the OK Button
///
/// CREATED:     02/06/2004 5:21:58 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CDeleteFileDlg::OnOK() 
{
   UINT uSelectedCount = m_list.GetSelectedCount();

   if (uSelectedCount > 0)
   {
      int  nItem= -1;
      for (UINT i=0;i < uSelectedCount;i++)
      {
         nItem = m_list.GetNextItem(nItem, LVNI_SELECTED);
         m_DelList.Add(m_list.GetItemData(nItem));
      }
      CDialog::OnOK();
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnInitDialog
///
/// CLASS: CDeleteFileDlg
///
/// DESCRIPTION: Initializes the dialog
///
/// CREATED:     02/06/2004 5:22:02 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CDeleteFileDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();

   m_list.InsertColumn(0,"File Name",LVCFMT_LEFT,100);
   m_list.InsertColumn(1,"Size",LVCFMT_LEFT,50);
   m_list.InsertColumn(2,"Time",LVCFMT_LEFT,150);


   for (WORD i = 0; i < m_pZip->GetNoEntries(); i++)
   {
      CZipFileHeader fh;
      m_pZip->GetFileInfo(fh, i);
      int index = m_list.InsertItem(i,fh.GetFileName());

      CString size;
      DWORD tempSize = fh.m_uUncomprSize;
      size.Format("%d B",tempSize);
      if( tempSize >= 1028)
      {
         tempSize = tempSize/1028;
         size.Format("%d KB",tempSize);
      }

      if( tempSize >= 1028)
      {
         tempSize = tempSize/1028;
         size.Format("%d MB",tempSize);
      }

      m_list.SetItem(index,1,LVIF_TEXT,size,0,0,0,0);
      m_list.SetItem(index,2,LVIF_TEXT,fh.GetTime().Format("%d/%m/%Y %I:%M %p"),0,0,0,0);
      m_list.SetItemData(index,i);
   }

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
