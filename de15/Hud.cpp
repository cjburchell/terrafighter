// Hud.cpp: implementation of the CHud class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "Hud.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define HUD_HIGHT 64



CHud::CHud() : CWinDialog()
{

}

CHud::~CHud()
{

}

void CHud::Display(CPlayer *pPlayer, C2DDraw *pDisplay)
{
	if(pPlayer->spells)
		if(pPlayer->spells->GetCurrentSpell())
			m_Spells.SetBitmap(pPlayer->spells->GetCurrentSpell()->m_pic);
	CString str;
	str.Format("Gold %d",pPlayer->gold);
	m_Gold.SetText(str);
	str.Format("Keys %d",pPlayer->k);
	m_Keys.SetText(str);

#ifdef _DEBUG
	m_Stuff.SetText(pPlayer->GetStateStr());
#endif
	
	int value = ((double)(pPlayer->m_hp)/(double)(pPlayer->maxhp)) * 100.0;
	m_Health.SetProgress(value);
	str.Format("Health %d/%d",pPlayer->m_hp,pPlayer->maxhp);
	m_Health.SetToolTipText(str);

	value = ((double)(pPlayer->mp)/(double)(pPlayer->maxmp)) * 100.0;
	m_Mana.SetProgress(value);
	str.Format("Magic %d/%d",pPlayer->mp,pPlayer->maxmp);
	m_Mana.SetToolTipText(str);

	CWinDialog::Display(pDisplay);
}

BOOL CHud::Create(C2DDraw *pDisplay, CWinBaceObj* pParent)
{
	ShowTitleBar(FALSE);
   CSize size(pDisplay->GetScreenWidth(),HUD_HIGHT);
   CPoint pos(0,pDisplay->GetScreenHight()-HUD_HIGHT);
	if(!CWinDialog::Create(pDisplay,pParent,pos,size,""))
		return FALSE;

	m_Gold.Create(pDisplay,this,CPoint(210,5),CSize(0,0),"Gold");
	m_Keys.Create(pDisplay,this,CPoint(210,25),CSize(0,0),"Keys");
	m_Stuff.Create(pDisplay,this,CPoint(280,5),CSize(0,0),"");

	m_Spells.Create(pDisplay,this,CPoint(170,5),CSize(32,32),"Spells");
	m_Spells.SetID('K');

	m_Items.Create(pDisplay,this,CPoint(100,5),CSize(60,20),"Items");
	m_Items.SetID('I');

	m_Status.Create(pDisplay,this,CPoint(100,27),CSize(60,20),"Status");
	m_Status.SetID(VK_RETURN);

	m_Health.Create(pDisplay,this,CPoint(10,5),CSize(32,54),"Health");
	m_Health.SetColor(RGB(255,0,0));

	m_Mana.Create(pDisplay,this,CPoint(50,5),CSize(32,54),"Mana");
	m_Mana.SetColor(RGB(0,0,255));


	return TRUE;
}

void CHud::Show(BOOL bShow)
{

}
