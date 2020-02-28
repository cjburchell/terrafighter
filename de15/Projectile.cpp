// Projectile.cpp: implementation of the CProjectile class.
//
//////////////////////////////////////////////////////////////////////
#include <afx.h>
#include "Projectile.h"
#include "SayDlg.h"
#include "deapp.h"
#include <d3dutil.h>

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

void CProjectile::SeekPlayer(CPlayer* pPlayer)
{
	ClearDirection();

	//if(pPlayer->GetDrawPoint().x>(m_pos.x)) 
		//m_SpeedDir[EAST_WEST]=PRO_SPEED;
	//else
		//m_SpeedDir[EAST_WEST]= -PRO_SPEED;

	//if(pPlayer->GetDrawPoint().y<(m_pos.y))
		//m_SpeedDir[NORTH_SOUTH]= -PRO_SPEED;
	//else
		//m_SpeedDir[NORTH_SOUTH]= PRO_SPEED;

	SeekPoint(pPlayer->GetCenterPoint());
	
}

BOOL CProjectile::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	Attack(player,pDisplay);
	return FALSE;
}

void CProjectile::MoveThisWay(double Angle)
{
	m_Angle = ToRad(Angle);
	//if((F_WEST&dirflag)!=0) m_SpeedDir[EAST_WEST]=PRO_SPEED;
	//if((F_EAST&dirflag)!=0) m_SpeedDir[EAST_WEST]= -PRO_SPEED;
	//if((F_NORTH&dirflag)!=0) m_SpeedDir[NORTH_SOUTH]= -PRO_SPEED;
	//if((F_SOUTH&dirflag)!=0) m_SpeedDir[NORTH_SOUTH]= PRO_SPEED;
	MoveOneSpace();
	
}

void CProjectile::MoveOneSpace()
{
	MoveToGoTo(42,NULL);
	m_move_state = MOVING;
}

void CProjectile::ClearDirection()
{
	//m_SpeedDir[EAST_WEST]=0;
	//m_SpeedDir[NORTH_SOUTH]=0;
}

void CProjectile::DoMove(CPlayer *player, IGameMap *pMap, C2DDraw *pDisplay)
{
	#define VELOCITY (((32.0/1000.0)*2.0)* PRO_SPEED)
	double r = VELOCITY * pMap->GetElapsedTime();
	MoveToGoTo(r,NULL);
	m_move_state = MOVING;
}

BOOL CProjectile::HitStaticObject()
{
	return FALSE;
}

BOOL CProjectile::HitOtherMonster(CMonster* pMonster,C2DDraw* pDisplay,IGameMap* pMap, BOOL& IsDead)
{
	IsDead = FALSE;
	if(pMonster->m_npctype == MON || pMonster->m_npctype ==ENDMON||pMonster->m_npctype ==FIRE||pMonster->m_npctype == LIT)
	{
		if(!Attack(pMonster,pDisplay))
		{
			if(m_pOwner->IsPlayer())
			{
				CPlayer* pPlayer = static_cast<CPlayer*>(m_pOwner);
				pPlayer->xp  += pMonster->m_xp;
				pPlayer->gold +=pMonster->m_gold;
				pPlayer->levelck(pDisplay,pMap);

				CPoint p = pMonster->GetDrawPoint();
				pMap->CreateChest(p,pMonster->m_gold);

				if(pMonster->m_npctype==ENDMON)
				{
					CSayDlg saydlg;
					saydlg.Create(pDisplay,CPoint(10,10),"You have defeted the game");
					saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
					pPlayer->PlayerOK=FALSE;
				}
			}
			IsDead = TRUE;
		}
	}
	else if(pMonster->m_npctype == PRO)
		IsDead = TRUE;
	return FALSE;
}

BOOL CProjectile::HitPlayer(CPlayer* player, C2DDraw* pDisplay)
{
	Attack(player,pDisplay);
	return FALSE;
}

void CProjectile::SeekPoint(CPoint p)
{
	CMonster::SeekPoint(p,NULL);
	MoveOneSpace();
}
