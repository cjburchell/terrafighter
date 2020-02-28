///////////////////////////////////////////////////////////////////////////////
///  Octtree.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 10:12:43 AM by Christiaan Burchell
///
///  PURPOSE:  interface for the COctTree class.
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

// QuadTree.h: interface for the COctTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OCTTREE_H__754B222B_BECD_4BA9_9487_8575B2736693__INCLUDED_)
#define AFX_OCTTREE_H__754B222B_BECD_4BA9_9487_8575B2736693__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FOVClipper.h"
#include "dxutil.h"
#include <list>

typedef BOOL (CALLBACK* PROCESSPROC)( void*,  void*);

#define NOCT_NODES 8

template<class TYPE>
class COctTree  
{
public:


   /////////////////////////////////////////////////
   ///
   /// NAME: Distroy
   ///
   /// DESCRIPTION: Distroys the OctTree
   ///
   /// CREATED:     02/06/2004 10:13:07 AM
   ///
   /// PARAMETERS: 
   ///             BOOL bDelete
   ///
   /// RETURN: BOOL 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   BOOL Distroy(BOOL bDelete)
   {
      for(int i=0;i<NOCT_NODES;i++)
      {
         if(m_q[i])
            m_q[i]->Distroy(bDelete);
      }

      if(bDelete && m_Obj)
      {
         for(DWORD i=0;i<m_MaxObjPerNode;i++)
         {
            SAFE_DELETE(m_Obj[i]);
         }
      }
      return TRUE;
   }



   /////////////////////////////////////////////////
   ///
   /// NAME: COctTree
   ///
   /// DESCRIPTION: Class Constructor
   ///
   /// CREATED:     02/06/2004 10:13:19 AM
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   COctTree()
   {
      m_bHasChildern = FALSE;
      m_q[0] = NULL;
      m_q[1] = NULL;
      m_q[2] = NULL;
      m_q[3] = NULL;
      m_q[4] = NULL;
      m_q[5] = NULL;
      m_q[6] = NULL;
      m_q[7] = NULL;

      m_nObj = 0;
   }



   /////////////////////////////////////////////////
   ///
   /// NAME: ~COctTree
   ///
   /// DESCRIPTION: Class Destructor
   ///
   /// CREATED:     02/06/2004 10:13:23 AM
   ///
   /// RETURN: virtual 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   virtual ~COctTree()
   {
      SAFE_DELETE(m_q[0]);
      SAFE_DELETE(m_q[1]);
      SAFE_DELETE(m_q[2]);
      SAFE_DELETE(m_q[3]);
      SAFE_DELETE(m_q[4]);
      SAFE_DELETE(m_q[5]);
      SAFE_DELETE(m_q[6]);
      SAFE_DELETE(m_q[7]);

      if(m_Obj)
         delete [] m_Obj;
   }



   /////////////////////////////////////////////////
   ///
   /// NAME: Create
   ///
   /// DESCRIPTION: Creates the OctTree
   ///
   /// CREATED:     02/06/2004 10:13:40 AM
   ///
   /// PARAMETERS: 
   ///             D3DXVECTOR3 p1
   ///             D3DXVECTOR3 p2
   ///             FLOAT minsize
   ///             DWORD MaxObjPerNode
   ///
   /// RETURN: BOOL 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   BOOL Create(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2 , FLOAT minsize, DWORD MaxObjPerNode)
   {
      m_p1 = p1;
      m_p2 = p2;
      m_CenterPoint = (p2-p1)/2.0f + p1;

      D3DXVECTOR3 r = m_CenterPoint - p1;
      m_Radius = D3DXVec3Length(&r);
      m_Size = p2.x - p1.x;

      m_MaxObjPerNode = MaxObjPerNode;
      m_MinSize = minsize;

      m_Obj = new TYPE*[m_MaxObjPerNode];

      for(DWORD i=0;i<m_MaxObjPerNode;i++)
      {
         m_Obj[i] = NULL;
      }

      return TRUE;
   }



   /////////////////////////////////////////////////
   ///
   /// NAME: CheckCollision
   ///
   /// DESCRIPTION: Checks the OctTree for a colliston
   ///
   /// CREATED:     02/06/2004 10:13:58 AM
   ///
   /// PARAMETERS: 
   ///             D3DXVECTOR3* point
   ///             FLOAT Rad
   ///
   /// RETURN: FLOAT 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   FLOAT CheckCollision(const D3DXVECTOR3& point, FLOAT Rad)
   {
      D3DXVECTOR3 v;
      D3DXVec3Subtract(&v,&m_CenterPoint,&point);
      FLOAT Distance = D3DXVec3Length(&v);
      FLOAT MinDistance = m_Radius+Rad;
      return Distance - MinDistance;
   }





