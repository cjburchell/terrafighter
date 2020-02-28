// ItemDlg.cpp: implementation of the CItemDlg class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "ItemDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemDlg::CItemDlg()
{
	m_pbuttons = NULL;
}

CItemDlg::~CItemDlg()
{
	if(m_pbuttons)
		delete [] m_pbuttons;

}

BOOL CItemDlg::Create(IDirect3DDevice9* device, C2DDraw *pDisplay, CPoint p, CItem* pItems,int count)
{
	if(m_pbuttons)
		delete [] m_pbuttons;

	m_pbuttons = new CWinBitmapButton[count];

	CPoint armorpos(79,30);
	CPoint weaponpos(79,67);
	CPoint shieldpos(79,104);

	CSize s(200,((count/2)*37)+35);
	for(int i=0;i<count;i++)
	{
		if(pItems[i].name != "")
		{
			CPoint pos;
			if(i%2)
				pos.x = 42;
			else
				pos.x = 5;

			int row = i/2;
			
			pos.y = (row*(37)) + 5;
			if(pItems[i].equip)
			{
				switch(pItems[i].type)
				{
				case ITEM_WEAPON:
					pos = weaponpos;
					break;
				case ITEM_ARMOR:
					pos = armorpos;
					break;
				case ITEM_SHIELD:
					pos = shieldpos;
					break;
				}
			}
			m_pbuttons[i].Create(pDisplay,this,pos,CSize(32,32),pItems[i].name);
         m_pbuttons[i].SetBitmap(pItems[i].m_image_file, device, NULL);
         m_pbuttons[i].SetID(i+'0');
		}
	}

	CPoint pos;
	int row = i/2;
	pos.y = (row*(37)) + 5;
	pos.x = 5;
	m_close.Create(pDisplay,this,pos,CSize(50,20),"Close");
   m_close.SetID(VK_RETURN);

	armorpos.x = 116;
	weaponpos.x = 116;
	shieldpos.x = 116;
	armorpos.y = 40;
	weaponpos.y = 77;
	shieldpos.y = 114;

	m_ArmorText.Create(pDisplay,this,armorpos,CSize(0,0),"Armor");
	m_WeaponText.Create(pDisplay,this,weaponpos,CSize(0,0),"Weapon");
	m_ShieldText.Create(pDisplay,this,shieldpos,CSize(0,0),"Shield");
	m_title.Create(pDisplay,this,CPoint(85,5),CSize(0,0),"Equiped With");

	return CWinDialog::Create(pDisplay,NULL,p,s,"Items");
}

BOOL CItemDlg::RegKeyPress(int key)
{
	return (key >= '0' && key <= '9') || key == VK_RETURN;
}
