// DolphinApp.h: interface for the CDolphinApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOLPHINAPP_H__E48D71E8_0052_4471_9113_A0D68DF5B67B__INCLUDED_)
#define AFX_DOLPHINAPP_H__E48D71E8_0052_4471_9113_A0D68DF5B67B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DUtil.h"

#include "input.h"
#include "D3DFile.h"
#include "FOVClipper.h"
#include "windowmgr.h"
#include "Hud.h"
#include <mmsystem.h>
#include <dsound.h>
#include "dsutil.h"
#include "Surface.h"
#include "3DTarget.h"
#include "MenueDlg.h"
#include "SelectShipDlg.h"
#include "Weapon.h"
#include "NetworkConnection.h"
#include "..\Zip\ZipArchive.h"
#include "Planet.h"

#include "XMLHelpers.h"
#include "ChatDlg.h"



#define MAX_LEVELS 9



class CTerraFighterApp : public CD3DApplication  
{
protected:

   // Font for drawing text
   CD3DFont* m_pFont;
   CInput* m_pInput;

   CD3DCamera m_Camera;
   CFOVClipper m_Clipper;


   BOOL m_bUseArcBall;
   CD3DArcBall m_ArcBall;

   C3DTarget m_Target;

   CPlanet* m_pPlanet;


   int m_nEnemyShips;
   int m_nGunTurrets;

   C2DDraw m_2dDraw;
   CWindowMgr m_WinMgr;
   CMenueDlg* m_pMenuedlg;
   CChatDlg* m_pChat;

   D3DXMATRIX m_matProj;

   CSoundManager           m_SoundManager;
   LPDIRECTSOUND3DLISTENER m_pDSListener;  // 3D listener object

   AppState_e                m_AppState;              // Current state the app is in
   CSurface                  m_SplashBitmap;
   CSurface                  m_SplashBitmap2;

   DWORD m_Level;

   FLOAT m_FadeValue;
   BOOL m_bFadeIn;

   BOOL m_bCreatedObj;
   BOOL m_bDisplayStats;
   BOOL m_bFirstPerson;
   BOOL m_bDisplayHUD;

   BOOL m_bCanContinueGame;


   BOOL LoadSpriteData();

   BOOL LoadLevelData();

   BOOL LoadWeaponData();

   BOOL LoadPlanetObjectData();


   CWeapon* m_Weapons;
   DWORD m_nWeapons;

   CLevelData* m_Levels;
   BOOL* m_pbLevelDone;

   int m_CurrentMap;
   DWORD m_MaxMaps;

   DWORD m_StartMap;

   CSpriteDataEx* m_SpriteData;
   DWORD m_MaxSpriteData;

   CPlanetObjectData* m_PlanetObjectData;
   DWORD m_MaxPlanetObjectData;

   int m_PlayerShipType;
   CString m_ObjStats;

   BOOL m_bDisplayShadows;
   BOOL m_b3DSound;
   BOOL m_bWaterEff;
   BOOL m_bLOD;
   BOOL m_bMulTex;

   CString m_PlayerName;

   CNetworkConnection* m_pNetConnect;
   BOOL m_bMultiPlayer;


public:
   BOOL GetNewMessages();

   CInput* GetInput(){return m_pInput;};
   C2DDraw* GetDisplay(){return &m_2dDraw;};



   static HRESULT WINAPI StaticDirectPlayMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );
   HRESULT DirectPlayMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );
   static HRESULT WINAPI StaticDirectPlayLobbyMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );
   HRESULT DirectPlayLobbyMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );


   void DisplayProgress(CString str, FLOAT pcentDone);
   void InitLights();

   HRESULT DrawShadow();

   void TakeScreenShot(const char* file_name);

   HRESULT MoveSprites();
   HRESULT MoveCamera();
   HRESULT MoveLights();
   HRESULT NextLevel();
   CWindowMgr* GetWindowMgr(){return &m_WinMgr;}
   HRESULT RenderScene();

   HRESULT OneTimeSceneInit();
   HRESULT InitDeviceObjects();
   HRESULT RestoreDeviceObjects();
   HRESULT InvalidateDeviceObjects();
   HRESULT DeleteDeviceObjects();
   HRESULT Render();
   HRESULT FrameMove();
   HRESULT FinalCleanup();
   HRESULT ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format );
   CTerraFighterApp();
   virtual ~CTerraFighterApp();
   LPDIRECT3DDEVICE9 GetDevice(){return m_pd3dDevice;}
   virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

   virtual BOOL LookForMSG();

   HRESULT UserSelectNewDevice();

   CZipArchive m_Zip;
};

CTerraFighterApp* GetApp();

inline Xfile::File* MeshCash_CreateMesh( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR const* strMesh, DWORD FVF)
{
   return MeshCash::createMesh(pd3dDevice, strMesh, FVF, &GetApp()->m_Zip);
}

namespace D3DUtil
{
   inline HRESULT CreateTexture( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* strTexture,
      CComPtr<IDirect3DTexture9>& pTexture,
      D3DFORMAT d3dFormat = D3DFMT_UNKNOWN)
   {
      return CreateTexture(pd3dDevice,  strTexture,
         pTexture,
         &GetApp()->m_Zip,
         d3dFormat);
   }
};


#endif // !defined(AFX_DOLPHINAPP_H__E48D71E8_0052_4471_9113_A0D68DF5B67B__INCLUDED_)
