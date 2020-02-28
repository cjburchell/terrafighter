///////////////////////////////////////////////////////////////////////////////
///  LevelData.cpp
///
///  PATH: D:\dev2\CommonDev
///
///  CREATED: 02/06/2004 4:59:43 PM by Christiaan Burchell
///
///  PURPOSE:  Discription of the level Data file objects
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

// LevelData.cpp: implementation of the CLevelData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LevelData.h"
#include "vertextypes.h"

#define MAP_SCALE 2.0f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: SunPos
///
/// DESCRIPTION: Calculates the Sun Postion
///
/// CREATED:     02/06/2004 4:57:48 PM
///
/// PARAMETERS: 
///             D3DXVECTOR3* orgin
///             D3DXVECTOR3* SunPos
///             FLOAT distance
///
/// RETURN: inline void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
inline void SunPos(D3DXVECTOR3* orgin,D3DXVECTOR3* SunPos, FLOAT distance)
{
   FLOAT ZRot = HALFPI - HALFPI/6.0f;//(Sec/SECINDAY * PI2) + PI;

   D3DXMATRIX matRotZ, matSun;
   D3DXMatrixTranslation( &matSun,0, distance, 0 );
   D3DXMatrixRotationZ( &matRotZ, ZRot );
   D3DXMatrixMultiply( &matSun, &matSun, &matRotZ );
   D3DXVECTOR3 result;
   D3DXVec3TransformCoord( SunPos,orgin,&matSun);
}



/////////////////////////////////////////////////
///
/// NAME: CLevelData
///
/// CLASS: CLevelData
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 4:57:51 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CLevelData::CLevelData()
{
   m_ObjType = DATA_OBJ_TYPE_LEVEL;

   m_MapFile  = _T("");
   m_SkyBoxFile  = _T("");
   m_bShowSkyBox = FALSE;
   m_bWater = FALSE;
   m_BGColour = 0xff0000ff;
   m_WaterLevel = 4.0f;

   m_fFogStart = 10.0f;
   m_fFogEnd = 128.0f;

   m_StartPos = D3DXVECTOR3(0.0f,0.0f,0.0f);

   m_MapHight = 20.0f;
   m_GroundTexture  = _T("");
   m_WaterTexture = _T("");
   m_WaterTexture2  = _T("");

   m_Name  = _T("");
   m_Discription  = _T("");
   m_hItem = NULL;

   m_MapScale = MAP_SCALE;

   m_SunPos = D3DXVECTOR3(0.0f,-2000.0f,0.0f);
   D3DXVECTOR3 orgin = D3DXVECTOR3(0,0,0);
   SunPos(&orgin,&m_SunPos,2000.0f);

   if(m_SunPos.y < 0)
      m_SunPos.y = -m_SunPos.y;


}



/////////////////////////////////////////////////
///
/// NAME: ~CLevelData
///
/// CLASS: CLevelData
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 4:57:54 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CLevelData::~CLevelData()
{
   for(UINT i=0;i<m_Events.size();i++)
   {
      delete m_Events[i];
   }

   for(i=0;i<m_Textures.size();i++)
   {
      delete m_Textures[i];
   }

   for(i=0;i<m_Sprites.size();i++)
   {
      delete m_Sprites[i];
   }

   for(i=0;i<m_Objects.size();i++)
   {
      delete m_Objects[i];
   }
}



/////////////////////////////////////////////////
///
/// NAME: Reset
///
/// CLASS: CLevelData
///
/// DESCRIPTION: Resets the level
///
/// CREATED:     02/06/2004 4:58:01 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelData::Reset()
{
   for(UINT i=0;i<m_Events.size();i++)
   {
      m_Events[i]->Reset();
   }
}



