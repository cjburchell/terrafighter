///////////////////////////////////////////////////////////////////////////////
///  EditSprite.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 02/06/2004 5:25:23 PM by Christiaan Burchell
///
///  PURPOSE:  Edit Sprite dialog
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

// EditSprite.cpp : implementation file
//

#include "stdafx.h"
#include "TerraEd.h"
#include "EditSprite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSprite dialog




/////////////////////////////////////////////////
///
/// NAME: CEditSprite
///
/// CLASS: CEditSprite
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:25:26 PM
///
/// PARAMETERS: 
///             CLevelDataSprite* pSprite
///             std::vector<CSpriteDataEx*>* pSpriteData
///             CLevelData* pLevel
///             CWnd* pParent /*=NULL*/
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CEditSprite::CEditSprite(CLevelDataSprite* pSprite,std::vector<CSpriteDataEx*>* pSpriteData,CLevelData* pLevel,CWnd* pParent /*=NULL*/)
: CDialog(CEditSprite::IDD, pParent)
{
   //{{AFX_DATA_INIT(CEditSprite)
   //}}AFX_DATA_INIT
   m_pSprite = pSprite;
   m_pSpriteData = pSpriteData;
   m_pLevel = pLevel;
}




/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CEditSprite
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     02/06/2004 5:25:28 PM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditSprite::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CEditSprite)
   DDX_Control(pDX, IDC_TYPE, m_TypeCtrl);
   DDX_Control(pDX, IDC_GOAL, m_GoalCtrl);
   DDX_Control(pDX, IDC_TEXTURE, m_TextureCtrl);
   //}}AFX_DATA_MAP

   DDX_CBIndex(pDX, IDC_TYPE, m_pSprite->m_Type);

   int temp = m_pSprite->m_AIType;
   DDX_CBIndex(pDX, IDC_AI, temp);
   m_pSprite->m_AIType = (AIType_e)temp;

   DDX_CBIndex(pDX, IDC_TEXTURE, m_pSprite->m_CurrentTexture);

   DDX_Text(pDX, IDC_POS_X, m_pSprite->m_pos.x);
   DDX_Text(pDX, IDC_POS_Y, m_pSprite->m_pos.y);
   DDX_Text(pDX, IDC_POS_Z, m_pSprite->m_pos.z);
   DDX_Text(pDX, IDC_TEAM, m_pSprite->m_Team);
   DDX_CBString(pDX, IDC_GOAL, m_pSprite->m_GoalName);
   DDX_Text(pDX, IDC_NAME, m_pSprite->m_Name);
}


BEGIN_MESSAGE_MAP(CEditSprite, CDialog)
   //{{AFX_MSG_MAP(CEditSprite)
   ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSprite message handlers



/////////////////////////////////////////////////
///
/// NAME: OnInitDialog
///
/// CLASS: CEditSprite
///
/// DESCRIPTION: Initializes the dialog
///
/// CREATED:     02/06/2004 5:25:30 PM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CEditSprite::OnInitDialog() 
{
   CDialog::OnInitDialog();

   m_TextureCtrl.AddString("default");

   for(int i=0;i<(*m_pSpriteData)[m_pSprite->m_Type]->m_Textures.size();i++)
   {
      m_TextureCtrl.AddString((*m_pSpriteData)[m_pSprite->m_Type]->m_Textures[i]);
   }


   for(i=0;i<m_pSpriteData->size();i++)
   {
      m_TypeCtrl.AddString((*m_pSpriteData)[i]->m_Name);
   }

   m_GoalCtrl.AddString("CURRENT_PLAYER");

   for(i=0;i<m_pLevel->m_Sprites.size();i++)
   {
      if(m_pLevel->m_Sprites[i]->m_Name != m_pSprite->m_Name)
      {
         m_GoalCtrl.AddString(m_pLevel->m_Sprites[i]->m_Name);
      }
   }

   for(i=0;i<m_pLevel->m_Events.size();i++)
   {
      for(int j=0;j<m_pLevel->m_Events[i]->m_CreateSprites.size();j++)
      {
         if(m_pLevel->m_Events[i]->m_CreateSprites[j]->m_Name != m_pSprite->m_Name)
         {
            m_GoalCtrl.AddString(m_pLevel->m_Events[i]->m_CreateSprites[j]->m_Name);
         }
      }
   }

   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}



/////////////////////////////////////////////////
///
/// NAME: OnSelchangeType
///
/// CLASS: CEditSprite
///
/// DESCRIPTION: User selected a new type
///
/// CREATED:     02/06/2004 5:25:42 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CEditSprite::OnSelchangeType() 
{
   int newtype = m_TypeCtrl.GetCurSel();
   m_TextureCtrl.ResetContent();
   m_TextureCtrl.AddString("default");

   for(int i=0;i<(*m_pSpriteData)[newtype]->m_Textures.size();i++)
   {
      m_TextureCtrl.AddString((*m_pSpriteData)[newtype]->m_Textures[i]);
   }

}
