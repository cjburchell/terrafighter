// DEStatic.cpp: implementation of the CDEStatic class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "DEStatic.h"
#include "icons.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDEStatic::CDEStatic() : CDEObject()
{
	m_CanMoveOver = TRUE;
	m_pic = NULL;
}

CDEStatic::~CDEStatic()
{
	if(m_pic)
		delete m_pic;

}

BOOL CDEStatic::Create(CPoint p, CSize s, IDirect3DDevice9 *pDisplay)
{
	GotoPos(p);
	m_size = s;
	return TRUE;
}

void CDEStatic::Display(IGameMap *pMap, C2DDraw *pDisplay, CCursor *pCursor)
{
	if(pMap->IsInFog(GetCenterPoint()))
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

		CPoint cursor = pMap->ScreenToMap(pCursor->GetPos());
		if(GetRect().PtInRect(cursor))
		{
			int size = GetSize().cx>>ICONSIZEB;
			pCursor->ShowSel(scr.x,scr.y,rect,pDisplay,size);
		}	
	}
}

BOOL CDEStatic::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	return TRUE;
}
