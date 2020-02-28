///////////////////////////////////////////////////////////////////////////////
///  LevelData.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 4:45:29 PM by Christiaan Burchell
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

// LevelData.h: interface for the CLevelData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVELDATA_H__3DCB3FC9_73C8_4B25_A9D0_AD2BD22D4C7F__INCLUDED_)
#define AFX_LEVELDATA_H__3DCB3FC9_73C8_4B25_A9D0_AD2BD22D4C7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLHelpers.h"
#include <vector>
#include <d3d9.h>
#include <D3DX9Math.h>

enum DataObjectType_e
{
   DATA_OBJ_TYPE_LEVEL =0,
   DATA_OBJ_TYPE_TEXTURE =1,
   DATA_OBJ_TYPE_SPRITE =2,
   DATA_OBJ_TYPE_OBJECT =3,
   DATA_OBJ_TYPE_EVENT =4
};

class CDataObjBase
{
public:
   virtual DataObjectType_e GetObjType(){return m_ObjType;}
   void SetIndex(int index){m_index = index;}
   int GetIndex(){return m_index;};
   HTREEITEM m_hItem;
protected:
   DataObjectType_e m_ObjType;
   int m_index;
};

class CLevelDataObject : public CDataObjBase
{
public:

   CLevelDataObject() : CDataObjBase()
   {
      m_ObjType = DATA_OBJ_TYPE_OBJECT;
      m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
      m_Type = 0;
      m_Name = "Object";
      m_bVisable = TRUE;
   }
   D3DXVECTOR3 m_pos;
   int m_Type;
   CString m_Name;
   BOOL m_bVisable;

   virtual void Load(MSXML2::IXMLDOMNodePtr& Item)
   {
      XMLHelper::GetData(Item,"Type",m_Type);

      XMLHelper::GetData(Item,"x",m_pos.x);
      XMLHelper::GetData(Item,"y",m_pos.y);
      XMLHelper::GetData(Item,"z",m_pos.z);

      XMLHelper::GetData(Item,"Name",m_Name);

      XMLHelper::GetData(Item,"Visable",m_bVisable,TRUE);
   }

   virtual void Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement)
   {
      XMLHelper::SetData(DocPtr,pElement,"Type",m_Type);							
      XMLHelper::SetData(DocPtr,pElement,"x",m_pos.x);
      XMLHelper::SetData(DocPtr,pElement,"y",m_pos.y);
      XMLHelper::SetData(DocPtr,pElement,"z",m_pos.z);
      XMLHelper::SetData(DocPtr,pElement,"Name",m_Name);
      XMLHelper::SetData(DocPtr,pElement,"Visable",m_bVisable);
   }
};

enum AIType_e
{
   AI_NORMAL = 0,
   AI_GARD = 1,
   AI_GOTO =2,

   MAX_AI_TYPES = 3
};

class CLevelDataSprite : public CLevelDataObject
{
public:
   CLevelDataSprite() : CLevelDataObject()
   {
      m_ObjType = DATA_OBJ_TYPE_SPRITE;
      m_Team = 0;
      m_Name = "Sprite";
      m_GoalName = _T("");
      m_AIType = AI_NORMAL;
      m_SpriteObjType = 0;
      m_CurrentTexture = 0;
   }
   DWORD m_Team;

   CString m_GoalName;

   AIType_e  m_AIType;

   int m_CurrentTexture;



   virtual void Load(MSXML2::IXMLDOMNodePtr& Item)
   {
      CLevelDataObject::Load(Item);

      XMLHelper::GetData(Item,"Team",m_Team);

      XMLHelper::GetData(Item,"CurrentTexture",m_CurrentTexture);

      XMLHelper::GetData(Item,"GoalName",m_GoalName);

      DWORD val;
      XMLHelper::GetData(Item,"AIType",val,AI_NORMAL);
      m_AIType = (AIType_e)val;
   }


   virtual void Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement)
   {
      CLevelDataObject::Save(DocPtr,pElement);

      XMLHelper::SetData(DocPtr,pElement,"Team",m_Team);
      XMLHelper::SetData(DocPtr,pElement,"CurrentTexture",m_CurrentTexture);
      XMLHelper::SetData(DocPtr,pElement,"GoalName",m_GoalName);
      XMLHelper::SetData(DocPtr,pElement,"AIType",m_AIType);
   }
   void SetSpriteObjType(int SpriteObjType){m_SpriteObjType = SpriteObjType;}
   int GetSpriteObjType(){return m_SpriteObjType;};
protected:
   int m_SpriteObjType;
};


enum EventType_e
{
   EVENT_SPRITE_DISTROYED = 0,
   EVENT_HIT_WAYPOINT = 1,
   EVENT_TIMEOUT = 2,
   EVENT_ALL_ENEMYS_DISTOYED = 3
};

