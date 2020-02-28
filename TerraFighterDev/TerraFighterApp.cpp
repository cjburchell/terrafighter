#include "stdafx.h"
#include "dxutil.h"
#include "TerraFighterApp.h"
#include "Station.h"
#include "npc.h"
#include "MenueDlg.h"
#include "YesNoDlg.h"
#include "OptionsDlg.h"
#include "SelectLevelDlg.h"
#include "SelectDeviceDlg.h"
#include "resource.h"
#include "NetMessages.h"
#include "SayDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GUID g_guidApp = { 0xe19df6da, 0x65f9, 0x4ae8, { 0x9f, 0x9c, 0x95, 0xcb, 0xf4, 0xdc, 0x80, 0xc9 } };
#define TERRAFIGHTER_PORT 4242



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTerraFighterApp::~CTerraFighterApp()
{
   SAFE_RELEASE( m_pDSListener );
   SAFE_DELETE(m_pMenuedlg);
   SAFE_DELETE(m_pChat);
   SAFE_DELETE_ARRAY(m_Weapons);
   SAFE_DELETE_ARRAY(m_Levels);
   SAFE_DELETE_ARRAY(m_pbLevelDone);
   SAFE_DELETE_ARRAY(m_SpriteData);
   SAFE_DELETE_ARRAY(m_PlanetObjectData);
   SAFE_DELETE(m_pNetConnect);

}

//-----------------------------------------------------------------------------
// Name: CTerraFighterApp()
// Desc: Constructor
//-----------------------------------------------------------------------------
CTerraFighterApp::CTerraFighterApp()
{
   // Override base class members
   m_strWindowTitle         = _T("Terra Fighter");
   m_bUseDepthBuffer        = TRUE;
   m_dwMinDepthBits    = 16;
   m_dwMinStencilBits  = 4;
   m_bShowCursorWhenFullscreen = TRUE;

   m_pFont                  = new CD3DFont( _T("Arial"), 15, 0 );
   m_pInput                 = new CInput();

   m_dwCreationWidth   = 640;
   m_dwCreationHeight  = 480;

   m_pDSListener         = NULL;

   m_AppState            = APPSTATE_STARTSPLASH;

   m_Level = MAX_LEVELS;

   m_bCreatedObj = FALSE;

   m_bWindowed = FALSE;
   m_bDisplayStats = FALSE;
   m_bFirstPerson = FALSE;
   m_bDisplayHUD = TRUE;

   m_bCanContinueGame = FALSE;
   m_pMenuedlg = NULL;
   m_pChat = NULL;

   m_Weapons = NULL;
   m_nWeapons = 0;

   m_Level = NULL;
   m_pbLevelDone = NULL;
   m_SpriteData = NULL;
   m_PlanetObjectData = NULL;
   m_MaxPlanetObjectData = 0;
   m_MaxMaps = 0;
   m_MaxSpriteData = 0;
   m_PlayerShipType = 0;
   m_StartMap = 0;
   m_CurrentMap = 0;

   m_bDisplayShadows = TRUE;
   m_b3DSound = TRUE;
   m_bLOD = TRUE;
   m_bMulTex = TRUE;
   m_bWaterEff = TRUE;

   DXUtil::ReadBoolRegKey("bLOD",&m_bLOD,TRUE);
   DXUtil::ReadBoolRegKey("bDisplayShadows",&m_bDisplayShadows,TRUE);
   DXUtil::ReadBoolRegKey("bMulTex",&m_bMulTex,TRUE);
   DXUtil::ReadBoolRegKey("bWaterEff",&m_bWaterEff,TRUE);
   DXUtil::ReadBoolRegKey("b3DSound",&m_b3DSound,TRUE);

   DXUtil::ReadBoolRegKey("b3DSound",&m_b3DSound,TRUE);


   TCHAR PlayerName[MAX_PATH];
   DXUtil::ReadStringRegKey("Player Name",PlayerName,MAX_PATH,"Player 1");
   m_PlayerName = PlayerName;


   m_pNetConnect = NULL;
   m_bMultiPlayer = FALSE;

   m_pPlanet = NULL;

   m_bUseArcBall = FALSE;
}




//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CTerraFighterApp::OneTimeSceneInit()
{
   DWORD Adapter = -1;
   DXUtil::ReadIntRegKey(TEXT("Adaptor"), &Adapter,-1);
   if(Adapter != -1)
   {
      m_dwAdapter = Adapter;
      DWORD dwDevice;
      DXUtil::ReadIntRegKey(TEXT("Device"), &dwDevice,0);
      m_Adapters[m_dwAdapter].dwCurrentDevice = dwDevice;
      DXUtil::ReadBoolRegKey(TEXT("bWindowed"), &m_bWindowed, TRUE);
      m_Adapters[m_dwAdapter].devices[dwDevice].bWindowed = m_bWindowed;


      DXUtil::ReadIntRegKey(TEXT("CurrentMode"), &m_Adapters[m_dwAdapter].devices[dwDevice].dwCurrentMode,0);

      DWORD temp;
      DXUtil::ReadIntRegKey(TEXT("MultiSampleTypeWindowed"), &temp,0);
      m_Adapters[m_dwAdapter].devices[dwDevice].MultiSampleTypeWindowed = (D3DMULTISAMPLE_TYPE)temp;
      DXUtil::ReadIntRegKey(TEXT("MultiSampleTypeFullscreen"), &temp,0);
      m_Adapters[m_dwAdapter].devices[dwDevice].MultiSampleTypeFullscreen = (D3DMULTISAMPLE_TYPE)temp;
   }

   try
   {
      m_Zip.Open("TerraFighter.dat");
   }
   catch(...)
   {
      MessageBox(NULL,"Unable To Open File TerraFighter.dat","ERROR",0);
      return E_FAIL;
   }
   m_AppState = APPSTATE_STARTSPLASH;

   m_pInput->CreateDevice(m_hWnd);


   m_SoundManager.Initialize( m_hWnd, DSSCL_PRIORITY, 2, 22050, 16 );

   // Get the 3D listener, so we can control its params
   m_SoundManager.Get3DListenerInterface( &m_pDSListener );
   srand(timeGetTime());

   if(!LoadSpriteData())
      return E_FAIL;

   if(!LoadLevelData())
      return E_FAIL;

   if(!LoadWeaponData())
      return E_FAIL;

   if(!LoadPlanetObjectData())
      return E_FAIL;

   return S_OK;
}


HRESULT CTerraFighterApp::MoveLights()
{
   m_pPlanet->UpdateLights(m_bDisplayShadows,&m_Camera);
   return S_OK;
}

HRESULT CTerraFighterApp::MoveCamera()
{
   D3DXVECTOR3 vEyePt;
   D3DXVECTOR3 vLookatPt;

   CSprite* pPlayer = m_pPlanet->GetPlayerObj();
   if(m_bFirstPerson)
   {
      vEyePt      = pPlayer->GetPos();
      vLookatPt   = pPlayer->GetDirectionVector(1.0f) + vEyePt;
   }
   else
   {
      vEyePt      = pPlayer->GetCameraPos();
      vLookatPt   = pPlayer->GetPos();
   }
   D3DXVECTOR3 vUpVec      = pPlayer->GetUpVector();

   m_Camera.SetViewParams( vEyePt, vLookatPt, vUpVec);
   m_Clipper.SetViewMatix(&m_Camera.GetViewMatrix());


   sys_verify_hr_return_hr(m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_Camera.GetViewMatrix()));

   return S_OK;
}

