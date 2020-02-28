#include <afx.h>
#include <afxwin.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "items.h"
#include "monst.h"
#include "map.h"
#include "SayDlg.h"
#include "ItemDlg.h"
#include "UseDlg.h"
#include "BuySelldlg.h"
#include "StoreDlg.h"
#include "deapp.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



BOOL Citems::showitems(CPlayer* player,IGameMap* pMap, C2DDraw* pDisplay)
{
	int key = '0';
	while(key != -1 && key != VK_RETURN)
	{
		CItemDlg dlg;
		dlg.Create(pDisplay,CPoint(10,10),player->items,PLAYER_MAX_ITEMS);
		key = dlg.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
		int index = key - '0';
		if(index >=0 && index <PLAYER_MAX_ITEMS)
		{
			CUseDlg usedlg;
			CString usetext;
			if(player->items[index].equip)
			{
				usetext = "Unequip";
			}
			else
			{
				if(player->items[index].type == ITEM_ONEUSE)
					usetext = "Use";
				else
					usetext = "Equip";
			}
			usedlg.Create(pDisplay,CPoint(10,10),usetext);
			int resp = usedlg.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
			if(resp == VK_RETURN)
			{
				useitem(player,pMap,pDisplay,index);
			}
			else if(resp == 2)
			{
				if(player->items[index].equip==TRUE)
					useitem(player,pMap,pDisplay,index);
				player->items[index].name = "";
				player->items[index].equip=FALSE;
			}
		}
	}
	return TRUE;
}
BOOL Citems::getitem(CPlayer* player,IGameMap* pMap,C2DDraw* pDisplay,int ItemNumb)
{
	int count=0;
	while(player->items[count].name != "" && ++count<10);
	if(count >=10)
	{
		CSayDlg saydlg;
		saydlg.Create(pDisplay,CPoint(10,10),"You have too many items you must drop one");
		saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
		return FALSE;
	}
	else
	{
		CString txt;
		txt.Format("You got a %-20s",Items[ItemNumb].name);
		CSayDlg saydlg;
		saydlg.Create(pDisplay,CPoint(10,10),txt);
		saydlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
		player->items[count].name = Items[ItemNumb].name;
		player->items[count].hp=Items[ItemNumb].hp;
		player->items[count].defence=Items[ItemNumb].defence;
		player->items[count].ha=Items[ItemNumb].ha;
		player->items[count].agility=Items[ItemNumb].agility;
		player->items[count].type=Items[ItemNumb].type;
		player->items[count].equip=0;
		player->items[count].cost=Items[ItemNumb].cost;
		player->items[count].m_image_file=Items[ItemNumb].m_image_file;
		return TRUE;
	}
}

Citems::Citems()
{
	Items = new CItem[MAXITEMS];
	FILE* data;
	UINT count;
	char line[100];
	data=fopen("items.dat","r");
	if(data!=NULL)
	{
		for(count=0;count<MAXITEMS;count++)
		{
			fgets(line,100,data);
			char tmpstr[100];
			char filename[100];
			sscanf(line,"%s %d %d %d %d %d %d %s",tmpstr,&Items[count].hp,&Items[count].defence,&Items[count].ha,&Items[count].agility,&Items[count].cost,&Items[count].type,filename);
			Items[count].name = tmpstr;
			Items[count].m_image_file = filename;
			Items[count].equip=0;
		}
		fclose(data);
	}
}
Citems::~Citems()
{
	if(Items)
		delete [] Items;
}
void Citems::useitem(CPlayer* player,IGameMap* pMap,C2DDraw* pDisplay,int itemnumb)
{
	if(player->items[itemnumb].name != "" )
		switch(player->items[itemnumb].type)
		{
			case ITEM_ONEUSE:
				if((player->m_hp=player->m_hp+player->items[itemnumb].hp)>player->maxhp)
					player->m_hp=player->maxhp;
				player->defence=player->defence+player->items[itemnumb].defence;
				player->ha=player->ha+player->items[itemnumb].ha;
				player->agility=player->agility+player->items[itemnumb].agility;
				player->items[itemnumb].name ="";
				break;
			case ITEM_WEAPON:
				if(player->items[itemnumb].equip==FALSE)
				{
					if(player->equip[SWORD]==TRUE)
					{   
						CSayDlg saydlg;
						saydlg.Create(pDisplay,CPoint(10,10),"You can't use more than one Weapon");
						saydlg.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
					}
					else
					{
						player->items[itemnumb].equip=TRUE;
						player->defence=player->defence+player->items[itemnumb].defence;
						player->ha=player->ha+player->items[itemnumb].ha;
						player->agility=player->agility+player->items[itemnumb].agility;
						player->equip[SWORD]=TRUE;
					}
				}
				else
				{
					player->items[itemnumb].equip=FALSE;
					player->defence= player->defence-player->items[itemnumb].defence;
					player->ha= player->ha-player->items[itemnumb].ha;
					player->agility= player->agility-player->items[itemnumb].agility;
					player->equip[SWORD]=FALSE;
				}
				break;
			case ITEM_ARMOR:
				if(player->items[itemnumb].equip==FALSE)
				{
					if(player->equip[ARMOR]==TRUE)
					{
						CSayDlg saydlg;
						saydlg.Create(pDisplay,CPoint(10,10),"You can't use more than one Armor");
						saydlg.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
					}
					else
					{
						player->items[itemnumb].equip=TRUE;
						player->defence=player->defence+player->items[itemnumb].defence;
						player->ha=player->ha+player->items[itemnumb].ha;
						player->agility=player->agility+player->items[itemnumb].agility;
						player->equip[ARMOR]=TRUE;
					}
				}
				else
				{
					player->items[itemnumb].equip=FALSE;
					player->defence= player->defence-player->items[itemnumb].defence;
					player->ha= player->ha-player->items[itemnumb].ha;
					player->agility= player->agility-player->items[itemnumb].agility;
					player->equip[ARMOR]=FALSE;
				}
				break;
			case ITEM_SHIELD:
				if(player->items[itemnumb].equip==FALSE)
				{
					if(player->equip[SHELD]==TRUE)
					{
						CSayDlg saydlg;
						saydlg.Create(pDisplay,CPoint(10,10),"You can't use more than one Shield");
						saydlg.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
					}
					else
					{
						player->items[itemnumb].equip=TRUE;
						player->defence=player->defence+player->items[itemnumb].defence;
						player->ha=player->ha+player->items[itemnumb].ha;
						player->agility=player->agility+player->items[itemnumb].agility;
						player->equip[SHELD]=TRUE;
					}
				}
				else
				{
					player->items[itemnumb].equip=FALSE;
					player->defence= player->defence-player->items[itemnumb].defence;
					player->ha= player->ha-player->items[itemnumb].ha;
					player->agility= player->agility-player->items[itemnumb].agility;
					player->equip[SHELD]=FALSE;
				}
				break;
		}
}

