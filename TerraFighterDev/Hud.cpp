// Hud.cpp: implementation of the CHud class.
//
//////////////////////////////////////////////////////////////////////

#include "Hud.h"

#define HUD_HIGHT 64

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHud::CHud() : CWinDialog()
{

}

CHud::~CHud()
{

}

void CHud::Display(C2DDraw *pDisplay)
{
	//m_Text1.SetText("test1");
	//m_Text2.SetText("test2");

	m_Armor.SetProgress(75);
	m_Armor.SetToolTipText("Armor 75/100");

    m_Shields.SetProgress(75);
	m_Shields.SetToolTipText("Shields 75/100");

	CWinDialog::Display(pDisplay);
}

BOOL CHud::Create(C2DDraw *pDisplay, CWinBaceObj* pParent)
{
	ShowTitleBar(FALSE);
	CSize size(pDisplay->GetScreenWidth(),HUD_HIGHT);
	CPoint pos(0,0);
	if(!CWinDialog::Create(pDisplay,pParent,pos,size,""))
		return FALSE;

	m_Text1.Create(pDisplay,this,CPoint(210,5),CSize(0,0),"Test1");
	m_Text2.Create(pDisplay,this,CPoint(210,25),CSize(0,0),"Test2");

	m_Status.Create(pDisplay,this,CPoint(100,27),CSize(60,20),"Status");
	m_Status.SetButtonResponce(VK_RETURN);

	m_Armor.Create(pDisplay,this,CPoint(10,5),CSize(32,54),"Armor");
	m_Armor.SetColor(D3DCOLOR_XRGB(255,0,0));

	m_Shields.Create(pDisplay,this,CPoint(50,5),CSize(32,54),"Shields");
	m_Shields.SetColor(D3DCOLOR_XRGB(0,0,255));

	return TRUE;
}

void CHud::Show(BOOL bShow)
{

}