HRESULT CTerraFighterApp::MoveSprites()
{
   BOOL bPlayerSucsess = FALSE;

   GetNewMessages();

   GameState_e GameState;

   m_pPlanet->MoveSprites(GameState,
      m_bMultiPlayer,
      m_fTime,
      m_fElapsedTime, 
      m_pInput ,
      m_pd3dDevice,
      m_Weapons,
      m_nWeapons,
      m_pNetConnect,
      m_bWaterEff,
      m_PlayerShipType,
      m_SpriteData,
      &m_SoundManager,
      m_pChat,
      m_PlanetObjectData);



   switch(GameState)
   {
   case GAME_WIN:
      {
         if(m_AppState != APPSTATE_ENDACTIVE)
         {
            m_AppState = APPSTATE_STARTENDACTIVE;
         }
         break;
      }
   case GAME_LOSE:
      {
         if(m_AppState != APPSTATE_ENDACTIVE)
         {
            m_AppState = APPSTATE_STARTENDACTIVE;

            // this will restart the levels
            m_Level = MAX_LEVELS;
         }
         break;
      }
   }




   CSprite* pPlayer = m_pPlanet->GetPlayerObj();
   m_Target.Update(pPlayer,m_Weapons);

   if(m_bDisplayHUD)
   {
      pPlayer->UpdateHud(m_pd3dDevice,&m_Camera,&m_2dDraw);
      m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );
   }

   return S_OK;
}

#define FADE_TIME 2.0f

