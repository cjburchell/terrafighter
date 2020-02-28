// Projectile.h: interface for the CProjectile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTILE_H__6C1CFBF2_A877_4EA5_B673_2B75CEF3D12C__INCLUDED_)
#define AFX_PROJECTILE_H__6C1CFBF2_A877_4EA5_B673_2B75CEF3D12C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"

class CProjectile : public CMonster  
{
public:
	CProjectile(CSprite* pOwner);
	virtual ~CProjectile();
protected:
	CSprite* m_pOwner;
};

#endif // !defined(AFX_PROJECTILE_H__6C1CFBF2_A877_4EA5_B673_2B75CEF3D12C__INCLUDED_)