// Weapon.cpp: implementation of the CWeapon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "terrafighterapp.h"
#include "Weapon.h"
#include "ShotSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeapon::CWeapon() : CWeaponData()
{

}

CWeapon::~CWeapon()
{
}

BOOL CWeapon::Create(CSoundManager* pSoundMgr,MSXML2::IXMLDOMNodePtr RowItem)
{
   Load(RowItem);

   for(UINT i=0;i<m_Beams.size();i++) // for each row
   {
      // get the row
      GetBeam(i)->Create(pSoundMgr);
   }

   return TRUE;
}

BOOL CWeapon::Fire(CSprite* pSprite,CDisplayObject* pTarget, FLOAT LastShot, CParticleSystem* pParticleSystem, BOOL bCurrentPlayer,CNetworkConnection* pNetConnect,FLOAT WeaponMod)
{
   if(LastShot> m_RateOfFire)
   {
      for(UINT i = 0;i<m_Beams.size();i++)
      {
         GetBeam(i)->Fire(pSprite,pTarget,pParticleSystem, bCurrentPlayer,pNetConnect,WeaponMod);
      }
      return TRUE;
   }
   else
      return FALSE;
}


CBeam::CBeam()
{
   m_pSoundShot = NULL;
}

CBeam::~CBeam()
{
   SAFE_DELETE( m_pSoundShot );
}

BOOL CBeam::Create(CSoundManager* pSoundMgr)
{	
   pSoundMgr->Create( &m_pSoundShot, m_SoundFile,&GetApp()->m_Zip, DSBCAPS_CTRL3D, DS3DALG_NO_VIRTUALIZATION,2);  
   return TRUE;
}

BOOL CBeam::Fire(CSprite* pSprite,CDisplayObject* pTarget,CParticleSystem* pParticleSystem, BOOL bCurrentPlayer,CNetworkConnection* pNetConnect,FLOAT WeaponMod)
{
   D3DXVECTOR3 vVelocity;


   D3DXVECTOR3 PortPos; 
   BOOL bFound = pSprite->GetGunPortPos(m_Port,PortPos);
   if(bFound)
   {

      if(m_bSeek && pTarget)
      {
         D3DXVECTOR3 v;
         D3DXVECTOR3 vTargetPos = pTarget->GetCenterPoint()+ pTarget->GetPos();
         D3DXVec3Subtract(&v,&pSprite->GetPos(),&vTargetPos);
         FLOAT Distance = D3DXVec3Length(&v);
         D3DXVec3Scale(&vVelocity,&v,1.0f/Distance * -m_fSpeed);
      }
      else
      {
         vVelocity = pSprite->GetDirectionVector(m_fSpeed+pSprite->GetSpeed());
      }


      FLOAT FullDamage = m_fDamage + m_fDamage*WeaponMod;


      static_cast<CShotSystem*>(pParticleSystem)->EmitNewParticles(1,
         m_Colour1,
         m_Colour2,
         PortPos, 
         vVelocity,
         pSprite,
         FullDamage,
         m_fGravity,
         m_fSize);

      if(pNetConnect)
      {
         GAMEMSG_SHOT_FIRED Msg;
         Msg.nType = GAME_MSGID_SHOT_FIRED;
         Msg.dwNumParticlesToEmit = 1;
         Msg.dwEmitColor = m_Colour1;
         Msg.dwFadeColor = m_Colour2;
         Msg.vPosition = PortPos;
         Msg.vEmitVel = vVelocity;
         Msg.fPower = FullDamage;
         Msg.fGravity = m_fGravity;
         Msg.fSize = m_fSize;
         pNetConnect->SendMessageToAll(sizeof(GAMEMSG_SHOT_FIRED),(BYTE*)&Msg);
      }

      if(m_pSoundShot)
      {
         if(bCurrentPlayer)
            m_pSoundShot->Play3D(DS3DMODE_HEADRELATIVE,vVelocity,D3DXVECTOR3(0,0,0));
         else
            m_pSoundShot->Play3D(DS3DMODE_NORMAL,vVelocity,PortPos);
      }
   }

   return TRUE;
}