//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CTerraFighterApp::FrameMove()
{
   HRESULT hr;
   sys_verify_hr_return_hr( m_pInput->Update());

   if(m_pInput->IsKeyDown(DIK_S) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
   {
      m_bDisplayStats = TRUE;
   }
   else if(m_pInput->IsKeyDown(DIK_D) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
   {
      m_bDisplayStats = FALSE;
   }

   if(m_pInput->IsKeyDown(DIK_F) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
   {
      m_bFirstPerson = TRUE;
   }
   else if(m_pInput->IsKeyDown(DIK_R) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
   {
      m_bFirstPerson = FALSE;
   }

   if(m_pInput->IsKeyDown(DIK_H) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
   {
      m_bDisplayHUD = TRUE;
   }
   else if(m_pInput->IsKeyDown(DIK_Y) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
   {
      m_bDisplayHUD = FALSE;
   }

   if(m_AppState == APPSTATE_ACTIVE)
   {
      if(m_pInput->IsKeyDown(DIK_C) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
      {
         m_WinMgr.AddChild(m_pChat);
      }
      else if(m_pInput->IsKeyDown(DIK_V) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
      {
         m_WinMgr.RemoveChild(m_pChat);
      }
   }

   if(m_pInput->IsKeyDown(DIK_W) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
   {
      m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
      m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
   }
   else if(m_pInput->IsKeyDown(DIK_Q) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
   {
      m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
      m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
   }


   if(m_pInput->IsKeyDown(DIK_K) && (m_pInput->IsKeyDown(DIK_LALT) || m_pInput->IsKeyDown(DIK_RALT)))
   {
      static int shot = 0;
      CString ScreenFile;
      ScreenFile.Format("Shot%d.bmp",shot);
      shot++;

      TakeScreenShot(ScreenFile);
   }

   switch( m_AppState )
   {
   case APPSTATE_STARTSPLASH:
      {
         m_FadeValue = 255.0f;
         m_bFadeIn = TRUE;
         m_AppState = APPSTATE_DISPLAYSPLASH;
         break;
      }

   case APPSTATE_STARTSPLASH2:
      {
         m_FadeValue = 255.0f;
         m_bFadeIn = TRUE;
         m_AppState = APPSTATE_DISPLAYSPLASH2;
         break;
      }
   case APPSTATE_DISPLAYSPLASH:
      {
         FLOAT inc = (m_fElapsedTime/FADE_TIME)*255.0f;
         if(m_bFadeIn)
         {
            m_FadeValue-= inc;
            if(m_FadeValue<0.0f)
            {
               m_FadeValue = 0.0f;
               m_bFadeIn = FALSE;
            }
         }
         else
         {
            m_FadeValue+= inc;
            if(m_FadeValue>255.0f)
            {
               m_AppState = APPSTATE_STARTSPLASH2;
            }
         }

         break;
      }

   case APPSTATE_DISPLAYSPLASH2:
      {
         FLOAT inc = (m_fElapsedTime/FADE_TIME)*255.0f;
         if(m_bFadeIn)
         {
            m_FadeValue-= inc;
            if(m_FadeValue<0.0f)
            {
               m_FadeValue = 0.0f;
               m_AppState = APPSTATE_START_MENU;
            }
         }
         break;
      }
   case APPSTATE_START_MENU:
      {
         if(!m_pMenuedlg)
         {
            m_pMenuedlg = new CMenueDlg();
            m_pMenuedlg->Create(&m_2dDraw,CPoint(200,300));
         }

         if(!m_pChat)
         {
            m_pChat = new CChatDlg();
            m_pChat->Create(&m_2dDraw,CPoint(0,10));
         }
         m_pInput->ClearButtonPressBuffer();

         m_WinMgr.AddChild(m_pMenuedlg);
         m_WinMgr.RemoveChild(m_pChat);
         m_AppState = APPSTATE_MENU;

         if(m_bCanContinueGame)
         {
            MoveCamera();
            MoveLights();
         }

         m_pMenuedlg->m_resume.EnableWin(m_bCanContinueGame);

         break;
      }
   case APPSTATE_MENU:
      {
         BOOL bFoundInput = FALSE;
         while(!m_pInput->IsButtonPressEmpty() && !bFoundInput)
         {
            int Id = m_pInput->RemoveButtonPress();
            switch(Id)
            {
            case IDC_MULTI_PLAYER:
               {
                  if(m_pNetConnect)
                  {
                     GAMEMSG_GENERIC msg;
                     msg.nType = GAME_MSGID_PLAYER_EXIT;
                     m_pNetConnect->SendMessageToAll(sizeof(GAMEMSG_GENERIC),(BYTE*)&msg);

                     m_pNetConnect->Shutdown();
                     delete m_pNetConnect;
                     m_pNetConnect = NULL;
                  }


                  m_pNetConnect = new CNetworkConnection;
                  if(!m_pNetConnect->Init((HINSTANCE)GetModuleHandle(NULL),m_strWindowTitle,&g_guidApp,TERRAFIGHTER_PORT,m_pInput,this,&m_2dDraw))
                     break;
                  m_pNetConnect->SetWnd(m_hWnd);
                  m_pChat->SetNetworkConnection(m_pNetConnect);
                  m_bMultiPlayer = TRUE;
               }
            case IDC_NEW:
               {
                  if(Id==IDC_NEW)
                  {
                     if(m_pNetConnect)
                     {
                        GAMEMSG_GENERIC msg;
                        msg.nType = GAME_MSGID_PLAYER_EXIT;
                        m_pNetConnect->SendMessageToAll(sizeof(GAMEMSG_GENERIC),(BYTE*)&msg);

                        m_pNetConnect->Shutdown();
                        delete m_pNetConnect;
                        m_pNetConnect = NULL;
                        m_pChat->SetNetworkConnection(NULL);
                     }
                     m_bMultiPlayer = FALSE;
                  }
                  m_Level = MAX_LEVELS;

                  if(m_pNetConnect && m_pNetConnect->IsHostPlayer() || !m_pNetConnect)
                  {
                     CSelectLevelDlg SelectLevelDlg(m_Levels,m_MaxMaps);
                     SelectLevelDlg.Create(&m_2dDraw,CPoint(200,100));

                     int ret = SelectLevelDlg.DoModal(m_pInput,this,GetWindowMgr(),m_pMenuedlg);

                     if(ret == IDOK)
                     {                       
                        m_StartMap = SelectLevelDlg.m_SelMap;

                        m_bUseArcBall = TRUE;
                        CSelectShipDlg ShipDlg(&m_ArcBall,m_SpriteData,m_MaxSpriteData,m_pNetConnect != NULL);
                        ShipDlg.m_ShipType = m_PlayerShipType;
                        ShipDlg.Create(&m_2dDraw,CPoint(160,120));
                        ShipDlg.m_PlayerName.SetText(m_PlayerName);
                        int ret = ShipDlg.DoModal(m_pInput,this,GetWindowMgr(),m_pMenuedlg);
                        if(ret == IDOK)
                        {
                           m_PlayerName = ShipDlg.m_PlayerName.GetText();
                           DXUtil::WriteStringRegKey(TEXT("Player Name"), (char*)((LPCSTR)m_PlayerName) );
                           m_AppState = APPSTATE_STARTLEVELSCREEN;
                           m_PlayerShipType = ShipDlg.m_ShipType;
                           bFoundInput = TRUE;
                        }
                        m_bUseArcBall = FALSE;
                     }
                  }
                  else
                  {
                     m_bUseArcBall = TRUE;
                     CSelectShipDlg ShipDlg(&m_ArcBall,m_SpriteData,m_MaxSpriteData,TRUE);
                     ShipDlg.m_ShipType = m_PlayerShipType;
                     ShipDlg.Create(&m_2dDraw,CPoint(160,120));
                     ShipDlg.m_PlayerName.SetText(m_PlayerName);
                     int ret = ShipDlg.DoModal(m_pInput,this,GetWindowMgr(),m_pMenuedlg);
                     if(ret == IDOK)
                     {
                        DXUtil::WriteStringRegKey(TEXT("Player Name"), (char*)((LPCSTR)m_PlayerName) );
                        m_AppState = APPSTATE_STARTLEVELSCREEN;
                        m_PlayerShipType = ShipDlg.m_ShipType;
                        bFoundInput = TRUE;
                     }
                     m_bUseArcBall = FALSE;
                  }
               }

               break;
            case IDC_QUIT:
               {
                  if(m_pNetConnect)
                  {
                     GAMEMSG_GENERIC msg;
                     msg.nType = GAME_MSGID_PLAYER_EXIT;
                     m_pNetConnect->SendMessageToAll(sizeof(GAMEMSG_GENERIC),(BYTE*)&msg);

                     m_pNetConnect->Shutdown();
                     delete m_pNetConnect;
                     m_pNetConnect = NULL;
                  }
                  return E_FAIL;
               }
            case IDC_RESUME:
               if(m_bCanContinueGame)
               {
                  bFoundInput = TRUE;
                  m_AppState = APPSTATE_ACTIVE;
                  m_WinMgr.AddChild(m_pChat);
               }
               break;
            case IDC_OPTIONS:
               {
                  COptionsDlg OptionsDlg;

                  OptionsDlg.Create(&m_2dDraw,CPoint(200,100));
                  OptionsDlg.m_CheckShadows.SetChecked(m_bDisplayShadows);
                  OptionsDlg.m_CheckLOD.SetChecked(m_bLOD);
                  OptionsDlg.m_CheckMultiTex.SetChecked(m_bMulTex);
                  OptionsDlg.m_CheckWater.SetChecked(m_bWaterEff);
                  OptionsDlg.m_Check3DSound.SetChecked(m_b3DSound);

                  int ret = OptionsDlg.DoModal(m_pInput,this,GetWindowMgr(),m_pMenuedlg);

                  if(ret == IDOK)
                  {
                     m_bDisplayShadows = OptionsDlg.m_CheckShadows.IsChecked();
                     m_bLOD = OptionsDlg.m_CheckLOD.IsChecked();
                     m_bMulTex = OptionsDlg.m_CheckMultiTex.IsChecked();
                     m_bWaterEff = OptionsDlg.m_CheckWater.IsChecked();
                     m_b3DSound = OptionsDlg.m_Check3DSound.IsChecked();

                     DXUtil::WriteBoolRegKey("bLOD",m_bLOD);
                     DXUtil::WriteBoolRegKey("bDisplayShadows",m_bDisplayShadows);
                     DXUtil::WriteBoolRegKey("bMulTex",m_bMulTex);
                     DXUtil::WriteBoolRegKey("bWaterEff",m_bWaterEff);
                     DXUtil::WriteBoolRegKey("b3DSound",m_b3DSound);
                  }
                  else if(ret == IDC_DISPLAY)
                  {
                     if( m_bActive && m_bReady )
                     {
                        Pause(TRUE);

                        if( FAILED( hr = UserSelectNewDevice() ) )
                           return E_FAIL;

                        Pause(FALSE);
                     }
                  }
               }
            }
         }

         if(bFoundInput)
         {
            m_WinMgr.RemoveChild(m_pMenuedlg);
            SAFE_DELETE(m_pMenuedlg);
         }
         break;
      }
   case APPSTATE_STARTLEVELSCREEN:
      {
         m_FadeValue = 255.0f;
         m_AppState = APPSTATE_DISPLAYLEVELSCREEN;
         NextLevel();
         MoveSprites();
         MoveCamera();
         MoveLights();
         m_bCanContinueGame = TRUE;

         // reset the time after loading
         m_fTime        = DXUtil::Timer( DXUtil::TIMER_GETAPPTIME );
         m_fElapsedTime = DXUtil::Timer( DXUtil::TIMER_GETELAPSEDTIME );

         m_WinMgr.AddChild(m_pChat);
         m_pChat->Reset();
         break;
      }
   case APPSTATE_DISPLAYLEVELSCREEN:
      {
         FLOAT inc = (m_fElapsedTime/FADE_TIME)*255.0f;
         m_FadeValue-= inc;
         if(m_FadeValue<0.0f)
         {
            m_AppState = APPSTATE_ACTIVE;
         }
         break;
      }

   case APPSTATE_STARTENDACTIVE:
      {
         m_FadeValue = 0.0f;
         m_AppState = APPSTATE_ENDACTIVE;
         m_WinMgr.RemoveChild(m_pChat);
         break;
      }
   case APPSTATE_ENDACTIVE:
      {
         FLOAT inc = (m_fElapsedTime/FADE_TIME)*255.0f;
         m_FadeValue+= inc;

         MoveSprites();

         if(m_FadeValue>255.0f)
         {
            if(m_Level != MAX_LEVELS)
               m_AppState = APPSTATE_STARTLEVELSCREEN;
            else
            {
               m_AppState = APPSTATE_STARTSPLASH2;
               m_bCanContinueGame = FALSE;
            }
         }

         break;
      }

   case APPSTATE_ACTIVE:
      {
         if(m_pInput->IsKeyDown(DIK_ESCAPE))
         {
            m_AppState = APPSTATE_START_MENU;
         }

         MoveSprites();
         MoveCamera();
         MoveLights();

         // set up the sound
         if(m_pDSListener && m_b3DSound)
         {
            CSprite* pPlayer = m_pPlanet->GetPlayerObj();
            m_pDSListener->SetPosition(pPlayer->GetPos().x,
               pPlayer->GetPos().y,
               pPlayer->GetPos().z,
               DS3D_DEFERRED );

            D3DXVECTOR3 vDVec = pPlayer->GetDirectionVector(1.0f);
            D3DXVECTOR3 vUpVec      = pPlayer->GetUpVector();
            m_pDSListener->SetOrientation(vDVec.x,vDVec.y,vDVec.z, 
               vUpVec.x, vUpVec.y, vUpVec.z,
               DS3D_DEFERRED);

            m_pDSListener->CommitDeferredSettings();
         }
      }
      break;
   }

   if( m_bActive && m_bReady && !m_bWindowed )
   {
      SetCursor( NULL );
      m_pd3dDevice->ShowCursor( m_bShowCursorWhenFullscreen );
   }


   return S_OK;
}

HRESULT CTerraFighterApp::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                        D3DFORMAT Format )
{
   // Make sure device supports point lights
   if( (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING ) ||
      (dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING ) )
   {
      if(pCaps->MaxTextureWidth>=640 && 
         pCaps->MaxTextureHeight>=512 && 
         pCaps->MaxTextureBlendStages>=2 &&
         pCaps->MaxPrimitiveCount>=65535 &&
         pCaps->MaxVertexIndex>=65535)
         return S_OK;
   }
   return E_FAIL;
}

//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CTerraFighterApp::FinalCleanup()
{
   SAFE_DELETE(m_pMenuedlg);
   SAFE_DELETE( m_pFont );
   SAFE_DELETE( m_pInput );

   SAFE_DELETE( m_pPlanet );


   //CleanupDirectPlay();

   CoUninitialize();

   //WriteSettings();

   return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CTerraFighterApp::DeleteDeviceObjects()
{
   m_pFont->DeleteDeviceObjects();
   m_Target.DeleteDeviceObjects();

   if(m_pPlanet)
      m_pPlanet->DeleteDeviceObjects();

   for(DWORD i=0;i<m_MaxSpriteData;i++)
   {
      m_SpriteData[i].DeleteDeviceObjects();
   }

   MeshCash::deleteDeviceObjects();

   m_2dDraw.DeleteDeviceObjects();
   m_SplashBitmap.DeleteDeviceObjects();
   m_SplashBitmap2.DeleteDeviceObjects();


   return S_OK;
}
//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT CTerraFighterApp::InvalidateDeviceObjects()
{
   m_pFont->InvalidateDeviceObjects();
   m_2dDraw.InvalidateDeviceObjects();
   return S_OK;
}

HRESULT CTerraFighterApp::Render()
{
   switch( m_AppState )
   {
   case APPSTATE_DISPLAYSPLASH:
      {
         m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
            0xff000000, 1.0f, 0L );


         if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
         {
            m_2dDraw.Start(m_pd3dDevice);
            m_SplashBitmap.Draw(m_pd3dDevice,0,0,NULL);
            m_2dDraw.DrawBlendRect(CRect(CPoint(0,0),CSize(m_d3dpp.BackBufferWidth,m_d3dpp.BackBufferHeight)),D3DCOLOR_ARGB((DWORD)m_FadeValue,0,0,0));
            m_2dDraw.End();
            m_pd3dDevice->EndScene();
         }
      }
      break;

   case APPSTATE_DISPLAYSPLASH2:
      {
         m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
            0xff000000, 1.0f, 0L );


         if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
         {
            m_2dDraw.Start(m_pd3dDevice);
            m_SplashBitmap2.Draw(m_pd3dDevice,0,0,NULL);
            m_2dDraw.DrawBlendRect(CRect(CPoint(0,0),CSize(m_d3dpp.BackBufferWidth,m_d3dpp.BackBufferHeight)),D3DCOLOR_ARGB((DWORD)m_FadeValue,0,0,0));
            m_2dDraw.End();

            m_pd3dDevice->EndScene();
         }
      }
      break;

   case APPSTATE_DISPLAYLEVELSCREEN:
      {
         m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
            0xff000000, 1.0f, 0L );

         // Begin the scene
         if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
         {
            RenderScene();

            m_pd3dDevice->SetTexture( 0, NULL );
            m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
            m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      FALSE );
            m_2dDraw.DrawBlendRect(CRect(CPoint(0,0),CSize(m_d3dpp.BackBufferWidth,m_d3dpp.BackBufferHeight)),D3DCOLOR_ARGB((DWORD)m_FadeValue,0,0,0));

            CString str;
            str.Format(m_Levels[m_CurrentMap].m_Name);
            m_pFont->DrawText(m_pd3dDevice, (m_d3dpp.BackBufferWidth/2) - 20, (m_d3dpp.BackBufferHeight/2) - 5, D3DCOLOR_ARGB(255,255,0,0), (LPCTSTR)str );

            m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      TRUE );
            m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

            m_pd3dDevice->EndScene();
         }
      }
      break;
   case APPSTATE_ENDACTIVE:
      {
         m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
            0xff000000, 1.0f, 0L );
         // Begin the scene
         if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
         {
            RenderScene();

            m_pd3dDevice->SetTexture( 0, NULL );
            m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
            m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      FALSE );
            m_2dDraw.DrawBlendRect(CRect(CPoint(0,0),CSize(m_d3dpp.BackBufferWidth,m_d3dpp.BackBufferHeight)),D3DCOLOR_ARGB((DWORD)m_FadeValue,0,0,0));
            m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      TRUE );
            m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );


            m_pd3dDevice->EndScene();
         }
      }
      break;

   case APPSTATE_MENU:
      {
         m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
            0xff000000, 1.0f, 0L );


         if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
         {
            if(m_bCanContinueGame)
               RenderScene();

            m_2dDraw.Start(m_pd3dDevice);
            if(m_bCanContinueGame)
               m_2dDraw.DrawBlendRect(CRect(CPoint(0,0),CSize(m_d3dpp.BackBufferWidth,m_d3dpp.BackBufferHeight)),D3DCOLOR_ARGB((DWORD)128,0,0,0));
            else
               m_SplashBitmap2.Draw(m_pd3dDevice,0,0,NULL);
            m_2dDraw.End();
            m_WinMgr.Display(m_pd3dDevice,&m_2dDraw);

            m_pd3dDevice->EndScene();
         }
         break;
      }
   case APPSTATE_ACTIVE:
      {
         // Clear the viewport, zbuffer, and stencil buffer
         m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
            D3DCOLOR_XRGB(0,255,0)//m_BGColour
            , 1.0f, 0L );
         // Begin the scene
         if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
         {
            RenderScene();
            m_pd3dDevice->EndScene();
         }
      }
      break;
   }    

   return S_OK;
}

