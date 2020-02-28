// Town.h: interface for the CTown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOWN_H__6E3CA03E_81AB_49F1_8F19_6856601778DE__INCLUDED_)
#define AFX_TOWN_H__6E3CA03E_81AB_49F1_8F19_6856601778DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapObject.h"

class CTown : public CMapObject  
{
public:
	CTown(char type);
	virtual ~CTown();

};

#endif // !defined(AFX_TOWN_H__6E3CA03E_81AB_49F1_8F19_6856601778DE__INCLUDED_)
