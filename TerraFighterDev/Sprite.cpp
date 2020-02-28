// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Sprite.h"
#include "ShotSystem.h"
#include "TerraFighterApp.h"
#include "WayPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT CSpriteDataEx::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
   m_pMesh = MeshCash_CreateMesh(pd3dDevice, m_FileName.GetBuffer(m_FileName.GetLength()) ,D3DFVF_D3DVERTEX);
   return m_pMesh == NULL?E_FAIL:S_OK;
}

#define CAMERA_Y 1.0f
#define CAMERA_DISTANCE (-4.0f)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite() : CDisplayObject()
{
   m_angleY = 0.0f;
   m_angleZ = 0.0f;
   m_angleX = 0.0f;
   m_Speed.x = 0;
   m_Speed.y = 0;
   m_Speed.z = 0;
   m_pExaust = new CExaust();
   m_bShowShield = FALSE;

   m_fShield_Str = 100.0f;
   m_fDamage = 100.0f;
   m_SpriteState = SPRITE_ALIVE;

   m_pSoundEngine= NULL;
   m_pDS3DBufferEngine= NULL;
   m_bMoving = FALSE;
   m_LastShot =0.0f;


   // object constants
   m_MaxSpeed = 0.0f;
   m_OldMaxSpeed = 0.0f;
   m_RotationTime = 0.0f;
   m_Acceleration = 0.0f;
   m_Friction = 0.0f;

   m_fMinSpeed = 0.0f;
   m_fMaxHight = 0.0f;
   m_fMaxShields = 100.0f;
   m_fMaxDamage = 100.0f;
   m_fRegenRate = 5.0f;
   m_fMinHight = 0.5f;
   m_CurrentWeapon = 0;
   m_zMeshRot = 0.0f;
   m_yMeshRot = 0.0f;

   m_bInvrunable = FALSE;
   m_bInvisable = FALSE;

   m_bRestrictRot = FALSE;

   m_bTargetLock = FALSE;
   m_pTarget = NULL;

   m_pMesh = NULL;
   m_pShield = NULL;

   m_AIType = AI_NORMAL;
   m_ID = 0;

   m_WeaponPowerMod = 0.0f;

   m_FragCount = 0;

   m_bSprite = TRUE;

   // animation keys
   m_fCurTime = 0.0f;
   m_iKeyFrame = -1;

   m_CurrentTextureFile = _T("");
   m_pCurrentTexture = NULL;

   m_CameraPos = D3DXVECTOR3(CAMERA_DISTANCE,CAMERA_Y,0.0f);
}

CSprite::~CSprite()
{
   if(m_pSoundEngine)
   {
      m_pSoundEngine->Stop();
      m_pSoundEngine->Reset();
   }
   SAFE_DELETE( m_pExaust );
   SAFE_RELEASE(m_pDS3DBufferEngine );
   SAFE_DELETE( m_pSoundEngine );
   MeshCash::releaseMesh(m_pMesh);
   MeshCash::releaseMesh(m_pShield);
}

HRESULT CSprite::Create(CSpriteDataEx* pData, D3DXVECTOR3 pos, CSoundManager* pSoundMgr,WORD TeamNo, WORD Type,int CurrentTexture)
{
   m_Type = Type;
   m_Friction = pData->m_Drag;
   m_Acceleration = pData->m_fAcceleration;
   m_MaxSpeed = pData->m_fMaxSpeed;
   m_RotationTime = pData->m_fTurningRate;
   m_Name = pData->m_Name;
   m_fMinSpeed = pData->m_fMinSpeed;
   m_fMaxHight= pData->m_fMaxHight;
   m_fMaxShields= pData->m_fMaxShields;
   m_fMaxDamage= pData->m_fMaxDamage;
   m_fRegenRate= pData->m_fRegenRate;
   m_fMinHight = pData->m_fMinHight;
   m_CurrentWeapon = pData->m_DefaultWeapon;
   m_zMeshRot = pData->m_zMeshRot;
   m_yMeshRot = pData->m_yMeshRot;
   m_bRestrictRot = pData->m_bRestrictRot;
   m_TeamNo = TeamNo;

   if(CurrentTexture != 0)
      m_CurrentTextureFile = pData->m_Textures[CurrentTexture-1];

   switch(m_TeamNo)
   {
   case 0:
      m_TeamColour = D3DCOLOR_XRGB(0,255,0);
      break;
   case 1:
      m_TeamColour = D3DCOLOR_XRGB(255,0,0);
      break;
   case 2:
      m_TeamColour = D3DCOLOR_XRGB(0,0,255);
      break;
   case 3:
      m_TeamColour = D3DCOLOR_XRGB(255,255,0);
      break;
   default:
      m_TeamColour = D3DCOLOR_XRGB(255,255,255);

   }

   return Create(pData->m_FileName, pData->m_fScale, pos, pSoundMgr);
}

HRESULT CSprite::Create(const char* object, FLOAT Scale,D3DXVECTOR3 pos, CSoundManager* pSoundMgr)
{
   CDisplayObject::Create(object,Scale,pos);

   m_MeshFile = object;

   HRESULT hr = pSoundMgr->Create( &m_pSoundEngine, "hum.wav",&GetApp()->m_Zip, DSBCAPS_CTRL3D, DS3DALG_NO_VIRTUALIZATION,1 );  
   if( SUCCEEDED( hr  ) )
   {
      // Get the 3D buffer from the secondary buffer
      hr = m_pSoundEngine->Get3DBufferInterface( 0, &m_pDS3DBufferEngine );
      if( SUCCEEDED( hr  ) )
      {
         if(m_pDS3DBufferEngine)
         {
            DS3DBUFFER              dsBufferParams;               // 3D buffer properties
            // Get the 3D buffer parameters
            dsBufferParams.dwSize = sizeof(DS3DBUFFER);
            m_pDS3DBufferEngine->GetAllParameters( &dsBufferParams );

            // Set new 3D buffer parameters
            dsBufferParams.dwMode = DS3DMODE_NORMAL;
            dsBufferParams.flMaxDistance = 40.0f;
            dsBufferParams.flMaxDistance = 0.0f;
            m_pDS3DBufferEngine->SetAllParameters( &dsBufferParams, DS3D_IMMEDIATE );
         }
      }
   }





   D3DXMATRIX matRotateY;
   // scale the object
   D3DXMatrixScaling( &m_MeshScaled, GetScale(), GetScale(), GetScale());

   // rotiate it 45
   D3DXMatrixRotationY( &matRotateY, HALFPI+PI );
   D3DXMatrixMultiply( &m_MeshScaled, &m_MeshScaled, &matRotateY );

   if(m_zMeshRot != 0.0f)
   {
      D3DXMATRIX matRotateZ;
      D3DXMatrixRotationZ( &matRotateZ, m_zMeshRot);
      D3DXMatrixMultiply( &m_MeshScaled, &m_MeshScaled, &matRotateZ );
   }

   if(m_yMeshRot != 0.0f)
   {
      D3DXMATRIX matRotateY;
      D3DXMatrixRotationY( &matRotateY, m_yMeshRot);
      D3DXMatrixMultiply( &m_MeshScaled, &m_MeshScaled, &matRotateY );
   }
   return S_OK;
}

