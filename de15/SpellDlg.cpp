// SpellDlg.cpp: implementation of the CSpellDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "SpellDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpellDlg::CSpellDlg()
{

}

CSpellDlg::~CSpellDlg()
{

}

BOOL CSpellDlg::Create(C2DDraw *pDisplay, CPoint p, CSpells* pSpells)
{
	CSize s(79,141);

	if(pSpells->spells[0].active)
	{
		CString str;
		str.Format("%s\nCost %d",pSpells->spells[0].name, pSpells->spells[0].costmp);
      m_buttons[0].SetBitmap(pSpells->spells[0].m_pic);
		m_buttons[0].Create(pDisplay,this,CPoint(5,5),CSize(32,32),str);
      m_buttons[0].SetID('1');
	}

	if(pSpells->spells[1].active)
	{
		CString str;
		str.Format("%s\nCost %d",pSpells->spells[1].name, pSpells->spells[1].costmp);
		m_buttons[1].SetBitmap(pSpells->spells[1].m_pic);
		m_buttons[1].Create(pDisplay,this,CPoint(42,5),CSize(32,32),str);
		m_buttons[1].SetID('2');
	}

	if(pSpells->spells[2].active)
	{
		CString str;
		str.Format("%s\nCost %d",pSpells->spells[2].name, pSpells->spells[2].costmp);
		m_buttons[2].SetBitmap(pSpells->spells[2].m_pic);
		m_buttons[2].Create(pDisplay,this,CPoint(5,42),CSize(32,32),str);
		m_buttons[2].SetID('3');
	}

	if(pSpells->spells[3].active)
	{
		CString str;
		str.Format("%s\nCost %d",pSpells->spells[3].name, pSpells->spells[3].costmp);
		m_buttons[3].SetBitmap(pSpells->spells[3].m_pic);
		m_buttons[3].Create(pDisplay,this,CPoint(42,42),CSize(32,32),str);
		m_buttons[3].SetID('4');
	}

	if(pSpells->spells[4].active)
	{
		CString str;
		str.Format("%s\nCost %d",pSpells->spells[4].name, pSpells->spells[4].costmp);
		m_buttons[4].SetBitmap(pSpells->spells[4].m_pic);
		m_buttons[4].Create(pDisplay,this,CPoint(5,79),CSize(32,32),str);
		m_buttons[4].SetID('5');
	}

	if(pSpells->spells[5].active)
	{
		CString str;
		str.Format("%s\nCost %d",pSpells->spells[5].name, pSpells->spells[5].costmp);
		m_buttons[5].SetBitmap(pSpells->spells[5].m_pic);
		m_buttons[5].Create(pDisplay,this,CPoint(42,79),CSize(32,32),str);
		m_buttons[5].SetID('6');
	}

	CPoint pos(5,116);
	m_close.Create(pDisplay,this,pos,CSize(50,20),"Close");
	m_close.SetID(VK_RETURN);


	return CWinDialog::Create(pDisplay,NULL,p,s,"Spells");
}

BOOL CSpellDlg::RegKeyPress(int key)
{
	return key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == VK_RETURN;
}
