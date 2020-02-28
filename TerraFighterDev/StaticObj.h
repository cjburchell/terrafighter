// StaticObj.h: interface for the CStaticObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICOBJ_H__76765F90_6B31_4EB1_983A_C96E377A6C28__INCLUDED_)
#define AFX_STATICOBJ_H__76765F90_6B31_4EB1_983A_C96E377A6C28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DisplayObject.h"

#define MAX_SUB_ITEMS 10

struct CSubObj
{
   D3DXVECTOR3 m_Offset;
   FLOAT m_Rotation;
   DWORD m_AniOffset;
};


#define NUM_RAND_STATIC_OBJ 5000

struct CObjPos
{
   D3DXVECTOR3 m_CenterPoint;  // Origin of tree
   FLOAT		m_Raidus;
   WORD		m_Type;
   DWORD		m_NumbSubItems;
   CSubObj m_SubItems[MAX_SUB_ITEMS];
   FLOAT		m_Distance;
};


class CStaticObj : public CDisplayObject  
{
public:
   CStaticObj(const char* object);
   virtual ~CStaticObj();
   virtual HRESULT RenderObj(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);
   virtual void SetObjData(CObjPos* pObjData);
   void SetType(PlanetObjectType_e Type){m_Type = Type;}
   PlanetObjectType_e GetType(){return m_Type;}

   BOOL IsVisable(){return m_bVisable;}
   void SetVisable(BOOL bVis = TRUE){m_bVisable = bVis;}


public:
   D3DXVECTOR3 m_CenterPoint; // the center point of the object
   FLOAT m_Distance; // distance from the eye to the object
   PlanetObjectType_e m_Type;
   BOOL m_bVisable;
};

#endif // !defined(AFX_STATICOBJ_H__76765F90_6B31_4EB1_983A_C96E377A6C28__INCLUDED_)