HRESULT CSprite::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{	
   HRESULT hr;
   // mesh
   ////////////////////////////////
   m_pMesh = MeshCash_CreateMesh(pd3dDevice, m_MeshFile.GetBuffer(m_MeshFile.GetLength()),D3DFVF_D3DVERTEX);
   m_pMesh->computeBoundingSphere(&m_Center,&m_Radius);
   m_Radius *= m_Scale; 
   /////////////////////////////////////////////


   if(!m_CurrentTextureFile.IsEmpty())
   {
      // Create the textures
      if( FAILED( D3DUtil::CreateTexture( pd3dDevice, (char*)((const char*)m_CurrentTextureFile),
         (m_pCurrentTexture) ) ) )
         return D3DAPPERR_MEDIANOTFOUND;
   }


   // Load the file-based mesh objects
   m_pShield = MeshCash_CreateMesh(pd3dDevice, _T("sphere0.x"),D3DFVF_D3DSHIELD);

   m_pShield->useMeshMaterials(FALSE);

   FLOAT Radius;
   D3DXVECTOR3 Center;

   m_pShield->computeBoundingSphere(&Center,&Radius);
   m_ShieldScale = 1.0f/Radius * GetRadius(); 

   if( FAILED(hr = m_pExaust->Create()))
      return hr;

   m_HudDisplay.m_bColorKey = TRUE;
   if( FAILED(hr = m_HudDisplay.CreateRenderSurface(128,128,pd3dDevice,GetApp()->GetModeInfo()->DepthStencilFormat)))
      return hr;

   D3DXVec3TransformCoord(&m_Center,&m_Center,&m_MeshScaled);
   return m_pExaust->RestoreDeviceObjects(pd3dDevice);
}

void CSprite::DeleteDeviceObjects()
{
   m_pExaust->DeleteDeviceObjects();
   m_HudDisplay.DeleteDeviceObjects();
   D3DUtil::ReleaseTexture( m_pCurrentTexture );
}

