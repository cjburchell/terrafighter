///////////////////////////////////////////////////////////////////////////////
///  EditObject.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:24:13 PM by Christiaan Burchell
///
///  PURPOSE:  Edit Object Dialog
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

// EditObject.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "EditObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditObject dialog




/////////////////////////////////////////////////
///
/// NAME: CEditObject
///
/// CLASS: CEditObject
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:24:16 PM
///
/// PARAMETERS: 
///             CLevelDataObject* pObject
///             std::vector<CPlanetObjectDataEx*>* pObjectsData
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CEditObject::CEditObject(CLevelDataObject* pObject,std::vector<CPlanetObjectDataEx*>* pObjectsData,CWnd* pParent /*=NULL*/)
: CDialog(CEditObject::IDD, pParent)
{
   //{{AFX_DATA_INIT(CEditObject)
   //}}AFX_DATA_INIT
   m_pObject = pObject;
   m_pObjectsData = pObjectsData;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CEditObject
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:24:18 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditObject::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CEditObject)
   DDX_Control(pDX, IDC_TYPE, m_TypeCtrl);
   //}}AFX_DATA_MAP
   DDX_CBIndex(pDX, IDC_TYPE, m_pObject->m_Type);
   DDX_Text(pDX, IDC_POS_X, m_pObject->m_pos.x);
   DDX_Text(pDX, IDC_POS_Y, m_pObject->m_pos.y);
   DDX_Text(pDX, IDC_POS_Z, m_pObject->m_pos.z);
   DDX_Text(pDX, IDC_NAME, m_pObject->m_Name);
   DDX_Check(pDX, IDC_VISABLE, m_pObject->m_bVisable);
}


BEGIN_MESSAGE_MAP(CEditObject, CDialog)
   //{{AFX_MSG_MAP(CEditObject)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditObject message handlers



/////////////////////////////////////////////////
///
/// NAME: OnInitDialog
///
/// CLASS: CEditObject
///
/// DESCRIPTION: Initializes the dialog
///
/// CREATED:     02/06/2004 5:24:20 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CEditObject::OnInitDialog() 
{
   CDialog::OnInitDialog();

   for(int i=0;i<m_pObjectsData->size();i++)
   {
      m_TypeCtrl.AddString((*m_pObjectsData)[i]->m_Name);
   }

   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
