// Bush.cpp: implementation of the CBush class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bush.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBush::CBush(FLOAT fWidth, FLOAT fHeight, const char* TextureFile) : CVegetation(fWidth, fHeight,TextureFile)
{
   m_nTri = 4;
   m_NumbSubItems = 0;
}

CBush::~CBush()
{

}

#define PERIOD_SEC 8.0f

HRESULT CBush::RenderObj(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   if(pClipper == NULL || pClipper->MeshFOVCheck(GetPos(),m_Radius))
   {
      FLOAT Tick = timeGetTime() / 1000.0f;

      D3DMATERIAL9 mtrl;
      D3DUtil::InitMaterial( mtrl, 128.0f, 128.0f, 128.0f);
      pd3dDevice->SetMaterial( &mtrl );

      pd3dDevice->SetTexture( 0, m_pTexture );
      pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TREEVERTEX) );
      pd3dDevice->SetVertexShader( NULL );
      pd3dDevice->SetFVF( D3DFVF_TREEVERTEX );

      for(DWORD i=0;i<m_NumbSubItems;i++)
      {
         D3DXMATRIX matWorld;
         D3DXVECTOR3 ObjPos = GetPos() + m_SubItems[i].m_Offset;
         ::D3DXMatrixTranslation(&matWorld,
            ObjPos.x,
            ObjPos.y,
            ObjPos.z);
         ::D3DXMatrixRotationY(&m_Rotate,m_SubItems[i].m_Rotation);
         ::D3DXMatrixMultiply(&matWorld,&m_Rotate,&matWorld);

         FLOAT fTick = Tick + (FLOAT)m_SubItems[i].m_AniOffset;

         D3DXMATRIX matAnimate;
         ::D3DXMatrixRotationX(&matAnimate,ToRad(25.0f * sin(fTick/PERIOD_SEC * (PI+PI))));
         ::D3DXMatrixMultiply(&matWorld,&matAnimate,&matWorld);

         pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld);
         pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST , 0, m_nTri );
      }
   }
   return S_OK;
}

void CBush::SetObjData(CObjPos* pObjData)
{
   m_NumbSubItems = pObjData->m_NumbSubItems;

   for(DWORD i=0;i<m_NumbSubItems;i++)
   {
      m_SubItems[i] = pObjData->m_SubItems[i];

      m_SubItems[i].m_AniOffset = rand()%8; // set some random number
   }
   CStaticObj::SetObjData(pObjData);
}
