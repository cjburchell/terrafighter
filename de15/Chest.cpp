// Chest.cpp: implementation of the CChest class.
//
//////////////////////////////////////////////////////////////////////
#include "Chest.h"
#include <dxutil.h>
#include "items.h"
#include "SayDlg.h"
#include "deapp.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChest::CChest() :CDEStatic()
{
	m_bDestroyAfterUse = FALSE;
}

CChest::~CChest()
{

}

BOOL CChest::Create(CPoint p, CSize s, int level, IDirect3DDevice9* device, BOOL bOpen, BOOL bKey, BOOL bDestroyAfterUse)
{
	m_bDestroyAfterUse = bDestroyAfterUse;
	m_bIsOpen = bOpen;
	if(m_pic!=NULL)
		delete m_pic;
   m_pic = new CSurface();

   if(FAILED(m_pic->Init(m_bIsOpen?"ochest.bmp":"chest.bmp",s.cx,s.cy,device,NULL)))
	{
		  sys_printf(__LOCATION__,0,"uable to open %s",m_bIsOpen?"ochest.bmp":"chest.bmp");
		  return FALSE;
	}

   m_pic->SetColorKey(RGB(255,0,255));

	if(!bKey)
	{
		if(rand()%4!=0)
		{	
			  m_Type = CHEST_GOLD;
			  m_gold=(rand()%100)+20;
		}
		else
		{
			m_Type = CHEST_ITEM;
			m_itemnumber = (level/2)+5;
			if(m_itemnumber>MAXITEMS)
				m_itemnumber=MAXITEMS;
			m_itemnumber = rand()%m_itemnumber;
		}
	}
	else
		m_Type = CHEST_KEY;

	return CDEStatic::Create(p,s,device);
}

BOOL CChest::Use(IDirect3DDevice9* device, CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	if(!m_bIsOpen)
	{
		switch(m_Type)
		{
		case CHEST_KEY:
			{
				CSayDlg saydlg;
				saydlg.Create(pDisplay,CPoint(10,10),"You found a key");
				saydlg.DoModal(GetInputObj(), GetApp(), GetWindowMgr());
				player->k++;
				m_bIsOpen = TRUE;
			}
			break;
		case CHEST_GOLD:
			{
			  CString txt;
			  txt.Format("You got %d gold",m_gold);
			  CSayDlg saydlg;
			  saydlg.Create(pDisplay,CPoint(10,10),txt);
			  saydlg.DoModal(GetInputObj(), GetApp(), GetWindowMgr());
			  player->gold+=m_gold;
			  m_bIsOpen = TRUE;
			}
			break;
		case CHEST_ITEM:
			Citems items;
			if(items.getitem(player,pMap,pDisplay,m_itemnumber)==TRUE)
			   m_bIsOpen = TRUE;

			break;
		}
		

		if(m_bIsOpen && !m_bDestroyAfterUse)
		{
			pMap->SetMapVal(GetCenterPoint(),'0');
			if(m_pic!=NULL)
				delete m_pic;
			m_pic = new CSurface();
         sys_verify(m_pic->Init("ochest.bmp",GetSize().cx,GetSize().cy,device,NULL));
			m_pic->SetColorKey(RGB(255,0,255));
		}
	}
	return m_bIsOpen?(m_bDestroyAfterUse?FALSE:TRUE):TRUE;
}
