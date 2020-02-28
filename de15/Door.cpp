// Door.cpp: implementation of the CDoor class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include <dxutil.h>
#include "player.h"
#include "Door.h"
#include "SayDlg.h"
#include "deapp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoor::CDoor() : CDEStatic()
{
	m_CanMoveOver = FALSE;
}

CDoor::~CDoor()
{

}

BOOL CDoor::Create(CPoint p, CSize s, IDirect3DDevice9 *pDisplay)
{
	if(m_pic!=NULL)
		delete m_pic;
	m_pic=NULL;

   m_pic = new CSurface();

   if(FAILED(m_pic->Init("door.bmp",s.cx,s.cy, pDisplay,NULL)))
	{
		  sys_msg("uable to open door.bmp");
		  return FALSE;
	}

	m_pic->SetColorKey(RGB(255,0,255));

	return CDEStatic::Create(p,s,pDisplay);
}

BOOL CDoor::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	if(player->k<=0)
	{
		CSayDlg saydlg;
		saydlg.Create(pDisplay,CPoint(10,10),"You don't have a key to open the door");
		saydlg.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
		return TRUE;
	}
	else
	{
		player->k--;
		pMap->SetMapVal(GetCenterPoint(),'b');
		return FALSE;
	}
}
