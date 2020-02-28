// KeySales.cpp: implementation of the CKeySales class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include "KeySales.h"
#include "YesNoDlg.h"
#include "saydlg.h"
#include "deapp.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeySales::CKeySales() :CNPC()
{

}

CKeySales::~CKeySales()
{

}

BOOL CKeySales::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	player->MoveBack();
	CYesNoDlg dlg;
	dlg.Create(pDisplay,CPoint(10,10),"Would you like to buy a key\nFor 250 gold?");
	int key = dlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
	if(key == 'y')
	{
		if(player->gold>=250)
		{
			player->gold=player->gold-250;
			player->k++;
			CSayDlg saydlg;
			saydlg.Create(pDisplay,CPoint(10,10),"Thank you come again!");
			saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
		}
		else
		{
			CSayDlg saydlg;
			saydlg.Create(pDisplay,CPoint(10,10),"You do not have 250 gold");
			saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
		}
	}
	return TRUE;
}
