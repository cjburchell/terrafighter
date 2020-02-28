// Saver.cpp: implementation of the CSaver class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include "Saver.h"
#include "YesNoDlg.h"
#include "saydlg.h"
#include "deapp.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaver::CSaver() :CNPC()
{

}

CSaver::~CSaver()
{

}

BOOL CSaver::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	player->MoveBack();
	CYesNoDlg dlg;
	dlg.Create(pDisplay,CPoint(10,10),"Would you like me to record your deeds?");
   int key = dlg.DoModal(GetInputObj(), GetApp(), GetWindowMgr());
	if(key == 'y')
	{
		player->Save(pMap);
		CString str;
		str.Format("It has been recorded\nYou have %10lu xp\nto the next level",player->nextlevel-player->xp);
		CSayDlg saydlg;
		saydlg.Create(pDisplay,CPoint(10,10),str);
		saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
	}
	return TRUE;
}
