///////////////////////////////////////////////////////////////////////////////
///  QuadTree.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 4:44:24 PM by Christiaan Burchell
///
///  PURPOSE:  Quad Tree Class
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

// QuadTree.h: interface for the CQuadTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADTREE_H__754B222B_BECD_4BA9_9487_8575B2736693__INCLUDED_)
#define AFX_QUADTREE_H__754B222B_BECD_4BA9_9487_8575B2736693__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "FOVClipper.h"
#include "dxutil.h"

typedef BOOL (CALLBACK* PROCESSPROC)( void*,  void*);



template<class TYPE>
class CQuadTree  
{
public: 
   BOOL Distroy(BOOL bDelete)
   {
      if(m_bHasChildern)
      {
         for(int i=0;i<4;i++)
         {
            if(m_q[i])
               m_q[i]->Distroy(bDelete);
         }
      }

      if(bDelete)
      {
         if(m_pObj)
            delete m_pObj;
      }

      return TRUE;
   }

   CQuadTree()
   {
      m_pObj = NULL;
      m_q[0] = NULL;
      m_q[1] = NULL;
      m_q[2] = NULL;
      m_q[3] = NULL;
      m_bHasChildern = FALSE;
   }

   virtual ~CQuadTree()
   {
      SAFE_DELETE(m_q[0]);
      SAFE_DELETE(m_q[1]);
      SAFE_DELETE(m_q[2]);
      SAFE_DELETE(m_q[3]);
   }

   BOOL Create(const D3DXVECTOR2& p1, const D3DXVECTOR2& p2,FLOAT depth)
   {
      m_p[0] = p1;
      m_p[1] = p2;
      m_CenterPoint = (p2-p1)/2.0f + p1;

      m_CenterPoint3 = D3DXVECTOR3(m_CenterPoint.x,0.0f,m_CenterPoint.y);
      D3DXVECTOR2 r = m_CenterPoint - p1;
      m_Raidus = D3DXVec2Length(&r);
      m_Size = p2.x - p1.x;
      if(m_Size <= depth)
         return TRUE;

      m_bHasChildern = TRUE;

      m_q[0] = new CQuadTree<TYPE>;
      m_q[0]->Create(m_p[0],m_CenterPoint,depth);

      m_q[1] = new CQuadTree<TYPE>;
      m_q[1]->Create(m_CenterPoint,m_p[1],depth);

      m_q[2] = new CQuadTree<TYPE>;
      m_q[2]->Create(D3DXVECTOR2(m_CenterPoint.x,p1.y),D3DXVECTOR2(p2.x,m_CenterPoint.y),depth);

      m_q[3] = new CQuadTree<TYPE>;
      m_q[3]->Create(D3DXVECTOR2(p1.x,m_CenterPoint.y),D3DXVECTOR2(m_CenterPoint.x,p2.y),depth);

      return TRUE;
   }

   BOOL Add(TYPE* pObj)
   {
      BOOL added = FALSE;
      D3DXVECTOR3 CenterPoint = pObj->m_CenterPoint;
      if(CenterPoint.x >= m_p[0].x && CenterPoint.x < m_p[1].x && CenterPoint.z >= m_p[0].y && CenterPoint.z < m_p[1].y)
      {
         if(m_bHasChildern)
         {
            for(int i=0;i<4;i++)
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
            if(!m_pObj)
            {
               m_pObj = pObj;
               added = TRUE;
            }
         }

      }
      return added;
   }

   BOOL Optimize()
   {
      if(m_bHasChildern)
      {
         BOOL bFound = FALSE;
         for(int i=0;i<4;i++)
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

      return m_pObj?TRUE:FALSE;
   }

   BOOL ProcessElements(CFOVClipper* pClipper,void* pData,PROCESSPROC pFunct)
   {
      if(m_bHasChildern)
      {
         if(pClipper == NULL || pClipper->MeshFOVCheck(&m_CenterPoint3,m_Raidus,TRUE,FALSE))
         {
            if(m_q[0])
               (m_q[0])->ProcessElements(pClipper,pData,pFunct);

            if(m_q[1])
               (m_q[1])->ProcessElements(pClipper,pData,pFunct);

            if(m_q[2])
               (m_q[2])->ProcessElements(pClipper,pData,pFunct);

            if(m_q[3])
               (m_q[3])->ProcessElements(pClipper,pData,pFunct);
         }
      }
      else
      {
         if(m_pObj)
         {
            if(pClipper == NULL || pClipper->MeshFOVCheck(&m_pObj->m_CenterPoint,m_pObj->m_Raidus))
            {
               (*pFunct)(m_pObj,pData);
            }
         }
      }
      return TRUE;
   }
protected:

   FLOAT m_Size;
   D3DXVECTOR2 m_p[2];
   TYPE* m_pObj;
   D3DXVECTOR2 m_CenterPoint;
   D3DXVECTOR3 m_CenterPoint3;
   FLOAT m_Raidus;
   CQuadTree* m_q[4];
   BOOL m_bHasChildern;
};



#endif // !defined(AFX_QUADTREE_H__754B222B_BECD_4BA9_9487_8575B2736693__INCLUDED_)
