// PowerUp.cpp: implementation of the CPowerUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PowerUp.h"
#include "Sprite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPowerUp::CPowerUp(CPowerUpData Data,const char* object) : CStaticMesh(object)
{
   m_Data = Data;
   m_bActive = TRUE;
}

CPowerUp::~CPowerUp()
{

}

HRESULT CPowerUp::Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   if(m_bActive)
      return CStaticMesh::Render(pd3dDevice,pClipper);

   return S_OK;
}

void CPowerUp::DoCollision(CDisplayObject* pObj,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg)
{
   if(m_bActive)
   {
      static_cast<CSprite*>(pObj)->AddPowerUp(&m_Data,GetName(),pChatDlg);
      m_bActive = FALSE;
      SetVisable(FALSE);
   }
}
