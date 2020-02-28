// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__EB6D8491_B5CE_4919_94CA_9D224AA19594__INCLUDED_)
#define AFX_ITEM_H__EB6D8491_B5CE_4919_94CA_9D224AA19594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>

enum ITEM_TYPE
{
	ITEM_ONEUSE = 0,
	ITEM_WEAPON = 1,
	ITEM_ARMOR = 2,
	ITEM_SHIELD = 3,
   ITEM_KEY = 4,
   ITEM_GOLD = 5
};

class CItem  
{
public:
	CItem();
	virtual ~CItem();
	CString name;
	int hp;
	int defence;
	int ha;
	int agility;
	unsigned long cost;
	bool equip;
	ITEM_TYPE type;
   CString m_image_file;
};

#endif // !defined(AFX_ITEM_H__EB6D8491_B5CE_4919_94CA_9D224AA19594__INCLUDED_)