/////////////////////////////////////////////////
///
/// NAME: Save
///
/// CLASS: CLevelData
///
/// DESCRIPTION: Saves the Level Data
///
/// CREATED:     02/06/2004 4:58:09 PM
///
/// PARAMETERS: 
///             MSXML2::IXMLDOMDocumentPtr& DocPtr
///             MSXML2::IXMLDOMElementPtr& pElement
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelData::Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement)
{
   XMLHelper::SetData(DocPtr,pElement,"Name",m_Name);
   XMLHelper::SetData(DocPtr,pElement,"Discription",m_Discription);

   XMLHelper::SetData(DocPtr,pElement,"MapFile",m_MapFile);
   XMLHelper::SetData(DocPtr,pElement,"ShowSkyBox",m_bShowSkyBox);

   XMLHelper::SetData(DocPtr,pElement,"SkyBoxTex",m_SkyBoxFile);
   XMLHelper::SetData(DocPtr,pElement,"ShowWater",m_bWater);

   XMLHelper::SetData(DocPtr,pElement,"GroundTexture",m_GroundTexture);
   XMLHelper::SetData(DocPtr,pElement,"WaterTexture",m_WaterTexture);
   XMLHelper::SetData(DocPtr,pElement,"WaterTexture2",m_WaterTexture2);

   XMLHelper::SetData(DocPtr,pElement,"BGColour",m_BGColour);
   XMLHelper::SetData(DocPtr,pElement,"WaterLevel",m_WaterLevel);
   XMLHelper::SetData(DocPtr,pElement,"MapHight",m_MapHight);

   XMLHelper::SetData(DocPtr,pElement,"FogStart",m_fFogStart);
   XMLHelper::SetData(DocPtr,pElement,"FogEnd",m_fFogEnd);

   //XMLHelper::SetData(DocPtr,pElement,"WC.Timer",m_Win.m_Timer);
   //XMLHelper::SetData(DocPtr,pElement,"WC.PObjectives",m_Win.m_PObjectives);
   //XMLHelper::SetData(DocPtr,pElement,"WC.SObjectives",m_Win.m_SObjectives);

   //XMLHelper::SetData(DocPtr,pElement,"LC.Timer",m_Lose.m_Timer);
   //XMLHelper::SetData(DocPtr,pElement,"LC.PObjectives",m_Lose.m_PObjectives);
   //XMLHelper::SetData(DocPtr,pElement,"LC.SObjectives",m_Lose.m_SObjectives);

   XMLHelper::SetData(DocPtr,pElement,"xStartPos",m_StartPos.x);
   XMLHelper::SetData(DocPtr,pElement,"yStartPos",m_StartPos.y);
   XMLHelper::SetData(DocPtr,pElement,"zStartPos",m_StartPos.z);

   XMLHelper::SetData(DocPtr,pElement,"xSunPos",m_SunPos.x);
   XMLHelper::SetData(DocPtr,pElement,"ySunPos",m_SunPos.y);
   XMLHelper::SetData(DocPtr,pElement,"zSunPos",m_SunPos.z);

   XMLHelper::SetData(DocPtr,pElement,"MapScale",m_MapScale);

   for(UINT i=0;i<m_Events.size();i++)
   {
      MSXML2::IXMLDOMElementPtr pEventElement;

      pEventElement = DocPtr->createElement("Event");
      pElement->appendChild(pEventElement);
      m_Events[i]->Save(DocPtr,pEventElement);
   }

   for(i=0;i<m_Textures.size();i++)
   {
      MSXML2::IXMLDOMElementPtr pTextureElement;

      pTextureElement = DocPtr->createElement("Texture");
      pElement->appendChild(pTextureElement);
      m_Textures[i]->Save(DocPtr,pTextureElement);
   }

   for(i=0;i<m_Sprites.size();i++)
   {
      MSXML2::IXMLDOMElementPtr pSpriteElement;

      pSpriteElement = DocPtr->createElement("Sprite");
      pElement->appendChild(pSpriteElement);
      m_Sprites[i]->Save(DocPtr,pSpriteElement);
   }

   for(i=0;i<m_Objects.size();i++)
   {
      MSXML2::IXMLDOMElementPtr pObjectElement;
      pObjectElement = DocPtr->createElement("Object");
      pElement->appendChild(pObjectElement);
      m_Objects[i]->Save(DocPtr,pObjectElement);
   }


}



