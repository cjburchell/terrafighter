///////////////////////////////////////////////////////////////////////////////
///  Terrain.cpp
///
///  PATH: D:\dev2\CommonDev
///
///  CREATED: 02/06/2004 5:00:49 PM by Christiaan Burchell
///
///  PURPOSE:  Terrain Display Object
///
///  COPYRIGHT NOTICE:   Copyright (C) 2004 Redpoint Games 
///
///   This software is provided 'as-is', without any express or implied 
///   warranty.  In no event will the author be held liable for any damages 
///   arising from the use of this software. 
/// 
///   Permission is granted to anyone to use this software for any purpose,
///   excluding commercial applications, and to alter it and redistribute it
///   freely, subject to the following restrictions:
/// 
///   1. The origin of this software must not be misrepresented; you must not
///      claim that you wrote the original software. If you use this software
///      in a product, an acknowledgment in the product documentation would be
///      appreciated but is not required.
///   2. Altered source versions must be plainly marked as such, and must not be
///      misrepresented as being the original software.
///   3. This notice may not be removed or altered from any source distribution.
///   4. The author permission is required to use this software in commercial 
///      applications 
///
///  LAST CHANGED: $Date$
///
///  REVISION HISTORY:
///  $Log$
/// 

// SeaFloor.cpp: implementation of the CTerrain class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Terrain.h"
#include "D3DApp.h"
#include "D3DFile.h"
#include "DXUtil.h"
//#include "vertextypes.h"
//#include "TerraFighterApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define D3DFVF_TERRAINVERTEX   D3DFVF_D3DVERTEX
typedef D3DVERTEX TERRAINVERTEX;


struct ViewData
{
   WORD* pDst;
   DWORD nIndexes;
   DWORD FacestoDraw;
   WORD sizez;
   D3DXVECTOR3 m_EyePos;
   BOOL m_bLowDetail;
};





