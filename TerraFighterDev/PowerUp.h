// PowerUp.h: interface for the CPowerUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWERUP_H__87DC897A_CE4A_48FF_A446_70A95C302664__INCLUDED_)
#define AFX_POWERUP_H__87DC897A_CE4A_48FF_A446_70A95C302664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StaticMesh.h"
#include "LevelData.h"

class CPowerUp : public CStaticMesh  
{
public:
   CPowerUp(CPowerUpData Data,const char* object);
   virtual ~CPowerUp();
   void SetActive(BOOL bActive = TRUE){m_bActive = bActive;}
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);
   virtual void DoCollision(CDisplayObject* pObj,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg);
private:
   BOOL m_bActive;
   CPowerUpData m_Data;

};

#endif // !defined(AFX_POWERUP_H__87DC897A_CE4A_48FF_A446_70A95C302664__INCLUDED_)
