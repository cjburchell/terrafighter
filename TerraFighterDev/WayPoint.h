// WayPoint.h: interface for the CWayPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAYPOINT_H__9D0BAFF6_1C4A_426A_8FA1_8C0D75E9E4C3__INCLUDED_)
#define AFX_WAYPOINT_H__9D0BAFF6_1C4A_426A_8FA1_8C0D75E9E4C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StaticMesh.h"

class CWayPoint : public CStaticMesh  
{
public:
   CWayPoint(const char* object);
   virtual ~CWayPoint();
   void SetActive(BOOL bActive = TRUE){m_bActive = bActive;}
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);
   virtual void DoCollision(CDisplayObject* pObj,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg);
private:
   BOOL m_bActive;
};

#endif // !defined(AFX_WAYPOINT_H__9D0BAFF6_1C4A_426A_8FA1_8C0D75E9E4C3__INCLUDED_)