/////////////////////////////////////////////////
///
/// NAME: ProcessElement
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Adds Visable Virtex data 
///
/// CREATED:     02/06/2004 5:01:02 PM
///
/// PARAMETERS: 
///              void* pvObj
///             void* pData
///
/// RETURN: BOOL AFXAPI 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL AFXAPI CTerrain::ProcessElement( void* pvObj, void* pData )
{
   ViewData* pViewData = static_cast<ViewData*>(pData);
   CTerrainObj* pObj = static_cast<CTerrainObj*>(pvObj);

   WORD vertex = pObj->nVertex;
   WORD* pDst = pViewData->pDst;
   WORD sizez = pViewData->sizez;
   DWORD nIndexes = pViewData->nIndexes;

   if(pObj->m_bFlatPlainAll || pViewData->m_bLowDetail)
   {
      //1
      pDst[nIndexes++] = vertex+2;
      pDst[nIndexes++] = vertex;
      pDst[nIndexes++] = vertex+sizez+sizez;

      //2
      pDst[nIndexes++] = vertex+sizez+sizez;
      pDst[nIndexes++] = vertex+2+sizez+sizez;
      pDst[nIndexes++] = vertex+2;

      pViewData->FacestoDraw+=2;
   }
   else
   {
      D3DXVECTOR3 vdist = pViewData->m_EyePos - pObj->m_CenterPoint;
      FLOAT d = D3DXVec3LengthSq(&vdist);
      if(d < TERRAIN_LOD1)
      {
         D3DXVECTOR3 vdist1 = pViewData->m_EyePos - pObj->m_Nabpos[0];
         if(pObj->m_bFlatPlain[1] || (pObj->m_bNab[0] && D3DXVec3LengthSq(&vdist1) > TERRAIN_LOD1))
         {
            //2
            pDst[nIndexes++] = vertex;
            pDst[nIndexes++] = vertex+sizez+sizez;
            pDst[nIndexes++] = vertex+1+sizez;
            pViewData->FacestoDraw++;
         }
         else
         {
            //1
            pDst[nIndexes++] = vertex;
            pDst[nIndexes++] = vertex+sizez;
            pDst[nIndexes++] = vertex+1+sizez;
            //5
            pDst[nIndexes++] = vertex+1+sizez;
            pDst[nIndexes++] = vertex+sizez;
            pDst[nIndexes++] = vertex+sizez+sizez;
            pViewData->FacestoDraw+=2;
         }



         D3DXVECTOR3 vdist2 = pViewData->m_EyePos - pObj->m_Nabpos[2];
         if(pObj->m_bFlatPlain[0] || (pObj->m_bNab[2] && D3DXVec3LengthSq(&vdist2) > TERRAIN_LOD1))
         {
            //1
            pDst[nIndexes++] = vertex+2;
            pDst[nIndexes++] = vertex;
            pDst[nIndexes++] = vertex+1+sizez;
            pViewData->FacestoDraw++;
         }
         else
         {
            //2
            pDst[nIndexes++] = vertex+1+sizez;
            pDst[nIndexes++] = vertex+1;
            pDst[nIndexes++] = vertex;
            //3
            pDst[nIndexes++] = vertex+2;
            pDst[nIndexes++] = vertex+1;
            pDst[nIndexes++] = vertex+1+sizez;
            pViewData->FacestoDraw+=2;
         }



         D3DXVECTOR3 vdist3 = pViewData->m_EyePos - pObj->m_Nabpos[1];
         if(pObj->m_bFlatPlain[3] || (pObj->m_bNab[1] && D3DXVec3LengthSq(&vdist3) > TERRAIN_LOD1))
         {
            //4
            pDst[nIndexes++] = vertex+2+sizez+sizez;
            pDst[nIndexes++] = vertex+2;
            pDst[nIndexes++] = vertex+1+sizez;
            pViewData->FacestoDraw++;
         }
         else
         {
            //4
            pDst[nIndexes++] = vertex+1+sizez;
            pDst[nIndexes++] = vertex+2+sizez;
            pDst[nIndexes++] = vertex+2;
            //8
            pDst[nIndexes++] = vertex+2+sizez+sizez;
            pDst[nIndexes++] = vertex+2+sizez;
            pDst[nIndexes++] = vertex+1+sizez;
            pViewData->FacestoDraw+=2;
         }



         D3DXVECTOR3 vdist4 = pViewData->m_EyePos - pObj->m_Nabpos[3];
         if(pObj->m_bFlatPlain[2] || (pObj->m_bNab[3] && D3DXVec3LengthSq(&vdist4) > TERRAIN_LOD1))
         {
            //3
            pDst[nIndexes++] = vertex+sizez+sizez;
            pDst[nIndexes++] = vertex+2+sizez+sizez;
            pDst[nIndexes++] = vertex+1+sizez;
            pViewData->FacestoDraw++;
         }
         else
         {
            //6
            pDst[nIndexes++] = vertex+sizez+sizez;
            pDst[nIndexes++] = vertex+1+sizez+sizez;
            pDst[nIndexes++] = vertex+1+sizez;
            //7
            pDst[nIndexes++] = vertex+1+sizez;
            pDst[nIndexes++] = vertex+1+sizez+sizez;
            pDst[nIndexes++] = vertex+2+sizez+sizez;
            pViewData->FacestoDraw+=2;
         }

      }
      else if(d < TERRAIN_LOD2)
      {
         //1
         pDst[nIndexes++] = vertex+2;
         pDst[nIndexes++] = vertex;
         pDst[nIndexes++] = vertex+1+sizez;

         //2
         pDst[nIndexes++] = vertex;
         pDst[nIndexes++] = vertex+sizez+sizez;
         pDst[nIndexes++] = vertex+1+sizez;


         //3
         pDst[nIndexes++] = vertex+sizez+sizez;
         pDst[nIndexes++] = vertex+2+sizez+sizez;
         pDst[nIndexes++] = vertex+1+sizez;

         //4
         pDst[nIndexes++] = vertex+2+sizez+sizez;
         pDst[nIndexes++] = vertex+2;
         pDst[nIndexes++] = vertex+1+sizez;

         pViewData->FacestoDraw+=4;
      }
      else
      {
         //1
         pDst[nIndexes++] = vertex+2;
         pDst[nIndexes++] = vertex;
         pDst[nIndexes++] = vertex+sizez+sizez;

         //2
         pDst[nIndexes++] = vertex+sizez+sizez;
         pDst[nIndexes++] = vertex+2+sizez+sizez;
         pDst[nIndexes++] = vertex+2;

         pViewData->FacestoDraw+=2;
      }
   }
   pViewData->nIndexes = nIndexes;
   return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CTerrain
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 5:01:58 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CTerrain::CTerrain()
{
   m_pHightMapTex = NULL;
   m_pHightMap = NULL;
   m_sizex = 0;
   m_sizez = 0;
   m_pRoot = NULL;
   m_pVB = NULL;
   m_pIB = NULL;

   m_MapHight = 20.0f;
   m_MapHightDev256 = m_MapHight/256.0f;

   m_MapScale = 2.0f;
}



/////////////////////////////////////////////////
///
/// NAME: ~CTerrain
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 5:02:00 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CTerrain::~CTerrain()
{
   if(m_pHightMap)
      delete [] m_pHightMap;
   //if(m_pObjects)
   //delete [] m_pObjects;

   if(m_pRoot)
   {
      m_pRoot->Distroy(TRUE);
      delete m_pRoot;
   }

}



/////////////////////////////////////////////////
///
/// NAME: RefreshHightMap
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Refreshes the Hight Map
///
/// CREATED:     02/06/2004 5:02:11 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerrain::RefreshHightMap()
{
   LoadDEM();
   RefreshOctTree();
}



/////////////////////////////////////////////////
///
/// NAME: RefreshOctTree
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Refreshes and reloads the Tree
///
/// CREATED:     02/06/2004 5:02:27 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerrain::RefreshOctTree()
{
   FLOAT halfx  = (FLOAT)m_sizex/2.0f;
   FLOAT halfz  = (FLOAT)m_sizez/2.0f;

   TERRAINVERTEX*              pSrc = NULL;
   D3DVERTEXBUFFER_DESC vbDesc;
   m_pVB->GetDesc(&vbDesc);
   m_pVB->Lock( 0, m_dwNumVertices * sizeof(TERRAINVERTEX), (void**)&pSrc, D3DLOCK_DISCARD );


   DWORD vertex = 0;
   for(WORD z = 0;z<m_sizez;z++)
   {
      for(WORD x = 0;x<m_sizex;x++)
      {
         FLOAT y = GetHightAtPos((FLOAT)x-halfx,(FLOAT)z-halfz);
         pSrc[vertex].p = D3DXVECTOR3(((FLOAT)x-halfx) * m_MapScale,y,((FLOAT)z-halfz) * m_MapScale);
         pSrc[vertex].n = D3DXVECTOR3(0.0f,1.0f,0.0f);
         pSrc[vertex].tu=((FLOAT)(x))/(FLOAT)m_sizex;
         pSrc[vertex].tv=((FLOAT)(m_sizez-z))/(FLOAT)m_sizez;
         vertex++;
      }
   }


   // calculate the normal of each vector
   vertex = 0;
   for(DWORD i=0; i < m_dwNumVertices; i++ )
   {
      DWORD a = vertex+1;
      DWORD b = vertex;
      DWORD c = vertex+m_sizez;
      if ( a >= m_dwNumVertices || c >= m_dwNumVertices )
      {
         vertex++;
         continue;
      }
      D3DXVECTOR3 v1 = pSrc[b].p - pSrc[a].p;
      D3DXVECTOR3 v2 = pSrc[c].p - pSrc[b].p;
      D3DXVECTOR3 n;
      D3DXVec3Cross(&n ,&v1, &v2 );
      D3DXVec3Normalize(&n,&n);
      pSrc[a].n += n;
      pSrc[b].n += n;
      pSrc[c].n += n;
      vertex++;
   }
   m_pVB->Unlock();



   if(m_pRoot)
   {
      m_pRoot->Distroy(TRUE);
      delete m_pRoot;
   }

   m_pRoot = new COctTree<CTerrainObj>();

   FLOAT xsize = (FLOAT)GetXSize()/2.0f;
   FLOAT zsize = (FLOAT)GetZSize()/2.0f;

   m_pRoot->Create(D3DXVECTOR3(-xsize,-zsize,-xsize),D3DXVECTOR3(xsize,zsize,xsize),m_MapScale,2);

   for(WORD zpos = 0;zpos<m_sizez-2;zpos+=2)
   {
      for(WORD xpos = 0;xpos<m_sizex-2;xpos+=2)
      {
         FLOAT cx = xpos - halfx;
         FLOAT cz = zpos - halfx;
         CTerrainObj* pObj = new CTerrainObj();
         FLOAT fx = (cx + 1.0f) * m_MapScale;
         FLOAT fz = (cz + 1.0f) * m_MapScale;
         pObj->nVertex = zpos*m_sizez + xpos;
         pObj->m_Raidus= m_MapScale/2.0f;
         pObj->m_CenterPoint = D3DXVECTOR3(fx,GetHightAtPosR(fx,fz),fz);


         {
            FLOAT h[9];

            h[0] = GetHightAtPos(cx,cz);
            h[1] = GetHightAtPos(cx+1,cz);
            h[2] = GetHightAtPos(cx+2,cz);



            h[3] = GetHightAtPos(cx,cz+1);
            h[4] = GetHightAtPos(cx+1,cz+1);
            h[5] = GetHightAtPos(cx+2,cz+1);

            h[6] = GetHightAtPos(cx,cz+2);      
            h[7] = GetHightAtPos(cx+1,cz+2);   
            h[8] = GetHightAtPos(cx+2,cz+2);


            pObj->m_bFlatPlainAll = TRUE;
            for(UINT j=0;j<9;j++)
            {
               if(!pObj->m_bFlatPlainAll)
                  break;

               for(UINT k=0;k<9;k++)
               {
                  pObj->m_bFlatPlainAll = h[j] == h[k];
                  if(!pObj->m_bFlatPlainAll)
                     break;
               }
            }


            if( h[0] == h[1] && h[0] == h[2] && h[0] == h[4] &&
               h[1] == h[2] && h[1] == h[4] &&
               h[2] == h[4])
               pObj->m_bFlatPlain[0]=TRUE;
            else
               pObj->m_bFlatPlain[0]=FALSE;

            if( h[0] == h[3] && h[0] == h[4] && h[0] == h[6] &&
               h[3] == h[4] && h[3] == h[6] &&
               h[4] == h[6])
               pObj->m_bFlatPlain[1]=TRUE;
            else
               pObj->m_bFlatPlain[1]=FALSE;

            if( h[4] == h[6] && h[4] == h[7] && h[4] == h[8] &&
               h[6] == h[7] && h[6] == h[8] &&
               h[7] == h[8])
               pObj->m_bFlatPlain[2]=TRUE;
            else
               pObj->m_bFlatPlain[2]=FALSE;

            if( h[2] == h[4] && h[2] == h[5] && h[2] == h[8] &&
               h[4] == h[5] && h[4] == h[8] &&
               h[5] == h[8])
               pObj->m_bFlatPlain[3]=TRUE;
            else
               pObj->m_bFlatPlain[3]=FALSE;


         }


         {
            int nx = (int)xpos-2;
            int nz = (int)zpos;
            if(nx<m_sizex-1 && nz<m_sizez-1 && nx>0 && nz>0)
            {
               FLOAT fnx = (nx - halfx + 1.0f) * m_MapScale;
               FLOAT fnz = (nz - halfx + 1.0f) * m_MapScale;
               pObj->m_Nabpos[0] = D3DXVECTOR3(fnx,GetHightAtPosR(fnx,fnz),fnz);
               pObj->m_bNab[0] = TRUE;
            }
            else
            {
               pObj->m_bNab[0] = FALSE;
            }

         }

         {
            int nx = (int)xpos+2;
            int nz = (int)zpos;
            if(nx<m_sizex-1 && nz<m_sizez-1 && nx>0 && nz>0)
            {
               FLOAT fnx = (nx - halfx + 1.0f) * m_MapScale;
               FLOAT fnz = (nz - halfx + 1.0f) * m_MapScale;
               pObj->m_Nabpos[1] = D3DXVECTOR3(fnx,GetHightAtPosR(fnx,fnz),fnz);
               pObj->m_bNab[1] = TRUE;
            }
            else
            {
               pObj->m_bNab[1] = FALSE;
            }

         }

         {
            int nx = (int)xpos;
            int nz = (int)zpos-2;
            if(nx<m_sizex-1 && nz<m_sizez-1 && nx>0 && nz>0)
            {
               FLOAT fnx = (nx - halfx + 1.0f) * m_MapScale;
               FLOAT fnz = (nz - halfx + 1.0f) * m_MapScale;
               pObj->m_Nabpos[2] = D3DXVECTOR3(fnx,GetHightAtPosR(fnx,fnz),fnz);
               pObj->m_bNab[2] = TRUE;
            }
            else
            {
               pObj->m_bNab[2] = FALSE;
            }

         }

         {
            int nx = (int)xpos;
            int nz = (int)zpos+2;
            if(nx<m_sizex-1 && nz<m_sizez-1 && nx>0 && nz>0)
            {
               FLOAT fnx = (nx - halfx + 1.0f) * m_MapScale;
               FLOAT fnz = (nz - halfx + 1.0f) * m_MapScale;
               pObj->m_Nabpos[3] = D3DXVECTOR3(fnx,GetHightAtPosR(fnx,fnz),fnz);
               pObj->m_bNab[3] = TRUE;
            }
            else
            {
               pObj->m_bNab[3] = FALSE;
            }

         }



         if(!m_pRoot->Add(pObj))
         {
            delete pObj;
            sys_msg("Unable to add Node\n");
         }
      }
   }
   m_pRoot->Optimize();
}






/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Creates the Terrain Object
///
/// CREATED:     02/06/2004 5:02:43 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             CZipArchive* pZip
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerrain::Create(LPDIRECT3DDEVICE9 pd3dDevice, CZipArchive* pZip)
{
   sys_verify_hr(D3DUtil::CreateTexture(pd3dDevice,(char*)((LPCSTR)m_HightMapFile),m_pHightMapTex,pZip));

   LoadDEM();

   {
      for(UINT i=0; i< m_Textrues.size();i++)
      {
         // Create textures for the seafloor
         sys_verify_hr_return(  D3DUtil::CreateTexture( pd3dDevice, (char*)((LPCSTR)m_Textrues[i].m_TexFile), m_Textrues[i].m_pTexture,pZip ), D3DAPPERR_MEDIANOTFOUND )
      


         if(!m_Textrues[i].m_AlphaFile.IsEmpty())
         {
            // Create textures for the seafloor
            sys_verify_hr_return( D3DUtil::CreateTexture( pd3dDevice, (char*)((LPCSTR)m_Textrues[i].m_AlphaFile), m_Textrues[i].m_pAlphaTexture,pZip ), D3DAPPERR_MEDIANOTFOUND);
         }
      }
   }



   sys_verify_hr(m_MiniMap.Init(m_HightMapFile,(DWORD)MINI_MAP_SIZE,(DWORD)MINI_MAP_SIZE,pd3dDevice,pZip));

   m_dwNumVertices = m_sizex*m_sizez;
   m_dwNumFaces = (((m_sizex-1)*(m_sizez-1))*2);


   sys_verify_hr_return_hr(pd3dDevice->CreateVertexBuffer( m_dwNumVertices * sizeof(TERRAINVERTEX),
      D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 
      D3DFVF_TERRAINVERTEX,
      D3DPOOL_DEFAULT,
      &m_pVB, NULL ));
  

   sys_verify_hr_return_hr(pd3dDevice->CreateIndexBuffer( m_dwNumFaces * 3 * sizeof(WORD),
      D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,
      D3DFMT_INDEX16 , D3DPOOL_DEFAULT,
      &m_pIB, NULL ));

   RefreshOctTree();

   return S_OK;
}




/////////////////////////////////////////////////
///
/// NAME: Render
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Renders the Terrain
///
/// CREATED:     02/06/2004 5:02:54 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             CFOVClipper* pClipper
///             DWORD& nTri
///             D3DXVECTOR3 vEyePt
///             BOOL bMulTex
///             BOOL bLOD
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerrain::Render(LPDIRECT3DDEVICE9 pd3dDevice, CFOVClipper* pClipper, DWORD& nTri,const D3DXVECTOR3& vEyePt,BOOL bMulTex,BOOL bLOD)
{
   D3DXMATRIX matWorld;
   D3DXMatrixIdentity( &matWorld );
   sys_verify_hr(pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld ));

   D3DMATERIAL9 mtrl;
   D3DUtil::InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
   sys_verify_hr(pd3dDevice->SetMaterial( &mtrl ));
   sys_verify_hr(pd3dDevice->SetVertexShader( NULL ));
   sys_verify_hr(pd3dDevice->SetFVF( D3DFVF_TERRAINVERTEX ));

   sys_verify_hr(pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TERRAINVERTEX) ));
   sys_verify_hr(pd3dDevice->SetIndices( m_pIB ));

   ViewData data;

   if(m_pIB == NULL)
      return E_FAIL;

   // Copy indices for the dolphin mesh
   data.FacestoDraw = 0;
   sys_verify_hr(m_pIB->Lock( 0, m_dwNumFaces * 3 * sizeof(WORD), (void**)&data.pDst, D3DLOCK_DISCARD ));
   data.nIndexes = 0;
   data.sizez=m_sizez;
   data.m_EyePos = vEyePt;
   data.m_bLowDetail = !bLOD;

   sys_verify(m_pRoot->ProcessElements(pClipper,&data,&CTerrain::ProcessElement));

   data.pDst[data.nIndexes++] = 0;
   data.pDst[data.nIndexes++] = 0;
   data.pDst[data.nIndexes++] = 0;

   m_pIB->Unlock();


   m_nTri = 0;



   if(data.FacestoDraw > 0)
   {
      for(UINT i=0; i < m_Textrues.size();i++)
      {

         sys_verify_hr(pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   i==0?FALSE:TRUE ));
         sys_verify_hr(pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA ));
         sys_verify_hr(pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA ));


         sys_verify_hr(pd3dDevice->SetTexture( 0, m_Textrues[i].m_pTexture ));
         sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 ));
         sys_verify_hr(pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ));
         sys_verify_hr(pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ));
         sys_verify_hr(pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT ));

         sys_verify_hr(pd3dDevice->SetTransform( D3DTS_TEXTURE0 , &m_Textrues[i].m_TexMatrix ));
         sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 ));

         sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   i==0?D3DTOP_SELECTARG1:D3DTOP_MODULATE ));
         sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ));
         sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT ));
         sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   i==0?D3DTOP_DISABLE:D3DTOP_SELECTARG1 ));
         sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ));

         sys_verify_hr(pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR));
         sys_verify_hr(pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR));

         //second texture for incresed level of detal
         if(bMulTex)
         {
            sys_verify_hr(pd3dDevice->SetTexture( 1, m_Textrues[i].m_pAlphaTexture ));
            sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 ));
            sys_verify_hr(pd3dDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ));
            sys_verify_hr(pd3dDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ));
            sys_verify_hr(pd3dDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_POINT ));

            sys_verify_hr(pd3dDevice->SetTransform( D3DTS_TEXTURE1 , &m_Textrues[i].m_TexAlphaMatrix));
            sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 ));

            sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE ));
            sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE ));
            sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT ));
            sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 ));
            sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ));
         }
         else
            sys_verify_hr(pd3dDevice->SetTexture( 1, NULL ));


         sys_verify_hr(pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0 ,0, m_dwNumVertices-1,0, data.FacestoDraw+1));

         m_nTri += data.FacestoDraw+1;

         if(!bMulTex) // only redner the first pass if mulit texture is off
            break;
      }
   }

   nTri+=m_nTri;

   sys_verify_hr(pd3dDevice->SetTexture( 0, NULL ));
   sys_verify_hr(pd3dDevice->SetTexture( 1, NULL ));
   sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE ));
   sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE ));
   sys_verify_hr(pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE ));
   sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE ));
   sys_verify_hr(pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP));
   sys_verify_hr(pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP));
   sys_verify_hr(pd3dDevice->SetSamplerState(1,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP));
   sys_verify_hr(pd3dDevice->SetSamplerState(1,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP));

   sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE ));
   sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ));
   sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT ));


   sys_verify_hr(pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE ));

   sys_verify_hr(pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE ));


   return S_OK;
}



