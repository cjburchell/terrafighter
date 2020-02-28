
#include "monst.h"
#include "icons.h"
#include "save.h"
#include "items.h"
#include "healer.h"
#include "endmonster.h"
#include "saver.h"
#include "firemonst.h"
#include "lightningmonst.h"
#include "Murchent.h"
#include "Projectile.h"
#include "KeySales.h"





#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// monst

CMonsters::CMonsters()
{
	return;
}

void CMonsters::move(CPlayer* player,IGameMap* pMap,C2DDraw* pDisplay)
{
	POSITION nextpos = m_pMonst.GetHeadPosition();
	// for each monster thats alive
	while(nextpos)
	{
		POSITION currentpos = nextpos;
		CMonster* pMonst = m_pMonst.GetNext(nextpos);
		if(pMonst)
		{
			if(!pMonst->Move(player,pMap,pDisplay))
			{
				TRACE1("Move killed %s\n",pMonst->m_name);
				m_pMonst.RemoveAt(currentpos);
				if(player->GetSeekMonster() == pMonst)
					player->StopSeekingMonster();
				delete pMonst;
				break;
			}
		}
	}
}

void CMonsters::display(IGameMap *pMap,C2DDraw* pDisplay,CCursor* pCursor)
{
	POSITION pos = m_pMonst.GetHeadPosition();
	// for each monster thats alive
	while(pos)
	{
		CMonster* pMonst = m_pMonst.GetNext(pos);
		if(pMonst)
		{
			pMonst->Display(pMap,pDisplay,pCursor);
		}
	}
}
CMonster* CMonsters::OtherMonstCheck(CMonster* pMonster)
{
	POSITION pos = m_pMonst.GetHeadPosition();
	// for each monster thats alive
	while(pos)
	{
		CMonster* pMonst = m_pMonst.GetNext(pos);
		if(pMonst)
		{
			if(pMonst != pMonster)
			{
				CRect rectInter;
				if(rectInter.IntersectRect(pMonster->GetRect(),pMonst->GetRect()))
					return pMonst;
			}
		}
	}
	return NULL;
}

BOOL CMonsters::checkpos(CPlayer* player, C2DDraw* pDisplay, IGameMap* pMap)
{
	POSITION nextpos = m_pMonst.GetHeadPosition();
	// for each monster thats alive
	BOOL bCanMove = TRUE;
	while(nextpos)
	{
		POSITION currentpos = nextpos;
		CMonster* pMonst = m_pMonst.GetNext(nextpos);
		if(pMonst)
		{
			CRect rectInter;
			if(rectInter.IntersectRect(pMonst->GetRect(),player->GetRect()))
			{	
				player->CheckMonster(pMonst,pDisplay,pMap);
				player->MoveBack();
				bCanMove = FALSE;
			}
		}
	}
	return bCanMove;
}
void CMonsters::start(CPlayer* player,IDirect3DDevice9* pDisplay,UINT type,UINT numb,BOOL IsRand)
{
	int NumbMonst;
	if(numb==0)
	{
		int max=player->level/4+2;
		if(max>MAX_RANDOM_MONST)
			max=MAX_RANDOM_MONST;
		NumbMonst=rand()%max+1;
	}
	else
		NumbMonst=numb;
	for(int i=0;i<NumbMonst;i++)
	{
		CMonster* pMonster = getmoster(player,pDisplay,type,IsRand,i);
		if(pMonster)
		{
			m_pMonst.AddHead(pMonster);
		}
	}
}
void CMonsters::end()
{
	CMonster* pMonster;
	while(m_pMonst.GetCount() != 0)
	{
		pMonster = m_pMonst.RemoveHead();
		delete pMonster;
	}
}
CMonsters::~CMonsters()
{
	end();
}
CMonster* CMonsters::getmoster(CPlayer* player,IDirect3DDevice9* device,int set, BOOL IsRand,int numb,CSprite* pOwner)
{
   char files[20];
   sprintf(files,"monstset%d.dat",set);
   char bmp_file[20];
   int count;
   int hp1 = 0;
   int constpoint = 0;
   int maxmonst = 0;
   char line[201];
   int ha;
   UINT xp;
   UINT gold;
   int size;
   CPoint pos;
   FILE* data;
   TYPE_NPC npctype;
   int movement; 
   char tmpname[500];
   if(player->level>9)
		maxmonst=20;
   else
	   maxmonst=15;
   if(IsRand==TRUE)
		numb=rand()%(maxmonst+1);
   if((data=fopen(files,"r"))!=NULL)
   {
      for(count=0;count<=numb;count++)
      {
		  if(fgets(line,200,data)==NULL)
		  {
			  fclose(data);
			  return NULL;
		  }

		
		 sscanf(line,"%d %d %d %U %s %s %U %d %d %d %d %d",
			 &hp1,
			 &constpoint,
			 &ha,
			 &xp,
			 bmp_file,
			 tmpname,
			 &gold,
			 &size,
			 &pos.x,
			 &pos.y,
			 &movement,
			 &npctype
			 );
		 pos.x <<=ICONSIZEB;
		 pos.y <<=ICONSIZEB;
      }
      fclose(data);
	  CMonster* pMonster;
	  switch(npctype)
	  { 
		case HEAL:
			pMonster = new CHealer();
			break;
		case MURCH:
			pMonster = new CMurchent();
			break;
		case SAVE:
			pMonster = new CSaver();
			break;
		case KEY:
			pMonster = new CKeySales();
			break;
		case NPC:
			pMonster = new CNPC();
			break;
		case ENDMON:
			pMonster = new CEndMonster();
			break;
		case FIRE:
			pMonster = new CFireMonst();
			break;
		case LIT:
			pMonster = new CLightningMonst();
			break;
		case PRO:
			pMonster = new CProjectile(pOwner);
			break;
		default:
			pMonster = new CMonster();
			break;
	  }
	  CSize s(size<<ICONSIZEB,size<<ICONSIZEB);
	  pMonster->Create(
		     hp1,
			 constpoint,
			 ha,
			 xp,
			 bmp_file,
			 tmpname,
			 gold,
			 s,
			 pos,
			 movement,
			 npctype,
			 device);

	  return pMonster;	  
   }
   return NULL;
}

