// WayPoint.cpp: implementation of the CWayPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WayPoint.h"
#include "Sprite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWayPoint::CWayPoint(const char* object) : CStaticMesh(object)
{
   m_bActive = TRUE;
}

CWayPoint::~CWayPoint()
{

}

HRESULT CWayPoint::Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   if(m_bVisable)
      return CStaticMesh::Render(pd3dDevice,pClipper);

   return S_OK;
}

void CWayPoint::DoCollision(CDisplayObject* pObj, std::vector<CGameAction>* pActions,CChatDlg* pChatDlg)
{
   //if(m_bActive)
   {
      static_cast<CSprite*>(pObj)->HitWaypoint(m_Name,pActions,pChatDlg,m_bVisable);
      //m_bActive = FALSE;
   }
}