/////////////////////////////////////////////////
///
/// NAME: LoadDEM
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Loads the Hightmap
///
/// CREATED:     02/06/2004 5:03:05 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerrain::LoadDEM()
{
   D3DSURFACE_DESC pDesc;
   sys_verify_hr(m_pHightMapTex->GetLevelDesc(0, &pDesc ));
   D3DLOCKED_RECT d3dlr;
   sys_verify_hr(m_pHightMapTex->LockRect( 0, &d3dlr, 0, 0 ));
   DWORD * pDst = (DWORD *)d3dlr.pBits;
   int DPitch = d3dlr.Pitch/4;

   m_sizex = pDesc.Width;
   m_sizez = pDesc.Height;
   m_halfx  = m_sizex/2;
   m_halfz  = m_sizez/2;

   m_RealSizeX = m_sizex*m_MapScale;
   m_RealSizeZ = m_sizez*m_MapScale;

   if(m_pHightMap)
      delete [] m_pHightMap;
   m_pHightMap = new BYTE[m_sizez*m_sizex];

   for (DWORD y=0; y<pDesc.Height; ++y)
   {
      for (DWORD x=0; x<pDesc.Width; ++x)
      {
         int index = y*DPitch + x;
         DWORD tmpx = ((m_sizex-1)-y);
         DWORD tmpy = x;//((m_sizex-1)-x);
         DWORD bitpos = ((tmpx)*m_sizex) + tmpy;
         D3DXCOLOR c(pDst[index]);
         m_pHightMap[bitpos] = c.r * 255.0f;
      }
   }

   sys_verify_hr(m_pHightMapTex->UnlockRect(0));
}