HRESULT CTerraFighterApp::RestoreDeviceObjects()
{
   D3DVIEWPORT9 vp;
   vp.X=0;
   vp.Y=0;
   vp.Width=m_d3dpp.BackBufferWidth;
   vp.Height=m_d3dpp.BackBufferHeight;
   vp.MinZ = 0.0;
   vp.MaxZ = 1.0;
   m_pd3dDevice->SetViewport(&vp);
   m_2dDraw.SetScreenSize(m_d3dpp.BackBufferHeight,m_d3dpp.BackBufferWidth);

   m_ArcBall.SetWindow(m_d3dpp.BackBufferWidth,m_d3dpp.BackBufferHeight);

   m_SplashBitmap.Init("splash.jpg",m_d3dpp.BackBufferWidth,m_d3dpp.BackBufferHeight,m_pd3dDevice,&m_Zip);
   m_SplashBitmap2.Init("splash2.jpg",m_d3dpp.BackBufferWidth,m_d3dpp.BackBufferHeight,m_pd3dDevice,&m_Zip);


   m_pFont->RestoreDeviceObjects(m_pd3dDevice);

   if(m_bCreatedObj && m_pPlanet)
   {
      m_pPlanet->RestoreDeviceObjects(m_pd3dDevice,&m_d3dsdBackBuffer);
   }

   for(UINT i=0;i<m_MaxSpriteData;i++)
   {
      m_SpriteData[i].RestoreDeviceObjects(m_pd3dDevice);
   }

   MeshCash::restoreDeviceObjects(m_pd3dDevice);



   //if(m_pWater)
   //m_pWater->RestoreDeviceObjects(m_pd3dDevice);
   m_2dDraw.RestoreDeviceObjects(m_pd3dDevice);

   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
   m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
   m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

   m_pd3dDevice->SetSamplerState( 1, D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
   m_pd3dDevice->SetSamplerState( 1, D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);
   m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
   m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
   m_pd3dDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
   m_pd3dDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );


   m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
   m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, FALSE );
   m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

   D3DXMATRIX matWorld;

   D3DXMatrixIdentity( &matWorld );
   m_pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld );

   FLOAT fAspect = m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;
   D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4.0f, fAspect, 1.0f, FOV_BACK_DISTACNE+128.0f );
   m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );

   m_Clipper.SetupFOVClipPlanes(D3DX_PI*0.4f/* 11.0f/24.0f*/, fAspect, 1.0f, FOV_BACK_DISTACNE);

   // Turn on fog
   FLOAT fFogStart =  10.0f;
   FLOAT fFogEnd   =  FOV_BACK_DISTACNE;
   m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      TRUE );
   m_pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_LINEAR );
   m_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
   m_pd3dDevice->SetRenderState( D3DRS_RANGEFOGENABLE, FALSE );
   m_pd3dDevice->SetRenderState( D3DRS_FOGSTART,       FtoDW(fFogStart) );
   m_pd3dDevice->SetRenderState( D3DRS_FOGEND,         FtoDW(fFogEnd) );

   InitLights();

   m_Target.Create(m_pd3dDevice);

   return S_OK;
}

