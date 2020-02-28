// 3DTarget.cpp: implementation of the C3DTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3DTarget.h"
#include "D3DApp.h"
#include "DXUtil.h"
#include "TerraFighterApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define TARGET_OBJ 2
#define TARGET_OBJ2 3

C3DTarget::C3DTarget()
{
   // Initialize the array of lens flares
   //              type   loc   scale   red   green blue


   m_Target[0].Set(  1,  20.00f, 1.0f,  0.0f, 1.0f, 0.0f );
   m_Target[1].Set(  1,  5.00f,  0.25f, 0.0f, 1.0f, 0.0f );

   m_Target[TARGET_OBJ].Set(  2,  5.00f,  1.0f, 0.0f, 1.0f, 0.0f );
   m_TargetColour.r = 0.0f;
   m_TargetColour.g = 1.0f;
   m_TargetColour.b = 0.0f;

   m_Target[TARGET_OBJ2].Set(  1,  5.00f,  1.0f, 1.0f, 0.0f, 0.0f );



   for( DWORD f=0; f<NUM_TARGET_TEX; f++ )
      m_pTargetTextures[f] = NULL;
   m_pVB = NULL;
   m_bTargetBox = FALSE;
}

C3DTarget::~C3DTarget()
{

}

//-----------------------------------------------------------------------------
// Name: RenderFlares()
// Desc: Draws the set of flares
//-----------------------------------------------------------------------------
HRESULT C3DTarget::Render(LPDIRECT3DDEVICE9 pd3dDevice,CD3DCamera* pCamera)
{
   m_matBillboardMatrix  = pCamera->GetBillboardMatrix();

   D3DMATERIAL9 mtrl;
   D3DUtil::InitMaterial( mtrl );

   // Turn on alpha blending for the flares

   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
   pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);

   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
   //pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_ONE );
   //pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_ONE );

   pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
   pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

   //pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

   pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(D3DVERTEX) );
   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_D3DVERTEX );

   //Enable alpha testing (skips pixels with less than a certain alpha.)
   //if( m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
   {
      pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
      pd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
      pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
   }

   mtrl.Emissive = m_TargetColour;
   pd3dDevice->SetMaterial( &mtrl );
   // Do the flares
   for( DWORD i=0; i<NUM_TARGET; i++ )
   {
      // Set up the emissive color of the flare


      pd3dDevice->SetTexture( 0, m_pTargetTextures[m_Target[i].m_wType] );

      // Translate the world matrix to the flare position
      D3DXMATRIX matWorld;// = m_matBillboardMatrix;

      D3DXMatrixScaling(&matWorld,m_Target[i].m_fRenderSize,m_Target[i].m_fRenderSize,1.0f);
      D3DXMatrixMultiply(&matWorld,&matWorld,&m_matBillboardMatrix);

      matWorld._41 = m_Target[i].m_vPositionPt.x;
      matWorld._42 = m_Target[i].m_vPositionPt.y;
      matWorld._43 = m_Target[i].m_vPositionPt.z;

      // Set the new world transform and render the flare
      pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

      pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
   }


   if(m_bTargetBox)
   {
      mtrl.Emissive.r = m_Target[TARGET_OBJ].m_r;
      mtrl.Emissive.g = m_Target[TARGET_OBJ].m_g;
      mtrl.Emissive.b = m_Target[TARGET_OBJ].m_b;
      pd3dDevice->SetMaterial( &mtrl );

      pd3dDevice->SetTexture( 0, m_pTargetTextures[m_Target[TARGET_OBJ].m_wType] );

      // Translate the world matrix to the flare position
      D3DXMATRIX matWorld;// = m_matBillboardMatrix;

      D3DXMatrixScaling(&matWorld,m_Target[TARGET_OBJ].m_fRenderSize,m_Target[TARGET_OBJ].m_fRenderSize,1.0f);
      D3DXMatrixMultiply(&matWorld,&matWorld,&m_matBillboardMatrix);

      matWorld._41 = m_Target[TARGET_OBJ].m_vPositionPt.x;
      matWorld._42 = m_Target[TARGET_OBJ].m_vPositionPt.y;
      matWorld._43 = m_Target[TARGET_OBJ].m_vPositionPt.z;

      // Set the new world transform and render the flare
      pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

      pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


      mtrl.Emissive.r = m_Target[TARGET_OBJ2].m_r;
      mtrl.Emissive.g = m_Target[TARGET_OBJ2].m_g;
      mtrl.Emissive.b = m_Target[TARGET_OBJ2].m_b;
      pd3dDevice->SetMaterial( &mtrl );

      pd3dDevice->SetTexture( 0, m_pTargetTextures[m_Target[TARGET_OBJ2].m_wType] );


      D3DXMatrixScaling(&matWorld,m_Target[TARGET_OBJ2].m_fRenderSize,m_Target[TARGET_OBJ2].m_fRenderSize,1.0f);
      D3DXMatrixMultiply(&matWorld,&matWorld,&m_matBillboardMatrix);

      matWorld._41 = m_Target[TARGET_OBJ2].m_vPositionPt.x;
      matWorld._42 = m_Target[TARGET_OBJ2].m_vPositionPt.y;
      matWorld._43 = m_Target[TARGET_OBJ2].m_vPositionPt.z;

      // Set the new world transform and render the flare
      pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

      pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
   }



   //D3DXMATRIX  matWorld;
   //D3DXMatrixIdentity( &matWorld );
   //pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );


   pd3dDevice->SetTexture( 0, NULL);
   pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        TRUE );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
   return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT C3DTarget::Update(CSprite* pSprite,CWeapon* Weapons)
{
   // Store the lens flares positions for each flare

   CDisplayObject* pCurrentTarget = pSprite->GetCurrentTarget();
   if(pCurrentTarget)
   {
      m_bTargetBox = TRUE;
      D3DXVECTOR3 Offsetpos = pCurrentTarget->GetPos();
      Offsetpos += pCurrentTarget->GetCenterPoint();

      m_Target[TARGET_OBJ].m_vPositionPt = Offsetpos;
      D3DXVECTOR3 v;
      D3DXVec3Subtract(&v,&pSprite->GetPos(),&Offsetpos);
      FLOAT Distance = fabs(D3DXVec3Length(&v));

      m_Target[TARGET_OBJ].m_fRenderSize = Distance/50.0f;
      if(m_Target[TARGET_OBJ].m_fRenderSize<1.0f)
         m_Target[TARGET_OBJ].m_fRenderSize = 1.0f;



      m_Target[TARGET_OBJ2].m_fRenderSize = Distance/50.0f;

      if(m_Target[TARGET_OBJ2].m_fRenderSize<0.5f)
         m_Target[TARGET_OBJ2].m_fRenderSize = 0.5f;

      FLOAT TimeToTarget = Distance/Weapons[pSprite->GetCurrentWeapon()].GetBeam(0)->GetSpeed();

      if(pCurrentTarget->IsSprite())
      {
         CSprite* pTargetSprite = static_cast<CSprite*>(pCurrentTarget);
         m_Target[TARGET_OBJ2].m_vPositionPt = Offsetpos + pTargetSprite->GetDirectionVector(pTargetSprite->GetSpeed()*TimeToTarget);
      }
      else
      {
         m_Target[TARGET_OBJ2].m_vPositionPt = Offsetpos;
      }

   }
   else
   {
      m_bTargetBox = FALSE;
   }


   for(  DWORD i=0; i<NUM_TARGET; i++ )
   {
      // Store the position of the flare along the axis
      m_Target[i].m_vPositionPt = pSprite->GetPos() + pSprite->GetDirectionVector(m_Target[i].m_fLoc);

      // Store the render size of the flare. This is the lens flare size
      // corrected for the orientation of the flaring axis.
      m_Target[i].m_fRenderSize = m_Target[i].m_fScale;

   }    
   return S_OK;
}