BOOL Citems::store(CPlayer* player, IGameMap* pMap, C2DDraw* pDisplay)
{
	player->MoveBack();
	CBuySellDlg buysell;
	buysell.Create(pDisplay,CPoint(10,10),"I buy and sell items and wepons");
	int key = buysell.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
	if(key == 'y')
	{
		//buy stuff
		CItem* pItems[5];
		int maxnumb = player->level+6;
		 if(maxnumb>MAXITEMS)
				maxnumb=MAXITEMS;
		UINT ItemNumb[5];
		ItemNumb[0]=0;
		ItemNumb[1]=rand()%maxnumb+1;

		ItemNumb[2]=rand()%maxnumb+1;
		while(ItemNumb[1] == ItemNumb[2])
			ItemNumb[2]=rand()%maxnumb+1;

		ItemNumb[3]=rand()%maxnumb+1;
		while(ItemNumb[1] == ItemNumb[3] || ItemNumb[2] == ItemNumb[3])
			ItemNumb[3]=rand()%maxnumb+1;

		ItemNumb[4]=rand()%maxnumb+1;
		while(ItemNumb[1] == ItemNumb[4] || ItemNumb[2] == ItemNumb[4] || ItemNumb[3] == ItemNumb[4])
			ItemNumb[4]=rand()%maxnumb+1;


		pItems[0]= &Items[ItemNumb[0]];
		pItems[1]= &Items[ItemNumb[1]];
		pItems[2]= &Items[ItemNumb[2]];
		pItems[3]= &Items[ItemNumb[3]];
		pItems[4]= &Items[ItemNumb[4]];
		CStoreDlg storedlg;
		storedlg.Create(pDisplay,CPoint(10,10),pItems,5);
		while(key != VK_RETURN && key != -1)
		{
			key = storedlg.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
			if(key != VK_RETURN && key < '5' && key >= '0')
			{
				int index = key - '0';
				if(player->gold>=pItems[index]->cost)
				{
					if(getitem(player,pMap,pDisplay,ItemNumb[index])==TRUE)
					{
						player->gold-=pItems[index]->cost;
						//CSayDlg saydlg;
						//saydlg.Create(pDisplay,CPoint(10,10),"Thank you come again");
						//saydlg.DoModal(GetInputObj(),pMap);
					}
				}
				else
				{
					CString txt;
					txt.Format("You do not have %5d Gold",pItems[index]->cost);
					CSayDlg saydlg;
					saydlg.Create(pDisplay,CPoint(10,10),txt);
					saydlg.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
				}
				
			}
		}

	}
	else if(key == 'n')
	{
		while(key != -1 && key != VK_RETURN)
		{
			CItemDlg dlg;
			dlg.Create(pDisplay,CPoint(10,10),player->items,PLAYER_MAX_ITEMS);
			key = dlg.DoModal(GetInputObj(), GetApp(), GetApp()->GetWindowMgr());
			int index = key - '0';
			if(index >=0 && index <PLAYER_MAX_ITEMS)
			{
				if(player->items[index].name != "")
				{
					CString str;
					str.Format("You can sell the %s to me for %d gold",player->items[index].name,((player->items[index].cost*3)/4));
					CYesNoDlg respdlg;
					respdlg.Create(pDisplay,CPoint(10,10),str);
					int resp = respdlg.DoModal(GetInputObj(),GetApp(), GetWindowMgr());
					if(resp == 'y')
					{
						player->gold=((player->items[index].cost*3)/4)+player->gold;
						if(player->items[index].equip==TRUE)
							useitem(player,pMap,pDisplay,index);
						player->items[index].name = "";
						player->items[index].equip=FALSE;
					}
				}
			}
		}
	}
	return TRUE;
}

