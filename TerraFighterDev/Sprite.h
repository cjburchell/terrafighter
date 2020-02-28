// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__E0D81295_4124_43ED_AE83_C0227557EC04__INCLUDED_)
#define AFX_SPRITE_H__E0D81295_4124_43ED_AE83_C0227557EC04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DisplayObject.h"
#include <vector>
#include "vertextypes.h"
#include "NetMessages.h"
#include "2DDraw.h"
#include "Terrain.h"
#include "input.h"
#include "ParticleSystem.h"
#include "Weapon.h"
#include "Exaust.h"
#include "xmlhelpers.h"
#include "trees.h"
#include "ChatDlg.h"
#include "SpriteData.h"


#define PLAYER_SPRITE 0

class CSpriteDataEx : public CSpriteData
{
public:
   HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice, D3DXMATRIX* pmatWorldMatrix)
   {
      if(m_pMesh)
         return m_pMesh->render(pd3dDevice,pmatWorldMatrix);
      return E_FAIL;
   }
private:
   Xfile::File*     m_pMesh;
public:

   ~CSpriteDataEx()
   {
      MeshCash::releaseMesh(m_pMesh);
   }


   HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);


   HRESULT DeleteDeviceObjects()
   {
      MeshCash::releaseMesh(m_pMesh);
      m_pMesh = NULL;
      return S_OK;
   }

};

class CSprite;

typedef std::vector<CSprite*> SpriteList;


