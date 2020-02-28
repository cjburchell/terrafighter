// Door.h: interface for the CDoor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOOR_H__93E5345E_1A9B_4E11_9FEC_EA7A809E00EA__INCLUDED_)
#define AFX_DOOR_H__93E5345E_1A9B_4E11_9FEC_EA7A809E00EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DEStatic.h"

class CDoor : public CDEStatic  
{
public:
	virtual BOOL Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap);
	virtual BOOL Create(CPoint p, CSize s, IDirect3DDevice9 *pDisplay);
	CDoor();
	virtual ~CDoor();

};

#endif // !defined(AFX_DOOR_H__93E5345E_1A9B_4E11_9FEC_EA7A809E00EA__INCLUDED_)
