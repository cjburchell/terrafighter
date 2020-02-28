// Weapon.h: interface for the CWeapon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPON_H__4F401219_BE3E_4954_8223_BB5E28125385__INCLUDED_)
#define AFX_WEAPON_H__4F401219_BE3E_4954_8223_BB5E28125385__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning( disable : 4786 )
#include <list>
#include "ParticleSystem.h"
#include "Sprite.h"
#include "DSUtil.h"
#include "NetworkConnection.h"
#include "SpriteData.h"

class CSprite;

class CBeam : public CBeamData
{
public:
   CBeam();
   virtual ~CBeam();
   BOOL Fire(CSprite* pSprite, CDisplayObject* pTarget,CParticleSystem* pParticleSystem, BOOL bCurrentPlayer,CNetworkConnection* pNetConnect,FLOAT WeaponMod);
   BOOL Create(CSoundManager* pSoundMgr);
   FLOAT GetSpeed(){return m_fSpeed;}
private:
   CSound* m_pSoundShot;

};

#define NO_WEAPON_SEL -1

class CWeapon : public CWeaponData
{
public:
   CWeapon();
   virtual ~CWeapon();
   BOOL Create(CSoundManager* pSoundMgr,MSXML2::IXMLDOMNodePtr RowItem);
   BOOL Fire(CSprite* pSprite,CDisplayObject* pTarget, FLOAT LastShot, CParticleSystem* pParticleSystem,BOOL bCurrentPlayer,CNetworkConnection* pNetConnect,FLOAT WeaponMod);
   CBeam* GetBeam(DWORD beam)
   {		
      return (CBeam*)GetBeamData(beam);
   }

   virtual CBeamData* CreateBeam()
   {
      return new CBeam;
   }
};

#endif // !defined(AFX_WEAPON_H__4F401219_BE3E_4954_8223_BB5E28125385__INCLUDED_)
