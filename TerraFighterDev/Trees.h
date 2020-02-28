// Trees.h: interface for the CPlanetObjects class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREES_H__CF01634A_1D31_4598_AD45_5D75407A2A62__INCLUDED_)
#define AFX_TREES_H__CF01634A_1D31_4598_AD45_5D75407A2A62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Terrain.h"
#include "FOVClipper.h"
#include "Water.h"
#include "QuadTree.h"
#include "StaticObj.h"
#include "SortedList.h"
#include "netmessages.h"
#include <vector>
#include "LevelData.h"


typedef  std::vector<CStaticObj*> ObjectList;



class CPlanetObjects  
{
public:
	static BOOL AFXAPI ProcessElement( void* pvObj, void* pData );
	static BOOL AFXAPI ProcessElement2( void* pvObj, void* pData );
    static BOOL AFXAPI ProcessShadows( void* pvObj, void* pData );

	HRESULT DeleteDeviceObjects();
	HRESULT Create(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper,DWORD& nTri,CD3DCamera* pCamera, BOOL bDisplayShadows);
	//HRESULT Sort(CD3DCamera * pCamera);
	CPlanetObjects();
	virtual ~CPlanetObjects();
	BOOL CreateRandomObjects(CTerrain* pFloor, CWater* pWater, CPlanetObjectData* pPlanetObjectData, DWORD MaxPlanetObjectData);
	BOOL CreateObjects(CTerrain* pFloor, CWater* pWater, CPlanetObjectData* pPlanetObjectData, DWORD MaxPlanetObjectData,CLevelData* pLevel);
	BOOL AddObject(CTerrain* pFloor, CWater* pWater, CPlanetObjectData* pPlanetObjectData,CLevelDataObject* pObjData);

    
    DWORD GetTri(){return m_nTri;};

    void BuildShadow(const D3DXVECTOR3& lightpos);
    void RenderShadow(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);

	void GetCollisions(const D3DXVECTOR3& pos,FLOAT rad,std::list<CStaticObj*>* pColList)
	{
		if(m_pRoot)
			m_pRoot->FindCollisions(pos,rad,pColList);
	}

	ObjectList* GetObjectList(){return &m_pObjectList;}

private:
	CSortedList<CStaticObj*> m_DisplayList;
	DWORD m_nTri;
	COctTree<CStaticObj>* m_pRoot;

    //std::vector<CStaticObj*> m_pObjectTypes;
    ObjectList m_pObjectList;

    //D3DXMATRIX    m_matBillboardMatrix; // Used for billboard orientation
    BOOL m_bShadowBuilt;
};

#endif // !defined(AFX_TREES_H__CF01634A_1D31_4598_AD45_5D75407A2A62__INCLUDED_)