BOOL CMonsters::CreateMonst(CSprite* pOwner,CPlayer* pPlayer,IGameMap* pMap,IDirect3DDevice9* pDisplay,TYPE_NPC type,CPoint pos,double angle,BOOL bUsePoint, CPoint SeekPoint)
{
	CProjectile* pMonster;
	switch(type)
	{
	case FIRE:
		pMonster = static_cast<CProjectile*>(getmoster(pPlayer,pDisplay,99,FALSE,0,pOwner));
		break;
	case LIT:
		pMonster = static_cast<CProjectile*>(getmoster(pPlayer,pDisplay,99,FALSE,1,pOwner));
		break;
	}
	pMonster->GotoPos(pos);
	if(pOwner->IsPlayer())
	{
		if(bUsePoint)
			pMonster->SeekPoint(SeekPoint);
		else
			pMonster->MoveThisWay(angle);
	}
	else
	{
		pMonster->SeekPlayer(pPlayer);
	}


	if(!pMap->CheckPos(pMonster->GetCenterPoint()))
	{
		delete pMonster;
		return FALSE;
	}
	if(OtherMonstCheck(pMonster))
	{
		delete pMonster;
		return FALSE;
	}
	CRect rectInter;
	if(rectInter.IntersectRect(pMonster->GetRect(),pPlayer->GetRect()))
	{
		delete pMonster;
		return FALSE;
	}

	m_pMonst.AddHead(pMonster);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// monst message handlers

int CMonsters::GetNumb()
{
	return m_pMonst.GetCount();
}

void CMonsters::KillMonster(CMonster *pMonster, CPlayer* pPlayer)
{
	POSITION nextpos = m_pMonst.GetHeadPosition();
	// for each monster thats alive
	while(nextpos)
	{
		POSITION currentpos = nextpos;
		CMonster* pMonst = m_pMonst.GetNext(nextpos);
		if(pMonst)
		{
			if(pMonst == pMonster)
			{
				TRACE1("Kill killed %s\n",pMonster->m_name);
				m_pMonst.RemoveAt(currentpos);
				if(pPlayer->GetSeekMonster() == pMonster)
					pPlayer->StopSeekingMonster();
				delete pMonster;
				break;
			}
		}
	}
}

CMonster* CMonsters::GetMonsterAtPos(CPoint p)
{
	if(!m_pMonst.IsEmpty())
	{
		POSITION pos = m_pMonst.GetHeadPosition();
		// for each monster thats alive
		while(pos)
		{
			CMonster* pMonst = m_pMonst.GetNext(pos);
			if(pMonst)
			{
				if(pMonst->GetRect().PtInRect(p))
					return pMonst;
			}
		}
	}
	return NULL;
}