HRESULT CTerraFighterApp::InitDeviceObjects()
{
   // Initialize the font's internal texture

   // Load an object to cast the shadow  

   return S_OK;
}

HRESULT CTerraFighterApp::RenderScene()
{
   m_dwTR = 0L;
   D3DXMATRIX matWorld;
   D3DXMatrixIdentity( &matWorld );
   m_pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld );


   if(m_pPlanet)
      m_pPlanet->Render(m_pd3dDevice,&m_Camera,
      m_dwTR,&m_Clipper, m_bMulTex, m_bLOD, m_bDisplayShadows, m_bWaterEff, m_bFirstPerson);

   if(m_bDisplayHUD)
   {

      m_Target.Render(m_pd3dDevice,&m_Camera);

      // render hud
      m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
      m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      FALSE );
      //m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
      //m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
      m_pd3dDevice->SetTexture( 0, NULL );


      // display the HUD

      m_2dDraw.Start(m_pd3dDevice);
      m_2dDraw.End();
      m_pPlanet->GetPlayerObj()->DisplayStats(m_pd3dDevice,m_pFont,&m_2dDraw,m_pPlanet->GetTerrain(),m_pPlanet->GetSpriteList(),m_pPlanet->GetObjectList(),m_bFirstPerson,&m_Camera);


      if(m_bDisplayStats)
      {

         static FLOAT fLastTime = 0.0f;
         FLOAT fTime = DXUtil::Timer( DXUtil::TIMER_GETABSOLUTETIME );
         if( fTime - fLastTime > 1.0f )
         {
            //if(m_pTrees && m_pWater && m_pTerrain)
            //    m_ObjStats.Format("Trees %d tpf Water %d tpf Terrain %d tpf",m_pTrees->GetTri(),m_pWater->GetTri(),m_pTerrain->GetTri());
            // else if(m_pTerrain)
            m_ObjStats.Format("Terrain %d tpf",m_pPlanet->GetTerrain()->GetTri());

         }
         m_pFont->DrawText(m_pd3dDevice, 2,  2, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );
         m_pFont->DrawText(m_pd3dDevice, 2,  18, D3DCOLOR_ARGB(255,255,255,0), m_strDeviceStats );
         m_pFont->DrawText(m_pd3dDevice, 2,  50, D3DCOLOR_ARGB(255,255,255,0), m_ObjStats );
      }

      m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      TRUE );
      m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

      m_WinMgr.Display(m_pd3dDevice,&m_2dDraw);
   }
   // End the scene.


   return S_OK;
}

BOOL CTerraFighterApp::LookForMSG()
{
   BOOL bGotMsg = FALSE;
   while( WM_QUIT != m_msg.message && !bGotMsg )
   {
      // Use PeekMessage() if the app is active, so we can use idle time to
      // render the scene. Else, use GetMessage() to avoid eating CPU time.
      if( m_bActive )
         bGotMsg = PeekMessage( &m_msg, NULL, 0U, 0U, PM_REMOVE );
      else
         bGotMsg = GetMessage( &m_msg, NULL, 0U, 0U );

      if( bGotMsg )
      {
         // Translate and dispatch the message
         TranslateMessage( &m_msg );
         DispatchMessage( &m_msg );
      }
   }
   return bGotMsg;
}



LRESULT CTerraFighterApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   if(m_bUseArcBall)
      m_ArcBall.HandleMouseMessages(hWnd,uMsg,wParam,lParam);

   switch(uMsg)
   {
   case WM_MOUSEMOVE:
      {
         //g_curs->setpos(LOWORD(lParam),HIWORD(lParam));
         CPoint p(LOWORD(lParam),HIWORD(lParam));
         if(!m_WinMgr.MouseOver(p))
         {
            if((MK_LBUTTON&wParam)!=0)
            {
               m_pInput->AddButtonPress(-1);
            }
         }
         break;
      }
   case WM_LBUTTONDOWN:
      {
         CPoint p(LOWORD(lParam),HIWORD(lParam));
         int Id;
         if(m_WinMgr.MouseDown(p,Id))
            m_pInput->AddButtonPress(Id);
         break;
      }
   case WM_LBUTTONUP:
      {
         CPoint p(LOWORD(lParam),HIWORD(lParam));
         int Id;
         if(m_WinMgr.MouseUp(p,Id))
            m_pInput->AddButtonPress(Id);
         break;
      }
   case WM_KEYDOWN:
      {
         if(m_AppState == APPSTATE_DISPLAYSPLASH || m_AppState ==APPSTATE_DISPLAYSPLASH2)
            m_AppState =APPSTATE_START_MENU;
      }
      break;
   case WM_CHAR:
      {
         char c = wParam;
         m_WinMgr.CharPressed(c,lParam);
      }
      break;
   }
   return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}

HRESULT CTerraFighterApp::NextLevel()
{
   m_Level++;


   if(MAX_LEVELS<m_Level)
   {
      m_Level = 0;
      for(UINT i=0;i<m_MaxMaps;i++)
         m_pbLevelDone[i] = FALSE;

      m_CurrentMap = m_StartMap;
   }
   else
      m_CurrentMap = rand() % m_MaxMaps;


   DisplayProgress("Deleting Old Objects",0.0f);
   // Invalidate and Delete the Deviece Objects
   //sys_msg("DEBUG: Start Deleting Old Objects\n");

   if(m_pPlanet)
   {
      m_pPlanet->DeleteDeviceObjects();
      SAFE_DELETE(m_pPlanet);
   }

   // Create the New Objects
   CNetworkQueueData* pData = NULL;
   GAMEMSG_WORLD_STAT* pWorldData = NULL;

   if(m_pNetConnect && !m_pNetConnect->IsHostPlayer())
   {
      GAMEMSG_GENERIC msg;
      msg.nType = GAME_MSGID_REQUEST_WORLD;
      m_pNetConnect->SendMessageToAll(sizeof(GAMEMSG_GENERIC),(BYTE*)&msg);

      BOOL bFoundMessage = FALSE;
      while(WaitForSingleObject(m_pNetConnect->m_MessageEvent,30000) == WAIT_OBJECT_0)
      {
         while(TRUE)
         {
            pData = m_pNetConnect->GetNextMessage();
            if(pData==NULL)
               break;

            if( pData->dwSize >= sizeof(GAMEMSG_GENERIC) )
            {

               GAMEMSG_GENERIC* pMsg = (GAMEMSG_GENERIC*) pData->pMsg;

               if(pMsg->nType == GAME_MSGID_WORLD_STAT)
               {
                  bFoundMessage = TRUE;
                  break;
               }
            }

            m_pNetConnect->ReturnBuffer(pData);
            SAFE_DELETE( pData );

         }
         if(bFoundMessage)
            break;
      }

      if(bFoundMessage == FALSE)
         return E_FAIL;

      pWorldData = (GAMEMSG_WORLD_STAT*) pData->pMsg;

      m_CurrentMap = pWorldData->MapNumber;
   }


   DisplayProgress("Creating Map",10.0f);

   m_pPlanet = new CPlanet();    
   sys_msg("SetupMap\n");

   while(m_pbLevelDone[m_CurrentMap])
      m_CurrentMap = rand() % m_MaxMaps;

   m_pbLevelDone[m_CurrentMap] = TRUE;
   m_Levels[m_CurrentMap].Reset();
   sys_msg(m_Levels[m_CurrentMap].m_MapFile);

   m_pPlanet->SetLevelData(&m_Levels[m_CurrentMap]);

   DisplayProgress("Creating Map",25.0f);
   sys_msg(" Create Map\n");

   m_pPlanet->CreateMap(m_pd3dDevice);
   m_pPlanet->RestoreDeviceObjects(m_pd3dDevice,&m_d3dsdBackBuffer);

   DisplayProgress("Creating Sky Box",50.0f);
   m_pPlanet->CreateSkyBox(m_pd3dDevice);

   if(m_Level == 0)
   {
      m_nEnemyShips = 1;
      m_nGunTurrets = 1;
   }
   else
   {
      if(m_nGunTurrets > m_nEnemyShips)
         m_nEnemyShips++;
      else
         m_nGunTurrets++;
   }

   DisplayProgress("Create Sprites",60.0f);


   m_pPlanet->CreateSprites(m_pd3dDevice,&m_SoundManager,m_pNetConnect,
      m_PlayerShipType,
      m_SpriteData,m_MaxSpriteData,pWorldData,m_PlayerName);


   DisplayProgress("Create Water",70.0f);
   m_pPlanet->CreateWater(m_pd3dDevice);

   DisplayProgress("Create Objects",85.0f);
   m_pPlanet->CreateObjects(m_pd3dDevice,&m_SoundManager,m_PlanetObjectData,m_MaxPlanetObjectData);


   if(pData && m_pNetConnect)
   {
      m_pNetConnect->ReturnBuffer(pData);
      SAFE_DELETE( pData );
   }

   m_bCreatedObj = TRUE;

   DisplayProgress("Initializing Lights",99.0f);
   InitLights();

   return S_OK;
}