HRESULT CSprite::FrameMove(FLOAT fTime,
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
                           CNetworkConnection *pNetConnect,
                           CPlanetObjects* pPlanetObjects,
                           CChatDlg* pChatDlg,
                           std::vector<CGameAction>& Actions)
{
   if(m_SpriteState != SPRITE_DEAD)
   {
      // animate mesh
      if(m_pMesh)
      {
         m_fCurTime += fElapsedTime * 4800;
         if (m_fCurTime > 1.0e15f)
            m_fCurTime = 0;
         if(m_iKeyFrame != -1)
            m_pMesh->setKey(m_iKeyFrame);
         else
            m_pMesh->setTime(-1,-1,m_fCurTime);
      }

      // scale the object

      D3DXMATRIX matFinal = m_MeshScaled;


      D3DXMATRIX matTrans, matRotateZ, matRotateY;

      if(m_bRestrictRot)
      {
         D3DXMatrixRotationY( &m_Rotate, m_angleY );
      }
      else
      {
         D3DXMatrixRotationX( &m_Rotate, m_angleX );

         D3DXMatrixRotationZ( &matRotateZ, m_angleZ);
         D3DXMatrixMultiply( &m_Rotate, &m_Rotate, &matRotateZ );
         // rotation of the ship
         D3DXMatrixRotationY( &matRotateY, m_angleY );
         D3DXMatrixMultiply( &m_Rotate, &m_Rotate, &matRotateY );
      }

      D3DXMatrixMultiply( &matFinal, &matFinal, &m_Rotate );

      FLOAT MinHight = 0.0f;
      FLOAT MinHightWater = 0.0f;
      if(pWater)
         MinHightWater = pWater->GetWaterLevel() + GetRadius();

      MinHight = GetRadius();

      D3DXVECTOR3 CurrentPos = GetPos();
      if(m_SpriteState == SPRITE_ALIVE)
      {
         // calulate the new positon
         {
            D3DXMATRIX matNewPos, matCurrentPos;
            FLOAT distatnce= m_Speed.x*fElapsedTime;
            // move x 
            D3DXMatrixTranslation( &matNewPos,distatnce, 0, 0 );
            D3DXMatrixMultiply( &matNewPos, &matNewPos, &m_Rotate );
            D3DXMatrixTranslation( &matCurrentPos,CurrentPos.x, CurrentPos.y, CurrentPos.z );
            D3DXMatrixMultiply( &matNewPos, &matNewPos, &matCurrentPos );
            D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);
            D3DXVec3TransformCoord( &CurrentPos,&orgin,&matNewPos);



            if(m_fMaxHight == m_fMinHight && m_fMaxHight == 0.0f)
            {
               CurrentPos.y = pFloor->GetHightAtPosR(CurrentPos.x,CurrentPos.z);
            }
            else
            {

               if(MinHight > CurrentPos.y)
                  CurrentPos.y = MinHight;

               if(m_fMaxHight < CurrentPos.y)
                  CurrentPos.y = m_fMaxHight;

               FLOAT maphight = pFloor->GetHightAtPosR(CurrentPos.x,CurrentPos.z) + m_fMinHight+GetRadius();
               if(maphight > CurrentPos.y)
                  CurrentPos.y = maphight;
               if(MinHightWater > CurrentPos.y)
               {
                  CurrentPos.y = MinHightWater;
               }
            }




            FLOAT halfx = pFloor->GetXSize()/2;
            FLOAT halfz = pFloor->GetZSize()/2;

            if(halfx < CurrentPos.x)
               CurrentPos.x = halfx;
            if(-halfx > CurrentPos.x)
               CurrentPos.x = -halfx;

            if(halfz < CurrentPos.z)
               CurrentPos.z = halfz;
            if(-halfz > CurrentPos.z)
               CurrentPos.z = -halfz;

         }


         for(UINT i=0;i< pSpriteList->size(); i++)
         {
            if((*pSpriteList)[i] && (*pSpriteList)[i] == this && (*pSpriteList)[i]->IsAlive())
            {
               FLOAT Distance = (*pSpriteList)[i]->CheckCollision(CurrentPos,GetRadius());
               if( Distance < 0.0f) // we hit the object
               {
                  (*pSpriteList)[i]->DoCollision(this,&Actions,pChatDlg);
               }
            }
         }


         std::list<CStaticObj*> ColList;
         pPlanetObjects->GetCollisions(CurrentPos,GetRadius(),&ColList);

         for(std::list<CStaticObj*>::iterator Col = ColList.begin();Col != ColList.end();Col++)
         {
            (*Col)->DoCollision(this,&Actions,pChatDlg);
         }


         for(i = 0; i<m_PowerUpList.size();i++)
         {
            m_PowerUpList[i].m_StartTime += fElapsedTime;

            if(m_PowerUpList[i].m_Duration < m_PowerUpList[i].m_StartTime)
            {
               // powerup has ended
               // reset the stats
               switch(m_PowerUpList[i].m_Type)
               {
               case PU_STATS:
                  if(m_PowerUpList[i].m_MaxSpeed != 0.0f)
                     m_MaxSpeed = m_OldMaxSpeed;
                  if(m_PowerUpList[i].m_WeaponPower != 0.0f)
                     m_WeaponPowerMod = 0.0f;
                  break;
               case PU_INVULNERABILITY:
                  m_bInvrunable = FALSE;
                  break;
               case PU_INVISIBILITY:
                  m_bInvisable = FALSE;
                  break;
               }

               m_PowerUpList.erase(m_PowerUpList.begin() + i);
               i--;
            }
         }

      }
      else if(m_SpriteState == SPRITE_DYING)
      {
         // do some animation to distroy the sprite
         D3DXVECTOR3 Offsetpos =CurrentPos + GetCenterPoint() ;

         static_cast<CShotSystem*>(pParticleSystem)->EmitBigExplosion(Offsetpos,pd3dDevice);
         m_SpriteState = SPRITE_DEAD;
         m_bMoving = FALSE;
         if(m_pSoundEngine)
         {
            m_pSoundEngine->Stop();
            m_pSoundEngine->Reset();
         }
      }

      SetPos(&CurrentPos);

      //the current positon of the dolphin

      D3DXMatrixTranslation( &matTrans,GetPos().x, GetPos().y, GetPos().z );
      D3DXMatrixMultiply( &matFinal, &matFinal, &matTrans );
      SetMatrix(&matFinal);

      D3DXMATRIX matShieldTrans;
      D3DXMatrixScaling( &m_ShieldMatrix, m_ShieldScale, m_ShieldScale, m_ShieldScale );
      D3DXMatrixMultiply( &m_ShieldMatrix, &m_ShieldMatrix, &m_Rotate );

      D3DXVECTOR3 Offsetpos = GetPos();
      Offsetpos+=GetCenterPoint();
      D3DXMatrixTranslation( &matShieldTrans,Offsetpos.x, Offsetpos.y, Offsetpos.z );
      D3DXMatrixMultiply( &m_ShieldMatrix, &m_ShieldMatrix, &matShieldTrans );


      CParticle*    pParticle = pParticleSystem->GetParticles();

#define FADE_TIME 1.0f
#define FADE_MAX 1.0f
#define FADE_START 0.5f

      CSprite* pSpriteLastHit = NULL;
      while(pParticle)
      {

         if(static_cast<CShot*>(pParticle)->m_pOwner != this)
         {
            if(CheckCollision(pParticle->m_vPos,0.0f) < 0.0f)
            {
               pSpriteLastHit = static_cast<CShot*>(pParticle)->m_pOwner;

               if(pSpriteLastHit->GetTeamNo() != GetTeamNo())
               {
                  m_bShowShield = TRUE;
                  m_ShieldLevel = FADE_START;
                  pParticle->m_bHit = TRUE;
                  pParticle->m_fFade = 0.0f;


                  if(!m_bInvrunable)
                  {
                     m_fShield_Str -= (static_cast<CShot*>(pParticle)->m_fPower * 1.5f);
                     if(m_fShield_Str < 0.0f)
                     {
                        m_fDamage -= static_cast<CShot*>(pParticle)->m_fPower;
                        //m_fShield_Str = 0.0f;
                        m_bShowShield = FALSE;
                     }
                  }
               }
               else
               {
                  pSpriteLastHit = NULL;
               }
            }
         }
         pParticle = pParticle->m_pNext;
      }

      if(m_fDamage < 0.0f && m_SpriteState == SPRITE_ALIVE)
      {
         m_SpriteState = SPRITE_DYING;

         CSprite* pPlayerSprite = (*pSpriteList)[PLAYER_SPRITE];

         CGameAction action;
         action.m_EventType = EVENT_SPRITE_DISTROYED;
         action.m_ObjectName = GetName();

         if(pSpriteLastHit)
         {
            if(pNetConnect && pPlayerSprite == this)
            {
               GAMEMSG_GENERIC msg;
               msg.nType = GAME_MSGID_ADDFRAG_COUNT;
               pNetConnect->SendMessageToOne(pSpriteLastHit->GetID(),sizeof(GAMEMSG_GENERIC),(BYTE*)&msg);
            }
            else if(pPlayerSprite == pSpriteLastHit)
            {
               CString str;
               str.Format("Destroyed %s",GetName());
               pChatDlg->AddString(str,D3DCOLOR_XRGB(255,255,255));
            }

            action.m_ResponsName = pSpriteLastHit->GetName();
         }
         else
         {
            action.m_ResponsName = _T("");
         }

         Actions.push_back(action);
      }

      if(m_fShield_Str > -(m_fMaxShields/4.0f))
      {
         m_fShield_Str+= (fElapsedTime/1.0f) * m_fRegenRate;
         if(m_fShield_Str> m_fMaxShields)
            m_fShield_Str = m_fMaxShields;
      }

      if(m_bShowShield && m_fShield_Str > 0.0f)
      {
         m_ShieldLevel += (fElapsedTime/FADE_TIME) * FADE_MAX;
         if(FADE_MAX<m_ShieldLevel)
            m_bShowShield = FALSE;
         D3DUtil::InitMaterial( m_ShieldMat, 255.0f, 255.0f, 255.0f , m_ShieldLevel);
      }
      else
         m_bShowShield = FALSE;



      if(m_Speed.x != 0)
      {
         D3DXVECTOR3 vVelocity = GetDirectionVector(-m_Speed.x/2.0f);

         D3DXVECTOR3 PortPos;
         BOOL bFoundPort = GetExaustPortPos(0,PortPos);

         if(bFoundPort)
         {
            DWORD nParticles = fElapsedTime/0.1f;
            if(nParticles<=0)
               nParticles = 1;

            m_pExaust->EmitNewParticles(nParticles,D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,255,0,0),PortPos,vVelocity,0.0f, 0.2f);


            if(m_pSoundEngine)
               m_pSoundEngine->Play(0,DSBPLAY_LOOPING);
            m_bMoving = TRUE;

            if(m_pDS3DBufferEngine)
            {
               if(bCurrentPlayer)
               {
                  m_pDS3DBufferEngine->SetMode(DS3DMODE_HEADRELATIVE,DS3D_DEFERRED);
                  m_pDS3DBufferEngine->SetPosition(0.0f,0.0f,0.0f,DS3D_DEFERRED );
               }
               else
               {
                  m_pDS3DBufferEngine->SetMode(DS3DMODE_NORMAL,DS3D_DEFERRED);
                  m_pDS3DBufferEngine->SetPosition(PortPos.x,PortPos.y,PortPos.z,DS3D_DEFERRED );
               }
               m_pDS3DBufferEngine->SetMinDistance(0.0f, DS3D_DEFERRED);
               m_pDS3DBufferEngine->SetMaxDistance(FOV_BACK_DISTACNE, DS3D_DEFERRED);
               m_pDS3DBufferEngine->SetVelocity(vVelocity.x,vVelocity.y,vVelocity.z,DS3D_DEFERRED );
            }
         }
      }
      else
      {
         m_bMoving = FALSE;
         if(m_pSoundEngine)
         {
            m_pSoundEngine->Stop();
            m_pSoundEngine->Reset();
         }
      }
      m_LastShot+=fElapsedTime;
   }

   m_pExaust->Update(fElapsedTime,pFloor,NULL);

   return S_OK;
}



