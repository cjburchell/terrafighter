#ifndef __PLAYER_H
#define __PLAYER_H

#include "sprite.h"
#include "item.h"
#include "spells.h"
#include "monster.h"
#include <input.h>
#include "DEStatic.h"

class CDEStatic;
class CSpells;
class CMonster;

#define SHIP_IN 1
#define SHIP_OUT 0
#define PLAYER_MAX_ITEMS 10



class CPlayer : public CSprite
{
public:
   int m_hp;
   int defence;
   int maxhp;
   unsigned long gold;
   int ha;
   int level;
   int k;
   unsigned long xp;
   int agility;
   unsigned long nextlevel;
   int map;
   BOOL monster;
   BOOL equip[3];
   CPoint m_savepos;
   CString name;
   int ship;
   int mp;
   int maxmp;
   CItem items[PLAYER_MAX_ITEMS];
   BOOL RandMonstOn;
   BOOL PlayerOK;
   int dirction;
   CSpells* spells;

protected:
   CDEStatic* m_pUseObj;
   CMonster* m_SeekMonster;
public:
   void SeekObject(CDEStatic* pObj,IGameMap* pMap);
   BOOL CheckMonster(CMonster* pMonst, C2DDraw* pDisplay, IGameMap* pMap);
   void ShowStatus(C2DDraw *pDisplay);
   void UseSpell(IGameMap* pMap, C2DDraw* pDisplay, CPoint p);
   CMonster* GetSeekMonster();
   void StopSeekingMonster();
   void SeekMonster(CMonster* pMonst,IGameMap* pMap);
   BOOL Save(IGameMap* pMap);
   void GotoSavedPos();
   void SetSavedPos();
   BOOL GetInput(CInput *pInput,IGameMap *pMap, C2DDraw *pDisplay);
   void levelck(C2DDraw* pDisplay,IGameMap* pMap);
   void intplayer(C2DDraw* pDisplay);
   CPlayer();
   ~CPlayer();
   void AninIcon(float time);
   BOOL playermove(IGameMap* pMap);
   void loadgame(IGameMap* m_map,C2DDraw* pDisplay);
   BOOL startnew(C2DDraw* pDisplay,CInput* pInput,IGameMap* pMap);
   BOOL GetName(CString& string ,C2DDraw* pDisplay);
};
#endif