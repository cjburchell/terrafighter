// MenueDlg.cpp: implementation of the CMenueDlg class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "MenueDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenueDlg::CMenueDlg() : CWinDialog()
{

}

CMenueDlg::~CMenueDlg()
{

}

BOOL CMenueDlg::RegKeyPress(int key)
{
	if(key == VK_RETURN || key == 'L' || key == 'Q')
		return TRUE;
	else
		return FALSE;
}

BOOL CMenueDlg::Create(C2DDraw *pDisplay, CPoint p)
{
	CSize s(60,80);

	m_new.Create(pDisplay,this,CPoint(5,5),CSize(50,20),"New");
   m_new.SetID(VK_RETURN);

	m_load.Create(pDisplay,this,CPoint(5,30),CSize(50,20),"Load");
	m_load.SetID('L');

	m_quit.Create(pDisplay,this,CPoint(5,55),CSize(50,20),"Quit");
	m_quit.SetID('Q');

	return CWinDialog::Create(pDisplay,NULL,p,s,"Menu");
}
