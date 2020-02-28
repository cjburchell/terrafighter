#include <stdio.h>
#include "spells.h"
#include "SpellDlg.h"
#include "map.h"
#include "deapp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CSpells::CSpells(C2DDraw* pDisplay)
{
	FILE* data;
	UINT count;
	char line[50];
	data=fopen("spells.dat","r");
	if(data!=NULL)
	{
		for(count=0;count<MAXSPELLS;count++)
		{
			fgets(line,50,data);
		    char tempstr[100];
			char filename[100];
			sscanf(line,"%s %d %d %d %s",tempstr,&spells[count].type,&spells[count].power,&spells[count].costmp,filename);
			spells[count].Create(filename,pDisplay);
			spells[count].name = tempstr;
			spells[count].active=FALSE;
#ifdef _DEBUG
			spells[count].active=TRUE;
#endif
			
		}
		fclose(data);
	}
	else
		return;
	numbspells=MAXSPELLS;
	current=0;
	return;
}

CSpells::~CSpells()
{
}

void CSpells::use(CPlayer* player,IGameMap* pMap, C2DDraw* pDisplay, BOOL bUsePoint, CPoint p)
{
	int max;
	if(current != -1)
	{
		if(player->monster==TRUE)
		{
			if(player->mp>=spells[current].costmp)
			{
				player->mp -= spells[current].costmp;
				switch(spells[current].type)
				{
				case S_HEAL:
					if(spells[current].power>=3)
							player->m_hp=player->maxhp;
					else	
					{
						max=40;
						player->m_hp+=(rand() % max)+5;
						if(player->maxhp < player->m_hp)
							player->m_hp = player->maxhp;
					}
					break;
				case S_OUTSIDE:
					pMap->outside();
					break;
				case S_FIREBALL:
					if(spells[current].power>=2)
					{
						
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),0);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),45);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),90);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),135);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),180);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),225);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),270);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),315);
					}
					else
					{
						if(bUsePoint)
						{
							static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),0,TRUE,p);

						}
						else
						{
							switch(player->dirction)
							{
							case NORTH:
								static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),270);
								break;
							case SOUTH:
								static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),90);
								break;
							case EAST:
								static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),0);
								break;
							case WEST:
								static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,FIRE,player->GetDrawPoint(),180);
								break;
							}
						}
					}
					break;
				case S_LITBALL:
					if(spells[current].power>=2)
					{
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),0);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),45);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),90);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),135);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),180);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),225);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),270);
						static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),315);
					}
					else
					{
						if(bUsePoint)
						{
							static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),0,TRUE,p);
						}
						else
						{
							switch(player->dirction)
							{
							case NORTH:
							    static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),270);
								break;
							case SOUTH:
							    static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),90);
								break;
							case EAST:
							    static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),0);
								break;
							case WEST:
							    static_cast<CGameMap*>(pMap)->m_pmonst->CreateMonst(player,player,pMap,pDisplay,LIT,player->GetDrawPoint(),180);
								break;
							}
						}
					}
					break;
				}
			}
		}
		else
		{
			switch(spells[current].type)
			{
			case S_HEAL:
				player->mp -= spells[current].costmp;
				if(spells[current].power>=3)
						player->m_hp=player->maxhp;
				else
				{
					max=40;
					player->m_hp+=(rand() % max)+5;
					if(player->maxhp < player->m_hp)
						player->m_hp = player->maxhp;
				}
				break;
			}
		}
	}
}

void CSpells::select(IGameMap* pMap,C2DDraw* pDirDraw)
{
	CSpellDlg dlg;
	dlg.Create(pDirDraw,CPoint(10,10),this);
	int key = dlg.DoModal(GetInputObj(), GetApp(), GetWindowMgr());
	if(key != -1)
	{
		key = key - '1';
		if(key >=0 && key <MAXSPELLS)
		{
			if(spells[key].active==TRUE)
				current=key;
		}
	}
}

BOOL CSpells::newspell(UINT numb)
{
	if(numb<MAXSPELLS)
	{
		if(spells[numb].active==FALSE)
		{
			spells[numb].active=TRUE;
			return TRUE;
		}
	}
	return FALSE;
}
void CSpells::save(FILE* data)
{
	UINT count;
	if(data!=NULL)
		for(count=0;count<MAXSPELLS;count++)
			fprintf(data,"%d\n",spells[count].active);
	else
		return;
	return;
}

void CSpells::load(FILE* data)
{
	UINT count;
	char line[50];
	if(data!=NULL)
		for(count=0;count<MAXSPELLS;count++)
		{
			fgets(line,50,data);
			sscanf(line,"%d\n",&(spells[count].active));
		}
	else
		return;
	return;
	current=0;
}

CSpell* CSpells::GetCurrentSpell()
{
	if(current != -1 && spells[current].active)
		return &(spells[current]);
	else
		return NULL;
}
