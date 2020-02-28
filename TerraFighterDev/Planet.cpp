// Planet.cpp: implementation of the CPlanet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Planet.h"
#include "ShipObject.h"
#include "npc.h"
#include "Station.h"
#include "terrafighterapp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WORLD_RAD 400.0f
#define WORLD_ATM_THICK 50.0f



CPlanet::CPlanet()
{
   m_pSkyBox      = NULL;
   m_pTerrain = NULL;
   m_pWater = NULL;
   m_pPlanetObjects = NULL;
   m_pParticleSystem = NULL;

   m_BGColour = 0xff0000ff;
   m_pLevel = NULL;
   m_LevelTime = 0.0f;
   m_PlayerName = "";

}

CPlanet::~CPlanet()
{
   SAFE_DELETE( m_pParticleSystem );
   SAFE_DELETE( m_pSkyBox );

   SAFE_DELETE(m_pTerrain);
   SAFE_DELETE(m_pWater);
   SAFE_DELETE(m_pPlanetObjects);

   for(UINT i=0;i< m_SpriteList.size(); i++)
   {
      SAFE_DELETE(m_SpriteList[i]);
   }
   m_SpriteList.clear();
}

BOOL CPlanet::Render(LPDIRECT3DDEVICE9 pd3dDevice,CD3DCamera* pCamera,
                     DWORD &dwTR,CFOVClipper* pClipper, BOOL bMulTex, BOOL bLOD, BOOL bDisplayShadows, BOOL bWaterEff,BOOL bFirstPerson)
{

   // Render the Skybox
   if(m_bShowSkyBox && m_pSkyBox)
   {
      m_pSkyBox->Render(pd3dDevice,pCamera->GetViewMatrix(),dwTR);
   }

   pd3dDevice->SetRenderState( D3DRS_FOGSTART,       FtoDW(m_pLevel->m_fFogStart) );
   pd3dDevice->SetRenderState( D3DRS_FOGEND,         FtoDW(m_pLevel->m_fFogEnd) );


   pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ); // we do not need lights for the terain
   m_pTerrain->Render(pd3dDevice,pClipper,dwTR,pCamera->GetEyePt(),bMulTex,bLOD);

   if(m_pWater)
      m_pWater->Render(pd3dDevice,pClipper,dwTR,bWaterEff);

   if(!bDisplayShadows) // only use the lights if the shadows are off
      pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
   pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
   pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );


   pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
   pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

   for(UINT i=0;i< m_SpriteList.size(); i++)
   {
      m_SpriteList[i]->Render(pd3dDevice,pClipper);
   }

   if(m_pPlanetObjects)
      m_pPlanetObjects->Render(pd3dDevice,pClipper,dwTR,pCamera,bDisplayShadows);

   pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );

   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

   if(bDisplayShadows)
   {
      CDisplayObject::StartRenderShadow(pd3dDevice);
      for(i=0;i< m_SpriteList.size(); i++)
      {
         if(!(i == PLAYER_SPRITE && bFirstPerson))
         {
            m_SpriteList[i]->RenderShadow(pd3dDevice,pClipper);
         }
      }

      if(m_pPlanetObjects)
         m_pPlanetObjects->RenderShadow(pd3dDevice,pClipper);

      CDisplayObject::EndRenderShadow(pd3dDevice);
      DrawShadow(pd3dDevice);
   }


   // Draw particles

   for(i=0;i< m_SpriteList.size(); i++)
   {
      m_SpriteList[i]->RenderClear(pd3dDevice,pClipper,pCamera);
   }

   m_pParticleSystem->Render(pd3dDevice,pClipper,pCamera);

   m_LensFlare.Render(pd3dDevice,pClipper);
   return TRUE;
}

//-----------------------------------------------------------------------------
// Name: DrawShadow()
// Desc: Draws a big gray polygon over scene according to the mask in the
//       stencil buffer. (Any pixel with stencil==1 is in the shadow.)
//-----------------------------------------------------------------------------
void CPlanet::DrawShadow(LPDIRECT3DDEVICE9 pd3dDevice)
{
   // Set renderstates (disable z-buffering, enable stencil, disable fog, and
   // turn on alphablending)
   pd3dDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );
   pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    TRUE );
   //m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
   pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

   pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
   pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
   pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
   pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
   pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
   pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

   // Only write where stencil val >= 1 (count indicates # of shadows that
   // overlap that pixel)
   pd3dDevice->SetRenderState( D3DRS_STENCILREF,  0x1 );
   pd3dDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL );
   pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );

   // Draw a big, gray square
   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_SHADOWVERTEX );
   pd3dDevice->SetStreamSource( 0, m_pBigSquareVB, 0, sizeof(SHADOWVERTEX) );
   pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

   // Restore render states
   pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
   pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
   //m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        TRUE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}



