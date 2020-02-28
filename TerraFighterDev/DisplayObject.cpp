// DisplayObject.cpp: implementation of the CDisplayObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DisplayObject.h"
#include "vertextypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CShadowVolume::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
   pd3dDevice->SetVertexShader( NULL );
   pd3dDevice->SetFVF( D3DFVF_XYZ );
   for(int i=0;i<m_Data.size();i++)
   {
      // set each shadow to its local space
      pd3dDevice->SetTransform( D3DTS_WORLD, &(m_Data[i]->m_MeshMat) );

      pd3dDevice->SetRenderState( D3DRS_STENCILPASS,      D3DSTENCILOP_INCR );
      pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
      // Draw front-side of shadow volume in stencil/z only
      pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_Data[i]->m_dwNumVertices/3,
         m_Data[i]->m_pVertices, sizeof(D3DXVECTOR3) );


      // Now reverse cull order so back sides of shadow volume are written.
      pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CW );
      // Decrement stencil buffer value
      pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_DECR );
      // Draw back-side of shadow volume in stencil/z only

      pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_Data[i]->m_dwNumVertices/3,
         m_Data[i]->m_pVertices, sizeof(D3DXVECTOR3) );

      //break;
   }


   return S_OK;
}


//-----------------------------------------------------------------------------
// Name: AddEdge()
// Desc: Adds an edge to a list of silohuette edges of a shadow volume.
//-----------------------------------------------------------------------------
inline void AddEdge( WORD* pEdges, DWORD& dwNumEdges2, WORD v0, WORD v1, BOOL bFullBuild )
{
   if(bFullBuild)
   {
      // Remove interior edges (which appear in the list twice)
      for( DWORD i=0; i < dwNumEdges2; i+=2 )
      {
         WORD* pEdgesplusi = pEdges+i;
         if( ( *(pEdgesplusi) == v0 && *(pEdgesplusi+1) == v1 ) ||
            ( *(pEdgesplusi) == v1 && *(pEdgesplusi+1) == v0 ) )
         {
            if( dwNumEdges2 > 2 )
            {
               *(pEdgesplusi) = *(pEdges+dwNumEdges2-2);
               *(pEdgesplusi+1) = *(pEdges+dwNumEdges2-1);
            }
            dwNumEdges2-=2;
            return;
         }
      }
   }
   *(pEdges+dwNumEdges2) = v0;
   *(pEdges+dwNumEdges2+1) = v1;
   dwNumEdges2+=2;
}




//-----------------------------------------------------------------------------
// Name: BuildFromMesh()
// Desc: Takes a mesh as input, and uses it to build a shadowvolume. The
//       technique used considers each triangle of the mesh, and adds it's
//       edges to a temporary list. The edge list is maintained, such that
//       only silohuette edges are kept. Finally, the silohuette edges are
//       extruded to make the shadow volume vertex list.
//-----------------------------------------------------------------------------
HRESULT CShadowVolume::BuildFromMesh( Xfile::Mesh* pMeshObj, const D3DXVECTOR3& LightPos, const D3DXMATRIX& MeshMat, BOOL bFullBuild, int shadowindex )
{
   D3DXVECTOR3 vLight;
   D3DXMATRIX m;
   D3DXMatrixInverse( &m, NULL, &MeshMat );
   vLight.x = LightPos.x*m._11 + LightPos.y*m._21 + LightPos.z*m._31 + m._41;
   vLight.y = LightPos.x*m._12 + LightPos.y*m._22 + LightPos.z*m._32 + m._42;
   vLight.z = LightPos.x*m._13 + LightPos.y*m._23 + LightPos.z*m._33 + m._43;

   CComPtr<ID3DXMesh> pMesh = pMeshObj->m_sysMemMesh;

   // Note: the MESHVERTEX format depends on the FVF of the mesh
   struct MESHVERTEX { D3DXVECTOR3 p, n; FLOAT tu, tv; };
   DWORD dwFVF = pMesh->GetFVF();

   MESHVERTEX* pVertices;
   WORD*       pIndices;

   // Lock the geometry buffers
   pMesh->LockVertexBuffer( 0L, (void**)&pVertices );
   pMesh->LockIndexBuffer( 0L, (void**)&pIndices );
   DWORD dwNumVertices = pMesh->GetNumVertices();
   DWORD dwNumFaces    = pMesh->GetNumFaces();

   // Allocate a temporary edge list
   WORD* pEdges = new WORD[dwNumFaces*6];
   //DWORD dwNumEdges = 0;

   // For each face
   DWORD dwNumEdges2 =0;//= dwNumEdges*2;
   DWORD dwIndices = dwNumFaces*3;
   for( DWORD i=0; i<dwIndices; i+=3 )
   {
      WORD wFace0 = pIndices[i];
      WORD wFace1 = pIndices[i+1];
      WORD wFace2 = pIndices[i+2];

      D3DXVECTOR3 v0 = pVertices[wFace0].p;
      D3DXVECTOR3 v1 = pVertices[wFace1].p;
      D3DXVECTOR3 v2 = pVertices[wFace2].p;

      // Transform vertices or transform light?
      D3DXVECTOR3 vNormal;
      D3DXVec3Cross( &vNormal, &(v2-v1), &(v1-v0) );

      if( D3DXVec3Dot( &vNormal, &vLight ) >= 0.0f )
      {
         AddEdge( pEdges, dwNumEdges2, wFace0, wFace1 , bFullBuild);
         AddEdge( pEdges, dwNumEdges2, wFace1, wFace2 , bFullBuild);
         AddEdge( pEdges, dwNumEdges2, wFace2, wFace0 , bFullBuild);
      }
   }

   if(m_Data.size() <= shadowindex)
   {
      CShadowVolumeData* pData = new CShadowVolumeData();
      shadowindex = m_Data.size();
      m_Data.push_back(pData);
   }

   CShadowVolumeData* pData = m_Data[shadowindex];

   if(bFullBuild)
   {
      if(pData->m_pVertices)
         delete [] pData->m_pVertices;
      pData->m_pVertices = new D3DXVECTOR3[(dwNumEdges2/2) * 6];
   }
   else
   {
      if(pData->m_pVertices == NULL)
         pData->m_pVertices = new D3DXVECTOR3[10000]; // create a vertex buffer
   }


   pData->m_MeshMat = MeshMat;

   D3DXVECTOR3 vLight10 = vLight*10;
   for( i=0; i<dwNumEdges2; i+=2 )
   {
      D3DXVECTOR3 v1 = pVertices[pEdges[i]].p;
      D3DXVECTOR3 v2 = pVertices[pEdges[i+1]].p;
      D3DXVECTOR3 v3 = v1 - vLight10;
      D3DXVECTOR3 v4 = v2 - vLight10;

      // Add a quad (two triangles) to the vertex list
      pData->m_pVertices[pData->m_dwNumVertices++] = v1;
      pData->m_pVertices[pData->m_dwNumVertices++] = v2;
      pData->m_pVertices[pData->m_dwNumVertices++] = v3;

      pData->m_pVertices[pData->m_dwNumVertices++] = v2;
      pData->m_pVertices[pData->m_dwNumVertices++] = v4;
      pData->m_pVertices[pData->m_dwNumVertices++] = v3;
   }

   // Delete the temporary edge list
   delete[] pEdges;

   // Unlock the geometry buffers
   pMesh->UnlockVertexBuffer();
   pMesh->UnlockIndexBuffer();

   return S_OK;
}