/////////////////////////////////////////////////
///
/// NAME: GetHightAtPos
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Gets the Approx Hight at a given postion
///
/// CREATED:     02/06/2004 5:03:54 PM
///
/// PARAMETERS: 
///             FLOAT x
///             FLOAT z
///
/// RETURN: FLOAT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
FLOAT CTerrain::GetHightAtPos(FLOAT x,FLOAT z)
{
   DWORD bitpos = (((DWORD)(z)+m_halfz) * m_sizex)+((DWORD)(x)+m_halfx);
   FLOAT y = -1.0f;
   if(m_pHightMap && x > -m_halfx && x < m_halfx  && z > -m_halfz && z < m_halfz)
      y = ((FLOAT)m_pHightMap[bitpos]) * (m_MapHightDev256);
   return y;
}



/////////////////////////////////////////////////
///
/// NAME: GetPaintPos
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Get the point on the texture given a point on the terrain
///
/// CREATED:     02/06/2004 5:06:24 PM
///
/// PARAMETERS: 
///             D3DXVECTOR3 point
///
/// RETURN: CPoint 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CPoint CTerrain::GetPaintPos(const D3DXVECTOR3& point)
{
   int xpos = (int)point.x/m_MapScale +m_halfx ;
   int ypos = ((m_sizex-1)-((int)point.z/m_MapScale +m_halfz));
   return CPoint(xpos,ypos);
}