BOOL CPlanet::MoveSprites(GameState_e &GameState,
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
                          CPlanetObjectData* pPlanetObjectData)
{
   BOOL bFoundAlive = FALSE;

   GameState = GAME_NOCHANGE;
   m_LevelTime+=fElapsedTime;

   BOOL bFoundEnemy = FALSE;
   WORD PlayerTeam = GetPlayerObj()->GetTeamNo();


   if(m_pWater)
      m_pWater->AnimateWater(fElapsedTime,bWaterEff);


   std::vector<CGameAction> Actions;

   for(UINT i=0;i< m_SpriteList.size(); i++)
   {
      // move each sprite
      BOOL bPlayer = m_SpriteList[i] == GetPlayerObj();
      sys_verify_hr_return_hr(m_SpriteList[i]->FrameMove(fTime,fElapsedTime,pInput,m_pTerrain,pd3dDevice,m_pParticleSystem,&m_SpriteList,bPlayer,m_pWater,Weapons,nWeapons,pNetConnect,m_pPlanetObjects,pChatDlg,Actions));

      if(!bPlayer)
      {
         // clean up all the dead ones
         if(m_SpriteList[i]->IsAlive())
         {
            if(PlayerTeam != m_SpriteList[i]->GetTeamNo())
               bFoundEnemy = TRUE;
            bFoundAlive = TRUE;
         }
         else
         {
            // delete unused sprites
            m_SpriteList[i]->DeleteDeviceObjects();

            if(m_SpriteList[i] == GetPlayerObj()->GetCurrentTarget())
               GetPlayerObj()->ClearTarget();

            delete m_SpriteList[i];
            m_SpriteList.erase(m_SpriteList.begin() + i);
            i--;
         }
      }
   }

   if(pNetConnect)
   {
      GAMEMSG_PLAYER_UPDATE msg;
      msg.nType = GAME_MSGID_PLAYER_UPDATE;
      GetPlayerObj()->GetPlayerMsg(&msg);
      pNetConnect->SendMessageToAll(sizeof(GAMEMSG_PLAYER_UPDATE),(BYTE*)&msg);
   }

   m_pParticleSystem->Update(fElapsedTime,m_pTerrain,m_pWater);


   // check for end game conditons and update events

   if(bMultiPlayer)
   {
      if(!m_SpriteList[PLAYER_SPRITE]->IsAlive())
      {

         // respan in multiplayer
         m_SpriteList[PLAYER_SPRITE]->DeleteDeviceObjects();

         DWORD FragCount = m_SpriteList[PLAYER_SPRITE]->GetFrags();
         SAFE_DELETE(m_SpriteList[PLAYER_SPRITE]);

         D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0);
         FLOAT fTheta  = 2.0f*D3DX_PI*(FLOAT)rand()/RAND_MAX;
         FLOAT fRadius = (FLOAT)m_pTerrain->GetXSize()/2.0f * (FLOAT)rand()/RAND_MAX;
         pos.x  = fRadius * sinf(fTheta);
         pos.z  = fRadius * cosf(fTheta);
         pos.y  = m_pTerrain->GetHightAtPosR( pos.x, pos.z );
         pos.y += 1 + (20 * rand()/RAND_MAX);

         CPlayer* pSprite = new CPlayer();
         pSprite->SetID(pNetConnect->m_dpnidLocalPlayer);
         pSprite->Create(&pShipData[PlayerShipType],pos,pSoundManager,0,PlayerShipType,0);
         pSprite->SetName(m_PlayerName);
         pSprite->SetFrags(FragCount);
         m_SpriteList[PLAYER_SPRITE] = pSprite; 
         m_SpriteList[PLAYER_SPRITE]->RestoreDeviceObjects(pd3dDevice);
      }
   }
   else
   {
      for(UINT i=0;i<m_pLevel->m_Events.size();i++)
      {

         if(m_pLevel->m_Events[i]->m_ResponsName == "CURRENT_PLAYER")
            m_pLevel->m_Events[i]->m_ResponsName = m_SpriteList[PLAYER_SPRITE]->GetName();

         if(m_pLevel->m_Events[i]->m_ObjectName == "CURRENT_PLAYER")
            m_pLevel->m_Events[i]->m_ObjectName = m_SpriteList[PLAYER_SPRITE]->GetName();

         if(m_pLevel->m_Events[i]->Check(m_LevelTime,Actions,bFoundEnemy))
         {
            // event has occured do somthing
            UINT size = m_pLevel->m_Events[i]->m_CreateSprites.size();
            for(UINT j=0;j<size;j++)
            {
               D3DXVECTOR3 pos = m_pLevel->m_Events[i]->m_CreateSprites[j]->m_pos;

               pos.y  = max(pos.y,m_pTerrain->GetHightAtPosR( pos.x, pos.z ));

               UINT ship = m_pLevel->m_Events[i]->m_CreateSprites[j]->m_Type;

               CSprite* pSprite = NULL;
               if(pShipData[ship].m_IsBuilding)
               {
                  pSprite = new CStation();
               }
               else
               {
                  pSprite = new CNpc();
               }

               pSprite->Create(&pShipData[ship],pos,pSoundManager,m_pLevel->m_Events[i]->m_CreateSprites[j]->m_Team,ship,m_pLevel->m_Events[i]->m_CreateSprites[j]->m_CurrentTexture);
               pSprite->SetName(m_pLevel->m_Events[i]->m_CreateSprites[j]->m_Name);
               pSprite->SetAIType(m_pLevel->m_Events[i]->m_CreateSprites[j]->m_AIType);

               CString GoalName = m_pLevel->m_Events[i]->m_CreateSprites[j]->m_GoalName;
               if(GoalName == "CURRENT_PLAYER")
                  GoalName = m_SpriteList[PLAYER_SPRITE]->GetName();
               pSprite->SetGoalName(GoalName);

               m_SpriteList.push_back(pSprite);
               pSprite->RestoreDeviceObjects(pd3dDevice);
            }

            size = m_pLevel->m_Events[i]->m_CreateObjects.size();
            for(j=0;j<size;j++)
            {
               m_pPlanetObjects->AddObject(m_pTerrain,m_pWater,pPlanetObjectData,m_pLevel->m_Events[i]->m_CreateObjects[j]);
            }


            // event has occured do somthing
            size = m_pLevel->m_Events[i]->m_SpriteChange.size();
            for(j=0;j<size;j++)
            {
               BOOL bFoundShip = FALSE;
               CSprite* pSprite = NULL;
               for(UINT k = 0; k<m_SpriteList.size();k++)
               {
                  if(m_SpriteList[k]->GetName() == m_pLevel->m_Events[i]->m_SpriteChange[j]->m_Name)
                  {
                     pSprite = m_SpriteList[k];
                     bFoundShip = TRUE;
                     break;
                  }
               }

               if(bFoundShip)
               {
                  pSprite->SetTeamNo(m_pLevel->m_Events[i]->m_SpriteChange[j]->m_Team);
                  pSprite->SetAIType(m_pLevel->m_Events[i]->m_SpriteChange[j]->m_AIType);

                  CString GoalName = m_pLevel->m_Events[i]->m_SpriteChange[j]->m_GoalName;
                  if(GoalName == "CURRENT_PLAYER")
                     GoalName = m_SpriteList[PLAYER_SPRITE]->GetName();
                  pSprite->SetGoalName(GoalName);
               }
            }

            if(pChatDlg && !m_pLevel->m_Events[i]->m_Message.IsEmpty())
            {
               pChatDlg->AddString(m_pLevel->m_Events[i]->m_Message,D3DCOLOR_XRGB(255,255,0));
            }

            if(m_pLevel->m_Events[i]->m_EventToActivate != _T(""))
            {
               for(UINT k = 0;k<m_pLevel->m_Events.size();k++)
               {
                  if(m_pLevel->m_Events[k]->m_Name == m_pLevel->m_Events[i]->m_EventToActivate)
                  {
                     m_pLevel->m_Events[k]->ActivateEvent(m_LevelTime);
                  }
               }
            }

            GameState = m_pLevel->m_Events[i]->m_GameState;

         }
      }


      if(!GetPlayerObj()->IsAlive())
      {
         GameState = GAME_LOSE;
      } 
   }

   return TRUE;
}