const D3DXVECTOR3& CSprite::GetCameraPos()
{
   D3DXMATRIX matTrans, matCamera;

   D3DXMatrixTranslation( &matCamera,m_CameraPos.x,m_CameraPos.y, m_CameraPos.z );



   D3DXMatrixMultiply( &matCamera, &matCamera, &m_Rotate );

   D3DXMatrixTranslation( &matTrans,GetPos().x, GetPos().y, GetPos().z );
   D3DXMatrixMultiply( &matCamera, &matCamera, &matTrans );

   D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);
   D3DXVECTOR3 result;
   D3DXVec3TransformCoord( &result,&orgin,&matCamera);
   return result;

}

const D3DXVECTOR3& CSprite::GetDirectionVector(FLOAT val)
{
   D3DXMATRIX matDirect;
   D3DXMatrixTranslation( &matDirect,val,0.0f, 0.0f );
   D3DXMatrixMultiply( &matDirect, &matDirect, &m_Rotate );
   D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);
   D3DXVECTOR3 result;
   D3DXVec3TransformCoord( &result,&orgin,&matDirect);
   return result;
}

const D3DXVECTOR3& CSprite::GetUpVector()
{
   //D3DXVECTOR3 result;
   //if(m_angleZ > HALFPI && m_angleZ < HALFPI+PI)
   //	 result = D3DXVECTOR3(0.0,-1.0f, 0.0f );
   //else
   //result = D3DXVECTOR3(0.0,1.0f, 0.0f );

   D3DXMATRIX matDirect;
   D3DXMatrixTranslation( &matDirect,0.0f,1.0f, 0.0f );
   D3DXMatrixMultiply( &matDirect, &matDirect, &m_Rotate );
   D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);
   D3DXVECTOR3 result;
   D3DXVec3TransformCoord( &result,&orgin,&matDirect);
   return result;
}

HRESULT CSprite::Render(LPDIRECT3DDEVICE9 pd3dDevice, CFOVClipper* pClipper)
{
   if(m_SpriteState != SPRITE_DEAD && !m_bInvisable)
   {
      if(pClipper == NULL || pClipper->MeshFOVCheck(GetPos(),GetRadius()))
      {
         //pd3dDevice->SetTransform( D3DTS_WORLD, GetMatrix() ); mesh file dose this

         if(m_pMesh)
         {
            if(m_iKeyFrame != -1)
               m_pMesh->setKey(m_iKeyFrame);
            else
               m_pMesh->setTime(-1,-1,m_fCurTime);

            if(m_pCurrentTexture)
            {
               pd3dDevice->SetTexture(0,m_pCurrentTexture);
               m_pMesh->render(pd3dDevice,&GetMatrix(),FALSE);
            }
            else
               m_pMesh->render(pd3dDevice,&GetMatrix());
         }
      }
   }
   return S_OK;

}

HRESULT CSprite::RenderShadow(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   if(m_SpriteState != SPRITE_DEAD && !m_bInvisable)
   {
      return CDisplayObject::RenderShadow(pd3dDevice,pClipper);
   }
   return S_OK;

}

void CSprite::BuildShadow(const D3DXVECTOR3& lightpos)
{
   if(m_SpriteState != SPRITE_DEAD)
   {
      // Build the shadow volume

      if(m_pShadowVolume == NULL)
      {
         m_pShadowVolume = new CShadowVolume();
      }

      if(m_pShadowVolume && m_pMesh)
      {
         m_pShadowVolume->Reset();
         if(m_iKeyFrame != -1)
            m_pMesh->setKey(m_iKeyFrame);
         else
            m_pMesh->setTime(-1,-1,m_fCurTime);
         m_pShadowVolume->BuildFromMeshFile( m_pMesh, lightpos, GetMatrix(), FALSE );
      }
   }
}

HRESULT CSprite::RenderClear(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper,CD3DCamera * pCamera)
{
   if(pClipper->MeshFOVCheck(GetPos(),GetRadius()) && m_SpriteState != SPRITE_DEAD)
   {
      if(m_bShowShield)
      {
         pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
         pd3dDevice->SetTexture( 0, NULL );
         pd3dDevice->SetMaterial( &m_ShieldMat );
         pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
         pd3dDevice->SetRenderState( D3DRS_SRCBLEND,     D3DBLEND_DESTCOLOR);//D3DBLEND_DESTCOLOR );
         pd3dDevice->SetRenderState( D3DRS_DESTBLEND,    D3DBLEND_SRCCOLOR);//D3DBLEND_SRCCOLOR );
         //pd3dDevice->SetTransform( D3DTS_WORLD,  &m_ShieldMatrix);
         if(m_pShield)
            m_pShield->render(pd3dDevice,&m_ShieldMatrix);
         pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
         pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        TRUE );
      }

      if(!m_bInvisable)
         m_pExaust->Render(pd3dDevice,pCamera,pClipper);

   }	
   return S_OK;
}