/////////////////////////////////////////////////
///
/// NAME: GetHightAtPosR
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Get the calulated hight at the current position
///
/// CREATED:     02/06/2004 5:05:24 PM
///
/// PARAMETERS: 
///             FLOAT x
///             FLOAT z
///
/// RETURN: FLOAT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
FLOAT CTerrain::GetHightAtPosR(FLOAT x,FLOAT z)
{
   LONG lx = (LONG)(x/m_MapScale);
   LONG lz = (LONG)(z/m_MapScale);
   FLOAT mapx = (FLOAT)lx;
   FLOAT mapz = (FLOAT)lz;
   int xfactor = 1;
   int zfactor = 1;
   if(x<0.0f)
      xfactor = -1;
   if(z<0.0f)
      zfactor = -1;

   FLOAT y = GetHightAtPos(mapx,mapz);
   D3DXVECTOR3 p1 = D3DXVECTOR3(mapx,y,mapz);

   y = GetHightAtPos(mapx+xfactor,mapz);
   D3DXVECTOR3 p2 = D3DXVECTOR3(mapx+xfactor,y,mapz);

   y = GetHightAtPos(mapx+xfactor,mapz+zfactor);
   D3DXVECTOR3 p3 = D3DXVECTOR3(mapx+xfactor,y,mapz+zfactor);

   y = GetHightAtPos(mapx,mapz+zfactor);
   D3DXVECTOR3 p4 = D3DXVECTOR3(mapx,y,mapz+zfactor);

   D3DXVECTOR3 cp = D3DXVECTOR3(x/m_MapScale,m_MapHight,z/m_MapScale);
   D3DXVECTOR3 d = D3DXVECTOR3(0.0f,-1.0f,0.0f);

   FLOAT Ix,Iy,Iz;
   //D3DXPLANE plane1;
   //D3DXPlaneFromPoints(&plane1,&p1,&p2,&p4);

   if(D3DXIntersectTri(&p1,&p2,&p4,&cp,&d,&Ix, &Iz,&Iy))
      return m_MapHight - Iy;

   if(D3DXIntersectTri(&p3,&p2,&p4,&cp,&d,&Ix, &Iz,&Iy))
      return m_MapHight - Iy;

   return 0.0f;
}




/////////////////////////////////////////////////
///
/// NAME: DeleteDeviceObjects
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Destroys all device-dependent objects
///
/// CREATED:     02/06/2004 4:56:24 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerrain::DeleteDeviceObjects()
{
   m_MiniMap.DeleteDeviceObjects();
   m_pVB = NULL;
   m_pIB = NULL;
   D3DUtil::ReleaseTexture( m_pHightMapTex );

   for(UINT i=0; i< m_Textrues.size();i++)
   {
      D3DUtil::ReleaseTexture( m_Textrues[i].m_pTexture );
      m_Textrues[i].m_pTexture = NULL;
      D3DUtil::ReleaseTexture( m_Textrues[i].m_pAlphaTexture );
      m_Textrues[i].m_pAlphaTexture = NULL;
   }
}



/////////////////////////////////////////////////
///
/// NAME: SetupMap
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Sets up the Terrain
///
/// CREATED:     02/06/2004 5:06:44 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             CString hmap
///             FLOAT MapHight
///             std::vector<CTextureData*>& Textures
///             FLOAT MapScale
///             CZipArchive* pZip
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerrain::SetupMap(LPDIRECT3DDEVICE9 pd3dDevice,const TCHAR* hmap, FLOAT MapHight, std::vector<CTextureData*>& Textures,FLOAT MapScale,CZipArchive* pZip)
{
   m_MapScale = MapScale;
   m_MapHight = MapHight;
   m_MapHightDev256 = m_MapHight/256.0f;
   m_HightMapFile = hmap;

   int BaseTextrue = -1;

   for(UINT i=0; i< Textures.size();i++)
   {
      if(Textures[i]->m_bBaseTextrue)
      {
         BaseTextrue = i;
         CPassData PassData;
         PassData.m_TexFile = Textures[i]->m_File;
         PassData.m_TexSize = Textures[i]->m_fScale;
         PassData.m_AlphaFile = Textures[i]->m_AlphaFile;
         PassData.m_AlphaSize = Textures[i]->m_fAlphaScale;

         if(PassData.m_TexSize==0.0f)	PassData.m_TexSize = 1.0f;
         if(PassData.m_AlphaSize==0.0f)	PassData.m_AlphaSize = 1.0f;

         D3DXMatrixScaling(&PassData.m_TexMatrix, PassData.m_TexSize, PassData.m_TexSize, 1.0f);
         D3DXMatrixScaling(&PassData.m_TexAlphaMatrix, PassData.m_AlphaSize, PassData.m_AlphaSize, 1.0f);
         m_Textrues.push_back(PassData);
         break;
      }
   }


   for(i=0; i< Textures.size();i++)
   {
      if(BaseTextrue != i)
      {
         CPassData PassData;
         PassData.m_TexFile = Textures[i]->m_File;
         PassData.m_TexSize = Textures[i]->m_fScale;
         PassData.m_AlphaFile = Textures[i]->m_AlphaFile;
         PassData.m_AlphaSize = Textures[i]->m_fAlphaScale;

         if(PassData.m_TexSize==0.0f)	PassData.m_TexSize = 1.0f;
         if(PassData.m_AlphaSize==0.0f)	PassData.m_AlphaSize = 1.0f;

         D3DXMatrixScaling(&PassData.m_TexMatrix, PassData.m_TexSize, PassData.m_TexSize, 1.0f);
         D3DXMatrixScaling(&PassData.m_TexAlphaMatrix, PassData.m_AlphaSize, PassData.m_AlphaSize, 1.0f);
         m_Textrues.push_back(PassData);
      }
   }
}



/////////////////////////////////////////////////
///
/// NAME: RenderMiniMap
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Renders the MiniMap
///
/// CREATED:     02/06/2004 5:06:53 PM
///
/// PARAMETERS: 
///             LPDIRECT3DDEVICE9 pd3dDevice
///             POINT p
///             C2DDraw* pDisplay
///             FLOAT angle
///
/// RETURN: HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
HRESULT CTerrain::RenderMiniMap(LPDIRECT3DDEVICE9 pd3dDevice,POINT p, FLOAT angle)
{
   return m_MiniMap.DrawAngle(pd3dDevice,p.x,p.y,NULL,angle);
}