void CTerraFighterApp::TakeScreenShot(const char* file_name)
{
   //now we copy the front buffer into our surface
   LPDIRECT3DSURFACE9 pBackBuffer;
   m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

   //now write our screen shot to a bitmap file
   //the last 2 params are NULL because we want the entire front buffer and no palette
   D3DXSaveSurfaceToFile(file_name, D3DXIFF_BMP, pBackBuffer, NULL, NULL);
   pBackBuffer->Release();
}

BOOL CTerraFighterApp::LoadWeaponData()
{
   try
   {
      MSXML2::IXMLDOMDocumentPtr docPtr;
      // create the xml document
      if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
         return FALSE;

      if(!m_Zip.ExtractFileTo("Weapons.xml","temp.xml"))
         return FALSE;

      _variant_t varXml("temp.xml");
      _variant_t varOut((bool)TRUE);

      varOut = docPtr->load(varXml); // load the xml document

      DeleteFile("temp.xml");

      if((bool)varOut == TRUE)
      {
         MSXML2::IXMLDOMNodeListPtr RowListPtr = docPtr->documentElement->selectNodes("Weapon");
         int ElementCount = RowListPtr->length;

         if(ElementCount > 0) // if there are any rows
         {
            if(m_Weapons)
               delete [] m_Weapons;

            m_nWeapons = ElementCount;
            m_Weapons = new CWeapon[m_nWeapons];

            for(UINT i=0;i<m_nWeapons;i++) // for each row
            {
               // get the row
               MSXML2::IXMLDOMNodePtr RowItem = RowListPtr->item[i];
               m_Weapons[i].Create(&m_SoundManager,RowItem);
            }
         }
         else
            return FALSE;
      }
   }
   catch(...)
   {
      sys_msg("Exception occurred LoadWeapons");
      return FALSE;
   }
   return TRUE;
}



BOOL CTerraFighterApp::LoadSpriteData()
{
   try
   {
      MSXML2::IXMLDOMDocumentPtr docPtr;
      // create the xml document
      if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
         return FALSE;

      if(!m_Zip.ExtractFileTo("Sprites.xml","temp.xml"))
      {
         sys_msg("Unable To open Sprites.xml\n");
         return FALSE;
      }

      _variant_t varXml("temp.xml");
      _variant_t varOut((bool)TRUE);

      varOut = docPtr->load(varXml); // load the xml document

      DeleteFile("temp.xml");

      if((bool)varOut == TRUE)
      {

         int ElementCount;
         MSXML2::IXMLDOMElementPtr docElementPtr = docPtr->documentElement;
         MSXML2::IXMLDOMNodeListPtr RowListPtr = docElementPtr->selectNodes("Sprite");
         ElementCount = RowListPtr->length;

         if(ElementCount > 0) // if there are any rows
         {
            if(m_SpriteData)
               delete [] m_SpriteData;

            m_MaxSpriteData = ElementCount;
            m_SpriteData = new CSpriteDataEx[m_MaxSpriteData];

            for(UINT i=0;i<m_MaxSpriteData;i++) // for each row
            {
               // get the row
               MSXML2::IXMLDOMNodePtr RowItem = RowListPtr->item[i];

               m_SpriteData[i].Load(RowItem);

            }
         }
         else
            return FALSE;
      }
   }
   catch(...)
   {
      sys_msg("Exception occurred LoadShips");
      return FALSE;
   }
   return TRUE;
}


BOOL CTerraFighterApp::LoadLevelData()
{
   try
   {
      MSXML2::IXMLDOMDocumentPtr docPtr;
      // create the xml document
      if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
         return FALSE;

      if(!m_Zip.ExtractFileTo("Levels.xml","temp.xml"))
         return FALSE;

      _variant_t varXml("temp.xml");
      _variant_t varOut((bool)TRUE);

      varOut = docPtr->load(varXml); // load the xml document

      DeleteFile("temp.xml");

      if((bool)varOut == TRUE)
      {
         int ElementCount;
         MSXML2::IXMLDOMElementPtr docElementPtr = docPtr->documentElement;
         MSXML2::IXMLDOMNodeListPtr RowListPtr = docElementPtr->selectNodes("Level");
         ElementCount = RowListPtr->length;

         if(ElementCount > 0) // if there are any rows
         {
            if(m_Levels)
               delete [] m_Levels;

            if(m_pbLevelDone)
               delete [] m_pbLevelDone;

            m_MaxMaps = ElementCount;
            m_Levels = new CLevelData[m_MaxMaps];
            m_pbLevelDone = new BOOL[m_MaxMaps];

            for(UINT i=0;i<m_MaxMaps;i++) // for each row
            {
               m_pbLevelDone[i] = FALSE;
               // get the row
               MSXML2::IXMLDOMNodePtr RowItem = RowListPtr->item[i];
               m_Levels[i].Load(RowItem);

            }
         }
         else
            return FALSE;
      }
   }
   catch(...)
   {
      sys_msg("Exception occurred LoadShips");
      return FALSE;
   }
   return TRUE;
}

void CTerraFighterApp::InitLights()
{
   if(m_pPlanet)
      m_pPlanet->InitLights(m_pd3dDevice);
   else
   {
      D3DXVECTOR3 SunPos = D3DXVECTOR3(0.0f,-2000.0f,0.0f);
      D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);
      //lightpos.y = -lightpos.y;

      if(SunPos.y < 0)
         SunPos.y = -SunPos.y;
      //else


      D3DXVECTOR3 LightVector = orgin - SunPos;

      D3DLIGHT9 light;
      D3DUtil::InitLight( light, D3DLIGHT_DIRECTIONAL , LightVector.x, LightVector.y, LightVector.z );
      light.Attenuation0 = 0.9f;
      light.Attenuation1 = 0.0f;
      m_pd3dDevice->SetLight( 0, &light );
      m_pd3dDevice->LightEnable( 0, TRUE );
      m_pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0x00303030);
      m_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR, 0x0);
   }
}


#define BAR_SIZE 200.0f
#define BAR_HIGHT 20.0f
#define COLOUR_TO D3DCOLOR_XRGB(0,0,0)

#define TOPBAR_FLAG GR_TR|GR_TL
#define BOTTOMBAR_FLAG GR_BR|GR_BL

