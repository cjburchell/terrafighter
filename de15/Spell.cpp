// Spell.cpp: implementation of the CSpell class.
//
//////////////////////////////////////////////////////////////////////
#include "Spell.h"
#include <dxutil.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpell::CSpell()
{
	 active = FALSE;
	 name = "";
	 type = S_HEAL;
	 power = 0;
	 costmp = 0;
	 m_pic=NULL;
}

CSpell::~CSpell()
{
	if(m_pic!=NULL)
		delete m_pic;
}

BOOL CSpell::Create(CString file,C2DDraw* pDisplay)
{
	if(FAILED(pDisplay->CreateSurfaceFromBitmap(&(m_pic),file,32,32)))
	{
        sys_msg("uable to open %s",file);
		  return FALSE;
	}
	if(m_pic!=NULL)
	{
		m_pic->SetColorKey(RGB(255,0,255));
	}
	return TRUE;
}
