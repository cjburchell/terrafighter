// EndMonster.cpp: implementation of the CEndMonster class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include "EndMonster.h"
#include "SayDlg.h"
#include "deapp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEndMonster::CEndMonster() :CMonster()
{

}

CEndMonster::~CEndMonster()
{

}

BOOL CEndMonster::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	if(!CMonster::Use(player, pDisplay, pMap))
	{
		CSayDlg saydlg;
		saydlg.Create(pDisplay,CPoint(10,10),"You have defeted the game");
		saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
		//PlaySound("Drumc0.wav", NULL, SND_SYNC);
		player->PlayerOK=FALSE;
		return FALSE;
	}
	return TRUE;
}
