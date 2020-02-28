// Murchent.h: interface for the CMurchent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MURCHENT_H__53D8DB43_289C_4FF8_8AA4_7F8B8D643EDA__INCLUDED_)
#define AFX_MURCHENT_H__53D8DB43_289C_4FF8_8AA4_7F8B8D643EDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NPC.h"

class CMurchent : public CNPC  
{
public:
	virtual BOOL Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap);
	CMurchent();
	virtual ~CMurchent();

};

#endif // !defined(AFX_MURCHENT_H__53D8DB43_289C_4FF8_8AA4_7F8B8D643EDA__INCLUDED_)
