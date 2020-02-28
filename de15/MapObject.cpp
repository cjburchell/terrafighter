// MapObject.cpp: implementation of the CMapObject class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "MapObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapObject::CMapObject(char type) : CDEStatic()
{
	m_MapType = type;
	m_bShown = FALSE;
}

CMapObject::~CMapObject()
{
	m_pic = NULL;
}

char CMapObject::GetMapType(BOOL bRealType)
{
	if(m_bShown || bRealType)
		return m_MapType;
	else
		return 'D';
}

void CMapObject::SetType(char type,CIcons* pIcons, int icon)
{
	m_MapType=type;
	m_icon = icon;
	m_pic = pIcons->GetIcon(icon);
}

BOOL CMapObject::Create(CPoint p,CIcons* pIcons,IDirect3DDevice9 *pDisplay, int icon)
{
	SetType(m_MapType,pIcons,icon);
	return CDEStatic::Create(p,CSize(32,32),pDisplay);
}

void CMapObject::Display(IGameMap *pMap, C2DDraw *pDisplay, CCursor *pCursor)
{
	if(!IsShown())
		return;
	CPoint scr = pMap->MapToScreen(GetDrawPoint());
	int h = GetSize().cy;
	int w = GetSize().cx;
   int maxh =  pDisplay->GetScreenHight();
   int maxw =  pDisplay->GetScreenWidth();
	if(scr.x < maxw && scr.y < maxh && scr.y+h>0 && scr.x+w>0)
	{
		int offsety = 0;
		int offsetx = 0;
		if(scr.x < 0)
		{
			w = w + scr.x;
			offsetx = -scr.x;
			scr.x = 0;
		}
		else if(scr.x+w > maxw)
		{
			w = maxw - scr.x;
		}

		if(scr.y < 0)
		{
			h = h + scr.y;
			offsety = -scr.y;
			scr.y = 0;
		}
		else if(scr.y+h > maxh)
		{
			h = maxh - scr.y;
		}

		CPoint rectPoint(0,0);
		CSize  size(w,h);
		CRect rect(rectPoint,size);
		rect.OffsetRect(offsetx,offsety);

		if(m_pic)
			pDisplay->Blt(scr.x,scr.y,m_pic,&rect);
	}
}


BOOL CMapObject::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	return TRUE;
}
