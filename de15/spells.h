#ifndef __SPELLS_H
#define __SPELLS_H
#include <2DDraw.h>
#include "spell.h"
#include "player.h"

class CPlayer;

#define MAXSPELLS 6
class CSpells
{
public:
	CSpell* GetCurrentSpell();
	CSpells(C2DDraw* pDisplay);
	~CSpells();
	void use(CPlayer* player,IGameMap* m_map,C2DDraw* pDisplay,BOOL bUsePoint = FALSE, CPoint p = CPoint(0,0));
	void select(IGameMap* m_map,C2DDraw* pDirDraw);
	BOOL newspell(UINT numb);
	void save(FILE* data);
	void load(FILE* data);
	int current;
	CSpell spells[MAXSPELLS];
protected:
	int numbspells;

};


#endif
