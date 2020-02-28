// Tree.h: interface for the CTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREE_H__D152A969_9144_4B29_96F1_D2E891619C1C__INCLUDED_)
#define AFX_TREE_H__D152A969_9144_4B29_96F1_D2E891619C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vegetation.h"

class CTree : public CVegetation  
{
public:
   CTree(FLOAT fWidth, FLOAT fHeight, const char* TextureFile);
   virtual ~CTree();

};

#endif // !defined(AFX_TREE_H__D152A969_9144_4B29_96F1_D2E891619C1C__INCLUDED_)