void CTerraFighterApp::DisplayProgress(CString str, FLOAT pcentDone)
{
   m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
      0xff000000, 1.0f, 0L );


   if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
   {
      m_2dDraw.Start(m_pd3dDevice);

      CPoint Pos((m_2dDraw.GetScreenWidth()- 200) / 2,(m_2dDraw.GetScreenHight()-(BAR_HIGHT+15))/2);

      CPoint TextPoint(0,0);
      TextPoint.Offset(Pos);
      m_2dDraw.OutTextc(0,D3DCOLOR_XRGB(255,255,255),TextPoint.x,TextPoint.y,TRUE,"Loading.... %s %2.0f%%",str,pcentDone);


      FLOAT BarLen = pcentDone/100.0f * BAR_SIZE;
      // Draw Shield Bar
      CRect rec(CPoint(0, 15),CSize(BarLen,BAR_HIGHT/2.0f));
      rec.OffsetRect(Pos);
      m_2dDraw.DrawGradientRect(rec,D3DCOLOR_XRGB(0,0,255),COLOUR_TO,TOPBAR_FLAG);

      CRect rec6(CPoint(0, 15+(BAR_HIGHT/2.0f)),CSize(BarLen,BAR_HIGHT/2.0f));
      rec6.OffsetRect(Pos);
      m_2dDraw.DrawGradientRect(rec6,D3DCOLOR_XRGB(0,0,255),COLOUR_TO,BOTTOMBAR_FLAG);

      // Draw Rect around Shield Bar
      CRect rec1(CPoint(0, 15),CSize(BAR_SIZE,BAR_HIGHT));
      rec1.OffsetRect(Pos);
      m_2dDraw.DrawRect(rec1,D3DCOLOR_XRGB(0,0,255));


      m_2dDraw.End();
      m_pd3dDevice->EndScene();
   }
   m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

//-----------------------------------------------------------------------------
// Name: UserSelectNewDevice()
// Desc: Displays a dialog so the user can select a new adapter, device, or
//       display mode, and then recreates the 3D environment if needed
//-----------------------------------------------------------------------------
HRESULT CTerraFighterApp::UserSelectNewDevice()
{
   HRESULT hr;

   CSelectDeviceDlg* pDlg = new CSelectDeviceDlg();

   pDlg->Create(&m_2dDraw,CPoint(10,10),this);

   // Prompt the user to change the mode
   if( IDOK != pDlg->DoModal(m_pInput,this,GetWindowMgr(),m_pMenuedlg) )
   {
      delete pDlg;
      return S_OK;
   }

   delete pDlg;

   // Get access to the newly selected adapter, device, and mode
   DWORD dwDevice;
   dwDevice  = m_Adapters[m_dwAdapter].dwCurrentDevice;
   m_bWindowed = m_Adapters[m_dwAdapter].devices[dwDevice].bWindowed;

   DXUtil::WriteIntRegKey(TEXT("Adaptor"), m_dwAdapter );
   DXUtil::WriteIntRegKey(TEXT("Device"), dwDevice);
   DXUtil::WriteBoolRegKey(TEXT("bWindowed"), m_Adapters[m_dwAdapter].devices[dwDevice].bWindowed );
   DXUtil::WriteIntRegKey(TEXT("MultiSampleTypeWindowed"), m_Adapters[m_dwAdapter].devices[dwDevice].MultiSampleTypeWindowed);
   DXUtil::WriteIntRegKey(TEXT("MultiSampleTypeFullscreen"), m_Adapters[m_dwAdapter].devices[dwDevice].MultiSampleTypeFullscreen );
   DXUtil::WriteIntRegKey(TEXT("CurrentMode"), m_Adapters[m_dwAdapter].devices[dwDevice].dwCurrentMode);

   // Release all scene objects that will be re-created for the new device
   InvalidateDeviceObjects();
   DeleteDeviceObjects();

   // Release display objects, so a new device can be created
   m_pd3dDevice = NULL;
   //if(  val > 0L )
   //    return DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

   // Inform the display class of the change. It will internally
   // re-create valid surfaces, a d3ddevice, etc.
   if( FAILED( hr = Initialize3DEnvironment() ) )
      return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

   // If the app is paused, trigger the rendering of the current frame
   if( FALSE == m_bFrameMoving )
   {
      m_bSingleStep = TRUE;
      DXUtil::Timer( DXUtil::TIMER_START );
      DXUtil::Timer( DXUtil::TIMER_STOP );
   }

   return S_OK;
}