CSprite* CPlanet::GetPlayerObj()
{
   return m_SpriteList[PLAYER_SPRITE];
}

BOOL CPlanet::UpdateLights(BOOL bDisplayShadows,CD3DCamera* pCamera)
{
   if(bDisplayShadows)
   {
      for(UINT i=0;i< m_SpriteList.size(); i++)
      {
         m_SpriteList[i]->BuildShadow(m_SunPos);
      }

      if(m_pPlanetObjects)
         m_pPlanetObjects->BuildShadow(m_SunPos);
   }

   m_LensFlare.Update(m_SunPos,pCamera,m_pTerrain);
   return TRUE;
}

HRESULT CPlanet::DeleteDeviceObjects()
{
   m_LensFlare.DeleteDeviceObjects();

   if(m_pTerrain)
      m_pTerrain->DeleteDeviceObjects();

   if(m_pPlanetObjects)
      m_pPlanetObjects->DeleteDeviceObjects();

   if(m_pWater)
      m_pWater->DeleteDeviceObjects();

   for(UINT i=0;i< m_SpriteList.size(); i++)
   {
      m_SpriteList[i]->DeleteDeviceObjects();
   }

   if(m_pParticleSystem)
      m_pParticleSystem->DeleteDeviceObjects();

   if(m_pSkyBox)
      m_pSkyBox->DeleteDeviceObjects();

   m_pBigSquareVB = NULL;

   return S_OK;
}