#define BAR_SIZE 90.0f

#define OFFSET_SIZE 25

void CSprite::UpdateHud(LPDIRECT3DDEVICE9 pd3dDevice,CD3DCamera* pCamera,C2DDraw* pDraw)
{
   if(m_pTarget)
   {
      sys_verify_hr(m_HudDisplay.BeginScene());

      sys_verify_hr(pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
         0x00000000, 1.0f, 0L ));

      D3DXMATRIX matProj;
      FLOAT fAspect = 128.0f/128.0f;
      D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4.0f, fAspect, 1.0f, FOV_BACK_DISTACNE+128.0f );
      pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

      D3DXVECTOR3 vEyePt = GetPos();
      D3DXVECTOR3 vLookatPt = m_pTarget->GetPos() + m_pTarget->GetCenterPoint();

      D3DXVECTOR3 vtemp = vEyePt - vLookatPt;

      D3DXVECTOR3 v;
      D3DXVec3Subtract(&v,&GetPos(),&m_pTarget->GetPos());
      FLOAT Distance2 = D3DXVec3Length(&v);

      FLOAT scale = (3.0f+m_pTarget->GetRadius())/Distance2;
      D3DXVec3Scale(&vtemp,&vtemp, scale);

      vtemp+=vLookatPt;
      D3DXMATRIX TmpCamaraMatrix = pCamera->GetViewMatrix();
      pCamera->SetViewParams( vtemp, vLookatPt, D3DXVECTOR3(0,1,0));

      pd3dDevice->SetTransform( D3DTS_VIEW, &pCamera->GetViewMatrix());

      pCamera->SetViewMatrix(&TmpCamaraMatrix);

      m_pTarget->RenderSimple(pd3dDevice);
      m_HudDisplay.EndScene();

   }
}