BOOL CTerraFighterApp::GetNewMessages()
{
   if(m_pNetConnect)
   {
      CNetworkQueueData* pData;
      while((pData = m_pNetConnect->GetNextMessage()) != NULL)
      {
         if(pData->dpnidPlayer != m_pNetConnect->m_dpnidLocalPlayer)
         {
            if( pData->dwSize >= sizeof(GAMEMSG_GENERIC) ) // ignore bad messages
            {
               GAMEMSG_GENERIC* pMsg = (GAMEMSG_GENERIC*) pData->pMsg;

               switch(pMsg->nType)
               {
               case GAME_MSGID_CHAT:
                  {
                     SpriteList* pSpriteList = m_pPlanet->GetSpriteList();
                     BOOL bFound = FALSE;
                     sys_msg("Chat Message\n");
                     for(UINT i=0;i< pSpriteList->size(); i++)
                     {
                        if((*pSpriteList)[i]->GetID() == pData->dpnidPlayer)
                        {
                           GAMEMSG_CHAT* pChatMsg = (GAMEMSG_CHAT*)pMsg;
                           CString Msg;
                           Msg.Format("%s : %s",(*pSpriteList)[i]->GetName(),pChatMsg->strChatString);
                           m_pChat->AddString(Msg,(*pSpriteList)[i]->GetTeamColour());
                           bFound = TRUE;
                           break;
                        }
                     }
                     if(!bFound)
                     {
                        // somthing happed to sprite
                        GAMEMSG_CHAT* pChatMsg = (GAMEMSG_CHAT*)pMsg;
                        CString Msg;
                        Msg.Format("???? : %s",pChatMsg->strChatString);
                        m_pChat->AddString(Msg,D3DCOLOR_XRGB(255,0,0));
                     }
                  }

                  break;
               case GAME_MSGID_PLAYER_UPDATE:
                  {
                     SpriteList* pSpriteList = m_pPlanet->GetSpriteList();
                     BOOL bFound = FALSE;
                     sys_msg("Update Message\n");
                     for(UINT i=0;i< pSpriteList->size(); i++)
                     {
                        if((*pSpriteList)[i]->GetID() == pData->dpnidPlayer)
                        {
                           GAMEMSG_PLAYER_UPDATE* pNewPlayerMsg = (GAMEMSG_PLAYER_UPDATE*)pMsg;
                           (*pSpriteList)[i]->SetPlayerMsg(pNewPlayerMsg);
                           bFound = TRUE;
                           break;
                        }
                     }

                     if(!bFound)
                     {
                        sys_msg("Update Error");
                        if(m_pNetConnect->IsHostPlayer())
                        {
                           GAMEMSG_GENERIC msg;
                           msg.nType = GAME_MSGID_REQUEST_FULL_PLAYER_UPDATE;
                           m_pNetConnect->SendMessageToAll(sizeof(GAMEMSG_GENERIC),(BYTE*)&msg);
                        }
                        else
                        {
                           GAMEMSG_GENERIC msg;
                           msg.nType = GAME_MSGID_REQUEST_WORLD;
                           m_pNetConnect->SendMessageToAll(sizeof(GAMEMSG_GENERIC),(BYTE*)&msg);
                        }


                     }
                  }
                  break;
               case GAME_MSGID_WORLD_STAT:
                  {
                     sys_msg("World Update\n");
                     GAMEMSG_WORLD_STAT* pWorldData = (GAMEMSG_WORLD_STAT*)pMsg;
                     SpriteList* pSpriteList = m_pPlanet->GetSpriteList();
                     for(UINT i=0;i< pWorldData->m_NumbPlayers; i++)
                     {
                        BOOL bFound = FALSE;
                        for(UINT j=0;j< pSpriteList->size(); j++)
                        {
                           if((*pSpriteList)[j]->GetID() == pWorldData->m_Players[i].m_dpID)
                           {
                              (*pSpriteList)[j]->SetPlayerMsg(&pWorldData->m_Players[i]);
                              bFound = TRUE;
                              break;
                           }
                        }

                        if(!bFound)
                        {
                           WORD ship = pWorldData->m_Players[i].m_PlayerShipType;
                           CSprite* pSprite;
                           if(pWorldData->m_Players[i].m_bBot)
                              pSprite = new CNpc();
                           else
                              pSprite = new CSprite();
                           D3DXVECTOR3 pos = pWorldData->m_Players[i].m_Pos;
                           pSprite->SetID(pWorldData->m_Players[i].m_dpID);
                           pSprite->Create(&m_SpriteData[ship],pos,&m_SoundManager,1,ship,0);
                           pSprite->SetName(pWorldData->m_Players[i].m_Name);
                           pSpriteList->push_back(pSprite);
                           pSprite->RestoreDeviceObjects(m_pd3dDevice);
                        }
                     }
                  }
                  break;
               case GAME_MSGID_SHOT_FIRED:
                  {
                     sys_msg("Shot Fired\n");
                     SpriteList* pSpriteList = m_pPlanet->GetSpriteList();
                     for(UINT i=0;i< pSpriteList->size(); i++)
                     {
                        if((*pSpriteList)[i]->GetID() == pData->dpnidPlayer)
                        {
                           GAMEMSG_SHOT_FIRED* pShotMsg = (GAMEMSG_SHOT_FIRED*)pMsg;
                           m_pPlanet->FireShot(pShotMsg,i);
                           break;
                        }
                     }
                  }
                  break;
               case GAME_MSGID_REQUEST_WORLD:
                  {
                     sys_msg("Request World Udate\n");
                     if(m_pNetConnect->IsHostPlayer())
                     {
                        GAMEMSG_WORLD_STAT NewMsg;
                        NewMsg.nType = GAME_MSGID_WORLD_STAT;
                        NewMsg.MapNumber = m_CurrentMap;
                        SpriteList* pSpriteList = m_pPlanet->GetSpriteList();
                        NewMsg.m_NumbPlayers = pSpriteList->size();
                        for(UINT i=0;i< NewMsg.m_NumbPlayers; i++)
                        {
                           (*pSpriteList)[i]->GetFullPlayerMsg(&NewMsg.m_Players[i]);
                        }
                        m_pNetConnect->SendMessageToOne(pData->dpnidPlayer,sizeof(GAMEMSG_WORLD_STAT),(BYTE*)&NewMsg);
                     }
                  }
                  break;
               case GAME_MSGID_REQUEST_FULL_PLAYER_UPDATE:
                  {
                     sys_msg("Request Udate\n");
                     if(m_pNetConnect->IsHostPlayer())
                     {
                        GAMEMSG_FULL_PLAYER_UPDATE NewMsg;
                        NewMsg.nType = GAME_MSGID_FULL_PLAYER_UPDATE;
                        m_pPlanet->GetPlayerObj()->GetFullPlayerMsg(&NewMsg);
                        m_pNetConnect->SendMessageToOne(pData->dpnidPlayer,sizeof(GAMEMSG_FULL_PLAYER_UPDATE),(BYTE*)&NewMsg);
                     }
                  }
               case GAME_MSGID_FULL_PLAYER_UPDATE:
                  {
                     sys_msg("Full Udate\n");
                     GAMEMSG_FULL_PLAYER_UPDATE* pNewPlayerMsg = (GAMEMSG_FULL_PLAYER_UPDATE*)pMsg;
                     SpriteList* pSpriteList = m_pPlanet->GetSpriteList();
                     if(pNewPlayerMsg->m_dpID != m_pNetConnect->m_dpnidLocalPlayer)
                     {
                        BOOL bFound = FALSE;
                        for(UINT j=0;j< pSpriteList->size(); j++)
                        {
                           if((*pSpriteList)[j]->GetID() == pNewPlayerMsg->m_dpID)
                           {
                              (*pSpriteList)[j]->SetPlayerMsg(pNewPlayerMsg);
                              (*pSpriteList)[j]->SetName(pNewPlayerMsg->m_Name);
                              bFound = TRUE;
                              break;
                           }
                        }
                        if(!bFound)
                        {
                           WORD ship = pNewPlayerMsg->m_PlayerShipType;
                           CSprite* pSprite = new CVehicle();
                           D3DXVECTOR3 pos = pNewPlayerMsg->m_Pos;
                           pSprite->SetID(pNewPlayerMsg->m_dpID);
                           pSprite->Create(&m_SpriteData[ship],pos,&m_SoundManager,1,ship,0);
                           pSprite->SetName(pNewPlayerMsg->m_Name);
                           m_pPlanet->GetSpriteList()->push_back(pSprite);
                           pSprite->RestoreDeviceObjects(m_pd3dDevice);
                        }
                     }
                  }
                  break;
               case GAME_MSGID_PLAYER_EXIT:
                  {
                     sys_msg("GAME_MSGID_PLAYER_EXIT");
                     if(pData->dpnidPlayer != m_pNetConnect->m_dpnidLocalPlayer)
                     {
                        SpriteList* pSpriteList = m_pPlanet->GetSpriteList();
                        for(UINT i=0;i< pSpriteList->size(); i++)
                        {   
                           if((*pSpriteList)[i]->GetID() == pData->dpnidPlayer)
                           {
                              (*pSpriteList)[i]->SetSpriteState(SPRITE_DEAD);
                              break;
                           }
                        }
                     }
                  }
                  break;
               case GAME_MSGID_ADDFRAG_COUNT:
                  {
                     SpriteList* pSpriteList = m_pPlanet->GetSpriteList();
                     BOOL bFound = FALSE;
                     for(UINT i=0;i< pSpriteList->size(); i++)
                     {
                        if((*pSpriteList)[i]->GetID() == pData->dpnidPlayer)
                        {
                           CString Msg;
                           Msg.Format("Destroyed Player : %s",(*pSpriteList)[i]->GetName());
                           m_pChat->AddString(Msg,(*pSpriteList)[i]->GetTeamColour());
                           bFound = TRUE;
                           break;
                        }
                     }
                     (*pSpriteList)[PLAYER_SPRITE]->AddFrag();

                  }
                  break;
               default:
                  sys_msg1("Unknown Message %d",pMsg->nType);
               }
            }
         }
         m_pNetConnect->ReturnBuffer(pData);
         SAFE_DELETE( pData );

      }
   }
   return TRUE;
}


BOOL CTerraFighterApp::LoadPlanetObjectData()
{
   try
   {
      MSXML2::IXMLDOMDocumentPtr docPtr;
      // create the xml document
      if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
         return FALSE;

      if(!m_Zip.ExtractFileTo("Objects.xml","temp.xml"))
         return FALSE;

      _variant_t varXml("temp.xml");
      _variant_t varOut((bool)TRUE);

      varOut = docPtr->load(varXml); // load the xml document

      DeleteFile("temp.xml");

      if((bool)varOut == TRUE)
      {

         int ElementCount;
         MSXML2::IXMLDOMElementPtr docElementPtr = docPtr->documentElement;

         MSXML2::IXMLDOMNodeListPtr RowListPtr = docElementPtr->selectNodes("Object");
         ElementCount = RowListPtr->length;

         if(ElementCount > 0) // if there are any rows
         {
            if(m_PlanetObjectData)
               delete [] m_PlanetObjectData;

            m_MaxPlanetObjectData = ElementCount;
            m_PlanetObjectData = new CPlanetObjectData[m_MaxPlanetObjectData];

            for(UINT i=0;i<m_MaxPlanetObjectData;i++) // for each row
            {
               // get the row
               MSXML2::IXMLDOMNodePtr RowItem = RowListPtr->item[i];
               m_PlanetObjectData[i].Load(RowItem);
            }
         }
         else
            return FALSE;
      }
   }
   catch(...)
   {
      sys_msg("Exception occurred LoadPlanetObjectData");
      return FALSE;
   }
   return TRUE;
}

