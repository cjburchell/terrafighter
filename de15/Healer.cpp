// Healer.cpp: implementation of the CHealer class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include "Healer.h"
#include "YesNoDlg.h"
#include "saydlg.h"
#include "deapp.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
CHealer::CHealer() :CNPC()
{

}

CHealer::~CHealer()
{

}

BOOL CHealer::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	player->MoveBack();
	CYesNoDlg dlg;
	dlg.Create(pDisplay,CPoint(10,10),"Would you like to be healed\nFor 25 gold?");
	int key = dlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
	if(key == 'y')
	{
		if(player->gold>=25)
		{
			player->gold=player->gold-25;
			player->m_hp=player->maxhp;
		    player->mp=player->maxmp;
			CSayDlg saydlg;
			saydlg.Create(pDisplay,CPoint(10,10),"Thank you come again!");
			saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
		}
		else
		{
			CSayDlg saydlg;
			saydlg.Create(pDisplay,CPoint(10,10),"You do not have 25 gold");
			saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
		}
	}
	return TRUE;
}
