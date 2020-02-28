// Trees.cpp: implementation of the CPlanetObjects class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Trees.h"
#include "D3DApp.h"
#include "DXUtil.h"
#include "Water.h"
#include "Bush.h"
#include "Tree.h"
#include "StaticMesh.h"
#include "PowerUp.h"
#include "WayPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TREE_DENC 1000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Tree textures to use



CPlanetObjects::CPlanetObjects()
{
   m_pRoot = NULL;
   m_bShadowBuilt = FALSE;
}

CPlanetObjects::~CPlanetObjects()
{
   if(m_pRoot)
   {
      m_pRoot->Distroy(TRUE);
      delete m_pRoot;
   }

   /*for(int i=0;i<m_pObjectList.size();i++)
   {
   delete m_pObjectList[i];
   }*/
}

BOOL CPlanetObjects::AddObject(CTerrain* pFloor, CWater* pWater, CPlanetObjectData* pPlanetObjectData,CLevelDataObject* pObjData)
{

   FLOAT WaterLevel = 0.0f;

   if(pWater)
      WaterLevel = pWater->GetWaterLevel();

   D3DXVECTOR3 pos = pObjData->m_pos;

   pos.y  = max(pos.y,pFloor->GetHightAtPosR( pos.x, pos.z ));
   pos.y  = max(WaterLevel,pos.y);



   CObjPos ObjData;
   ObjData.m_CenterPoint = pos;
   ObjData.m_Raidus = 1.0f;
   ObjData.m_Type = pObjData->m_Type;

   CStaticObj* pGameObj;



   switch(pPlanetObjectData[ObjData.m_Type].m_Type)
   {
   case OBJECTTYPE_BUSH:
      {
         pGameObj = new CBush(pPlanetObjectData[ObjData.m_Type].m_xSize,
            pPlanetObjectData[ObjData.m_Type].m_ySize,
            pPlanetObjectData[ObjData.m_Type].m_File);

         ObjData.m_NumbSubItems = MAX_SUB_ITEMS;
         for(DWORD j=0;j<ObjData.m_NumbSubItems;j++)
         {
            FLOAT x = (rand()%20 * 0.1f) - 1.0f;
            FLOAT z = (rand()%20 * 0.1f) - 1.0f;
            FLOAT y = pFloor->GetHightAtPosR( ObjData.m_CenterPoint.x+x, ObjData.m_CenterPoint.z+z) - ObjData.m_CenterPoint.y;
            ObjData.m_SubItems[j].m_Offset = D3DXVECTOR3(x,y,z);
            ObjData.m_SubItems[j].m_Rotation = ToRad(rand()%360);
         }
      }

      break;
   case OBJECTTYPE_TREE:
      pGameObj = new CTree(pPlanetObjectData[ObjData.m_Type].m_xSize,
         pPlanetObjectData[ObjData.m_Type].m_ySize,
         pPlanetObjectData[ObjData.m_Type].m_File);
      break;
   case OBJECTTYPE_MESH:
      pGameObj = new CStaticMesh(pPlanetObjectData[ObjData.m_Type].m_File);
      pGameObj->Create(pPlanetObjectData[ObjData.m_Type].m_File,pPlanetObjectData[ObjData.m_Type].m_fScale,D3DXVECTOR3(0,0,0));
      static_cast<CStaticMesh*>(pGameObj)->ShowShadow(pPlanetObjectData[ObjData.m_Type].m_bShadow);
      break;
   case  OBJECTTYPE_POWERUP:
      pGameObj = new CPowerUp(pPlanetObjectData[ObjData.m_Type].m_PowerUp,pPlanetObjectData[ObjData.m_Type].m_File);
      pGameObj->Create(pPlanetObjectData[ObjData.m_Type].m_File,pPlanetObjectData[ObjData.m_Type].m_fScale,D3DXVECTOR3(0,0,0));
      static_cast<CStaticMesh*>(pGameObj)->ShowShadow(pPlanetObjectData[ObjData.m_Type].m_bShadow);
      break;

   case OBJECTTYPE_WAYPOINT:
      pGameObj = new CWayPoint(pPlanetObjectData[ObjData.m_Type].m_File);
      pGameObj->Create(pPlanetObjectData[ObjData.m_Type].m_File,pPlanetObjectData[ObjData.m_Type].m_fScale,D3DXVECTOR3(0,0,0));
      static_cast<CStaticMesh*>(pGameObj)->ShowShadow(pPlanetObjectData[ObjData.m_Type].m_bShadow);
      break;

   }

   pGameObj->SetVisable(pObjData->m_bVisable);
   pGameObj->SetObjData(&ObjData);
   pGameObj->SetName(pObjData->m_Name);
   pGameObj->SetType(pPlanetObjectData[ObjData.m_Type].m_Type);


   if(!m_pRoot->Add(pGameObj))
   {
      delete pGameObj;
   }
   else
   {
      m_pObjectList.push_back(pGameObj);
   }
   return TRUE;
}

