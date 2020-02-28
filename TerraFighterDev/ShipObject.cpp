// DolphinObject.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShipObject.h"
#include "D3DApp.h"
#include "D3DFile.h"
#include "DXUtil.h"
#include "vertextypes.h"
#include "Water.h"
#include "ShotSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define REPEAT_TIME 0.5f

////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayer::CPlayer() : CVehicle()
{
   //m_bRunningLightsOn = FALSE;
   //m_LightTime = 0.0f;

   m_MaxSpeed = 20.0f;
   m_RotationTime = 3.0f;
   m_Acceleration = 1.0f;
   m_Friction = 2.0f;
   m_SetSpeed = 0.0f;
   m_bHeadLight = FALSE;
   m_SelectTime = 0.0f;

   m_fAngleCameraY =  0.0f;
   m_fAngleCameraX = 1.0f;
   m_fCameraDistance = -4.0f;
}

CPlayer::~CPlayer()
{

}

HRESULT CPlayer::FrameMove(	FLOAT fTime,
                           FLOAT fElapsedTime,
                           CInput* pInput, 
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
                           std::vector<CGameAction>& Actions)
{

   if(m_SpriteState == SPRITE_ALIVE)
   {
      // Animation attributes for the ship
      //FLOAT fKickFreq    = 2*fTime;

      // update the players direction

      if(pInput->IsKeyDown(DIK_LALT) || pInput->IsKeyDown(DIK_RALT))
      {
         if(pInput->IsKeyDown(DIK_LEFT))
            m_fAngleCameraY-= fElapsedTime/m_RotationTime * (PI2);
         else if(pInput->IsKeyDown(DIK_RIGHT))
            m_fAngleCameraY+= fElapsedTime/m_RotationTime * (PI2);

         if(pInput->IsKeyDown(DIK_LSHIFT) || pInput->IsKeyDown(DIK_RSHIFT))
         {
            if(pInput->IsKeyDown(DIK_UP))
               m_fCameraDistance+= fElapsedTime;
            else if(pInput->IsKeyDown(DIK_DOWN))
               m_fCameraDistance-= fElapsedTime;
         }
         else
         {
            if(pInput->IsKeyDown(DIK_UP))
               m_fAngleCameraX+= fElapsedTime;
            else if(pInput->IsKeyDown(DIK_DOWN))
               m_fAngleCameraX-= fElapsedTime;
         }

         m_CameraPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
         D3DXMATRIX matCamera,matCameraRotY;
         D3DXMatrixRotationY( &matCameraRotY, m_fAngleCameraY);
         D3DXMatrixTranslation( &matCamera,m_fCameraDistance,m_fAngleCameraX,0.0f);
         D3DXMatrixMultiply(&matCamera,&matCamera,&matCameraRotY);
         D3DXVec3TransformCoord( &m_CameraPos,&m_CameraPos,&matCamera);
      }
      else
      {
         if(pInput->IsKeyDown(DIK_LEFT))
            m_angleY-= fElapsedTime/m_RotationTime * (PI2);
         else if(pInput->IsKeyDown(DIK_RIGHT))
            m_angleY+= fElapsedTime/m_RotationTime * (PI2);

         if(pInput->IsKeyDown(DIK_UP))
            m_angleZ+= fElapsedTime/m_RotationTime * (PI2);
         else if(pInput->IsKeyDown(DIK_DOWN))
            m_angleZ-=fElapsedTime/m_RotationTime * (PI2);
      }

      if(pInput->FoundJoystick())
      {
         FLOAT RotYRate = ((FLOAT)pInput->GetJoyXPos())/1000.0f;
         if(RotYRate > 0.1f || RotYRate < -0.1f)
            m_angleY+= (fElapsedTime/m_RotationTime)*RotYRate * (PI2);

         FLOAT RotZRate = ((FLOAT)pInput->GetJoyYPos())/1000.0f;
         if(RotZRate > 0.1f || RotZRate < -0.1f)
            m_angleZ+= (fElapsedTime/m_RotationTime)*RotZRate * (PI2);

         FLOAT RotXRate = ((FLOAT)pInput->GetJoyZRot())/1000.0f;
         if(RotXRate > 0.1f || RotXRate < -0.1f)
            m_angleX-= (fElapsedTime/m_RotationTime)*RotXRate * (PI2);
      }

      if(pInput->FoundMouse() && pInput->IsMouseButtonDown(1))
      {
         FLOAT RotYRate = ((FLOAT)pInput->GetMouseXPos())/20.0f;
         m_angleY+= (fElapsedTime/m_RotationTime)*RotYRate * (PI2);

         FLOAT RotZRate = ((FLOAT)pInput->GetMouseYPos())/20.0f;
         m_angleZ+= (fElapsedTime/m_RotationTime)*-RotZRate * (PI2);


      }

      if(m_angleY>PI2)
         m_angleY -=PI2;	
      else if(m_angleY<0)
         m_angleY +=PI2;

      if(m_angleZ>PI2)
         m_angleZ -=PI2;	
      else if(m_angleZ<0)
         m_angleZ +=PI2;

      if(m_angleX>PI2)
         m_angleX -=PI2;	
      else if(m_angleX<0)
         m_angleX +=PI2;


      // update the players current target
      if((pInput->IsKeyDown(DIK_GRAVE) || pInput->IsJoyButtonDown(1)) && m_SelectTime > REPEAT_TIME)
      {
         m_pTarget = GetNextTarget(m_pTarget,pSpriteList,pPlanetObjects->GetObjectList());
         m_SelectTime = 0.0f;
      }


      if(pInput->IsKeyDown(DIK_T) && (pInput->IsKeyDown(DIK_LALT) || pInput->IsKeyDown(DIK_RALT)) && m_SelectTime > REPEAT_TIME)
      {
         FLOAT Distance = 256.0f;
         m_pTarget = this->GetClosestSprite(pSpriteList,Distance,FALSE);
         m_SelectTime = 0.0f;
      }


      // update the players current speed

      LONG sliderval = 0;
      if(pInput->IsKeyDown(DIK_HOME) || pInput->IsJoyButtonDown(5))
      {
         m_SetSpeed = m_MaxSpeed;
      }
      else if(pInput->IsKeyDown(DIK_END) || pInput->IsJoyButtonDown(6))
      {
         m_SetSpeed = m_fMinSpeed;
      }
      else if(pInput->IsKeyDown(DIK_SUBTRACT) || pInput->IsKeyDown(DIK_MINUS))
      {
         m_Speed.x -= m_Friction*fElapsedTime;
         m_SetSpeed = m_Speed.x;
      }
      else if(pInput->IsKeyDown(DIK_ADD) || pInput->IsKeyDown(DIK_EQUALS))
      {
         m_Speed.x += m_Acceleration*fElapsedTime;
         m_SetSpeed = m_Speed.x;
      }
      else if(pInput->FoundJoystick() && (sliderval = pInput->GetJoySlider(0)) != 1000)
      {
         sliderval = -sliderval;
         sliderval += 1000;
         FLOAT SetSpeed = ((FLOAT)sliderval)/2000.0f * m_MaxSpeed;

         if(SetSpeed>m_Speed.x)
         {
            m_Speed.x += m_Acceleration*fElapsedTime;
            if(m_Speed.x>SetSpeed)
               m_Speed.x = SetSpeed;
         }
         else if(SetSpeed<m_Speed.x)
         {
            m_Speed.x -= m_Friction*fElapsedTime;
            if(m_Speed.x<SetSpeed)
               m_Speed.x = SetSpeed;
         }

         m_SetSpeed = m_Speed.x;
      }
      else if(pInput->FoundMouse())
      {
         FLOAT val = pInput->GetMouseZPos();

         m_SetSpeed += val/480.0f;

         if(m_MaxSpeed <m_SetSpeed)
            m_SetSpeed = m_MaxSpeed;
         if(0 > m_SetSpeed)
            m_SetSpeed = 0;

         if(m_SetSpeed>m_Speed.x)
         {
            m_Speed.x += m_Acceleration*fElapsedTime;
            if(m_Speed.x>m_SetSpeed)
               m_Speed.x = m_SetSpeed;
         }
         else if(m_SetSpeed<m_Speed.x)
         {
            m_Speed.x -= m_Friction*fElapsedTime;
            if(m_Speed.x<m_SetSpeed)
               m_Speed.x = m_SetSpeed;
         }
      }
   }
   else
   {
      m_Speed.x -= m_Friction*fElapsedTime;
   }

   if(m_Speed.x>m_MaxSpeed)
      m_Speed.x = m_MaxSpeed;

   if(m_fMinSpeed>m_Speed.x)
      m_Speed.x = m_fMinSpeed;

   CSprite::FrameMove(fTime,fElapsedTime,pInput,pFloor,pd3dDevice,pParticleSystem,pSpriteList,bCurrentPlayer,pWater,Weapons,nWeapons,pNetConnect,pPlanetObjects,pChatDlg,Actions);


   if(m_SpriteState == SPRITE_ALIVE)
   {
      // cycle wepons
      if((pInput->IsKeyDown(DIK_TAB) || pInput->IsJoyButtonDown(2))  && m_SelectTime > REPEAT_TIME)
      {
         m_SelectTime = 0.0f;
         m_CurrentWeapon++;
         if(m_CurrentWeapon>=nWeapons)
            m_CurrentWeapon=0;


         pChatDlg->AddString("Selected Weapon : " + Weapons[m_CurrentWeapon].m_Name,D3DCOLOR_XRGB(255,255,0));
      }

      // Update particle system
      if((pInput->IsKeyDown(DIK_LCONTROL) || pInput->IsKeyDown(DIK_RCONTROL) || pInput->IsJoyButtonDown(0) || pInput->IsMouseButtonDown(0)))
      {
         if(Weapons[m_CurrentWeapon].Fire(this,GetCurrentTarget(),m_LastShot,pParticleSystem,bCurrentPlayer,pNetConnect,m_WeaponPowerMod))
            m_LastShot = 0.0f;
      }


      // turn on/off hedlight
      if(pInput->IsKeyDown(DIK_L) && (pInput->IsKeyDown(DIK_LALT) || pInput->IsKeyDown(DIK_RALT)) && m_SelectTime > REPEAT_TIME)
      {
         m_SelectTime = 0.0f;
         m_bHeadLight = !m_bHeadLight;
      }
   }

   // setup headlight
   if(m_bHeadLight)
   {
      D3DLIGHT9 light;
      D3DUtil::InitLight( light, D3DLIGHT_SPOT , GetPos().x, GetPos().y, GetPos().z );
      light.Attenuation0 = 0.9f;
      light.Attenuation1 = 0.0f;

      D3DXVECTOR3 vdir = GetDirectionVector(1.0f);
      light.Direction.x = vdir.x;
      light.Direction.y = vdir.y;
      light.Direction.z = vdir.z;
      light.Theta = HALFPI/4.0f;
      light.Phi = HALFPI/2.0f;
      pd3dDevice->SetLight( 1, &light );
   }
   pd3dDevice->LightEnable( 1, m_bHeadLight );


   m_SelectTime+=fElapsedTime;
   return S_OK;
}

void CPlayer::AddPowerUp(CPowerUpData* pPowerUp,CString Name,CChatDlg* pChatDlg)
{
   CSprite::AddPowerUp(pPowerUp,Name,pChatDlg);
   pChatDlg->AddString("Picked up "+Name,D3DCOLOR_XRGB(255,255,0));
}

void CPlayer::HitWaypoint(CString Name,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg,BOOL bVisable)
{
   CSprite::HitWaypoint(Name,pActions,pChatDlg,bVisable);

   if(bVisable)
   {
      for(UINT i =0; i<m_WaypointList.size();i++)
      {
         if(m_WaypointList[i] == Name)
            return;
      }

      m_WaypointList.push_back(Name);
      pChatDlg->AddString("Reached Waypoint "+Name,D3DCOLOR_XRGB(255,255,0));
   }
}
