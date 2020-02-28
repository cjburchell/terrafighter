// Projectile.cpp: implementation of the CProjectile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Projectile.h"

#define PRO_SPEED 3



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjectile::CProjectile(CSprite* pOwner) : CMonster()
{
	m_pOwner = pOwner;
}

CProjectile::~CProjectile()
{

}
