// DEStatic.h: interface for the CDEStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESTATIC_H__4659DEF3_4DF1_4978_99B7_FA3296BBE461__INCLUDED_)
#define AFX_DESTATIC_H__4659DEF3_4DF1_4978_99B7_FA3296BBE461__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DEObject.h"
#include <2ddraw.h>
#include <surface.h>
#include "player.h"

class CPlayer;

class CDEStatic : public CDEObject  
{
public:
	virtual BOOL Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap);
	virtual void Display(IGameMap *pMap, C2DDraw *pDisplay, CCursor *pCursor);
	virtual BOOL Create(CPoint p,CSize s,IDirect3DDevice9* pDisplay);
	CDEStatic();
	virtual ~CDEStatic();
protected:
	BOOL m_CanMoveOver;
	CSurface* m_pic;
};

#endif // !defined(AFX_DESTATIC_H__4659DEF3_4DF1_4978_99B7_FA3296BBE461__INCLUDED_)
