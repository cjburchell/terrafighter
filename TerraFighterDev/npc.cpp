// NPC.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "NPC.h"
#include "vertextypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CNpc::CNpc() : CVehicle()
{
   m_StartState = AIS_IDLE;
   m_RectionTime = 0.0f;

   m_MaxSpeed = 21.0f;
   m_RotationTime = 3.0f;
   m_Acceleration = 1.0f;
   m_Friction = 2.0f;

   m_pCurrentGoal = NULL;
}

CNpc::~CNpc()
{

}

HRESULT CNpc::Create(char* object, FLOAT Scale,D3DXVECTOR3 pos,CSoundManager* pSoundMgr)
{
   if(FAILED(CSprite::Create(object,Scale,pos,pSoundMgr)))
      return E_FAIL;
   m_SeekPos = pos;
   return S_OK;
}

HRESULT CNpc::Create(CSpriteDataEx* pData, D3DXVECTOR3 pos, CSoundManager* pSoundMgr,WORD TeamNo, WORD Type, int CurrentTexture)
{
   if(FAILED(CSprite::Create(pData,pos,pSoundMgr,TeamNo,Type,CurrentTexture)))
      return E_FAIL;
   m_SeekPos = pos;
   return S_OK;
}


void CNpc::GetRandomPoint(CTerrain* pFloor)
{
   m_SeekPos = D3DXVECTOR3(0,0,0);
   FLOAT fTheta  = 2.0f*D3DX_PI*(FLOAT)rand()/RAND_MAX;
   FLOAT fRadius = (FLOAT)pFloor->GetXSize()/2.0f * (FLOAT)rand()/RAND_MAX;
   m_SeekPos.x  = fRadius * sinf(fTheta);
   m_SeekPos.z  = fRadius * cosf(fTheta);
   m_SeekPos.y  = pFloor->GetHightAtPosR( m_SeekPos.x, m_SeekPos.z );
   m_SeekPos.y += 1 + (20 * rand()/RAND_MAX);
}

enum AIResponse_e
{
   AIR_FOUND_NEW_TARGET = 0,
   AIR_LOST_CURRENT_TARGET = 1,

   AIR_FOUND_NEW_GOAL = 2,
   AIR_LOST_CURRENT_GOAL = 3,

   AIR_TOO_CLOSE_TO_TARGET = 4,
   AIR_TOO_CLOSE_TO_GOAL = 5,

   AIR_TARGET_IN_RANGE = 6,
   AIR_GOAL_IN_RANGE = 7,

   AIR_SHIELDS_ZERO = 8,
   AIR_REACHED_GOAL = 9,

   AIR_TOO_CLOSE_TO_FRIEND = 10,

   MAX_AI_RESPONSE = 11
};

const char* GetAIResponseStr(AIResponse_e val)
{
   static const char* Strings[MAX_AI_RESPONSE] =
   {
      "AIR_FOUND_NEW_TARGET",
         "AIR_LOST_CURRENT_TARGET",
         "AIR_FOUND_NEW_GOAL",
         "AIR_LOST_CURRENT_GOAL",
         "AIR_TOO_CLOSE_TO_TARGET",
         "AIR_TOO_CLOSE_TO_GOAL",
         "AIR_TARGET_IN_RANGE",
         "AIR_GOAL_IN_RANGE",
         "AIR_SHIELDS_ZERO",
         "AIR_REACHED_GOAL",
         "AIR_TOO_CLOSE_TO_FRIEND"
   };

   if(MAX_AI_RESPONSE <= val)
   {
      sys_msg1("InvalidResponse %d", val);
      return "Unknown Response";
   }

   return Strings[val];
}


const char* GetAIStateStr(AI_States_e val)
{
   static const char* Strings[MAX_AIS_STATES] =
   {
      "AIS_IDLE",
         "AIS_ATTACKING",
         "AIS_EVADING",
         "AIS_RUNNING",

         "AIS_GT_SEEK",
         "AIS_GT_ATTACKING",
         "AIS_GT_EVADING",

         "AIS_GARD_SEEK",
         "AIS_GARD_ATTACKING",
         "AIS_GARD_EVADING",
         "AIS_GARD_PATROL"
   };

   if(MAX_AIS_STATES <= val)
   {
      sys_msg1("InvalidState %d", val);
      return "Unknown State";
   }

   return Strings[val];
}