/////////////////////////////////////////////////
///
/// NAME: ConvertToMiniMapPoint
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Gets a minimap point
///
/// CREATED:     02/06/2004 5:07:06 PM
///
/// PARAMETERS: 
///             FLOAT& x
///             FLOAT& y
///             FLOAT angle
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerrain::ConvertToMiniMapPoint(FLOAT& x,FLOAT& y,FLOAT angle)
{
   D3DXMATRIX matRotate2;
   D3DXMatrixRotationZ( &matRotate2, angle);

   x = ((x+((FLOAT)GetXSize()/2.0f))/(FLOAT)GetXSize())*MINI_MAP_SIZE - MINI_MAP_SIZE/2.0f;
   y = ((y+((FLOAT)GetZSize()/2.0f))/(FLOAT)GetZSize())*MINI_MAP_SIZE;
   y = -(y - MINI_MAP_SIZE) - MINI_MAP_SIZE/2.0f;


   D3DXVECTOR2 v(x,y);
   D3DXVec2TransformCoord(&v,&v,&matRotate2);
   x = v.x;
   y = v.y;
}

#define UINT_SIZE 2.0f



/////////////////////////////////////////////////
///
/// NAME: Intersect
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Checks to see if the vector intersects with the terrain
///
/// CREATED:     02/06/2004 5:07:49 PM
///
/// PARAMETERS: 
///             D3DXVECTOR3 p1
///             D3DXVECTOR3 p2
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerrain::Intersect(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2)
{
   D3DXVECTOR3 v = p2-p1;
   D3DXVECTOR3 projXZ = v;
   projXZ.y = 0.0f;
   FLOAT l = D3DXVec3Length(&projXZ);
   D3DXVec3Scale(&v,&v,UINT_SIZE/l);
   D3DXVECTOR3  currentpos = p1;

   FLOAT currentlenght = 0.0f;

   while(currentlenght < l)
   {
      currentlenght+=UINT_SIZE;
      currentpos+=v;
      FLOAT hight = GetHightAtPosR(currentpos.x,currentpos.z);
      if(hight > currentpos.y)
         return TRUE;
      if(hight<0.0f)
         return FALSE;
   }

   return FALSE;
}



/////////////////////////////////////////////////
///
/// NAME: IntersectV
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Checks to see if the vector intesects with the terrain
///
/// CREATED:     02/06/2004 5:09:14 PM
///
/// PARAMETERS: 
///             D3DXVECTOR3 Origin
///             D3DXVECTOR3 Dir
///
/// RETURN: D3DXVECTOR3 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
D3DXVECTOR3 CTerrain::IntersectV(const D3DXVECTOR3& Origin,const D3DXVECTOR3& Dir)
{
   D3DXVECTOR3 hit = D3DXVECTOR3(0,0,0);
   D3DXVECTOR3 v = Dir;
   D3DXVECTOR3 projXZ = v;
   projXZ.y = 0.0f;
   FLOAT l = D3DXVec3Length(&projXZ);

   D3DXVec3Scale(&v,&v,UINT_SIZE/l);
   D3DXVECTOR3  currentpos = Origin;

   l = 500;

   FLOAT currentlenght = 0.0f;

   while(currentlenght < l)
   {
      currentlenght+=UINT_SIZE;
      currentpos+=v;
      FLOAT hight = GetHightAtPosR(currentpos.x,currentpos.z);
      if(hight > currentpos.y)
      {
         hit = currentpos;
         return hit;
      }
      if(hight<0.0f)
         return hit;
   }

   return hit;
}


/////////////////////////////////////////////////
///
/// NAME: computeNormals
///
/// DESCRIPTION: Calculates the Hightmaps normals
///
/// CREATED:     02/06/2004 5:09:51 PM
///
/// PARAMETERS: 
///             BYTE *hmap
///             int sizex
///
/// RETURN: D3DXVECTOR3* 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
D3DXVECTOR3* computeNormals(BYTE *hmap,int sizex)
{

   /////////////////////////////////////////////////////////

   int w = sizex;
   D3DXVECTOR3* n = new D3DXVECTOR3[w*w];
   if (!n) 
      return NULL;

   memset(n,0,sizeof(D3DXVECTOR3)*w*w);

   /*
   v2
   v1*---*---*v3
   |0 /|2 /|
   | / | / |
   |/ 1|/ 3|
   v8*---v0--*v4
   |7 /|4 /|
   | / | / |
   |/ 6|/ 5|
   v7*---*--*v5
   v6
   */

   for (int j=1; j<w-1; j++)
   {
      for (int i=1; i<w-1; i++)
      {
         /*
         D3DXVECTOR3 v0 = D3DXVECTOR3((i  )-w/2,hmap[(j  )*w+i  ],(j  )-w/2);
         D3DXVECTOR3 v1 = D3DXVECTOR3((i-1)-w/2,hmap[(j-1)*w+i-1],(j-1)-w/2);
         D3DXVECTOR3 v2 = D3DXVECTOR3((i  )-w/2,hmap[(j-1)*w+i  ],(j-1)-w/2);
         D3DXVECTOR3 v3 = D3DXVECTOR3((i+1)-w/2,hmap[(j-1)*w+i+1],(j-1)-w/2);
         D3DXVECTOR3 v4 = D3DXVECTOR3((i+1)-w/2,hmap[(j  )*w+i+1],(j  )-w/2);
         D3DXVECTOR3 v5 = D3DXVECTOR3((i+1)-w/2,hmap[(j+1)*w+i+1],(j+1)-w/2);
         D3DXVECTOR3 v6 = D3DXVECTOR3((i  )-w/2,hmap[(j+1)*w+i  ],(j+1)-w/2);
         D3DXVECTOR3 v7 = D3DXVECTOR3((i-1)-w/2,hmap[(j+1)*w+i-1],(j+1)-w/2);
         D3DXVECTOR3 v8 = D3DXVECTOR3((i-1)-w/2,hmap[(j  )*w+i-1],(j  )-w/2);

         D3DXVECTOR3 t1,t2;
         D3DXVECTOR3 f0,f1,f2,f3,f4,f5,f6,f7;

         t1 = v0-v2;
         t2 = v2-v8;
         D3DXVec3Cross(&f1,D3DXVec3Normalize(&t1,&t1),D3DXVec3Normalize(&t2,&t2));


         D3DXVec3Cross(&f2,D3DXVec3Normalize(&t1,&(v3-v2)),D3DXVec3Normalize(&t2,&(v2-v0)));
         D3DXVec3Cross(&f3,D3DXVec3Normalize(&t1,&(v4-v3)),D3DXVec3Normalize(&t2,&(v3-v0)));
         D3DXVec3Cross(&f4,D3DXVec3Normalize(&t1,&(v5-v4)),D3DXVec3Normalize(&t2,&(v4-v6)));
         D3DXVec3Cross(&f5,D3DXVec3Normalize(&t1,&(v6-v0)),D3DXVec3Normalize(&t2,&(v0-v7)));
         D3DXVec3Cross(&f6,D3DXVec3Normalize(&t1,&(v0-v8)),D3DXVec3Normalize(&t2,&(v8-v7)));
         D3DXVECTOR3 vNormal = f1 + f2 + f3 + f5 + f6 + f7;
         */

         D3DXVECTOR3 v0 = D3DXVECTOR3((i  )-w/2,hmap[(j  )*w+i  ],(j  )-w/2);
         D3DXVECTOR3 v2 = D3DXVECTOR3((i  )-w/2,hmap[(j-1)*w+i  ],(j-1)-w/2);
         D3DXVECTOR3 v4 = D3DXVECTOR3((i+1)-w/2,hmap[(j  )*w+i+1],(j  )-w/2);
         D3DXVECTOR3 v6 = D3DXVECTOR3((i  )-w/2,hmap[(j+1)*w+i  ],(j+1)-w/2);
         D3DXVECTOR3 v8 = D3DXVECTOR3((i-1)-w/2,hmap[(j  )*w+i-1],(j  )-w/2);

         D3DXVECTOR3 t1,t2;
         D3DXVECTOR3 f1,f2,f3,f4;

         t1 = v0-v2;
         t2 = v0-v8;
         D3DXVec3Cross(&f1,D3DXVec3Normalize(&t1,&t1),D3DXVec3Normalize(&t2,&t2));

         t1 = v0-v4;
         t2 = v0-v2;
         D3DXVec3Cross(&f2,D3DXVec3Normalize(&t1,&t1),D3DXVec3Normalize(&t2,&t2));

         t1 = v0-v6;
         t2 = v0-v4;
         D3DXVec3Cross(&f3,D3DXVec3Normalize(&t1,&t1),D3DXVec3Normalize(&t2,&t2));

         t1 = v0-v8;
         t2 = v0-v6;
         D3DXVec3Cross(&f4,D3DXVec3Normalize(&t1,&t1),D3DXVec3Normalize(&t2,&t2));
         D3DXVECTOR3 vNormal = f1+f2+f3+f4;

         D3DXVec3Normalize(&vNormal,&vNormal);
         n[(j*w+i)] =  vNormal;
      }
   }
   return n;
}