class CSprite : public CDisplayObject
{
public:
   CDisplayObject* GetNextTarget(CDisplayObject *pTarget,SpriteList* pSpriteList,ObjectList* pObjectList);
   virtual void DoCollision(CDisplayObject* pObj, std::vector<CGameAction>* pActions,CChatDlg* pChatDlg);
   void SetPlayerMsg(GAMEMSG_PLAYER_UPDATE* pMsg);
   void GetPlayerMsg(GAMEMSG_PLAYER_UPDATE* pMsg);
   void GetFullPlayerMsg(GAMEMSG_FULL_PLAYER_UPDATE* pMsg);
   void UpdateHud(LPDIRECT3DDEVICE9 pd3dDevice,CD3DCamera* pCamera,C2DDraw* pDraw);
   BOOL m_bMoving;
   BOOL IsAlive(){return m_SpriteState != SPRITE_DEAD;}
   void SetSpriteState(Sprite_State_e state){m_SpriteState=state;}
   virtual void DisplayStats(LPDIRECT3DDEVICE9 pd3dDevice,CD3DFont* pFont,C2DDraw* pDraw,CTerrain* pTerrain,SpriteList* pSpriteList,ObjectList* pObjectList,BOOL bFirstPerson,CD3DCamera* pCamera);
   CSprite();
   virtual ~CSprite();
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);
   virtual HRESULT RenderClear(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper,CD3DCamera * pCamera);
   const D3DXVECTOR3& GetDirectionVector(FLOAT val);
   virtual HRESULT FrameMove( FLOAT fTime,
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
   virtual void DeleteDeviceObjects();
   const D3DXVECTOR3& GetCameraPos();
   const D3DXVECTOR3& GetUpVector();
   virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
   virtual HRESULT Create(CSpriteDataEx* pData, D3DXVECTOR3 pos, CSoundManager* pSoundMgr, WORD TeamNo,WORD Type,int CurrentTexture);

   virtual void BuildShadow(const D3DXVECTOR3& lightpos);
   virtual HRESULT RenderShadow(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);

   const D3DXMATRIX& GetRotationMat(){return m_Rotate;};

   virtual FLOAT GetSpeed()
   {
      return m_Speed.x;
   }

   BOOL IsTargetLockOn(){return m_bTargetLock;}

   CDisplayObject* GetCurrentTarget(){return m_pTarget;}
   void SetCurrentTarget(CSprite* pTarget){m_pTarget = pTarget;}

   void SetTeamNo(WORD Team){m_TeamNo = Team;}
   WORD GetTeamNo(){return m_TeamNo;}

   CSprite* GetClosestSprite(SpriteList* pSpriteList,FLOAT& Distance,BOOL bSameTeam);
   virtual DWORD GetTeamColour(){return m_TeamColour;}
   CDisplayObject* GetGoal(SpriteList* pSpriteList,ObjectList* pObjectList);

   void ClearTarget();

   int GetCurrentWeapon(){return m_CurrentWeapon;}

   DPNID GetID(){return m_ID;}
   void SetID(DPNID ID){m_ID=ID;}

   virtual HRESULT RenderSimple(LPDIRECT3DDEVICE9 pd3dDevice);

   virtual Xfile::File* GetMesh()
   {
      return m_pMesh;
   }



   virtual void SetAIType(AIType_e Type){m_AIType = Type;}
   AIType_e GetAIType(){return m_AIType;}

   const TCHAR* GetGoalName(){return m_GoalName;}
   void SetGoalName(const TCHAR* GoalName){m_GoalName = GoalName;}

   virtual void AddPowerUp(CPowerUpData* pPowerUp,const TCHAR* Name,CChatDlg* pChatDlg);
   virtual void HitWaypoint(CString Name,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg,BOOL bVisable);

   virtual BOOL IsInvisable(){return m_bInvisable;}

   void AddFrag(){m_FragCount++;}
   void ClearFrags(){m_FragCount = 0;}
   DWORD GetFrags(){return m_FragCount;}

   void SetFrags(DWORD FragCount){m_FragCount = FragCount;}

   BOOL GetGunPortPos(int port,D3DXVECTOR3& pPos);
   BOOL GetExaustPortPos(int port,D3DXVECTOR3& pPos);
protected:

   DWORD m_FragCount;


   WORD			   m_Type;
   DPNID              m_ID; //ID of the object;

   virtual HRESULT Create(const TCHAR* object, FLOAT Scale,D3DXVECTOR3 pos,CSoundManager* pSoundMgr);


   // Hud Texture
   CSurface   m_HudDisplay;

   // guns
   FLOAT m_LastShot;

   int m_CurrentWeapon;
   FLOAT m_WeaponPowerMod;

   std::vector<CPowerUpData> m_PowerUpList;


   // sound
   //CSound* m_pSoundShot;
   CSound* m_pSoundEngine;
   LPDIRECTSOUND3DBUFFER   m_pDS3DBufferEngine;

   Sprite_State_e m_SpriteState;

   // object status
   FLOAT m_fShield_Str;
   FLOAT m_fDamage;

   BOOL m_bInvrunable;
   BOOL m_bInvisable;
   FLOAT m_OldMaxSpeed;

   Xfile::File* m_pShield;
   D3DMATERIAL9 m_ShieldMat;
   BOOL m_bShowShield;
   D3DXMATRIX m_ShieldMatrix;
   FLOAT m_ShieldScale;
   FLOAT m_ShieldLevel;

   CString m_CurrentTextureFile;
   CComPtr<IDirect3DTexture9> m_pCurrentTexture;

   // object constants
   FLOAT m_MaxSpeed;
   FLOAT m_RotationTime;
   FLOAT m_Acceleration;
   FLOAT m_Friction;

   FLOAT m_fMinSpeed;
   FLOAT m_fMaxHight;
   FLOAT m_fMaxShields;
   FLOAT m_fMaxDamage;
   FLOAT m_fRegenRate;
   FLOAT m_fMinHight;
   FLOAT m_zMeshRot;
   FLOAT m_yMeshRot;
   BOOL m_bRestrictRot;
   D3DXMATRIX m_MeshScaled;

   // particle system
   CExaust* m_pExaust;

   FLOAT m_angleX;
   FLOAT m_angleY;
   FLOAT m_angleZ;

   D3DXVECTOR3 m_Speed;

   CDisplayObject* m_pTarget;
   BOOL m_bTargetLock;

   WORD m_TeamNo;
   DWORD m_TeamColour;
   CString m_GoalName;

   AIType_e m_AIType;


   // mesh stuff
   CString m_MeshFile;
   Xfile::File* m_pMesh;
   // animation keys
   FLOAT m_fCurTime;
   int m_iKeyFrame;

   D3DXVECTOR3 m_CameraPos;

};



#endif // !defined(AFX_SPRITE_H__E0D81295_4124_43ED_AE83_C0227557EC04__INCLUDED_)
