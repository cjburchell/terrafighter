// Murchent.cpp: implementation of the CMurchent class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include "Murchent.h"
#include "items.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMurchent::CMurchent() : CNPC()
{

}

CMurchent::~CMurchent()
{

}

BOOL CMurchent::Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap)
{
	Citems* pitems=NULL;
	if((pitems= new Citems())!=NULL)
	{
		pitems->store(player,pMap,pDisplay);
		pMap->ResetTime();
		delete pitems;
	}
	return TRUE;
}