/////////////////////////////////////////////////
///
/// NAME: Load
///
/// CLASS: CLevelData
///
/// DESCRIPTION: Loads the Level data
///
/// CREATED:     02/06/2004 4:58:21 PM
///
/// PARAMETERS: 
///             MSXML2::IXMLDOMNodePtr& Item
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CLevelData::Load(MSXML2::IXMLDOMNodePtr& Item)
{
   XMLHelper::GetData(Item,"Name",m_Name);
   XMLHelper::GetData(Item,"Discription",m_Discription);


   XMLHelper::GetData(Item,"MapFile",m_MapFile);
   XMLHelper::GetData(Item,"ShowSkyBox",m_bShowSkyBox);

   XMLHelper::GetData(Item,"SkyBoxTex",m_SkyBoxFile);

   XMLHelper::GetData(Item,"ShowWater",m_bWater);

   XMLHelper::GetData(Item,"GroundTexture",m_GroundTexture);
   XMLHelper::GetData(Item,"WaterTexture",m_WaterTexture);
   XMLHelper::GetData(Item,"WaterTexture2",m_WaterTexture2);

   XMLHelper::GetData(Item,"BGColour",m_BGColour,0xff0000ff);
   XMLHelper::GetData(Item,"WaterLevel",m_WaterLevel);
   XMLHelper::GetData(Item,"MapHight",m_MapHight,20.0f);

   XMLHelper::GetData(Item,"FogStart",m_fFogStart,10.0f);
   XMLHelper::GetData(Item,"FogEnd",m_fFogEnd,128.0f);

   //XMLHelper::GetData(Item,"WC.Timer",m_Win.m_Timer,0);
   //XMLHelper::GetData(Item,"WC.PObjectives",m_Win.m_PObjectives,-1);
   //XMLHelper::GetData(Item,"WC.SObjectives",m_Win.m_SObjectives,-1);

   //XMLHelper::GetData(Item,"LC.Timer",m_Lose.m_Timer,0);
   //XMLHelper::GetData(Item,"LC.PObjectives",m_Lose.m_PObjectives,-1);
   //XMLHelper::GetData(Item,"LC.SObjectives",m_Lose.m_SObjectives,-1);

   XMLHelper::GetData(Item,"xStartPos",m_StartPos.x);
   XMLHelper::GetData(Item,"yStartPos",m_StartPos.y);
   XMLHelper::GetData(Item,"zStartPos",m_StartPos.z);

   XMLHelper::GetData(Item,"xSunPos",m_SunPos.x,m_SunPos.x);
   XMLHelper::GetData(Item,"ySunPos",m_SunPos.y,m_SunPos.y);
   XMLHelper::GetData(Item,"zSunPos",m_SunPos.z,m_SunPos.z);

   XMLHelper::GetData(Item,"MapScale",m_MapScale,m_MapScale);

   {

      MSXML2::IXMLDOMNodeListPtr SpriteListPtr = Item->selectNodes("Sprite");
      int ElementCount = SpriteListPtr->length;
      if(ElementCount > 0) // if there are any rows
      {
         for(int j=0;j<ElementCount;j++)
         {
            MSXML2::IXMLDOMNodePtr SpriteItem = SpriteListPtr->item[j];
            CLevelDataSprite* SpriteData = new CLevelDataSprite;
            SpriteData->Load(SpriteItem);
            SpriteData->SetIndex(m_Sprites.size());
            SpriteData->SetSpriteObjType(0);
            m_Sprites.push_back(SpriteData);
         }
      }
   }


   {
      MSXML2::IXMLDOMNodeListPtr ObjectListPtr = Item->selectNodes("Object");
      int ElementCount = ObjectListPtr->length;
      if(ElementCount > 0) // if there are any rows
      {
         for(int j=0;j<ElementCount;j++)
         {
            MSXML2::IXMLDOMNodePtr ObjectItem = ObjectListPtr->item[j];
            CLevelDataObject* ObjectData = new CLevelDataObject;
            ObjectData->Load(ObjectItem);
            ObjectData->SetIndex(m_Objects.size());
            m_Objects.push_back(ObjectData);

         }
      }
   }

   MSXML2::IXMLDOMNodeListPtr EventListPtr = Item->selectNodes("Event");
   int ElementCount = EventListPtr->length;
   if(ElementCount > 0) // if there are any rows
   {
      for(int j=0;j<ElementCount;j++)
      {
         MSXML2::IXMLDOMNodePtr ObjectItem = EventListPtr->item[j];
         CGameEvent* GameEvent = new CGameEvent;
         GameEvent->Load(ObjectItem);
         GameEvent->SetIndex(m_Events.size());
         m_Events.push_back(GameEvent);

      }
   }

   MSXML2::IXMLDOMNodeListPtr TextureListPtr = Item->selectNodes("Texture");
   ElementCount = TextureListPtr->length;
   if(ElementCount > 0) // if there are any rows
   {
      for(int j=0;j<ElementCount;j++)
      {
         MSXML2::IXMLDOMNodePtr ObjectItem = TextureListPtr->item[j];
         CTextureData* TextureItem = new CTextureData;
         TextureItem->Load(ObjectItem);
         TextureItem->SetIndex(m_Textures.size());
         m_Textures.push_back(TextureItem);

      }
   }
}




