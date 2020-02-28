// Planet.h: interface for the CPlanet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANET_H__D4A2668A_B680_427E_90AC_6EA37F58C1C7__INCLUDED_)
#define AFX_PLANET_H__D4A2668A_B680_427E_90AC_6EA37F58C1C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sprite.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Water.h"
#include "LensFlare.h"
#include "ShotSystem.h"
#include "trees.h"
#include "ChatDlg.h"




enum AppState_e
{
   APPSTATE_DISPLAYSPLASH,
   APPSTATE_STARTSPLASH,
   APPSTATE_DISPLAYSPLASH2,
   APPSTATE_STARTSPLASH2,
   APPSTATE_START_MENU,
   APPSTATE_MENU,
   APPSTATE_ACTIVE, 
   APPSTATE_ENDACTIVE,
   APPSTATE_STARTENDACTIVE,
   APPSTATE_STARTLEVELSCREEN, 
   APPSTATE_DISPLAYLEVELSCREEN,
};


class CPlanet  
{
public:
   CPlanet();
   virtual ~CPlanet();
   BOOL Render(LPDIRECT3DDEVICE9 pd3dDevice,CD3DCamera* pCamera,
      DWORD &dwTR,CFOVClipper* pClipper, BOOL bMulTex, BOOL bLOD, BOOL bDisplayShadows, BOOL bWaterEff,BOOL bFirstPerson);
   BOOL MoveSprites(GameState_e &GameState,
      BOOL bMultiPlayer,
      FLOAT fTime,
      FLOAT fElapsedTime, 
      CInput* pInput ,
      LPDIRECT3DDEVICE9 pd3dDevice,
      CWeapon* Weapons,
      DWORD nWeapons,
      CNetworkConnection *pNetConnect,
      BOOL bWaterEff,
      int PlayerShipType,
      CSpriteDataEx* pShipData,
      CSoundManager* pSoundManager,
      CChatDlg* pChatDlg,
      CPlanetObjectData* pPlanetObjectData);

   BOOL UpdateLights(BOOL bDisplayShadows,CD3DCamera* pCamera);
   CSprite* GetPlayerObj();

   SpriteList* GetSpriteList(){return &m_SpriteList;}

   ObjectList* GetObjectList(){return m_pPlanetObjects->GetObjectList();}

   CTerrain* GetTerrain(){return m_pTerrain;}

   HRESULT DeleteDeviceObjects();
   HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice,D3DSURFACE_DESC* d3dsdBackBuffer);


   BOOL SetLevelData(CLevelData* pLevel);

   BOOL CreateMap(LPDIRECT3DDEVICE9 pd3dDevice);

   BOOL CreateSprites(LPDIRECT3DDEVICE9 pd3dDevice,CSoundManager* pSoundManager,CNetworkConnection *pNetConnect,
      int PlayerShipType,
      CSpriteDataEx* pShipData,DWORD MaxShipData,GAMEMSG_WORLD_STAT* pWorldData,CString PlayerName);

   BOOL CreateSkyBox(LPDIRECT3DDEVICE9 pd3dDevice);
   BOOL CreateWater(LPDIRECT3DDEVICE9 pd3dDevice);
   BOOL CreateObjects(LPDIRECT3DDEVICE9 pd3dDevice,CSoundManager* pSoundManager,CPlanetObjectData* pPlanetObjectData,DWORD MaxPlanetObjectData);

   void InitLights(LPDIRECT3DDEVICE9 pd3dDevice);
   void FireShot(GAMEMSG_SHOT_FIRED* pShotMsg,int SpriteIndex);
   void DrawShadow(LPDIRECT3DDEVICE9 pd3dDevice);

private:
   CString m_PlayerName;

   CTerrain* m_pTerrain;
   CWater* m_pWater;
   CLensFlare m_LensFlare;

   //sprites
   SpriteList m_SpriteList;

   // static objects
   CPlanetObjects* m_pPlanetObjects;

   // Particle stuff
   CShotSystem*   m_pParticleSystem;

   BOOL m_bShowSkyBox;
   CSkyBox*     m_pSkyBox;            // Skybox background object

   DWORD m_BGColour;  // planits abent color

   D3DXVECTOR3 m_SunPos;

   CLevelData* m_pLevel;

   // shadow stuff
   CComPtr<IDirect3DVertexBuffer9> m_pBigSquareVB;
   FLOAT m_LevelTime;

};

#endif // !defined(AFX_PLANET_H__D4A2668A_B680_427E_90AC_6EA37F58C1C7__INCLUDED_)