HRESULT C3DTarget::Create(LPDIRECT3DDEVICE9 pd3dDevice)
{

   if( FAILED( D3DUtil::CreateTexture( pd3dDevice, "flare3.jpg",
      m_pTargetTextures[0] ) ) )
      return D3DAPPERR_MEDIANOTFOUND;

   if( FAILED( D3DUtil::CreateTexture( pd3dDevice, "target.dds",
      m_pTargetTextures[1] ) ) )
      return D3DAPPERR_MEDIANOTFOUND;

   if( FAILED( D3DUtil::CreateTexture( pd3dDevice, "CurrentTarget.dds",
      m_pTargetTextures[2] ) ) )
      return D3DAPPERR_MEDIANOTFOUND;


   // Create a quad for rendering each tree
   if( FAILED( pd3dDevice->CreateVertexBuffer(   4*sizeof(D3DVERTEX),
      D3DUSAGE_WRITEONLY, D3DFVF_D3DVERTEX,
      D3DPOOL_MANAGED, &m_pVB, NULL ) ) )
   {
      return E_FAIL;
   }

   // Copy tree mesh data into vertexbuffer
   D3DVERTEX* v;
   m_pVB->Lock( 0, 0, (void**)&v, 0 );

   // Initializes vertices used to render the flares
   //D3DXVECTOR3 vNorm = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
   D3DXVECTOR3 vNorm = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
   DWORD dwDiffuse = D3DCOLOR_ARGB(255,0,255,0);
   v[0].p = D3DXVECTOR3(-1.0f,-1.0f, 0.0f );
   v[0].n = vNorm;
   //v[0].color = dwDiffuse;
   v[0].tu = 0.0f;
   v[0].tv = 1.0f;
   v[1].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f );
   v[1].n = vNorm;
   //v[1].color = dwDiffuse;
   v[1].tu = 0.0f;
   v[1].tv = 0.0f;
   v[2].p = D3DXVECTOR3( 1.0f,-1.0f, 0.0f );
   v[2].n = vNorm;
   //v[2].color = dwDiffuse;
   v[2].tu = 1.0f;
   v[2].tv = 1.0f;
   v[3].p = D3DXVECTOR3( 1.0f, 1.0f, 0.0f );
   v[3].n = vNorm;
   //v[3].color = dwDiffuse;
   v[3].tu = 1.0f;
   v[3].tv = 0.0f;

   m_pVB->Unlock();
   return S_OK;
}

HRESULT C3DTarget::DeleteDeviceObjects()
{
   for(int i=0; i<NUM_TARGET_TEX; i++ )
   {
      D3DUtil::ReleaseTexture( m_pTargetTextures[i] );
      m_pTargetTextures[i] = NULL;
   }

   m_pVB = NULL;
   return S_OK;
}
