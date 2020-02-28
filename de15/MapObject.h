// MapObject.h: interface for the CMapObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPOBJECT_H__64B1979D_5226_43E0_A533_843A87BAC555__INCLUDED_)
#define AFX_MAPOBJECT_H__64B1979D_5226_43E0_A533_843A87BAC555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <2ddraw.h>
#include "DEStatic.h"
#include "icons.h"
#include "game_map.h"
#include "mouse.h"
#include "player.h"

class CMapObject : public CDEStatic  
{
public:
	virtual void Display(IGameMap *pMap, C2DDraw *pDisplay, CCursor *pCursor);
	virtual BOOL Create(CPoint p,CIcons* pIcons,IDirect3DDevice9* pDisplay,int icon);
	void SetType(char type,CIcons* pIcons,int icon);
	char GetMapType(BOOL bRealType = FALSE);
	CMapObject(char type);
	virtual ~CMapObject();

   virtual BOOL Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap);

	BOOL IsShown()
	{
		return m_bShown;
	}
	void Show(BOOL bShown)
	{
		m_bShown = bShown;
	}
protected:
	char m_MapType;
	char m_MoveValue;
	BOOL m_bShown;

};

#endif // !defined(AFX_MAPOBJECT_H__64B1979D_5226_43E0_A533_843A87BAC555__INCLUDED_)
