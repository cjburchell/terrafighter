// Warp.h: interface for the CWarp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WARP_H__ED9657B1_B66A_4809_9872_F16D6BEBFC85__INCLUDED_)
#define AFX_WARP_H__ED9657B1_B66A_4809_9872_F16D6BEBFC85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapObject.h"

class CWarp : public CMapObject  
{
public:
	CWarp(char type);
	virtual ~CWarp();

};

#endif // !defined(AFX_WARP_H__ED9657B1_B66A_4809_9872_F16D6BEBFC85__INCLUDED_)
