// WinStatic.cpp: implementation of the CWinStatic class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "player.h"
#include "WinStatic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinStatic::CWinStatic() : CWinBaceObj()
{

}

CWinStatic::~CWinStatic()
{

}

void CWinStatic::Display(CPlayer *pPlayer, CDisplay *pDisplay)
{
	pDisplay->OutTextc(m_BackgroundColor,m_ForgoundColor,GetDrawPoint().x,GetDrawPoint().y,FALSE,"%s",GetText());
	CWinBaceObj::Display(pPlayer,pDisplay);
}