   /////////////////////////////////////////////////
   ///
   /// NAME: FindCollisions
   ///
   /// DESCRIPTION: Finds a collision in the OctTree
   ///
   /// CREATED:     02/06/2004 10:14:19 AM
   ///
   /// PARAMETERS: 
   ///             D3DXVECTOR3* point
   ///             FLOAT Rad
   ///             std::list<TYPE*>* pColList
   ///
   /// RETURN: BOOL 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   BOOL FindCollisions(const D3DXVECTOR3& point, FLOAT Rad, std::list<TYPE*>* pColList)
   {
      if(CheckCollision(point,Rad) < 0.0f)
      {
         if(m_bHasChildern)
         {
            if(m_q[0])
               (m_q[0])->FindCollisions(point,Rad,pColList);

            if(m_q[1])
               (m_q[1])->FindCollisions(point,Rad,pColList);

            if(m_q[2])
               (m_q[2])->FindCollisions(point,Rad,pColList);

            if(m_q[3])
               (m_q[3])->FindCollisions(point,Rad,pColList);

            if(m_q[4])
               (m_q[4])->FindCollisions(point,Rad,pColList);

            if(m_q[5])
               (m_q[5])->FindCollisions(point,Rad,pColList);

            if(m_q[6])
               (m_q[6])->FindCollisions(point,Rad,pColList);

            if(m_q[7])
               (m_q[7])->FindCollisions(point,Rad,pColList);
         }
         else
         {
            for(DWORD i =0;i<m_nObj;i++)
            {
               if(m_Obj[i]->CheckCollision(point,Rad) < 0.0f)
                  pColList->push_back(m_Obj[i]);
            }
         }
      }
      return TRUE;
   }




   /////////////////////////////////////////////////
   ///
   /// NAME: Add
   ///
   /// DESCRIPTION: Adds a node to the OctTree
   ///
   /// CREATED:     02/06/2004 10:14:36 AM
   ///
   /// PARAMETERS: 
   ///             TYPE* pObj
   ///
   /// RETURN: BOOL 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   BOOL Add(TYPE* pObj)
   {
      BOOL added = FALSE;
      D3DXVECTOR3 CenterPoint = pObj->m_CenterPoint;
      if(CenterPoint.x >= m_p1.x && CenterPoint.x < m_p2.x && CenterPoint.z >= m_p1.z && CenterPoint.z < m_p2.z  && CenterPoint.y >= m_p1.y && CenterPoint.y < m_p2.y )
      {
         if(m_bHasChildern)
         {
            for(int i=0;i<NOCT_NODES;i++)
            {
               if(m_q[i] && m_q[i]->Add(pObj))
               {
                  added = TRUE;
                  break;
               }
            }
         }
         else
         {
            added = TRUE;
            m_Obj[m_nObj] = pObj;
            m_nObj++;
            if(m_MaxObjPerNode <= m_nObj)
            {
               m_bHasChildern = TRUE;
               m_q[0] = new COctTree<TYPE>;
               m_q[0]->Create(m_p1,m_CenterPoint,m_MinSize,m_MaxObjPerNode);

               m_q[1] = new COctTree<TYPE>;
               m_q[1]->Create(D3DXVECTOR3(m_p1.x,m_CenterPoint.y,m_p1.z),D3DXVECTOR3(m_CenterPoint.x,m_p2.y,m_CenterPoint.z),m_MinSize,m_MaxObjPerNode);

               m_q[2] = new COctTree<TYPE>;
               m_q[2]->Create(D3DXVECTOR3(m_CenterPoint.x,m_p1.y,m_p1.z),D3DXVECTOR3(m_p2.x,m_CenterPoint.y,m_CenterPoint.z),m_MinSize,m_MaxObjPerNode);

               m_q[3] = new COctTree<TYPE>;
               m_q[3]->Create(D3DXVECTOR3(m_CenterPoint.x,m_CenterPoint.y,m_p1.z),D3DXVECTOR3(m_p2.x,m_p2.y,m_CenterPoint.z),m_MinSize,m_MaxObjPerNode);

               m_q[4] = new COctTree<TYPE>;
               m_q[4]->Create(D3DXVECTOR3(m_p1.x,m_p1.y,m_CenterPoint.z),D3DXVECTOR3(m_CenterPoint.x,m_CenterPoint.y,m_p2.z),m_MinSize,m_MaxObjPerNode);

               m_q[5] = new COctTree<TYPE>;
               m_q[5]->Create(D3DXVECTOR3(m_p1.x,m_CenterPoint.y,m_CenterPoint.z),D3DXVECTOR3(m_CenterPoint.x,m_p2.y,m_p2.z),m_MinSize,m_MaxObjPerNode);

               m_q[6] = new COctTree<TYPE>;
               m_q[6]->Create(D3DXVECTOR3(m_CenterPoint.x,m_p1.y,m_CenterPoint.z),D3DXVECTOR3(m_p2.x,m_CenterPoint.y,m_p2.z),m_MinSize,m_MaxObjPerNode);

               m_q[7] = new COctTree<TYPE>;
               m_q[7]->Create(m_CenterPoint,m_p2,m_MinSize,m_MaxObjPerNode);

               for(DWORD i =0;i<m_MaxObjPerNode;i++)
               {
                  Add(m_Obj[i]);
                  m_Obj[i] = NULL;
               }
               m_nObj = 0;
            }
         }
      }
      return added;
   }