bool CShadowVolume::BuildMeshCB(Xfile::Mesh* pMeshObj,const D3DXMATRIX& MeshMat,VOID* pData)
{
   CShadowVolume* pThis = (CShadowVolume*)pData;
   pThis->BuildFromMesh(pMeshObj,pThis->m_TempLightPos,MeshMat,pThis->m_bTempFullBuild,pThis->m_CurrIndex);
   pThis->m_CurrIndex++;
   return true;
}

HRESULT CShadowVolume::BuildFromMeshFile( Xfile::File* pMeshObj, const D3DXVECTOR3& LightPos, const D3DXMATRIX& MeshMat, BOOL bFullBuild )
{
   m_TempLightPos = LightPos;
   m_bTempFullBuild = bFullBuild;
   m_CurrIndex = 0;

   pMeshObj->enumMeshes(BuildMeshCB,MeshMat,this);


   return S_OK;
}


CDisplayObject::CDisplayObject()
{
   m_Pos.x = 0;
   m_Pos.y = 0;
   m_Pos.z = 0;
   m_pShadowVolume = NULL;

   D3DXMatrixIdentity(&m_Rotate);
   D3DXMatrixIdentity(&m_Matrix);

   m_bSprite = FALSE;
   m_Name = _T("");
}

CDisplayObject::~CDisplayObject()
{
   if(m_pShadowVolume)
      delete m_pShadowVolume;
}

FLOAT CDisplayObject::CheckCollision(const D3DXVECTOR3& point, FLOAT Rad)
{
   D3DXVECTOR3 v;
   D3DXVECTOR3 Offsetpos = GetPos();
   Offsetpos+=GetCenterPoint();
   D3DXVec3Subtract(&v,&Offsetpos,&point);
   FLOAT Distance = D3DXVec3Length(&v);
   FLOAT MinDistance = m_Radius+Rad;
   return Distance - MinDistance;
}



HRESULT CDisplayObject::Create(const char* object, FLOAT Scale,D3DXVECTOR3 pos)
{
   m_Scale = Scale;
   m_Pos = pos; 
   return S_OK;
}



void CDisplayObject::StartRenderShadow(LPDIRECT3DDEVICE9 pd3dDevice)
{
   // Disable z-buffer writes (note: z-testing still occurs), and enable the
   // stencil-buffer
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
   pd3dDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );

   // Dont bother with interpolating color
   pd3dDevice->SetRenderState( D3DRS_SHADEMODE,     D3DSHADE_FLAT );

   // Set up stencil compare fuction, reference value, and masks.
   // Stencil test passes if ((ref & mask) cmpfn (stencil & mask)) is true.
   // Note: since we set up the stencil-test to always pass, the STENCILFAIL
   // renderstate is really not needed.
   pd3dDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_ALWAYS );
   pd3dDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
   pd3dDevice->SetRenderState( D3DRS_STENCILFAIL,  D3DSTENCILOP_KEEP );

   // If ztest passes, inc/decrement stencil buffer value
   pd3dDevice->SetRenderState( D3DRS_STENCILREF,       0x1 );
   pd3dDevice->SetRenderState( D3DRS_STENCILMASK,      0xffffffff );
   pd3dDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0xffffffff );

   // Make sure that no pixels get drawn to the frame buffer
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
   pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
}

void CDisplayObject::EndRenderShadow(LPDIRECT3DDEVICE9 pd3dDevice)
{
   // Restore render states
   pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
   pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
   pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,     TRUE );
   pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
   pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

//-----------------------------------------------------------------------------
// Name: RenderShadow()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplayObject::RenderShadow(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   if((pClipper == NULL || pClipper->MeshFOVCheck(GetPos(),m_Radius)) && m_pShadowVolume  && !IsInvisable())
   {
      //pd3dDevice->SetTransform( D3DTS_WORLD, GetMatrix() );
      m_pShadowVolume->Render( pd3dDevice );
   }
   return S_OK;
}
