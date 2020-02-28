// Animation.cpp: implementation of the CAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Animation.h"
#include "DXUtil.h"
#include "vertextypes.h"
#include "TerraFighterApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnimation::CAnimation()
{
   m_fMaxTime = 0.0f;
   m_bRepeat = FALSE;
   m_pVB = NULL;
   m_fCurrentTime = 0;
}

CAnimation::~CAnimation()
{
   ClearAllFrames();
   m_pVB = NULL;
}

BOOL CAnimation::Create(D3DXVECTOR3 p1,D3DXVECTOR3 p2,D3DXVECTOR3 p3,D3DXVECTOR3 p4, BOOL bRepeat)
{
   m_bRepeat = bRepeat;
   m_p[0] = p1;
   m_p[1] = p2;
   m_p[2] = p3;
   m_p[3] = p4;

   m_fCurrentTime = 0;

   return TRUE;
}

BOOL CAnimation::Create(BOOL bRepeat)
{

   return Create(D3DXVECTOR3( -0.5f, -0.5f, 0.0f ),
      D3DXVECTOR3( -0.5f, 0.5f, 0.0f ),
      D3DXVECTOR3( 0.5f, -0.5f, 0.0f ),
      D3DXVECTOR3( 0.5f, 0.5f, 0.0f ),
      bRepeat);
   return TRUE;
}

BOOL CAnimation::AddFrame(char* strFilename,FLOAT fTime)
{
   CAnimationFrame* pNew = new CAnimationFrame();
   pNew->m_fTime = fTime;
   pNew->m_strTexture = strFilename;
   m_fMaxTime += fTime;
   m_Frames.push_back(pNew);
   return TRUE;
}

void CAnimation::ClearAllFrames()
{
   for (std::vector<CAnimationFrame *>::iterator i = m_Frames.begin(); 
      i != m_Frames.end(); ++i) {
         delete *i;
      }
      m_Frames.clear();
}

void CAnimation::Render(LPDIRECT3DDEVICE9 pd3dDevice,CD3DCamera * pCamera,D3DXVECTOR3 vPos,FLOAT fSize, FLOAT TimeOffset)
{
   if(m_fCurrentTime+TimeOffset<0)
      return;

   D3DXMATRIX matBillboardMatrix = pCamera->GetBillboardMatrix();
   D3DMATERIAL9 mtrl;
   D3DUtil::InitMaterial( mtrl );
   pd3dDevice->SetMaterial( &mtrl );

   D3DXMATRIX matFinal;
   D3DXMatrixScaling( &matFinal, fSize, fSize, fSize );
   matBillboardMatrix._41 = vPos.x;
   matBillboardMatrix._42 = vPos.y;
   matBillboardMatrix._43 = vPos.z;
   D3DXMatrixMultiply( &matFinal, &matFinal,  &matBillboardMatrix );

   pd3dDevice->SetTransform( D3DTS_WORLD,      &matFinal );

   int iCurFrame = GetCurFrame(m_fCurrentTime+TimeOffset);

   if (iCurFrame >= m_Frames.size()) return;

   CAnimationFrame *f = m_Frames[iCurFrame];

   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
   pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

   pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TREEVERTEX) );
   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_TREEVERTEX );

   pd3dDevice->SetTexture(0, f->m_pTexture);
   pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

   // Reset render states
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        TRUE );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

int CAnimation::GetCurFrame(FLOAT fTime)
{

   int iCurFrame = 0; 
   float fTimeCount = 0.0f;

   int size = m_Frames.size();
   for (int i = 0; i<size ; i++)
   {
      fTimeCount += m_Frames[i]->m_fTime;
      if (fTime <= fTimeCount)
      {
         iCurFrame = i;
         break;
      }
   }
   return iCurFrame;
}

void CAnimation::Update(FLOAT eTime)
{
   m_fCurrentTime+=eTime;
   while(m_fCurrentTime > m_fMaxTime && m_bRepeat)
      m_fCurrentTime -= m_fMaxTime;
}

HRESULT CAnimation::DeleteDeviceObjects()
{
   m_pVB = NULL;
   for (std::vector<CAnimationFrame *>::iterator i = m_Frames.begin(); i != m_Frames.end(); ++i)
   {
      D3DUtil::ReleaseTexture((*i)->m_pTexture );
      (*i)->m_pTexture = NULL;
   }
   return S_OK;
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CAnimation::RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
{


   if(m_pVB)
      return S_OK;

   if( FAILED( pd3dDevice->CreateVertexBuffer( 4*sizeof(D3DVERTEX),
      D3DUSAGE_WRITEONLY, D3DFVF_D3DVERTEX,
      D3DPOOL_MANAGED, &m_pVB , NULL) ) )
   {
      return E_FAIL;
   }



   D3DVERTEX* v;
   m_pVB->Lock( 0, 0, (void**)&v, 0 );
   v[0].p = m_p[0];
   v[0].n = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
   v[0].tu=0;
   v[0].tv=0;



   v[1].p = m_p[1];
   v[1].n = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
   v[1].tu=0;
   v[1].tv=1;

   v[2].p = m_p[2];
   v[2].n = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
   v[2].tu=1;
   v[2].tv=0;

   v[3].p = m_p[3];
   v[3].n = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
   v[3].tu=1;
   v[3].tv=1;

   m_pVB->Unlock();

   for (std::vector<CAnimationFrame *>::iterator i = m_Frames.begin(); i != m_Frames.end(); ++i)
   {
      D3DUtil::CreateTexture(pd3dDevice,(char*)((LPCSTR)(*i)->m_strTexture),(*i)->m_pTexture);
   }



   return S_OK;
}
