// Saver.h: interface for the CSaver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVER_H__BD144C40_66FC_43EC_9ADA_E56A218A85BA__INCLUDED_)
#define AFX_SAVER_H__BD144C40_66FC_43EC_9ADA_E56A218A85BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NPC.h"

class CSaver : public CNPC  
{
public:
	virtual BOOL Use(CPlayer *player, C2DDraw *pDisplay, IGameMap *pMap);
	CSaver();
	virtual ~CSaver();

};

#endif // !defined(AFX_SAVER_H__BD144C40_66FC_43EC_9ADA_E56A218A85BA__INCLUDED_)