/////////////////////////////////////////////////
///
/// NAME: Check
///
/// CLASS: CGameEvent
///
/// DESCRIPTION: Check to see if the event has occured
///
/// CREATED:     02/06/2004 4:58:36 PM
///
/// PARAMETERS: 
///             FLOAT LevelTimer
///             std::vector<CGameAction>& Actions
///             BOOL bFoundEnemy
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CGameEvent::Check(FLOAT LevelTimer,std::vector<CGameAction>& Actions,BOOL bFoundEnemy)
{
   BOOL retval = FALSE;
   if(!bOccured && m_bCurrenlyActive)
   {

      switch(m_EventType)
      {
      case EVENT_SPRITE_DISTROYED:
      case EVENT_HIT_WAYPOINT:
         {
            for(UINT i=0;i<Actions.size();i++)
            {
               EventType_e Type = Actions[i].m_EventType;
               CString ObjName = Actions[i].m_ObjectName;
               CString ResponsName = Actions[i].m_ResponsName;
               if( Type == m_EventType &&  ObjName == m_ObjectName && (m_ResponsName == _T("") || ResponsName == m_ResponsName))
               {
                  retval = TRUE;
                  break;
               }
            }
         }
         break;
      case EVENT_TIMEOUT:
         if(LevelTimer > m_Timer+m_StartTime)
            retval = TRUE;
         break;
      case EVENT_ALL_ENEMYS_DISTOYED:
         if(!bFoundEnemy)
            retval = TRUE;
         break;
      }
      if(retval)
         bOccured = TRUE;
   }
   return retval;
}



/////////////////////////////////////////////////
///
/// NAME: ActivateEvent
///
/// CLASS: CGameEvent
///
/// DESCRIPTION: Activates the event
///
/// CREATED:     02/06/2004 4:58:47 PM
///
/// PARAMETERS: 
///             FLOAT LevelTimer
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CGameEvent::ActivateEvent(FLOAT LevelTimer)
{
   m_bCurrenlyActive = TRUE;
   bOccured = FALSE;
   m_StartTime = LevelTimer;
}




/////////////////////////////////////////////////
///
/// NAME: CGameEvent
///
/// CLASS: CGameEvent
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 4:58:50 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CGameEvent::CGameEvent() : CDataObjBase()
{
   bOccured = FALSE;
   m_ObjType = DATA_OBJ_TYPE_EVENT;
   m_Message = _T("");


   m_EventType = EVENT_SPRITE_DISTROYED;
   m_ObjectName = _T("");
   m_ResponsName = _T("");
   m_Timer = 0;

   m_GameState = GAME_NOCHANGE;
   m_EventToActivate = _T("");

   m_Name = _T("");
   m_bActive = TRUE;
   m_bCurrenlyActive = m_bActive;
   m_StartTime = 0;

   m_bDisplay = FALSE;
}



/////////////////////////////////////////////////
///
/// NAME: ~CGameEvent
///
/// CLASS: CGameEvent
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 4:58:52 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CGameEvent::~CGameEvent()
{
   for(UINT i=0;i<m_CreateSprites.size();i++)
   {
      delete m_CreateSprites[i];
   }

   for(i=0;i<m_CreateObjects.size();i++)
   {
      delete m_CreateObjects[i];
   }

   for(i=0;i<m_SpriteChange.size();i++)
   {
      delete m_SpriteChange[i];
   }
}



/////////////////////////////////////////////////
///
/// NAME: Reset
///
/// CLASS: CGameEvent
///
/// DESCRIPTION: Resets the event
///
/// CREATED:     02/06/2004 4:58:59 PM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CGameEvent::Reset()
{
   bOccured = FALSE;
   m_bCurrenlyActive = m_bActive;
   m_StartTime = 0;
}