HRESULT CPlanet::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice,D3DSURFACE_DESC* d3dsdBackBuffer)
{
   for(UINT i=0;i< m_SpriteList.size(); i++)
   {
      m_SpriteList[i]->RestoreDeviceObjects(pd3dDevice);
   }

   if(m_pParticleSystem)
      m_pParticleSystem->RestoreDeviceObjects(pd3dDevice);


   // Create a big square for rendering the stencilbuffer contents
   if( FAILED( pd3dDevice->CreateVertexBuffer( 4*sizeof(SHADOWVERTEX),
      D3DUSAGE_WRITEONLY, D3DFVF_SHADOWVERTEX,
      D3DPOOL_MANAGED, &m_pBigSquareVB, NULL ) ) )
      return E_FAIL;


   // Set the size of the big square shadow
   SHADOWVERTEX* v;
   FLOAT sx = (FLOAT)d3dsdBackBuffer->Width;
   FLOAT sy = (FLOAT)d3dsdBackBuffer->Height;
   m_pBigSquareVB->Lock( 0, 0, (void**)&v, 0 );
   v[0].p = D3DXVECTOR4(  0, sy, 0.0f, 1.0f );
   v[1].p = D3DXVECTOR4(  0,  0, 0.0f, 1.0f );
   v[2].p = D3DXVECTOR4( sx, sy, 0.0f, 1.0f );
   v[3].p = D3DXVECTOR4( sx,  0, 0.0f, 1.0f );
   v[0].color = 0x7f000000;
   v[1].color = 0x7f000000;
   v[2].color = 0x7f000000;
   v[3].color = 0x7f000000;
   m_pBigSquareVB->Unlock();


   if(m_pSkyBox)
   {
      if( FAILED( m_pSkyBox->Create( pd3dDevice,20,WORLD_RAD,WORLD_RAD+WORLD_ATM_THICK/*800.0f,850.0f*/,1.0f,1.0f, &GetApp()->m_Zip) ) )
         return D3DAPPERR_MEDIANOTFOUND;
   }

   if(m_pPlanetObjects)
      m_pPlanetObjects->Create(pd3dDevice);

   if(m_pWater)
      m_pWater->Create(pd3dDevice,m_pTerrain->GetXSize(),m_pTerrain->GetZSize(),m_pWater->GetWaterLevel(),m_pTerrain,&GetApp()->m_Zip);

   m_LensFlare.Create(pd3dDevice);

   m_pTerrain->Create(pd3dDevice,&GetApp()->m_Zip);

   return S_OK;
}

BOOL CPlanet::SetLevelData(CLevelData* pLevel)
{
   m_pLevel = pLevel;
   m_SunPos = m_pLevel->m_SunPos;
   return TRUE;
}

BOOL CPlanet::CreateMap(LPDIRECT3DDEVICE9 pd3dDevice)
{
   m_pTerrain = new CTerrain();
   m_pTerrain->SetupMap(pd3dDevice,
      m_pLevel->m_MapFile,
      m_pLevel->m_MapHight,
      m_pLevel->m_Textures,
      m_pLevel->m_MapScale,
      &GetApp()->m_Zip);

   m_BGColour = m_pLevel->m_BGColour;
   return TRUE;
}