void CSprite::DisplayStats(LPDIRECT3DDEVICE9 pd3dDevice,CD3DFont* pFont,C2DDraw* pDraw,CTerrain* pTerrain,SpriteList* pSpriteList,ObjectList* pObjectList, BOOL bFirstPerson,CD3DCamera* pCamera)
{

   CPoint HudPos1(0,pDraw->GetScreenHight()-101);
   pDraw->DrawBlendQuad(HudPos1,
      CPoint(HudPos1.x,HudPos1.y+100),
      CPoint(HudPos1.x+100,HudPos1.y),
      CPoint(HudPos1.x+100+OFFSET_SIZE,HudPos1.y+100),
      D3DCOLOR_ARGB(100,0,0,0));

   pDraw->DrawQuadOutline(HudPos1,
      CPoint(HudPos1.x,HudPos1.y+100),
      CPoint(HudPos1.x+100,HudPos1.y),
      CPoint(HudPos1.x+100+OFFSET_SIZE,HudPos1.y+100),
      D3DCOLOR_ARGB(0,0,94,231));
   HudPos1.Offset(1,1);
   pDraw->DrawQuadOutline(HudPos1,
      CPoint(HudPos1.x,HudPos1.y+98),
      CPoint(HudPos1.x+98,HudPos1.y),
      CPoint(HudPos1.x+98+OFFSET_SIZE,HudPos1.y+98),
      D3DCOLOR_ARGB(0,63,151,255));
   HudPos1.Offset(1,1);
   pDraw->DrawQuadOutline(HudPos1,
      CPoint(HudPos1.x,HudPos1.y+96),
      CPoint(HudPos1.x+96,HudPos1.y),
      CPoint(HudPos1.x+96+OFFSET_SIZE,HudPos1.y+96),
      D3DCOLOR_ARGB(0,3,93,236));

   CPoint HudPos2(pDraw->GetScreenWidth() - 101, pDraw->GetScreenHight() - 101);
   pDraw->DrawBlendQuad(HudPos2,
      CPoint(HudPos2.x-OFFSET_SIZE,HudPos2.y+100),
      CPoint(HudPos2.x+100,HudPos2.y),
      CPoint(HudPos2.x+100,HudPos2.y+100),
      D3DCOLOR_ARGB(100,0,0,0));

   pDraw->DrawQuadOutline(HudPos2,
      CPoint(HudPos2.x-OFFSET_SIZE,HudPos2.y+100),
      CPoint(HudPos2.x+100,HudPos2.y),
      CPoint(HudPos2.x+100,HudPos2.y+100),
      D3DCOLOR_ARGB(0,0,94,231));
   HudPos2.Offset(1,1);
   pDraw->DrawQuadOutline(HudPos2,
      CPoint(HudPos2.x-OFFSET_SIZE,HudPos2.y+98),
      CPoint(HudPos2.x+98,HudPos2.y),
      CPoint(HudPos2.x+98,HudPos2.y+98),
      D3DCOLOR_ARGB(0,63,151,255));
   HudPos2.Offset(1,1);
   pDraw->DrawQuadOutline(HudPos2,
      CPoint(HudPos2.x-OFFSET_SIZE,HudPos2.y+96),
      CPoint(HudPos2.x+96,HudPos2.y),
      CPoint(HudPos2.x+96,HudPos2.y+96),
      D3DCOLOR_ARGB(0,3,93,236));

   CPoint HudPos3(pDraw->GetScreenWidth()/2 - 76, pDraw->GetScreenHight() - 129);
   pDraw->DrawBlendQuad(HudPos3,
      CPoint(HudPos3.x-OFFSET_SIZE,HudPos3.y+150),
      CPoint(HudPos3.x+150,HudPos3.y),
      CPoint(HudPos3.x+150+OFFSET_SIZE,HudPos3.y+150),
      D3DCOLOR_ARGB(100,0,0,0));


   pDraw->DrawQuadOutline(HudPos3,
      CPoint(HudPos3.x-OFFSET_SIZE,HudPos3.y+150),
      CPoint(HudPos3.x+150,HudPos3.y),
      CPoint(HudPos3.x+150+OFFSET_SIZE,HudPos3.y+150),
      D3DCOLOR_ARGB(0,0,94,231));
   HudPos3.Offset(1,1);
   pDraw->DrawQuadOutline(HudPos3,
      CPoint(HudPos3.x-OFFSET_SIZE,HudPos3.y+148),
      CPoint(HudPos3.x+148,HudPos3.y),
      CPoint(HudPos3.x+148+OFFSET_SIZE,HudPos3.y+148),
      D3DCOLOR_ARGB(0,63,151,255));
   HudPos3.Offset(1,1);
   pDraw->DrawQuadOutline(HudPos3,
      CPoint(HudPos3.x-OFFSET_SIZE,HudPos3.y+146),
      CPoint(HudPos3.x+146,HudPos3.y),
      CPoint(HudPos3.x+146+OFFSET_SIZE,HudPos3.y+146)
      ,D3DCOLOR_ARGB(0,3,93,236));

   if(m_pTarget)
   {

      CPoint HudDisplay(11,1);
      HudDisplay.Offset(HudPos3);
      m_HudDisplay.Draw(pd3dDevice,HudDisplay.x,HudDisplay.y,NULL,FALSE);


      CRect r = pDraw->GetTextRect(m_pTarget->GetName());
      CPoint TextPoint3(75-r.CenterPoint().x,5-r.CenterPoint().y);
      TextPoint3.Offset(HudPos3);
      pDraw->OutTextc(0,m_pTarget->GetTeamColour(),TextPoint3.x,TextPoint3.y,TRUE,"%s",m_pTarget->GetName());


      if(m_pTarget->IsSprite())
      {
         CSprite* pTargetSprite = static_cast<CSprite*>(m_pTarget);
         CPoint TextPoint(0,15);
         TextPoint.Offset(HudPos3);
         pDraw->OutTextc(0,D3DCOLOR_XRGB(0,0,255),TextPoint.x,TextPoint.y,TRUE,"SHD%%\n%2.0f",pTargetSprite->m_fShield_Str>0?pTargetSprite->m_fShield_Str/pTargetSprite->m_fMaxShields * 100.0f:0.0f);

         CPoint TextPoint1(-10,70);
         TextPoint1.Offset(HudPos3);
         pDraw->OutTextc(0,D3DCOLOR_XRGB(0,0,255),TextPoint1.x,TextPoint1.y,TRUE,"HULL%%\n%2.0f",(pTargetSprite->m_fDamage/pTargetSprite->m_fMaxDamage)*100.0f);
      }

      D3DXVECTOR3 v;
      D3DXVECTOR3 point = m_pTarget->GetPos();
      D3DXVec3Subtract(&v,&GetPos(),&point);
      FLOAT Distance2 = D3DXVec3Length(&v);

      CPoint TextPoint2(130,70);
      TextPoint2.Offset(HudPos3);
      pDraw->OutTextc(0,D3DCOLOR_XRGB(0,0,255),TextPoint2.x,TextPoint2.y,TRUE,"DIST\n% 4.2f",Distance2);


      CPoint TextPoint4(120,15);
      TextPoint4.Offset(HudPos3);
      pDraw->OutTextc(0,D3DCOLOR_XRGB(0,0,255),TextPoint4.x,TextPoint4.y,TRUE,"SPD\n% 4.2f",m_pTarget->GetSpeed());
      CRect TargetRec(HudPos3,CSize(150,100));
   }

   {
      CString FragString;
      FragString.Format("Frags %d",m_FragCount);
      CRect r = pDraw->GetTextRect(FragString);

      CPoint TextPoint5(75-r.CenterPoint().x,120-r.CenterPoint().y);
      TextPoint5.Offset(HudPos3);
      pDraw->OutTextc(0,D3DCOLOR_XRGB(0,0,255),TextPoint5.x,TextPoint5.y,TRUE,"%s",FragString);
   }




#define COLOUR_TO D3DCOLOR_XRGB(0,0,0)

#define TOPBAR_FLAG GR_TR|GR_TL
#define BOTTOMBAR_FLAG GR_BR|GR_BL

   CPoint TextPoint(5,5);
   TextPoint.Offset(HudPos2);
   pDraw->OutTextc(0,D3DCOLOR_XRGB(0,255,0),TextPoint.x,TextPoint.y,TRUE,"Shields");


   FLOAT ShieldPos = m_fShield_Str>0?m_fShield_Str/m_fMaxShields * BAR_SIZE:0;
   // Draw Shield Bar
   CRect rec(CPoint(5, 20),CSize(ShieldPos,5));
   rec.OffsetRect(HudPos2);
   pDraw->DrawGradientRect(rec,D3DCOLOR_XRGB(0,255,0),COLOUR_TO,TOPBAR_FLAG);

   CRect rec6(CPoint(5, 25),CSize(ShieldPos,5));
   rec6.OffsetRect(HudPos2);
   pDraw->DrawGradientRect(rec6,D3DCOLOR_XRGB(0,255,0),COLOUR_TO,BOTTOMBAR_FLAG);

   // Draw Rect around Shield Bar
   CRect rec1(CPoint(5, 20),CSize(BAR_SIZE,10));
   rec1.OffsetRect(HudPos2);
   pDraw->DrawRect(rec1,D3DCOLOR_XRGB(0,0,0));



   FLOAT DamagePos = m_fDamage/m_fMaxDamage * BAR_SIZE;
   CPoint TextPoint2(5,35);
   TextPoint2.Offset(HudPos2);
   pDraw->OutTextc(0,D3DCOLOR_XRGB(255,0,0),TextPoint2.x,TextPoint2.y,TRUE,"Hull");
   // Draw damage bar
   CRect rec2(CPoint(5, 50),CSize(DamagePos,5));
   rec2.OffsetRect(HudPos2);
   pDraw->DrawGradientRect(rec2,D3DCOLOR_XRGB(255,0,0),COLOUR_TO,TOPBAR_FLAG);
   CRect rec7(CPoint(5, 55),CSize(DamagePos,5));
   rec7.OffsetRect(HudPos2);
   pDraw->DrawGradientRect(rec7,D3DCOLOR_XRGB(255,0,0),COLOUR_TO,BOTTOMBAR_FLAG);

   CRect rec3(CPoint(5, 50),CSize(BAR_SIZE,10));
   rec3.OffsetRect(HudPos2);
   pDraw->DrawRect(rec3,D3DCOLOR_XRGB(0,0,0));



   CPoint TextPoint3(5,65);
   TextPoint3.Offset(HudPos2);



   pDraw->OutTextc(0,D3DCOLOR_XRGB(255,255,0),TextPoint3.x,TextPoint3.y,TRUE,"Speed %2.0f%%",m_Speed.x/m_MaxSpeed * 100.0f);

   // draw speed bar
   FLOAT Speed = 0.0f;

   if(m_MaxSpeed != 0.0f)
   {
      Speed = ((m_Speed.x-m_fMinSpeed)/(m_MaxSpeed-m_fMinSpeed)) * BAR_SIZE;
   }

   CRect rec4(CPoint(5, 80),CSize(Speed,5));
   rec4.OffsetRect(HudPos2);
   pDraw->DrawGradientRect(rec4,D3DCOLOR_XRGB(255,255,0),COLOUR_TO,TOPBAR_FLAG);
   CRect rec8(CPoint(5, 85),CSize(Speed,5));
   rec8.OffsetRect(HudPos2);
   pDraw->DrawGradientRect(rec8,D3DCOLOR_XRGB(255,255,0),COLOUR_TO,BOTTOMBAR_FLAG);

   CRect rec5(CPoint(5, 80),CSize(BAR_SIZE,10));
   rec5.OffsetRect(HudPos2);
   pDraw->DrawRect(rec5,D3DCOLOR_XRGB(0,0,0));


   CPoint MapPos(50,50);
   MapPos+=HudPos1;
   pTerrain->RenderMiniMap(pd3dDevice,MapPos,-m_angleY - HALFPI);




   for(UINT i=0;i< pSpriteList->size(); i++)
   {
      if((*pSpriteList)[i]->IsAlive() && (*pSpriteList)[i] != this)
      {
         FLOAT x = (*pSpriteList)[i]->GetPos().x;
         FLOAT y = (*pSpriteList)[i]->GetPos().z;
         pTerrain->ConvertToMiniMapPoint(x,y,-m_angleY - HALFPI);

         CRect rec(CPoint(x,y) + MapPos ,CSize(3,3));

         DWORD colour = (*pSpriteList)[i]->GetTeamColour();

         if(m_pTarget == (*pSpriteList)[i])
         {
            colour = D3DCOLOR_XRGB(255,255,0);
         }

         pDraw->DrawFilledRect(rec,colour);
      }
   }

   {
      FLOAT x = GetPos().x;
      FLOAT y = GetPos().z;
      pTerrain->ConvertToMiniMapPoint(x,y,-m_angleY - HALFPI);
      CRect rec(CPoint(x,y) + MapPos ,CSize(3,3));
      DWORD colour = D3DCOLOR_XRGB(255,255,255);
      pDraw->DrawFilledRect(rec,colour);
   }

   for(i=0;i< pObjectList->size(); i++)
   {
      if((*pObjectList)[i]->GetType() == OBJECTTYPE_WAYPOINT)
      {
         if(static_cast<CWayPoint*>((*pObjectList)[i])->IsVisable())
         {
            FLOAT x = (*pObjectList)[i]->GetPos().x;
            FLOAT y = (*pObjectList)[i]->GetPos().z;
            pTerrain->ConvertToMiniMapPoint(x,y,-m_angleY - HALFPI);

            CRect rec(CPoint(x,y) + MapPos ,CSize(3,3));

            DWORD colour = D3DCOLOR_XRGB(0,0,255);

            if(m_pTarget == (*pObjectList)[i])
            {
               colour = D3DCOLOR_XRGB(255,255,0);
            }

            pDraw->DrawFilledRect(rec,colour);
         }
      }
   }

}

