// DolphinObject.h: interface for the CDolphinObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOLPHINOBJECT_H__79A2B326_7F42_4159_9C30_86A6D10D3BE1__INCLUDED_)
#define AFX_DOLPHINOBJECT_H__79A2B326_7F42_4159_9C30_86A6D10D3BE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vehicle.h"
#include "input.h"


class CPlayer : public CVehicle
{
public:
   virtual HRESULT FrameMove(FLOAT fTime,FLOAT fElapsedTime, CInput* pInput,CTerrain* pFloor,LPDIRECT3DDEVICE9 pd3dDevice,
      CParticleSystem* pParticleSystem,
      SpriteList* pSpriteList,
      BOOL bCurrentPlayer,CWater* pWater,
      CWeapon* Weapons,
      DWORD nWeapons,
      CNetworkConnection* pNetConnect,
      CPlanetObjects* pPlanetObjects,
      CChatDlg* pChatDlg,std::vector<CGameAction>& Actions);
   CPlayer();
   virtual ~CPlayer();

   virtual void HitWaypoint(CString Name,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg,BOOL bVisable);
   virtual void AddPowerUp(CPowerUpData* pPowerUp,CString Name,CChatDlg* pChatDlg);
protected:
   FLOAT m_bHeadLight;
   FLOAT m_SetSpeed;

   FLOAT m_SelectTime;

   FLOAT m_fAngleCameraY;
   FLOAT m_fAngleCameraX;
   FLOAT m_fCameraDistance;

   std::vector<CString> m_WaypointList;
};

#endif // !defined(AFX_DOLPHINOBJECT_H__79A2B326_7F42_4159_9C30_86A6D10D3BE1__INCLUDED_)
