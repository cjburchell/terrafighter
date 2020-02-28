// UseDlg.cpp: implementation of the CUseDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "UseDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUseDlg::CUseDlg() : CWinDialog()
{

}

CUseDlg::~CUseDlg()
{

}

BOOL CUseDlg::Create(C2DDraw *pDisplay, CPoint p, CString Usetext)
{
	usebut.Create(pDisplay,this,CPoint(5,5),CSize(65,20),Usetext);
   usebut.SetID(VK_RETURN);
	discardbut.Create(pDisplay,this,CPoint(5,30),CSize(65,20),"Discard");
	discardbut.SetID(2);
	return CWinDialog::Create(pDisplay,NULL,p,CSize(75,65),"Use");
}

BOOL CUseDlg::RegKeyPress(int key)
{
	return key == VK_RETURN || key == 2;
}