static const AI_States_e g_AIStates[MAX_AI_RESPONSE][MAX_AIS_STATES] =  
{
   //AIS_IDLE      AIS_ATTACKING  AIS_EVADING    AIS_RUNNING    AIS_GT_SEEK       AIS_GT_ATTACKING  AIS_GT_EVADING    AIS_GARD_SEEK      AIS_GARD_ATTACKING   AIS_GARD_EVADING     AIS_GARD_PATROL
   /*AIR_FOUND_NEW_TARGET*/      {AIS_ATTACKING,AIS_ATTACKING, AIS_ATTACKING, AIS_RUNNING,   AIS_GT_ATTACKING, AIS_GT_ATTACKING, AIS_GT_ATTACKING, AIS_GARD_ATTACKING,AIS_GARD_ATTACKING,  AIS_GARD_ATTACKING,  AIS_GARD_ATTACKING},
   /*AIR_LOST_CURRENT_TARGET*/   {AIS_IDLE,     AIS_IDLE,      AIS_IDLE,      AIS_IDLE,      AIS_GT_SEEK,      AIS_GT_SEEK,      AIS_GT_SEEK,      AIS_GARD_SEEK,     AIS_GARD_SEEK,       AIS_GARD_SEEK,       AIS_GARD_PATROL},
   /*AIR_FOUND_NEW_GOAL*/        {AIS_ATTACKING,AIS_ATTACKING, AIS_ATTACKING, AIS_RUNNING,   AIS_GT_SEEK,      AIS_GT_ATTACKING, AIS_GT_EVADING,   AIS_GARD_SEEK,     AIS_GARD_SEEK,       AIS_GARD_SEEK,       AIS_GARD_SEEK},
   /*AIR_LOST_CURRENT_GOAL*/     {AIS_IDLE,     AIS_IDLE,      AIS_IDLE,      AIS_IDLE,      AIS_IDLE,         AIS_IDLE,         AIS_IDLE,         AIS_IDLE,          AIS_IDLE,            AIS_IDLE,            AIS_IDLE},
   /*AIR_TOO_CLOSE_TO_TARGET*/   {AIS_EVADING,  AIS_EVADING,   AIS_EVADING,   AIS_EVADING,   AIS_GT_EVADING,   AIS_GT_EVADING,   AIS_GT_EVADING,   AIS_GARD_EVADING,  AIS_GARD_EVADING,    AIS_GARD_EVADING,    AIS_GARD_EVADING},
   /*AIR_TOO_CLOSE_TO_GOAL*/     {AIS_EVADING,  AIS_EVADING,   AIS_EVADING,   AIS_EVADING,   AIS_GT_SEEK,      AIS_GT_ATTACKING, AIS_GT_EVADING,   AIS_GARD_PATROL,   AIS_GARD_ATTACKING,  AIS_GARD_EVADING,    AIS_GARD_PATROL},
   /*AIR_TARGET_IN_RANGE*/       {AIS_ATTACKING,AIS_ATTACKING, AIS_ATTACKING, AIS_RUNNING,   AIS_GT_ATTACKING, AIS_GT_ATTACKING, AIS_GT_ATTACKING, AIS_GARD_ATTACKING,AIS_GARD_ATTACKING,  AIS_GARD_ATTACKING,  AIS_GARD_ATTACKING},
   /*AIR_GOAL_IN_RANGE*/         {AIS_ATTACKING,AIS_ATTACKING, AIS_ATTACKING, AIS_RUNNING,   AIS_GT_SEEK,      AIS_GT_ATTACKING, AIS_GT_SEEK,      AIS_GARD_SEEK,     AIS_GARD_ATTACKING,  AIS_GARD_SEEK,       AIS_GARD_SEEK},
   /*AIR_SHIELDS_ZERO*/          {AIS_RUNNING,  AIS_RUNNING,   AIS_EVADING,   AIS_RUNNING,   AIS_GT_SEEK,      AIS_GT_ATTACKING, AIS_GT_EVADING,   AIS_GARD_SEEK,     AIS_GARD_ATTACKING,  AIS_GARD_EVADING,    AIS_GARD_PATROL},
   /*AIR_REACHED_GOAL*/          {AIS_IDLE,     AIS_ATTACKING, AIS_EVADING,   AIS_RUNNING,   AIS_IDLE,         AIS_IDLE,         AIS_IDLE,         AIS_GARD_PATROL,   AIS_GARD_ATTACKING,  AIS_GARD_PATROL,     AIS_GARD_PATROL},
   /*AIR_TOO_CLOSE_TO_FRIEND*/   {AIS_EVADING,  AIS_EVADING,   AIS_EVADING,   AIS_EVADING,   AIS_GT_EVADING,   AIS_GT_EVADING,   AIS_GT_EVADING,   AIS_GARD_EVADING,  AIS_GARD_EVADING,    AIS_GARD_EVADING,    AIS_GARD_EVADING}
};


