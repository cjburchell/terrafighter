// WinStatic.h: interface for the CWinStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSTATIC_H__1CEC680A_807E_4540_9815_20CAAD69565C__INCLUDED_)
#define AFX_WINSTATIC_H__1CEC680A_807E_4540_9815_20CAAD69565C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"

class CWinStatic : public CWinBaceObj
{
public:
	virtual void Display(CPlayer *pPlayer, CDisplay *pDisplay);
	CWinStatic();
	virtual ~CWinStatic();

};

#endif // !defined(AFX_WINSTATIC_H__1CEC680A_807E_4540_9815_20CAAD69565C__INCLUDED_)
