// Station.h: interface for the CStaticObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICOBJ_H__3C54BBA7_C309_4991_9960_AC5C89235000__INCLUDED_)
#define AFX_STATICOBJ_H__3C54BBA7_C309_4991_9960_AC5C89235000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprite.h"
#include <microthread.hpp>

class CStation : public CSprite, public Microthread  
{
   struct ThreadParam
   {
      FLOAT fElapsedTime;
      SpriteList* pSpriteList;
      CWeapon* Weapons;
      CTerrain* pFloor;
      CParticleSystem* pParticleSystem;
      BOOL bCurrentPlayer;
      CNetworkConnection* pNetConnect;
   };

public:
   CStation();
   virtual ~CStation();
   virtual HRESULT FrameMove(FLOAT fTime,
      FLOAT fElapsedTime, 
      CInput* pInput ,
      CTerrain* pFloor,
      LPDIRECT3DDEVICE9 pd3dDevice,
      CParticleSystem* pParticleSystem,
      SpriteList* pSpriteList,
      BOOL bCurrentPlayer,
      CWater* pWater,
      CWeapon* Weapons,
      DWORD nWeapons,
      CNetworkConnection* pNetConnect,
      CPlanetObjects* pPlanetObjects,
      CChatDlg* pChatDlg,
      std::vector<CGameAction>& Actions);

   void thread_proc();

   ThreadParam m_ThreadParam;

   FLOAT m_AnimationAngleZ;
};

#endif // !defined(AFX_STATICOBJ_H__3C54BBA7_C309_4991_9960_AC5C89235000__INCLUDED_)
