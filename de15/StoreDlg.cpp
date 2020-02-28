// StoreDlg.cpp: implementation of the CStoreDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StoreDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStoreDlg::CStoreDlg()
{
	m_pbuttons = NULL;
	m_pStatic = NULL;
}

CStoreDlg::~CStoreDlg()
{
	if(m_pbuttons)
		delete [] m_pbuttons;
	if(m_pStatic)
		delete [] m_pStatic;
}

BOOL CStoreDlg::RegKeyPress(int key)
{
	return (key >= '0' && key <= '9') || key == VK_RETURN;
}

BOOL CStoreDlg::Create(IDirect3DDevice9* pDevice, C2DDraw *pDisplay, CPoint p, CItem** pItems,int count)
{
	if(m_pbuttons)
		delete [] m_pbuttons;
	m_pbuttons = new CWinBitmapButton[count];

	if(m_pStatic)
		delete [] m_pStatic;
	m_pStatic = new CWinStatic[count];

	CSize s(200,(count*37)+35);
	for(int i=0;i<count;i++)
	{
		CPoint pos;
		pos.x = 5;
		pos.y = (i*(37)) + 5;
		m_pbuttons[i].Create(pDisplay,this,pos,CSize(32,32),pItems[i]->name);
      m_pbuttons[i].SetBitmap(pItems[i]->m_image_file, pDevice, NULL);
      m_pbuttons[i].SetID(i+'0');
		CString str;
		pos.x = 45;
		pos.y += 10;
		str.Format("%s %d",pItems[i]->name,pItems[i]->cost);
		m_pStatic[i].Create(pDisplay,this,pos,CSize(0,0),str);
	}

	CPoint pos;
	pos.y = (i*(37)) + 5;
	pos.x = 5;
	m_close.Create(pDisplay,this,pos,CSize(50,20),"Close");
	m_close.SetID(VK_RETURN);

	return CWinDialog::Create(pDisplay,NULL,p,s,"Store");
}