CSprite* CSprite::GetClosestSprite(SpriteList* pSpriteList,FLOAT& Distance,BOOL bSameTeam)
{
   CSprite* pClosest = NULL;
   for(UINT i=0;i<pSpriteList->size();i++)
   {
      if((*pSpriteList)[i]->IsInvisable())
         continue;
      if((*pSpriteList)[i] == this)
         continue;
      if((GetTeamNo() != (*pSpriteList)[i]->GetTeamNo()) && bSameTeam)
         continue;
      else if((GetTeamNo() == (*pSpriteList)[i]->GetTeamNo()) && !bSameTeam)
         continue;

      D3DXVECTOR3 v;
      D3DXVec3Subtract(&v,&GetPos(),&(*pSpriteList)[i]->GetPos());
      FLOAT Distance2 = D3DXVec3Length(&v);

      if(Distance2 < Distance)
      {
         Distance = Distance2;
         pClosest = (*pSpriteList)[i];
      }
   }
   return pClosest;
}

CDisplayObject* CSprite::GetGoal(SpriteList* pSpriteList,ObjectList* pObjectList)
{
   CDisplayObject* pClosest = NULL;
   if(!m_GoalName.IsEmpty())
   {
      for(UINT i=0;i<pSpriteList->size();i++)
      {
         if((*pSpriteList)[i]->IsInvisable())
            continue;
         if((*pSpriteList)[i] == this)
            continue;

         if(m_GoalName == (*pSpriteList)[i]->GetName())
         {
            pClosest = (*pSpriteList)[i];
            break;
         }
      }

      for(i=0;i<pObjectList->size();i++)
      {
         if(m_GoalName == (*pObjectList)[i]->GetName())
         {
            if(!(*pObjectList)[i]->IsVisable())
               continue;
            pClosest = (*pObjectList)[i];
            break;
         }
      }
   }
   return pClosest;
}

void CSprite::ClearTarget()
{
   m_pTarget = NULL;
}

void CSprite::GetFullPlayerMsg(GAMEMSG_FULL_PLAYER_UPDATE *pMsg)
{
   pMsg->m_PlayerShipType = m_Type;
   pMsg->m_TeamNo = m_TeamNo;
   pMsg->m_dpID = m_ID;
   pMsg->m_bBot = FALSE;
   ::_snprintf(pMsg->m_Name,MAX_CHAT_STRING_LENGTH,GetName());
   GetPlayerMsg(pMsg);
}

void CSprite::GetPlayerMsg(GAMEMSG_PLAYER_UPDATE *pMsg)
{
   pMsg->m_SpriteState = m_SpriteState;
   pMsg->m_fShield_Str= m_fShield_Str;
   pMsg->m_fDamage= m_fDamage;
   pMsg->m_Speed= m_Speed;
   pMsg->m_Pos= GetPos();
   pMsg->m_angleX= m_angleX;
   pMsg->m_angleY= m_angleY;
   pMsg->m_angleZ= m_angleZ;
   pMsg->m_bInvisable = m_bInvisable;
   pMsg->m_bInvurnable = m_bInvrunable;
}

void CSprite::SetPlayerMsg(GAMEMSG_PLAYER_UPDATE *pMsg)
{
   m_SpriteState =pMsg->m_SpriteState;
   m_fShield_Str = pMsg->m_fShield_Str;
   m_fDamage = pMsg->m_fDamage;
   m_Speed =pMsg->m_Speed;
   SetPos(&pMsg->m_Pos);
   m_angleX = pMsg->m_angleX;
   m_angleY = pMsg->m_angleY;
   m_angleZ = pMsg->m_angleZ;

   m_bInvisable = pMsg->m_bInvisable;
   m_bInvrunable = pMsg->m_bInvurnable;
}