struct ray
{
   D3DXVECTOR3 direction;
   D3DXVECTOR3 origin;
};

inline int __stdcall ifloor(FLOAT  x)
{
   unsigned long e = (0x7F + 31) - ((* (unsigned long *) &x & 0x7F800000) >> 23);
   unsigned long m = 0x80000000 | (* (unsigned long *) &x << 8);
   return (m >> e) & -(e < 32);
}



/////////////////////////////////////////////////
///
/// NAME: intersect_map
///
/// DESCRIPTION: Creates the Intersect map
///
/// CREATED:     02/06/2004 5:10:15 PM
///
/// PARAMETERS: 
///             const D3DXVECTOR3& iv
///             const ray& r
///             BYTE *hm
///             int sizex
///             FLOAT fHeightScale
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int intersect_map(const D3DXVECTOR3& iv,const ray& r,BYTE *hm,int sizex,FLOAT fHeightScale)
{
   D3DXVECTOR3 v = iv + r.direction;
   int w = sizex;
   int hits = 0;

   while (!(( v.x >= w-1 ) || ( v.x <= 0 ) || ( v.z >= w-1 ) || ( v.z <= 0 )))
   {
      FLOAT D = D3DXVec3Length(&(D3DXVECTOR3(v.x,0,v.z)-D3DXVECTOR3(r.origin.x,0,r.origin.z))); // length of lightdir's projection
      FLOAT d = D3DXVec3Length(&(iv-v));
      FLOAT h = iv.y + (d*r.origin.y) / D;
      if (hm[ifloor(v.z)* w + ifloor(v.x)] * fHeightScale > h){
         hits++;
         break;
      };
      D3DXVECTOR3 dir = r.direction;
      dir.y = 0;
      D3DXVec3Normalize(&dir,&dir);
      v += dir;
   };
   return hits;
}



/////////////////////////////////////////////////
///
/// NAME: genLightmap
///
/// DESCRIPTION: Generates the light map for a hight map
///
/// CREATED:     02/06/2004 5:10:36 PM
///
/// PARAMETERS: 
///             D3DXVECTOR3* normal
///             BYTE* hm
///             D3DXVECTOR3 fSunDir
///             D3DXCOLOR SunColor
///             int w
///             D3DXCOLOR Ambient
///             int MaxHight
///             FLOAT MapScale
///
/// RETURN: D3DXCOLOR* 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
D3DXCOLOR* genLightmap(D3DXVECTOR3* normal,BYTE* hm,D3DXVECTOR3 fSunDir,D3DXCOLOR SunColor,int w,D3DXCOLOR Ambient,int MaxHight, FLOAT MapScale)
{
   //int hits;
   FLOAT fHeightScale = ((FLOAT)MaxHight / MapScale) / 255.0f;
   D3DXCOLOR* lmap = new D3DXCOLOR[w*w];
   if (!lmap)
   {
      return NULL;
   };
   D3DXVec3Normalize(&fSunDir,&fSunDir);

   for (int j=0; j<w; j++)
   {
      for (int i=0; i<w; i++)
      {
         FLOAT hight = ((FLOAT)hm[j*w+i]) * fHeightScale;
         D3DXVECTOR3 fVertex = D3DXVECTOR3(i,hight,j);
         D3DXCOLOR f = Ambient ;

         ray r;
         r.origin = fVertex + fSunDir * 2000.0f;
         r.direction = fSunDir;

         if (!intersect_map(fVertex,r,hm,w,fHeightScale))
         {
            D3DXVECTOR3 n = normal[(j*w+i)];
            FLOAT dot = D3DXVec3Dot(&n,&fSunDir);
            f.r += 0.5f*(1.0f+dot) * SunColor.r;
            f.g += 0.5f*(1.0f+dot) * SunColor.g;
            f.b += 0.5f*(1.0f+dot) * SunColor.b;
            if (f.r>1.0f) f.r = 1.0f;
            if (f.g>1.0f) f.g = 1.0f;
            if (f.b>1.0f) f.b = 1.0f;
         };

         lmap[j*w+i] = f;
      };
   };
   return lmap;
};

struct FillData
{
   int m_Sizex;
   D3DXCOLOR* m_pShadowmap;
   FLOAT* m_pAlpha;
   BOOL m_bInvertAlpha;
};




