#ifndef __ITEMS_H
#define __ITEMS_H
#include "2DDraw.h"
#include "types.h"
#include "player.h"
#include "item.h"

#define MAXITEMS 25
 class Citems
 {
 public:
	BOOL showitems(CPlayer* player,IGameMap* map, C2DDraw* pDisplay);
	BOOL getitem(CPlayer* player,IGameMap* pMap,C2DDraw* pDisplay,int ItemNumb);
	Citems();
	~Citems();
	BOOL store(CPlayer* player, IGameMap* pMap, C2DDraw* pDisplay);
 protected:
	void useitem(CPlayer* player,IGameMap* pMap,C2DDraw* pDisplay,int itemnumb);
	CItem* Items;
 };
#endif
