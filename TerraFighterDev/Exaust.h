// Exaust.h: interface for the CExaust class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXAUST_H__8FA496F9_67AC_47B9_A20C_B55E7E7B22C0__INCLUDED_)
#define AFX_EXAUST_H__8FA496F9_67AC_47B9_A20C_B55E7E7B22C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParticleSystem.h"

class CExaust  :public CParticleSystem
{
public:
   CExaust();
   virtual ~CExaust();
   BOOL Create();
   void Render(LPDIRECT3DDEVICE9 pd3dDevice ,CD3DCamera* pCamera, CFOVClipper* pClipper);
};

#endif // !defined(AFX_EXAUST_H__8FA496F9_67AC_47B9_A20C_B55E7E7B22C0__INCLUDED_)