enum GameState_e
{
   GAME_NOCHANGE = 0,
   GAME_LOSE = 1,
   GAME_WIN = 2
};

class CGameAction
{
public:
   EventType_e m_EventType;
   CString m_ObjectName;
   CString m_ResponsName;
};


class CGameEvent : public CDataObjBase
{
public:

   virtual BOOL Check(FLOAT LevelTimer, std::vector<CGameAction>& Actions,BOOL bFoundEnemy);
   CGameEvent();
   virtual ~CGameEvent();
   void Reset();
   virtual void Load(MSXML2::IXMLDOMNodePtr& Item);
   virtual void Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement);

   //inputs
   EventType_e m_EventType;
   CString m_ObjectName;
   CString m_ResponsName;
   FLOAT m_Timer;


   // actions
   std::vector<CLevelDataSprite*> m_CreateSprites;
   std::vector<CLevelDataObject*> m_CreateObjects;
   std::vector<CLevelDataSprite*> m_SpriteChange;
   CString m_Message;
   GameState_e m_GameState;
   CString m_EventToActivate;

   void ActivateEvent(FLOAT LevelTimer);

   CString m_Name;
   BOOL m_bActive;

   BOOL m_bDisplay;

private:
   BOOL bOccured;
   FLOAT m_StartTime;
   BOOL m_bCurrenlyActive;
};


class CTextureData : public CDataObjBase
{
public:
   CTextureData() : CDataObjBase()
   {
      m_ObjType = DATA_OBJ_TYPE_TEXTURE;
      m_bBaseTextrue = FALSE;
      m_File = _T("");
      m_AlphaFile = _T("");
      m_fScale = 64.0f;
      m_fAlphaScale = 1.0f;
   }
   BOOL m_bBaseTextrue;
   CString m_File;
   FLOAT m_fScale;
   CString m_AlphaFile;
   FLOAT m_fAlphaScale;

   virtual void Load(MSXML2::IXMLDOMNodePtr& Item)
   {
      XMLHelper::GetData(Item,"File",m_File);		
      XMLHelper::GetData(Item,"Scale",m_fScale);
      XMLHelper::GetData(Item,"AlphaFile",m_AlphaFile);
      XMLHelper::GetData(Item,"AlphaScale",m_fAlphaScale);		
      XMLHelper::GetData(Item,"BaseTexture",m_bBaseTextrue,FALSE);
   }


   virtual void Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement)
   {		
      XMLHelper::SetData(DocPtr,pElement,"File",m_File);		
      XMLHelper::SetData(DocPtr,pElement,"Scale",m_fScale);
      XMLHelper::SetData(DocPtr,pElement,"AlphaFile",m_AlphaFile);
      XMLHelper::SetData(DocPtr,pElement,"AlphaScale",m_fAlphaScale);		
      XMLHelper::SetData(DocPtr,pElement,"BaseTexture",m_bBaseTextrue);
   }
};

class CLevelData : public CDataObjBase
{
public:
   CLevelData();
   virtual ~CLevelData();
   CString m_Name;
   CString m_Discription;


   CString m_MapFile;
   CString m_SkyBoxFile;
   CString m_GroundTexture;
   CString m_WaterTexture;
   CString m_WaterTexture2;

   BOOL m_bShowSkyBox;
   BOOL m_bWater;

   DWORD m_BGColour;
   FLOAT m_WaterLevel;
   FLOAT m_MapHight;

   FLOAT m_fFogStart;
   FLOAT m_fFogEnd;

   D3DXVECTOR3 m_StartPos;
   //CLevelCondition m_Win;
   //CLevelCondition m_Lose;

   std::vector<CLevelDataSprite*> m_Sprites;
   std::vector<CLevelDataObject*> m_Objects;
   std::vector<CGameEvent*> m_Events;
   std::vector<CTextureData*> m_Textures;

   D3DXVECTOR3 m_SunPos;
   FLOAT m_MapScale;

   virtual void Load(MSXML2::IXMLDOMNodePtr& Item);

   virtual void Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement);

   void Reset();

};

enum PlanetObjectType_e
{
   OBJECTTYPE_TREE = 0,
   OBJECTTYPE_BUSH = 1,
   OBJECTTYPE_MESH = 2,
   OBJECTTYPE_POWERUP = 3,
   OBJECTTYPE_WAYPOINT = 4
};

enum PowerUpType_e
{
   PU_INVULNERABILITY = 0,
   PU_INVISIBILITY = 1,
   PU_STATS = 2,
};


class CPowerUpData
{
public:
   CPowerUpData()
   {
      m_Type = PU_STATS;
      m_Duration = 0;
      m_WeaponPower = 0;
      m_Health = 0;
      m_Shields = 0;
      m_MaxSpeed = 0;
      m_StartTime = 0;
   }
   FLOAT m_Duration;
   PowerUpType_e m_Type;