/////////////////////////////////////////////////
///
/// NAME: FillShadowMap
///
/// DESCRIPTION: Fills the shadow map
///
/// CREATED:     02/06/2004 5:13:17 PM
///
/// PARAMETERS: 
///              D3DXVECTOR4* pOut
///             D3DXVECTOR2* pTexCoord
///             D3DXVECTOR2* pTexelSize
///             LPVOID pData
///
/// RETURN: VOID 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
VOID FillShadowMap( D3DXVECTOR4* pOut, CONST D3DXVECTOR2* pTexCoord, CONST D3DXVECTOR2* pTexelSize, LPVOID pData)
{
   FillData* pFillData = (FillData*)pData;

   int x = (pTexCoord->x * pFillData->m_Sizex);
   int y = pFillData->m_Sizex - (pTexCoord->y * pFillData->m_Sizex);

   int index = (y*pFillData->m_Sizex)+x;

   D3DXCOLOR val = pFillData->m_pShadowmap[index];
   FLOAT Alpha = 1.0f;
   if(pFillData->m_bInvertAlpha)
      Alpha = 0.0f;

   if(pFillData->m_pAlpha)
   {
      y = (pTexCoord->y * pFillData->m_Sizex);
      index = (y*pFillData->m_Sizex)+x;
      Alpha = pFillData->m_pAlpha[index];
   }
   *pOut = D3DXVECTOR4(val.r,val.g,val.b,Alpha);

}



/////////////////////////////////////////////////
///
/// NAME: GetAlphaFromTexture
///
/// DESCRIPTION: Gets the Alpha map from the textrue
///
/// CREATED:     02/06/2004 5:12:13 PM
///
/// PARAMETERS: 
///             LPDIRECT3DTEXTURE9 pTexture
///             int w
///
/// RETURN: FLOAT* 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
FLOAT* GetAlphaFromTexture(LPDIRECT3DTEXTURE9 pTexture,UINT w)
{
   FLOAT* lmap = new FLOAT[w*w];
   if (lmap==NULL || pTexture == NULL)
   {
      return NULL;
   };

   D3DSURFACE_DESC pDesc;
   pTexture->GetLevelDesc(0, &pDesc );

   D3DLOCKED_RECT d3dlr;
   pTexture->LockRect( 0, &d3dlr, 0, 0 );
   DWORD * pDst = (DWORD *)d3dlr.pBits;
   int DPitch = d3dlr.Pitch/4;

   //memset(pDst,c,pDesc.Height*DPitch);

   for (DWORD y=0; y<w; ++y)
   {
      for (DWORD x=0; x<w; ++x)
      {
         int index = y*DPitch + x;
         D3DXCOLOR c(pDst[index]);
         lmap[y*w+x] = c.a;
      }
   }

   pTexture->UnlockRect(0);
   return lmap;
}






/////////////////////////////////////////////////
///
/// NAME: GenerateLightMap
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Generates the Light map
///
/// CREATED:     02/06/2004 5:11:24 PM
///
/// PARAMETERS: 
///             const char* filename
///             LPDIRECT3DDEVICE9 pd3dDevice
///             D3DXVECTOR3 vSunDir
///             D3DXCOLOR SunColor
///             D3DXCOLOR AmbientColor
///             LPDIRECT3DTEXTURE9 pAlphaTexture
///             BOOL bInvertAlpha
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerrain::GenerateLightMap(const TCHAR* filename,LPDIRECT3DDEVICE9 pd3dDevice,const D3DXVECTOR3& vSunDir, D3DXCOLOR SunColor, D3DXCOLOR AmbientColor,LPDIRECT3DTEXTURE9 pAlphaTexture,BOOL bInvertAlpha)
{
   D3DXVECTOR3* normalMap=NULL;
   D3DXCOLOR* shadowmap = NULL;
   FLOAT* pAlpha = NULL;
   normalMap = computeNormals(m_pHightMap,m_sizex);
   shadowmap = genLightmap(normalMap,m_pHightMap,vSunDir,SunColor,m_sizex,AmbientColor,m_MapHight,m_MapScale);
   if(pAlphaTexture)
   {
      pAlpha = GetAlphaFromTexture(pAlphaTexture,m_sizex);
   }

   LPDIRECT3DTEXTURE9 pTexture;
   // render shadowmap to surface
   D3DXCreateTexture(pd3dDevice,m_sizex,m_sizez,0,0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&pTexture );
   if(pTexture)
   {
      FillData data;
      data.m_Sizex = m_sizex;
      data.m_pShadowmap = shadowmap;
      data.m_pAlpha = pAlpha;
      data.m_bInvertAlpha = bInvertAlpha;
      D3DXFillTexture(pTexture,(LPD3DXFILL2D) &FillShadowMap,(void*)&data);
      D3DXSaveTextureToFile(filename,D3DXIFF_DDS,pTexture,NULL);
      SAFE_RELEASE(pTexture);
   }

   if (normalMap) 
      delete [] normalMap;
   if (shadowmap) 
      delete [] shadowmap;
   if(pAlpha)
      delete [] pAlpha;
}



/////////////////////////////////////////////////
///
/// NAME: SaveHightMap
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Saves the current hight map
///
/// CREATED:     02/06/2004 5:11:02 PM
///
/// PARAMETERS: 
///             CZipArchive* pZip
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerrain::SaveHightMap(CZipArchive* pZip)
{
   D3DXSaveTextureToFile(m_HightMapFile,D3DXIFF_BMP,m_pHightMapTex,NULL);

   if(pZip != NULL)
   {
      int index = pZip->FindFile(m_HightMapFile);
      if( index != -1)
         pZip->DeleteFile(index);
      pZip->AddNewFile(m_HightMapFile);
      DeleteFile(m_HightMapFile);
   }
}



/////////////////////////////////////////////////
///
/// NAME: SaveTextures
///
/// CLASS: CTerrain
///
/// DESCRIPTION: Saves the current textures
///
/// CREATED:     02/06/2004 5:11:12 PM
///
/// PARAMETERS: 
///             CZipArchive* pZip
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerrain::SaveTextures(CZipArchive* pZip)
{
   for(UINT i=1; i< m_Textrues.size();i++)
   {
      if(!m_Textrues[i].m_AlphaFile.IsEmpty())
      {
         D3DXSaveTextureToFile(m_Textrues[i].m_AlphaFile,D3DXIFF_DDS,m_Textrues[i].m_pAlphaTexture,NULL);
         if(pZip != NULL)
         {
            int index = pZip->FindFile(m_Textrues[i].m_AlphaFile);
            if( index != -1)
               pZip->DeleteFile(index);
            pZip->AddNewFile(m_Textrues[i].m_AlphaFile);
            DeleteFile(m_Textrues[i].m_AlphaFile);
         }
      }
   }
}