BOOL CPlanetObjects::CreateObjects(CTerrain* pFloor, CWater* pWater, CPlanetObjectData* pPlanetObjectData, DWORD MaxPlanetObjectData,CLevelData* pLevel)
{
   if(m_pRoot)
   {
      m_pRoot->Distroy(TRUE);
      delete m_pRoot;
   }

   m_pRoot = new COctTree<CStaticObj>();

   FLOAT xsize = (FLOAT)pFloor->GetXSize()/2.0f;
   FLOAT zsize = (FLOAT)pFloor->GetZSize()/2.0f;
   //FLOAT ysize = (FLOAT)pFloor->GetXSize()/2.0f;
   m_pRoot->Create(D3DXVECTOR3(-xsize,-zsize,-xsize),D3DXVECTOR3(xsize,zsize,xsize),1.0f,9);


   for(int i=0;i<pLevel->m_Objects.size();i++)
   {
      CLevelDataObject* pObj = pLevel->m_Objects[i];
      AddObject(pFloor,pWater,pPlanetObjectData,pObj);
   }
   m_pRoot->Optimize();
   return TRUE;
}


struct ObjectData
{
   LPDIRECT3DDEVICE9 pd3dDevice;
   D3DXVECTOR3 m_EyePoint;
   CSortedList<CStaticObj*>* m_pDisplayList;
   BOOL m_bDisplayShadows;
};

bool Compare(CStaticObj* pObja,CStaticObj* pObjb)
{
   return pObja->m_Distance > pObjb->m_Distance;
}

BOOL AFXAPI CPlanetObjects::ProcessElement( void* pvObj, void* pData )
{
   ObjectData* pObjData = (ObjectData*)pData;
   CStaticObj* pObj  = (CStaticObj*)pvObj;


   D3DXVECTOR3 vdist = pObjData->m_EyePoint - pObj->GetPos();
   FLOAT d = D3DXVec3LengthSq(&vdist);
   if(d < TERRAIN_LOD2)
   {
      pObj->m_Distance = d;
      pObjData->m_pDisplayList->Add(pObj);
   }
   return TRUE;
}

BOOL AFXAPI CPlanetObjects::ProcessElement2( void* pvObj, void* pData )
{
   ObjectData* pObjData = (ObjectData*)pData;
   CStaticObj* pObj  = (CStaticObj*)pvObj;


   pObj->RenderObj(pObjData->pd3dDevice,NULL);

   if(!pObjData->m_bDisplayShadows) // only use the lights if the shadows are off
      pObjData->pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

   return TRUE;
}

//-----------------------------------------------------------------------------
// Name: DrawTrees()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CPlanetObjects::Render(LPDIRECT3DDEVICE9 pd3dDevice, CFOVClipper* pClipper, DWORD& nTri,CD3DCamera* pCamera, BOOL bDisplayShadows)
{
   // Set diffuse blending for alpha set in vertices.

   // Loop through and render all trees

   // Set the tree texture

   ObjectData data;
   data.pd3dDevice = pd3dDevice;
   data.m_EyePoint = pCamera->GetEyePt();
   data.m_bDisplayShadows = bDisplayShadows;
   m_DisplayList.Clear();
   data.m_pDisplayList = &m_DisplayList;

   m_pRoot->ProcessElements(pClipper,&data, &CPlanetObjects::ProcessElement);
   m_DisplayList.ProcessElements(&data, &CPlanetObjects::ProcessElement2);

   pd3dDevice->SetTexture( 0, NULL );
   // Restore state
   D3DXMATRIX  matWorld;
   D3DXMatrixIdentity( &matWorld );
   pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );


   return S_OK;
}

HRESULT CPlanetObjects::Create(LPDIRECT3DDEVICE9 pd3dDevice)
{
   for(int i=0;i<m_pObjectList.size();i++)
   {
      m_pObjectList[i]->RestoreDeviceObjects(pd3dDevice);
   }
   return S_OK;
}

HRESULT CPlanetObjects::DeleteDeviceObjects()
{
   for(int i=0;i<m_pObjectList.size();i++)
   {
      m_pObjectList[i]->DeleteDeviceObjects();
   }
   return S_OK;
}

void CPlanetObjects::BuildShadow(const D3DXVECTOR3& lightpos)
{
   if(!m_bShadowBuilt)
   {
      for(int i=0;i<m_pObjectList.size();i++)
      {
         m_pObjectList[i]->BuildShadow(lightpos);
      }
      m_bShadowBuilt = TRUE;
   }
}

BOOL AFXAPI CPlanetObjects::ProcessShadows( void* pvObj, void* pData )
{
   LPDIRECT3DDEVICE9 pd3dDevice = (LPDIRECT3DDEVICE9)pData;
   CStaticObj* pObj  = (CStaticObj*)pvObj;

   pObj->RenderShadow(pd3dDevice,NULL);

   return TRUE;
}

void CPlanetObjects::RenderShadow(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper)
{
   m_DisplayList.ProcessElements(pd3dDevice, &CPlanetObjects::ProcessShadows);
}