   /////////////////////////////////////////////////
   ///
   /// NAME: Optimize
   ///
   /// DESCRIPTION: Optimizes the oct tree
   ///
   /// CREATED:     02/06/2004 10:14:48 AM
   ///
   /// RETURN: BOOL 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   BOOL Optimize()
   {
      if(m_bHasChildern)
      {
         BOOL bFound = FALSE;
         for(int i=0;i<NOCT_NODES;i++)
         {
            if(m_q[i])
            {
               if(m_q[i]->Optimize())
                  bFound = TRUE;
               else
               {
                  SAFE_DELETE(m_q[i]);
                  m_q[i] = NULL;
               }
            }
         }
         return bFound;
      }

      return m_nObj!=0?TRUE:FALSE;
   }





   /////////////////////////////////////////////////
   ///
   /// NAME: ProcessElements
   ///
   /// DESCRIPTION: Preformes a user given fuction to each visable element in the OctTree
   ///
   /// CREATED:     02/06/2004 10:15:42 AM
   ///
   /// PARAMETERS: 
   ///             CFOVClipper* pClipper
   ///             void* pData
   ///             PROCESSPROC pFunct
   ///
   /// RETURN: BOOL 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   BOOL ProcessElements(CFOVClipper* pClipper,void* pData,PROCESSPROC pFunct)
   {
      e_fov_location fov = FOV_Inside;
      if(pClipper == NULL || (fov = pClipper->MeshFOVCheck(m_CenterPoint,m_Radius,TRUE,TRUE)) != FOV_Ouside)
      {
         if(m_bHasChildern)
         {
            if(m_q[0])
               (m_q[0])->ProcessElements(fov==FOV_Bound?pClipper:NULL,pData,pFunct);

            if(m_q[1])
               (m_q[1])->ProcessElements(fov==FOV_Bound?pClipper:NULL,pData,pFunct);

            if(m_q[2])
               (m_q[2])->ProcessElements(fov==FOV_Bound?pClipper:NULL,pData,pFunct);

            if(m_q[3])
               (m_q[3])->ProcessElements(fov==FOV_Bound?pClipper:NULL,pData,pFunct);

            if(m_q[4])
               (m_q[4])->ProcessElements(fov==FOV_Bound?pClipper:NULL,pData,pFunct);

            if(m_q[5])
               (m_q[5])->ProcessElements(fov==FOV_Bound?pClipper:NULL,pData,pFunct);

            if(m_q[6])
               (m_q[6])->ProcessElements(fov==FOV_Bound?pClipper:NULL,pData,pFunct);

            if(m_q[7])
               (m_q[7])->ProcessElements(fov==FOV_Bound?pClipper:NULL,pData,pFunct);
         }
         else
         {
            for(DWORD i =0;i<m_nObj;i++)
            {
               (*pFunct)(m_Obj[i],pData);
            }
         }
      }
      return TRUE;
   }


protected:

   FLOAT m_Size;
   D3DXVECTOR3 m_p1;
   D3DXVECTOR3 m_p2;
   TYPE** m_Obj;
   DWORD m_nObj;
   DWORD m_MaxObjPerNode;
   FLOAT m_MinSize;

   BOOL m_bHasChildern;
   D3DXVECTOR3 m_CenterPoint;
   FLOAT m_Radius;
   COctTree* m_q[NOCT_NODES];
};




#endif // !defined(AFX_OCTTREE_H__754B222B_BECD_4BA9_9487_8575B2736693__INCLUDED_)