BOOL CPlanet::CreateSkyBox(LPDIRECT3DDEVICE9 pd3dDevice)
{
   m_bShowSkyBox = m_pLevel->m_bShowSkyBox;

   if(m_bShowSkyBox)
   {
      m_pSkyBox      = new CSkyBox();
      m_pSkyBox->SetTex(m_pLevel->m_SkyBoxFile);

      if( FAILED( m_pSkyBox->Create( pd3dDevice,20,WORLD_RAD,WORLD_RAD+WORLD_ATM_THICK/*800.0f,850.0f*/,1.0f,1.0f,&GetApp()->m_Zip) ) )
         return D3DAPPERR_MEDIANOTFOUND;
   }	
   return TRUE;
}

BOOL CPlanet::CreateWater(LPDIRECT3DDEVICE9 pd3dDevice)
{
   m_pWater = new CWater();

   m_pWater->Init(m_pLevel->m_bWater,m_pLevel->m_GroundTexture,m_pLevel->m_WaterTexture,m_pLevel->m_WaterTexture2);
   m_pWater->Create(pd3dDevice,m_pTerrain->GetXSize(),m_pTerrain->GetZSize(),m_pLevel->m_WaterLevel,m_pTerrain,&GetApp()->m_Zip);

   m_pTerrain->SetWaterLevel(m_pLevel->m_WaterLevel);
   return TRUE;
}

BOOL CPlanet::CreateObjects(LPDIRECT3DDEVICE9 pd3dDevice,CSoundManager* pSoundManager,CPlanetObjectData* pPlanetObjectData,DWORD MaxPlanetObjectData)
{
   m_pParticleSystem = new CShotSystem();
   if( FAILED(m_pParticleSystem->Create(pSoundManager)))
      return E_FAIL;
   m_pParticleSystem->RestoreDeviceObjects(pd3dDevice);

   m_pPlanetObjects = new CPlanetObjects();
   m_pPlanetObjects->CreateObjects(m_pTerrain,m_pWater,pPlanetObjectData,MaxPlanetObjectData,m_pLevel);
   m_pPlanetObjects->Create(pd3dDevice);

   //m_LensFlare.Create(pd3dDevice);
   return TRUE;
}

#define SECINMIN (60)
#define SECINHOUR (3600)
#define SECINDAY (86400.0f) 

void GetSunPos(D3DXVECTOR3* orgin,D3DXVECTOR3* SunPos, FLOAT distance)
{
   time_t ltime;
   tm *today;
   time( &ltime );
   today = localtime( &ltime );

   FLOAT Sec = today->tm_sec + (today->tm_min*SECINMIN) + (today->tm_hour*SECINHOUR);
   FLOAT ZRot = HALFPI - HALFPI/6.0f;//(Sec/SECINDAY * PI2) + PI;

   D3DXMATRIX matRotZ, matSun;
   D3DXMatrixTranslation( &matSun,0, distance, 0 );
   D3DXMatrixRotationZ( &matRotZ, ZRot );
   D3DXMatrixMultiply( &matSun, &matSun, &matRotZ );
   D3DXVECTOR3 result;
   D3DXVec3TransformCoord( SunPos,orgin,&matSun);
}

void CPlanet::InitLights(LPDIRECT3DDEVICE9 pd3dDevice)
{
   D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);	 
   D3DXVECTOR3 LightVector = orgin - m_SunPos;

   D3DLIGHT9 light;
   D3DUtil::InitLight( light, D3DLIGHT_DIRECTIONAL , LightVector.x, LightVector.y, LightVector.z );
   light.Attenuation0 = 0.9f;
   light.Attenuation1 = 0.0f;
   pd3dDevice->SetLight( 0, &light );
   pd3dDevice->LightEnable( 0, TRUE );
   pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0x00303030);
   pd3dDevice->SetRenderState( D3DRS_FOGCOLOR, m_BGColour);
}

void CPlanet::FireShot(GAMEMSG_SHOT_FIRED* pShotMsg,int SpriteIndex)
{
   m_pParticleSystem->EmitNewParticles(
      pShotMsg->dwNumParticlesToEmit,
      pShotMsg->dwEmitColor,
      pShotMsg->dwFadeColor,
      pShotMsg->vPosition,
      pShotMsg->vEmitVel,
      m_SpriteList[SpriteIndex],
      pShotMsg->fPower,
      pShotMsg->fGravity,
      pShotMsg->fSize);
}