   FLOAT m_StartTime;
   // Stats
   FLOAT m_WeaponPower; // %
   FLOAT m_Health; // %
   FLOAT m_Shields; // %
   FLOAT m_MaxSpeed; // %
};


class CPlanetObjectData
{
public:
   CPlanetObjectData()
   {
      m_Type = OBJECTTYPE_MESH;
      m_Name = "";
      m_File = "";
      m_fScale = 1;
      m_xSize = 0; 
      m_ySize = 0;
      m_bShadow = FALSE;
   }
   PlanetObjectType_e m_Type;
   CString m_Name;
   CString m_File;
   FLOAT m_fScale;
   FLOAT m_xSize;
   FLOAT m_ySize;
   BOOL m_bShadow;
   CPowerUpData m_PowerUp;
   int m_Index;
public:
   void Copy(const CPlanetObjectData& pOther)
   {
      m_Type = pOther.m_Type;
      m_Name= pOther.m_Name;
      m_File= pOther.m_File;
      m_fScale= pOther.m_fScale;
      m_xSize= pOther.m_xSize;
      m_ySize= pOther.m_ySize;
      m_bShadow = pOther.m_bShadow;
      m_PowerUp.m_Duration= pOther.m_PowerUp.m_Duration;
      m_PowerUp.m_Type= pOther.m_PowerUp.m_Type;

      m_PowerUp.m_WeaponPower= pOther.m_PowerUp.m_WeaponPower;
      m_PowerUp.m_Health= pOther.m_PowerUp.m_Health;
      m_PowerUp.m_Shields= pOther.m_PowerUp.m_Shields;
      m_PowerUp.m_MaxSpeed= pOther.m_PowerUp.m_MaxSpeed;
      m_Index= pOther.m_Index;
   }


   virtual void Load(MSXML2::IXMLDOMNodePtr& Item)
   {
      XMLHelper::GetData(Item,"File",m_File);
      XMLHelper::GetData(Item,"Name",m_Name);
      XMLHelper::GetData(Item,"Scale",m_fScale);
      int type;
      XMLHelper::GetData(Item,"Type",type);
      m_Type = (PlanetObjectType_e)type;
      XMLHelper::GetData(Item,"xSize",m_xSize);
      XMLHelper::GetData(Item,"ySize",m_ySize);
      XMLHelper::GetData(Item,"bShadow",m_bShadow,FALSE);
      if(m_Type == OBJECTTYPE_POWERUP)
      {
         XMLHelper::GetData(Item,"PowerUp.Duration",m_PowerUp.m_Duration);
         XMLHelper::GetData(Item,"PowerUp.WeaponPower",m_PowerUp.m_WeaponPower);
         XMLHelper::GetData(Item,"PowerUp.Health",m_PowerUp.m_Health);
         XMLHelper::GetData(Item,"PowerUp.Shields",m_PowerUp.m_Shields);
         XMLHelper::GetData(Item,"PowerUp.MaxSpeed",m_PowerUp.m_MaxSpeed);
         XMLHelper::GetData(Item,"PowerUp.Type",type);
         m_PowerUp.m_Type = (PowerUpType_e)type;
      }
   }

   virtual void Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement)
   {		
      XMLHelper::SetData(DocPtr,pElement,"File",m_File);
      XMLHelper::SetData(DocPtr,pElement,"Name",m_Name);
      XMLHelper::SetData(DocPtr,pElement,"Scale",m_fScale);
      XMLHelper::SetData(DocPtr,pElement,"Type",m_Type);
      XMLHelper::SetData(DocPtr,pElement,"xSize",m_xSize);
      XMLHelper::SetData(DocPtr,pElement,"ySize",m_ySize);
      XMLHelper::SetData(DocPtr,pElement,"bShadow",m_bShadow);
      if(m_Type == OBJECTTYPE_POWERUP)
      {
         XMLHelper::SetData(DocPtr,pElement,"PowerUp.Duration",m_PowerUp.m_Duration);
         XMLHelper::SetData(DocPtr,pElement,"PowerUp.WeaponPower",m_PowerUp.m_WeaponPower);
         XMLHelper::SetData(DocPtr,pElement,"PowerUp.Health",m_PowerUp.m_Health);
         XMLHelper::SetData(DocPtr,pElement,"PowerUp.Shields",m_PowerUp.m_Shields);
         XMLHelper::SetData(DocPtr,pElement,"PowerUp.MaxSpeed",m_PowerUp.m_MaxSpeed);
         XMLHelper::SetData(DocPtr,pElement,"PowerUp.Type",m_PowerUp.m_Type);
      }
   }
};



#endif // !defined(AFX_LEVELDATA_H__3DCB3FC9_73C8_4B25_A9D0_AD2BD22D4C7F__INCLUDED_)
