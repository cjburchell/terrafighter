// StaticObj.cpp: implementation of the CStaticObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StaticObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticObj::CStaticObj(const char* object) : CDisplayObject()
{
   Create(object,1,D3DXVECTOR3(0,0,0));
   m_bVisable = TRUE;
}

CStaticObj::~CStaticObj()
{

}

HRESULT CStaticObj::RenderObj(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   return this->Render(pd3dDevice,pClipper);
}

void CStaticObj::SetObjData(CObjPos* pObjData)
{
   m_CenterPoint = pObjData->m_CenterPoint;
   D3DXMATRIX matWorld;
   D3DXMATRIX MeshScaled;
   ::D3DXMatrixTranslation(&matWorld,pObjData->m_CenterPoint.x,pObjData->m_CenterPoint.y,pObjData->m_CenterPoint.z);	
   ::D3DXMatrixScaling( &MeshScaled, GetScale(), GetScale(), GetScale());
   ::D3DXMatrixMultiply( &MeshScaled, &MeshScaled, &m_Rotate );
   ::D3DXMatrixMultiply(&matWorld,&MeshScaled,&matWorld);
   SetPos(&pObjData->m_CenterPoint);
   SetMatrix(&matWorld);
}