BOOL CPlanet::CreateSprites(LPDIRECT3DDEVICE9 pd3dDevice,CSoundManager* pSoundManager,CNetworkConnection *pNetConnect,
                            int PlayerShipType,
                            CSpriteDataEx* pShipData,DWORD MaxShipData,GAMEMSG_WORLD_STAT* pWorldData,CString PlayerName)
{
   m_PlayerName = PlayerName;


   if(pNetConnect)
   {
      {
         D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0);
         FLOAT fTheta  = 2.0f*D3DX_PI*(FLOAT)rand()/RAND_MAX;
         FLOAT fRadius = (FLOAT)m_pTerrain->GetXSize()/2.0f * (FLOAT)rand()/RAND_MAX;
         pos.x  = fRadius * sinf(fTheta);
         pos.z  = fRadius * cosf(fTheta);
         pos.y  = m_pTerrain->GetHightAtPosR( pos.x, pos.z );
         pos.y += 1 + (20 * rand()/RAND_MAX);

         CPlayer* pSprite = new CPlayer();
         pSprite->SetID(pNetConnect->m_dpnidLocalPlayer);
         pSprite->Create(&pShipData[PlayerShipType],pos,pSoundManager,0,PlayerShipType,0);
         pSprite->SetName(m_PlayerName);
         m_SpriteList.push_back(pSprite); 
         if(pNetConnect && !pNetConnect->IsHostPlayer())
         {
            GAMEMSG_FULL_PLAYER_UPDATE msg;
            msg.nType = GAME_MSGID_FULL_PLAYER_UPDATE;
            pSprite->GetFullPlayerMsg(&msg);
            pNetConnect->SendMessageToAll(sizeof(GAMEMSG_FULL_PLAYER_UPDATE),(BYTE*)&msg);
         }
      }

      if(!pNetConnect->IsHostPlayer() && pWorldData)
      {
         for(UINT i=0;i< pWorldData->m_NumbPlayers; i++)
         {
            WORD ship = pWorldData->m_Players[i].m_PlayerShipType;

            CSprite* pSprite;
            if(pWorldData->m_Players[i].m_bBot)
               pSprite = new CNpc();
            else
               pSprite = new CSprite();
            D3DXVECTOR3 pos = pWorldData->m_Players[i].m_Pos;
            pSprite->SetID(pWorldData->m_Players[i].m_dpID);
            pSprite->Create(&pShipData[ship],pos,pSoundManager,1,ship,0);
            m_SpriteList.push_back(pSprite);
         }
      }
   }
   else
   {

      {
         D3DXVECTOR3 pos = m_pLevel->m_StartPos;
         pos.y  = max(pos.y,m_pTerrain->GetHightAtPosR( pos.x, pos.z ));
         CPlayer* pSprite = new CPlayer();
         pSprite->Create(&pShipData[PlayerShipType],pos,pSoundManager,0,PlayerShipType,0);
         pSprite->SetName(m_PlayerName);
         m_SpriteList.push_back(pSprite);
      }

      // create staic sprites

      for(UINT i=0;i<m_pLevel->m_Sprites.size();i++)
      {
         D3DXVECTOR3 pos = m_pLevel->m_Sprites[i]->m_pos;

         pos.y  = max(pos.y,m_pTerrain->GetHightAtPosR( pos.x, pos.z ));

         UINT ship = m_pLevel->m_Sprites[i]->m_Type;

         CSprite* pSprite = NULL;
         if(pShipData[ship].m_IsBuilding)
            pSprite = new CStation();
         else
            pSprite = new CNpc();

         pSprite->Create(
            &pShipData[ship],
            pos,pSoundManager,
            m_pLevel->m_Sprites[i]->m_Team,
            ship,
            m_pLevel->m_Sprites[i]->m_CurrentTexture
            );
         pSprite->SetName(m_pLevel->m_Sprites[i]->m_Name);
         pSprite->SetAIType(m_pLevel->m_Sprites[i]->m_AIType);

         CString GoalName = m_pLevel->m_Sprites[i]->m_GoalName;
         if(GoalName == "CURRENT_PLAYER")
            GoalName = m_SpriteList[PLAYER_SPRITE]->GetName();
         pSprite->SetGoalName(GoalName);
         m_SpriteList.push_back(pSprite);
      }
   }

   for(UINT i=0;i< m_SpriteList.size(); i++)
   {
      m_SpriteList[i]->RestoreDeviceObjects(pd3dDevice);
   }
   return TRUE;
}