AI_States_e UpdateState(AIResponse_e Responce,AI_States_e CurrentState)
{
   AI_States_e NewState = g_AIStates[Responce][CurrentState];

   if(CurrentState != NewState)
   {
      sys_printf(__LOCATION__, 0, "State changed to %s from %s:%s",GetAIStateStr(NewState),GetAIResponseStr(Responce),GetAIStateStr(CurrentState));
   }

   return NewState;
}


void CNpc::SetAIType(AIType_e Type)
{
   static const AI_States_e StartStates[MAX_AI_TYPES] =
   {
      AIS_IDLE,
         AIS_GARD_SEEK,
         AIS_GT_SEEK
   };
   CVehicle::SetAIType(Type);
   m_StartState = StartStates[Type];
}


HRESULT CNpc::FrameMove(FLOAT fTime,
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
                        std::vector<CGameAction>& Actions
                        )
{
   m_ThreadParam.fElapsedTime = fElapsedTime;
   m_ThreadParam.pSpriteList  = pSpriteList;
   m_ThreadParam.Weapons      = Weapons;
   m_ThreadParam.pFloor       = pFloor;
   m_ThreadParam.pParticleSystem = pParticleSystem;
   m_ThreadParam.bCurrentPlayer  = bCurrentPlayer;
   m_ThreadParam.pNetConnect     = pNetConnect;
   m_ThreadParam.pPlanetObjects     = pPlanetObjects;
   update();
   return CSprite::FrameMove(
      fTime,
      fElapsedTime,
      pInput,
      pFloor,
      pd3dDevice,
      pParticleSystem,
      pSpriteList,
      bCurrentPlayer,
      pWater,
      Weapons,
      nWeapons,
      pNetConnect,
      pPlanetObjects,
      pChatDlg,
      Actions
      );
}

void CNpc::GetFullPlayerMsg(GAMEMSG_FULL_PLAYER_UPDATE *pMsg)
{
   CSprite::GetFullPlayerMsg(pMsg);
   pMsg->m_bBot = TRUE;
}