/////////////////////////////////////////////////
///
/// NAME: Load
///
/// CLASS: CGameEvent
///
/// DESCRIPTION: Loads the event
///
/// CREATED:     02/06/2004 4:59:06 PM
///
/// PARAMETERS: 
///             MSXML2::IXMLDOMNodePtr& Item
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CGameEvent::Load(MSXML2::IXMLDOMNodePtr& Item)
{
   //CDataObjBase::Load(Item);
   XMLHelper::GetData(Item,"Name",m_Name);
   XMLHelper::GetData(Item,"Message",m_Message);


   DWORD val;
   XMLHelper::GetData(Item,"EventType",val);
   m_EventType = (EventType_e)val;

   XMLHelper::GetData(Item,"ObjectName",m_ObjectName);
   XMLHelper::GetData(Item,"ResponsName",m_ResponsName);
   XMLHelper::GetData(Item,"Timer",m_Timer);
   XMLHelper::GetData(Item,"GameState",val);
   m_GameState = (GameState_e)val;
   XMLHelper::GetData(Item,"EventToActivate",m_EventToActivate);
   XMLHelper::GetData(Item,"Active",m_bActive);
   m_bCurrenlyActive = m_bActive;


   {

      MSXML2::IXMLDOMNodeListPtr SpriteListPtr = Item->selectNodes("Sprite");
      int ElementCount = SpriteListPtr->length;
      if(ElementCount > 0) // if there are any rows
      {
         for(int j=0;j<ElementCount;j++)
         {
            MSXML2::IXMLDOMNodePtr SpriteItem = SpriteListPtr->item[j];
            CLevelDataSprite* SpriteData = new CLevelDataSprite;
            SpriteData->Load(SpriteItem);
            SpriteData->SetIndex(m_CreateSprites.size());
            SpriteData->SetSpriteObjType(2);
            m_CreateSprites.push_back(SpriteData);
         }
      }
   }


   {
      MSXML2::IXMLDOMNodeListPtr ObjectListPtr = Item->selectNodes("Object");
      int ElementCount = ObjectListPtr->length;
      if(ElementCount > 0) // if there are any rows
      {
         for(int j=0;j<ElementCount;j++)
         {
            MSXML2::IXMLDOMNodePtr ObjectItem = ObjectListPtr->item[j];
            CLevelDataObject* ObjectData = new CLevelDataObject;
            ObjectData->Load(ObjectItem);
            ObjectData->SetIndex(m_CreateObjects.size());
            m_CreateObjects.push_back(ObjectData);

         }
      }
   }

   {

      MSXML2::IXMLDOMNodeListPtr SpriteListPtr = Item->selectNodes("ChangeSprite");
      int ElementCount = SpriteListPtr->length;
      if(ElementCount > 0) // if there are any rows
      {
         for(int j=0;j<ElementCount;j++)
         {
            MSXML2::IXMLDOMNodePtr SpriteItem = SpriteListPtr->item[j];
            CLevelDataSprite* SpriteData = new CLevelDataSprite;
            SpriteData->Load(SpriteItem);
            SpriteData->SetIndex(m_SpriteChange.size());
            SpriteData->SetSpriteObjType(1);
            m_SpriteChange.push_back(SpriteData);
         }
      }
   }
}



/////////////////////////////////////////////////
///
/// NAME: Save
///
/// CLASS: CGameEvent
///
/// DESCRIPTION: Saves the Event
///
/// CREATED:     02/06/2004 4:59:15 PM
///
/// PARAMETERS: 
///             MSXML2::IXMLDOMDocumentPtr& DocPtr
///             MSXML2::IXMLDOMElementPtr& pElement
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CGameEvent::Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement)
{
   //CDataObjBase::Save(DocPtr,pElement);
   XMLHelper::SetData(DocPtr,pElement,"Name",m_Name);						
   XMLHelper::SetData(DocPtr,pElement,"Message",m_Message);

   XMLHelper::SetData(DocPtr,pElement,"EventType",m_EventType);
   XMLHelper::SetData(DocPtr,pElement,"ObjectName",m_ObjectName);
   XMLHelper::SetData(DocPtr,pElement,"ResponsName",m_ResponsName);
   XMLHelper::SetData(DocPtr,pElement,"Timer",m_Timer);
   XMLHelper::SetData(DocPtr,pElement,"GameState",m_GameState);
   XMLHelper::SetData(DocPtr,pElement,"EventToActivate",m_EventToActivate);
   XMLHelper::SetData(DocPtr,pElement,"Active",m_bActive);

   for(UINT i=0;i<m_CreateSprites.size();i++)
   {
      MSXML2::IXMLDOMElementPtr pSpriteElement;

      pSpriteElement = DocPtr->createElement("Sprite");
      pElement->appendChild(pSpriteElement);
      m_CreateSprites[i]->Save(DocPtr,pSpriteElement);
   }

   for(i=0;i<m_CreateObjects.size();i++)
   {
      MSXML2::IXMLDOMElementPtr pObjectElement;
      pObjectElement = DocPtr->createElement("Object");
      pElement->appendChild(pObjectElement);
      m_CreateObjects[i]->Save(DocPtr,pObjectElement);
   }

   for(i=0;i<m_SpriteChange.size();i++)
   {
      MSXML2::IXMLDOMElementPtr pObjectElement;
      pObjectElement = DocPtr->createElement("ChangeSprite");
      pElement->appendChild(pObjectElement);
      m_SpriteChange[i]->Save(DocPtr,pObjectElement);
   }
}
