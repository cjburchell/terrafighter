// DisplayObject.h: interface for the CDisplayObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYOBJECT_H__3D117DB7_F0F1_46B9_9B5C_E20EDA3CE4D1__INCLUDED_)
#define AFX_DISPLAYOBJECT_H__3D117DB7_F0F1_46B9_9B5C_E20EDA3CE4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3dfile.h"
#include "FOVClipper.h"
#include "LevelData.h"
#include "ChatDlg.h"

//-----------------------------------------------------------------------------
// Name: struct ShadowVolume
// Desc: A shadow volume object
//-----------------------------------------------------------------------------
class CShadowVolumeData
{
public:
   CShadowVolumeData()
   {
      m_pVertices = NULL;
      m_dwNumVertices = 0;
   }

   ~CShadowVolumeData()
   {
      if(m_pVertices)
         delete [] m_pVertices;
   }


   D3DXVECTOR3* m_pVertices; // Vertex data for rendering shadow volume
   DWORD       m_dwNumVertices;
   D3DXMATRIX m_MeshMat;
};

class CShadowVolume
{
   std::vector<CShadowVolumeData*> m_Data;


   // used for bulding the mesh
   D3DXVECTOR3 m_TempLightPos;
   BOOL m_bTempFullBuild;
   int m_CurrIndex;

public:
   CShadowVolume()
   {
   }

   ~CShadowVolume()
   {
      for(UINT i=0;i<m_Data.size();i++)
      {


         delete m_Data[i];
      }
   }


   void Reset() 
   {	
      for(UINT i=0;i<m_Data.size();i++)
      {
         m_Data[i]->m_dwNumVertices = 0;
      } 
   }

   static bool BuildMeshCB(Xfile::Mesh* pMeshObj,const D3DXMATRIX& MeshMat,VOID* pData);

   HRESULT BuildFromMesh( Xfile::Mesh* pMesh, const D3DXVECTOR3& LightPos, const D3DXMATRIX& MeshMat, BOOL bFullBuild, int shadowindex = 0);
   HRESULT BuildFromMeshFile( Xfile::File* pMeshObj, const D3DXVECTOR3& LightPos,const D3DXMATRIX& MeshMat, BOOL bFullBuild );
   HRESULT Render( LPDIRECT3DDEVICE9 pd3dDevice );
};




class CDisplayObject  
{
public:
   CDisplayObject();
   virtual ~CDisplayObject();
public:
   virtual void BuildShadow(const D3DXVECTOR3& lightpos) = 0;
   virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper) = 0;
   virtual HRESULT RenderSimple(LPDIRECT3DDEVICE9 pd3dDevice) = 0;

   virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice) = 0;

   virtual void DeleteDeviceObjects() = 0;


   const D3DXMATRIX& GetMatrix()
   {
      return m_Matrix;
   }

   void SetMatrix(const D3DXMATRIX* pMat)
   {
      m_Matrix = *pMat;
   }



   const D3DXVECTOR3& GetPos()
   {
      return m_Pos;
   }

   FLOAT GetScale()
   {
      return m_Scale;
   }

   virtual FLOAT GetRadius()
   {
      return m_Radius;
   }

   void SetPos(const D3DXVECTOR3* p)
   {
      m_Pos = *p;
   }

   virtual FLOAT GetSpeed()
   {
      return 0.0f;
   }

   virtual BOOL IsInvisable(){return FALSE;}

   virtual DWORD GetTeamColour(){return D3DCOLOR_XRGB(255,255,255);}

   FLOAT CheckCollision(const D3DXVECTOR3& point, FLOAT Rad);
   virtual void DoCollision(CDisplayObject* pObj,std::vector<CGameAction>* pActions,CChatDlg* pChatDlg) = 0;

   virtual HRESULT Create(const char* object, FLOAT Scale,D3DXVECTOR3 pos);
   virtual HRESULT RenderShadow(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);



   static void EndRenderShadow(LPDIRECT3DDEVICE9 pd3dDevice);
   static void StartRenderShadow(LPDIRECT3DDEVICE9 pd3dDevice);



   const D3DXVECTOR3& GetCenterPoint(){return m_Center;}

   BOOL IsSprite(){return m_bSprite;}

   CString GetName(){return m_Name;}
   void SetName(const char* name){m_Name = name;}

protected:

   CString m_Name;

   D3DXMATRIX m_Rotate;
   D3DXVECTOR3 m_Center;
   CShadowVolume* m_pShadowVolume;
   FLOAT m_Radius;
   D3DXMATRIX m_Matrix;
   D3DXVECTOR3 m_Pos;
   FLOAT m_Scale;
   BOOL m_bSprite;
};

#endif // !defined(AFX_DISPLAYOBJECT_H__3D117DB7_F0F1_46B9_9B5C_E20EDA3CE4D1__INCLUDED_)