HRESULT CSprite::RenderSimple(LPDIRECT3DDEVICE9 pd3dDevice)
{
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      FALSE );

   if(m_pMesh)
   {
      if(m_iKeyFrame != -1)
         m_pMesh->setKey(m_iKeyFrame);
      else
         m_pMesh->setTime(-1,-1,m_fCurTime);

      if(m_pCurrentTexture)
      {
         pd3dDevice->SetTexture(0,m_pCurrentTexture);
         m_pMesh->render(pd3dDevice,&GetMatrix(),FALSE);
      }
      else
         m_pMesh->render(pd3dDevice,&GetMatrix());
   }

   pd3dDevice->SetTexture( 0, NULL );

   pd3dDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
   return S_OK;
}

void CSprite::DoCollision(CDisplayObject* pObj, std::vector<CGameAction>* pActions,CChatDlg* pChatDlg)
{
   // do nothing right now
}

void CSprite::AddPowerUp(CPowerUpData* pPowerUp,const TCHAR* Name,CChatDlg* pChatDlg)
{
   switch(pPowerUp->m_Type)
   {
   case PU_STATS:
      m_fDamage += m_fMaxDamage * pPowerUp->m_Health;
      if(m_fDamage > m_fMaxDamage)
         m_fDamage = m_fMaxDamage;

      if(pPowerUp->m_Shields != 0.0f && m_fShield_Str<0.0f)
         m_fShield_Str = 0.0f;

      m_fShield_Str += m_fMaxShields * pPowerUp->m_Shields;
      if(m_fShield_Str > m_fMaxShields)
         m_fShield_Str = m_fMaxShields;

      if(m_OldMaxSpeed == 0.0f)
      {
         m_OldMaxSpeed = m_MaxSpeed;
      }
      else
      {
         m_MaxSpeed = m_OldMaxSpeed;
      }
      m_MaxSpeed += m_MaxSpeed * pPowerUp->m_MaxSpeed;

      m_WeaponPowerMod = pPowerUp->m_WeaponPower;

      break;
   case PU_INVULNERABILITY:
      m_bInvrunable = TRUE;
      break;
   case PU_INVISIBILITY:
      m_bInvisable = TRUE;
      break;
   }

   if(pPowerUp->m_Duration != 0)
   {
      pPowerUp->m_StartTime = 0.0f;
      m_PowerUpList.push_back(*pPowerUp);
   }
}

void CSprite::HitWaypoint(CString Name,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg,BOOL bVisable)
{
   CGameAction action;
   action.m_EventType = EVENT_HIT_WAYPOINT;
   action.m_ObjectName = Name;
   action.m_ResponsName = GetName();
   pActions->push_back(action);
}

CDisplayObject* CSprite::GetNextTarget(CDisplayObject *pTarget,SpriteList* pSpriteList,ObjectList* pObjectList)
{
   CDisplayObject* pNewTarget = NULL;

   for(UINT i=1;i<pSpriteList->size();i++)
   {
      CDisplayObject* pTempObj = (*pSpriteList)[i];
      if(pTarget == NULL)
      {
         pNewTarget = pTempObj;
         break;
      }

      if(pTempObj == pTarget)
      {
         for(UINT index = i+1;index < pSpriteList->size();index++)
         {
            if(!(*pSpriteList)[index]->IsInvisable())
            {
               pNewTarget = (*pSpriteList)[index];
               break;
            }

         }

         if(pNewTarget == NULL)
         {
            for(index = 0;index < pObjectList->size();index++)
            {
               if((*pObjectList)[i]->IsVisable())
               {
                  if((*pObjectList)[index]->GetType() == OBJECTTYPE_POWERUP || (*pObjectList)[index]->GetType() == OBJECTTYPE_WAYPOINT)
                  {
                     pNewTarget = (*pObjectList)[index];
                     break;
                  }
               }
            }
         }
         break;
      }
   }

   if(pNewTarget == NULL)
   {
      for(i=0;i<pObjectList->size();i++)
      {

         if(pTarget == NULL)
         {
            if((*pObjectList)[i]->GetType() == OBJECTTYPE_POWERUP || (*pObjectList)[i]->GetType() == OBJECTTYPE_WAYPOINT)
            {
               pNewTarget = (*pObjectList)[i];
               break;
            }
            continue;
         }

         if((*pObjectList)[i] == pTarget)
         {

            for(UINT index = i+1;index < pObjectList->size();index++)
            {
               if((*pObjectList)[index]->IsVisable())
               {
                  if((*pObjectList)[index]->GetType() == OBJECTTYPE_POWERUP || (*pObjectList)[index]->GetType() == OBJECTTYPE_WAYPOINT)
                  {
                     pNewTarget = (*pObjectList)[index];
                     break;
                  }
               }
            }


            if(pNewTarget == NULL && pSpriteList->size() > 1)
            {
               for(index = 1;index < pSpriteList->size();index++)
               {
                  if(!(*pSpriteList)[index]->IsInvisable())
                  {
                     pNewTarget = (*pSpriteList)[index];
                     break;
                  }	
               }
            }
            else if(pNewTarget == NULL)
            {
               for(index = 0;index < pObjectList->size();index++)
               {
                  if((*pObjectList)[index]->IsVisable())
                  {
                     if((*pObjectList)[index]->GetType() == OBJECTTYPE_POWERUP || (*pObjectList)[index]->GetType() == OBJECTTYPE_WAYPOINT)
                     {
                        pNewTarget = (*pObjectList)[index];
                        break;
                     }
                  }

               }
            }

            break;
         }
      }
   }

   if(pNewTarget == NULL) // we could not find a target set it back to the old one
      pNewTarget = pTarget;

   return pNewTarget;
}

BOOL CSprite::GetGunPortPos(int port,D3DXVECTOR3& pPos)
{
   pPos = D3DXVECTOR3(0,0,0);
   CString portname;
   portname.Format("GunPort%d",port);
   if(m_pMesh)
      return m_pMesh->findFramePos(portname,&pPos, &GetMatrix());
   return FALSE;

}

BOOL CSprite::GetExaustPortPos(int port,D3DXVECTOR3& pPos)
{
   pPos = D3DXVECTOR3(0,0,0);
   CString portname;
   portname.Format("Exaust%d",port);
   if(m_pMesh)
      return m_pMesh->findFramePos(portname,&pPos,&GetMatrix());
   return FALSE;
}
