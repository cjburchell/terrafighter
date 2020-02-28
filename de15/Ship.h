// Ship.h: interface for the CShip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIP_H__1A097862_803D_443F_B3F7_EF71EDF7F367__INCLUDED_)
#define AFX_SHIP_H__1A097862_803D_443F_B3F7_EF71EDF7F367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapObject.h"

class CShip : public CMapObject  
{
public:
	CShip();
	virtual ~CShip();

};

#endif // !defined(AFX_SHIP_H__1A097862_803D_443F_B3F7_EF71EDF7F367__INCLUDED_)
