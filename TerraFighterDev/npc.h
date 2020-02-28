// NPC.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__8A3E4B3B_00E0_48AB_9163_150CE7BB09D6__INCLUDED_)
#define AFX_NPC_H__8A3E4B3B_00E0_48AB_9163_150CE7BB09D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vehicle.h"
#include <Microthread.hpp>

enum AI_States_e
{
   AIS_IDLE = 0,
   AIS_ATTACKING = 1,
   AIS_EVADING = 2,
   AIS_RUNNING = 3,

   AIS_GT_SEEK = 4,
   AIS_GT_ATTACKING = 5,
   AIS_GT_EVADING = 6,

   AIS_GARD_SEEK = 7,
   AIS_GARD_ATTACKING = 8,
   AIS_GARD_EVADING = 9,
   AIS_GARD_PATROL = 10,

   MAX_AIS_STATES = 11
};

class CNpc : public CVehicle, public Microthread
{
   AI_States_e m_StartState;
   D3DXVECTOR3 m_SeekPos;
   FLOAT m_CurrentMaxSpeed;

   FLOAT m_DestAngleZ;
   FLOAT m_DestAngleY;

   FLOAT m_RectionTime;

   CDisplayObject* m_pCurrentGoal;

   struct ThreadParam
   {
      FLOAT fElapsedTime;
      SpriteList* pSpriteList;
      CWeapon* Weapons;
      CTerrain* pFloor;
      CParticleSystem* pParticleSystem;
      BOOL bCurrentPlayer;
      CNetworkConnection* pNetConnect;
      CPlanetObjects* pPlanetObjects;
   };

   ThreadParam m_ThreadParam;


   virtual HRESULT Create(char* object, FLOAT Scale,D3DXVECTOR3 pos,CSoundManager* pSoundMgr);

public:
   void GetRandomPoint(CTerrain* pFloor);
   void SetAIType(AIType_e Type);


   FLOAT GetCurrentMaxSpeed(){return m_CurrentMaxSpeed;}
   void SetCurrentMaxSpeed(FLOAT CurrentMaxSpeed){m_CurrentMaxSpeed = CurrentMaxSpeed;}

   void GetFullPlayerMsg(GAMEMSG_FULL_PLAYER_UPDATE *pMsg);

   D3DXVECTOR3 GetSeekPos(){return m_SeekPos;}
   void SetSeekPos(D3DXVECTOR3 SeekPos){m_SeekPos = SeekPos;}


   CNpc();
   virtual ~CNpc();
   virtual HRESULT FrameMove(FLOAT fTime,FLOAT fElapsedTime, CInput* pInput ,CTerrain* pFloor,LPDIRECT3DDEVICE9 pd3dDevice,
      CParticleSystem* pParticleSystem,
      SpriteList* pSpriteList,
      BOOL bCurrentPlayer,CWater* pWater,
      CWeapon* Weapons,
      DWORD nWeapons,CNetworkConnection *pNetConnect,
      CPlanetObjects* pPlanetObjects,
      CChatDlg* pChatDlg,
      std::vector<CGameAction>& Actions);

   virtual HRESULT Create(CSpriteDataEx* pData, D3DXVECTOR3 pos, CSoundManager* pSoundMgr,WORD TeamNo,WORD Type,int CurrentTexture);

   void thread_proc();

   
};

#endif // !defined(AFX_NPC_H__8A3E4B3B_00E0_48AB_9163_150CE7BB09D6__INCLUDED_)