void CNpc::thread_proc()
{

   // these constants should be varaables that can be configrued for each NPC
#define SEE_DISTANCE 40.0f
#define RUN_DISTANCE 70.0f
#define CHASE_DISTANCE 10.0f
#define GOTO_DISTANCE 3.0f 
#define COLLISION_DISTANCE 4.0f

#define CHASE_SPEED m_MaxSpeed*(2.0f/3.0f)
#define SEEK_SPEED m_MaxSpeed/2.0f
#define RUN_SPEED m_MaxSpeed
#define REACTION_TIME 1.00f

   AI_States_e LastState    = m_StartState;
   AI_States_e CurrentState = m_StartState;
   FLOAT Distance = SEE_DISTANCE;
   while(true)
   { 
      if(m_SpriteState == SPRITE_ALIVE)
      {
         m_RectionTime += m_ThreadParam.fElapsedTime;
         CSprite* pClosest = GetClosestSprite(m_ThreadParam.pSpriteList,Distance,FALSE);
         CSprite* pFriend =  GetClosestSprite(m_ThreadParam.pSpriteList,Distance,TRUE);

         if(pClosest != NULL && pClosest != m_pTarget)
         {
            // new target has been found
            CurrentState = UpdateState(AIR_FOUND_NEW_TARGET,CurrentState);
         }
         else if(pClosest == NULL && m_pTarget != NULL)
         {
            //we cant see any new targets
            CurrentState = UpdateState(AIR_LOST_CURRENT_TARGET,CurrentState);
         }

         m_pTarget = pClosest;


         CDisplayObject* pGoalSprite = GetGoal(m_ThreadParam.pSpriteList,m_ThreadParam.pPlanetObjects->GetObjectList());
         if(pGoalSprite != NULL && pGoalSprite != m_pCurrentGoal)
         {
            // new goal has been found
            CurrentState = UpdateState(AIR_FOUND_NEW_GOAL,CurrentState);
         }
         else if(pGoalSprite == NULL && m_pCurrentGoal != NULL)
         {
            //we cant find any goals
            CurrentState = UpdateState(AIR_LOST_CURRENT_GOAL,CurrentState);
         }
         m_pCurrentGoal = pGoalSprite;

         if(m_pTarget != NULL && CheckCollision(m_pTarget->GetPos(),SEE_DISTANCE) < 0.0f)
         {
            if(CheckCollision(m_pTarget->GetPos(),COLLISION_DISTANCE) < 0.0f)
            {
               CurrentState = UpdateState(AIR_TOO_CLOSE_TO_TARGET,CurrentState);
            }
            else
            {
               CurrentState = UpdateState(AIR_TARGET_IN_RANGE,CurrentState);
            }
         }

         //if(m_pCurrentGoal != NULL && CheckCollision(m_pCurrentGoal->GetPos(),SEE_DISTANCE) < 0.0f)
         if(m_pCurrentGoal != NULL)
         {
            if(CheckCollision(m_pCurrentGoal->GetPos(),COLLISION_DISTANCE) < 0.0f)
            {
               CurrentState = UpdateState(AIR_TOO_CLOSE_TO_GOAL,CurrentState);
            }
            else
            {
               CurrentState = UpdateState(AIR_GOAL_IN_RANGE,CurrentState);
            }
         }


         if(pFriend)
         {
            if(CheckCollision(pFriend->GetPos(),COLLISION_DISTANCE) < 0.0f)
            {
               CurrentState = UpdateState(AIR_TOO_CLOSE_TO_FRIEND,CurrentState);
            }
         }

         if(m_pCurrentGoal)
         {
            FLOAT SeekDistance = CheckCollision(m_pCurrentGoal->GetPos(), GOTO_DISTANCE);
            if(SeekDistance < 0.0f)
            {
               CurrentState = UpdateState(AIR_REACHED_GOAL,CurrentState);
            }
         }

         if(m_fShield_Str < 10.0f)
         {
            CurrentState = UpdateState(AIR_SHIELDS_ZERO,CurrentState);
         }

         switch(CurrentState)
         {
         case AIS_ATTACKING:

            if(m_RectionTime > REACTION_TIME)
            {
               if(m_pTarget)
               {
                  m_SeekPos = m_pTarget->GetPos();
                  m_CurrentMaxSpeed = m_pTarget->GetSpeed();
               }
               else if(m_pCurrentGoal)
               {
                  m_SeekPos = m_pCurrentGoal->GetPos();
                  m_CurrentMaxSpeed = m_pCurrentGoal->GetSpeed();
               }

               // if the target not is within CHASE_DISTANCE
               if(CheckCollision(m_SeekPos, CHASE_DISTANCE) > 0.0f)
               {
                  m_CurrentMaxSpeed = CHASE_SPEED;
               }

               //reduce speed to player to chase
               if(m_CurrentMaxSpeed > CHASE_SPEED || m_CurrentMaxSpeed == 0.0f)
                  m_CurrentMaxSpeed = CHASE_SPEED;

               m_RectionTime = 0.0f;
            }

            break;

         case AIS_RUNNING:
         case AIS_EVADING:
         case AIS_GT_EVADING:
         case AIS_GARD_EVADING:
         case AIS_IDLE:
         case AIS_GARD_PATROL:
            {
               if(CurrentState != LastState)
               {
                  // state has changed
                  GetRandomPoint(m_ThreadParam.pFloor);
               }

               FLOAT SeekDistance = CheckCollision(m_SeekPos, GOTO_DISTANCE);
               if(SeekDistance < 0.0f)
               {
                  // we reached the patrol point
                  // get another one
                  GetRandomPoint(m_ThreadParam.pFloor);
               }

               FLOAT SpeedFactor = 1.0f;
               if(SeekDistance < 3.0f) // slow down the closer you get to the seek point
               {
                  SpeedFactor = SeekDistance/3.0f;
               }

               if(CurrentState == AIS_IDLE || CurrentState == AIS_GARD_PATROL)
                  m_CurrentMaxSpeed = SEEK_SPEED * SpeedFactor;
               else
                  m_CurrentMaxSpeed = RUN_SPEED * SpeedFactor;
            }
            break;
         case AIS_GT_SEEK:
         case AIS_GARD_SEEK:

            if(m_RectionTime > REACTION_TIME)
            {
               if(m_pCurrentGoal)
               {
                  m_SeekPos = m_pCurrentGoal->GetPos();
                  m_CurrentMaxSpeed = m_pCurrentGoal->GetSpeed();
               }
               if(m_CurrentMaxSpeed == 0.0f)
               {
                  FLOAT SeekDistance = CheckCollision(m_SeekPos, GOTO_DISTANCE);
                  FLOAT SpeedFactor = 1.0f;
                  if(SeekDistance < 3.0f) // slow down the closer you get to the seek point
                  {
                     SpeedFactor = SeekDistance/3.0f;
                  }
                  m_CurrentMaxSpeed = SEEK_SPEED * SpeedFactor;
               }
               else
               {
                  // if the target not is within CHASE_DISTANCE
                  if(CheckCollision(m_SeekPos, CHASE_DISTANCE) > 0.0f)
                  {
                     m_CurrentMaxSpeed = SEEK_SPEED;
                  }
               }
               m_RectionTime = 0.0f;
            }


            break;
         case AIS_GT_ATTACKING:
         case AIS_GARD_ATTACKING:
            if(m_RectionTime > REACTION_TIME)
            {
               if(m_pTarget)
               {
                  m_SeekPos = m_pTarget->GetPos();
                  m_CurrentMaxSpeed = static_cast<CSprite*>(m_pTarget)->GetSpeed();
                  m_CurrentMaxSpeed = 0.0f;
               }

               // if the target not is within CHASE_DISTANCE
               if(CheckCollision(m_SeekPos, CHASE_DISTANCE) > 0.0f)
               {
                  m_CurrentMaxSpeed = CHASE_SPEED;
               }

               //reduce speed to player to chase
               if(m_CurrentMaxSpeed > CHASE_SPEED || m_CurrentMaxSpeed == 0.0f)
                  m_CurrentMaxSpeed = CHASE_SPEED;

               m_RectionTime = 0.0f;
            }
            break;

         }; 

         FLOAT x = m_SeekPos.x-GetPos().x;
         FLOAT y = m_SeekPos.y-GetPos().y;
         FLOAT z = m_SeekPos.z-GetPos().z;

         D3DXVECTOR3 v = D3DXVECTOR3(x,y,z);
         FLOAT d = ABS(D3DXVec3Length(&v));

         if(v.x != 0)
            m_DestAngleY = atan2(v.z,v.x);
         else if(v.z > 0)
            m_DestAngleY = 0.0f;
         else
            m_DestAngleY = PI;
         m_DestAngleY += PI2;
         m_DestAngleY = - m_DestAngleY;

         if(d != 0)
            m_DestAngleZ = atan2(v.y,d);
         else if(v.y > 0)
            m_DestAngleZ = 0.0f;
         else
            m_DestAngleZ = PI;

         NormalizeAngle(m_DestAngleY);


         //m_angleY = m_DestAngleY;
         //m_angleZ = m_DestAngleZ;

         if(m_Speed.x < m_CurrentMaxSpeed)
            m_Speed.x += m_Acceleration*m_ThreadParam.fElapsedTime;
         else if(m_Speed.x > m_CurrentMaxSpeed) 
            m_Speed.x -= m_Friction*m_ThreadParam.fElapsedTime;

         if(m_CurrentMaxSpeed<m_Speed.x)
            m_Speed.x = m_CurrentMaxSpeed;

         if(m_fMinSpeed>m_Speed.x)
            m_Speed.x = m_fMinSpeed;

         FLOAT degy = ::ToDeg(m_angleY);
         FLOAT desty = ::ToDeg(m_DestAngleY);

         FLOAT diff = m_DestAngleY-m_angleY;
         if(diff > 0.0f && diff < PI)
         {
            m_angleY+= m_ThreadParam.fElapsedTime/m_RotationTime * (PI2);
            NormalizeAngle(m_angleY);

            if(m_DestAngleY-m_angleY < 0.0f && m_DestAngleY-m_angleY > PI)
               m_angleY = m_DestAngleY;
         }
         else
         {
            m_angleY-= m_ThreadParam.fElapsedTime/m_RotationTime * (PI2);
            NormalizeAngle(m_angleY);
            if(m_DestAngleY-m_angleY > 0.0f && m_DestAngleY-m_angleY < PI)
               m_angleY = m_DestAngleY;
         }

         //m_angleZ = 0.0f;

         //NormalizeAngle(m_DestAngleZ); 

         if(m_DestAngleZ-m_angleZ > 0.0f)
         {
            m_angleZ+= m_ThreadParam.fElapsedTime/m_RotationTime * (PI2);
            if(m_DestAngleZ-m_angleZ < 0.0f)
               m_angleZ = m_DestAngleZ;
         }
         else
         {
            m_angleZ-= m_ThreadParam.fElapsedTime/m_RotationTime * (PI2);
            if(m_DestAngleZ-m_angleZ > 0.0f)
               m_angleZ = m_DestAngleZ;
         }

         //NormalizeAngle(m_angleZ);

         FLOAT diffZ = ABS(ToDeg(m_angleZ)-ToDeg(m_DestAngleZ));
         FLOAT diffZ2 = ABS(ToDeg(m_DestAngleZ)-ToDeg(m_angleZ));
         FLOAT diffY = ABS(ToDeg(m_angleY)-ToDeg(m_DestAngleY));
         FLOAT diffY2 = ABS(ToDeg(m_DestAngleY)-ToDeg(m_angleY));
         diffY = min(diffY,diffY2);
         diffZ = min(diffZ,diffZ2);

         if(diffZ < 5.0f &&  diffY < 5.0f && (CurrentState == AIS_GARD_ATTACKING || CurrentState==AIS_GT_ATTACKING ||CurrentState ==AIS_ATTACKING))
         {
            if(m_CurrentWeapon!=NO_WEAPON_SEL)
            {
               if(m_ThreadParam.Weapons[m_CurrentWeapon].Fire(
                  this,
                  m_pTarget?m_pTarget:m_pCurrentGoal,
                  m_LastShot,
                  m_ThreadParam.pParticleSystem,
                  m_ThreadParam.bCurrentPlayer,
                  m_ThreadParam.pNetConnect,
                  m_WeaponPowerMod))
                  m_LastShot = 0.0f;
            }
         }
      }
      else
      {
         m_Speed.x -= m_Friction*m_ThreadParam.fElapsedTime;
         if(m_Speed.x < 0.0f)
            m_Speed.x = 0.0f;
      }
      wait_one_frame();
   }
}
