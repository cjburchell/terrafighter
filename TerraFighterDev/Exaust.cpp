// Exaust.cpp: implementation of the CExaust class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Exaust.h"
#include "DXUtil.h"
#include "vertextypes.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CExaust::CExaust() : CParticleSystem(512, 2048, 0.03f, 0.5f,TRUE)
{
   m_bZEnable = TRUE;
}

CExaust::~CExaust()
{

}

BOOL CExaust::Create()
{
   return CParticleSystem::Create("Particle.jpg");
}




void CExaust::Render(LPDIRECT3DDEVICE9 pd3dDevice ,CD3DCamera* pCamera, CFOVClipper* pClipper)
{
   CParticleSystem::Render( pd3dDevice ,  pCamera,pClipper , TRUE);
}
