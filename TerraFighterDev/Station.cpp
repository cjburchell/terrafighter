// StaticObj.cpp: implementation of the CStation class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Station.h"
#include "vertextypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStation::CStation() : CSprite(), Microthread()
{
   m_MaxSpeed = 0.0f;
   m_RotationTime = 10.0f;
   m_Acceleration = 0.0f;
   m_Friction = 5.0f;
   m_AnimationAngleZ = 0.0f;
}

CStation::~CStation()
{

}


#define RANGE_DISTACE 30.0f

HRESULT CStation::FrameMove(FLOAT fTime,
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
                            std::vector<CGameAction>& Actions)
{
   if(m_SpriteState == SPRITE_ALIVE)
   {
      m_ThreadParam.fElapsedTime = fElapsedTime;
      m_ThreadParam.pSpriteList  = pSpriteList;
      m_ThreadParam.Weapons      = Weapons;
      m_ThreadParam.pFloor       = pFloor;
      m_ThreadParam.pParticleSystem = pParticleSystem;
      m_ThreadParam.bCurrentPlayer  = bCurrentPlayer;
      m_ThreadParam.pNetConnect     = pNetConnect;
      update();
   }
   else
   {
      if(m_pMesh)
      {
         m_iKeyFrame = static_cast<int>(ToDeg(m_AnimationAngleZ));
      }
   }

   CSprite::FrameMove(fTime,fElapsedTime,pInput,pFloor,pd3dDevice,pParticleSystem,pSpriteList, bCurrentPlayer,pWater,Weapons,nWeapons,pNetConnect,pPlanetObjects,pChatDlg,Actions);

   return S_OK;
}

void CStation::thread_proc()
{
   FLOAT Distance = RANGE_DISTACE;
   while(true)
   {
      CSprite* pTarget = GetClosestSprite(m_ThreadParam.pSpriteList,Distance,FALSE); // find the clocest target
      if(NULL != pTarget) // there is a enemy sprite within Range
      {
         D3DXVECTOR3 v = pTarget->GetPos();
         D3DXVECTOR3 vO = GetPos();
         FLOAT DestAngleZ = 0.0f;
         v = v - vO;


         if(v.x != 0)
            DestAngleZ = atan2(v.z,v.x);
         else if(v.z > 0)
            DestAngleZ = 0.0f;
         else
            DestAngleZ = PI;
         DestAngleZ += PI2;
         DestAngleZ = - DestAngleZ;

         /*
         FLOAT d = ABS(D3DXVec3Length(&v));
         if(d != 0)
         DestAngleZ = atan2(v.y,d);
         else if(v.y > 0)
         DestAngleZ = 0.0f;
         else
         DestAngleZ = PI;
         */


         NormalizeAngle(DestAngleZ);
         FLOAT DiffRad = DestAngleZ-m_AnimationAngleZ;
         FLOAT DiffRad2 = m_AnimationAngleZ - DestAngleZ;

         NormalizeAngle(DiffRad2);
         NormalizeAngle(DiffRad);


         if(m_RotationTime != 0.0f) // can we animate the object
         {
            if(DiffRad > 0.0f && DiffRad < PI)
            {
               m_AnimationAngleZ+= m_ThreadParam.fElapsedTime/m_RotationTime * (PI2);
               NormalizeAngle(m_AnimationAngleZ);
               if(DestAngleZ-m_AnimationAngleZ < 0.0f && DestAngleZ-m_AnimationAngleZ > PI)
                  m_AnimationAngleZ = DestAngleZ;
            }
            else
            {
               m_AnimationAngleZ-= m_ThreadParam.fElapsedTime/m_RotationTime * (PI2);
               NormalizeAngle(m_AnimationAngleZ);
               if(DestAngleZ-m_AnimationAngleZ > 0.0f && DestAngleZ-m_AnimationAngleZ < PI)
                  m_AnimationAngleZ = DestAngleZ;
            }
         }

         if(m_pMesh)
         {
            m_iKeyFrame = static_cast<int>(ToDeg(m_AnimationAngleZ));
            m_pMesh->setKey(m_iKeyFrame);
         }

         // DO AI here
         FLOAT diffZ = ABS(ToDeg(DiffRad));
         FLOAT diffZ2 = ABS(ToDeg(DiffRad2));
         diffZ = diffZ<diffZ2?diffZ:diffZ2;

         if(diffZ < 5.0f)
         {
            if(m_CurrentWeapon!=NO_WEAPON_SEL)
            {
               D3DXVECTOR3 point = pTarget->GetPos();
               if(!m_ThreadParam.pFloor->Intersect(GetPos(),point)) // can we see the ship
               {
                  D3DXVECTOR3 v;
                  D3DXVec3Subtract(&v,&GetPos(),&point);
                  D3DXVECTOR3 vVelocity;
                  D3DXVec3Scale(&vVelocity,&v,1.0f/Distance * -40.0f);
                  if(m_ThreadParam.Weapons[m_CurrentWeapon].Fire(this,
                     pTarget,
                     m_LastShot,
                     m_ThreadParam.pParticleSystem,
                     m_ThreadParam.bCurrentPlayer,
                     m_ThreadParam.pNetConnect,
                     m_WeaponPowerMod))
                     m_LastShot = 0.0f;
               }
            }
         }
      }
      wait_one_frame();
   }
}
