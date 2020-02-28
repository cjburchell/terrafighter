///////////////////////////////////////////////////////////////////////////////
///  SpriteData.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 4:44:59 PM by Christiaan Burchell
///
///  PURPOSE:  Discription of the Sprite Data file
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

#ifndef __SPRITE_DATA_H___
#define __SPRITE_DATA_H___

#include <vector>
#include "xmlhelpers.h"
#include <d3d9.h>
#include <D3DX9Math.h>

class CSpriteData
{
public:
   virtual void Load(MSXML2::IXMLDOMNodePtr& Item)
   {
      XMLHelper::GetData(Item,"File",m_FileName);
      XMLHelper::GetData(Item,"Name",m_Name);

      XMLHelper::GetData(Item,"Scale",m_fScale);
      XMLHelper::GetData(Item,"MaxSpeed",m_fMaxSpeed);
      XMLHelper::GetData(Item,"Turning_Rate",m_fTurningRate);
      XMLHelper::GetData(Item,"Acceleration",m_fAcceleration);
      XMLHelper::GetData(Item,"Drag",m_Drag);

      XMLHelper::GetData(Item,"DefaultWeapon",m_DefaultWeapon);


      XMLHelper::GetData(Item,"IsBuilding",m_IsBuilding,FALSE);
      XMLHelper::GetData(Item,"UserSelectable",m_bCanBeUser,TRUE);


      XMLHelper::GetData(Item,"MinSpeed",  m_fMinSpeed);
      XMLHelper::GetData(Item,"MaxHight",  m_fMaxHight);
      XMLHelper::GetData(Item,"MaxShields",m_fMaxShields);
      XMLHelper::GetData(Item,"MaxDamage", m_fMaxDamage);
      XMLHelper::GetData(Item,"RegenRate", m_fRegenRate);
      XMLHelper::GetData(Item,"MinHight",  m_fMinHight);
      XMLHelper::GetData(Item,"zMeshRot",  m_zMeshRot);
      XMLHelper::GetData(Item,"yMeshRot",  m_yMeshRot);
      XMLHelper::GetData(Item,"RestrictRot",  m_bRestrictRot);

      MSXML2::IXMLDOMNodeListPtr TextureListPtr = Item->selectNodes("Texture");
      int ElementCount = TextureListPtr->length;
      if(ElementCount > 0) // if there are any rows
      {
         for(int j=0;j<ElementCount;j++)
         {
            MSXML2::IXMLDOMNodePtr TextureItem = TextureListPtr->item[j];
            CString TexFile;
            XMLHelper::GetData(TextureItem,"File",TexFile);
            m_Textures.push_back(TexFile);
         }
      }
   }


   virtual void Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement)
   {		
      XMLHelper::SetData(DocPtr,pElement,"File",m_FileName);
      XMLHelper::SetData(DocPtr,pElement,"Name",m_Name);

      XMLHelper::SetData(DocPtr,pElement,"Scale",m_fScale);
      XMLHelper::SetData(DocPtr,pElement,"MaxSpeed",m_fMaxSpeed);
      XMLHelper::SetData(DocPtr,pElement,"Turning_Rate",m_fTurningRate);
      XMLHelper::SetData(DocPtr,pElement,"Acceleration",m_fAcceleration);
      XMLHelper::SetData(DocPtr,pElement,"Drag",m_Drag);

      XMLHelper::SetData(DocPtr,pElement,"DefaultWeapon",m_DefaultWeapon);


      XMLHelper::SetData(DocPtr,pElement,"IsBuilding",m_IsBuilding);
      XMLHelper::SetData(DocPtr,pElement,"UserSelectable",m_bCanBeUser);


      XMLHelper::SetData(DocPtr,pElement,"MinSpeed",  m_fMinSpeed);
      XMLHelper::SetData(DocPtr,pElement,"MaxHight",  m_fMaxHight);
      XMLHelper::SetData(DocPtr,pElement,"MaxShields",m_fMaxShields);
      XMLHelper::SetData(DocPtr,pElement,"MaxDamage", m_fMaxDamage);
      XMLHelper::SetData(DocPtr,pElement,"RegenRate", m_fRegenRate);
      XMLHelper::SetData(DocPtr,pElement,"MinHight",  m_fMinHight);
      XMLHelper::SetData(DocPtr,pElement,"zMeshRot",  m_zMeshRot);
      XMLHelper::SetData(DocPtr,pElement,"yMeshRot",  m_yMeshRot);
      XMLHelper::SetData(DocPtr,pElement,"RestrictRot",  m_bRestrictRot);

      for(UINT i=0;i<m_Textures.size();i++)
      {
         MSXML2::IXMLDOMElementPtr pTextElement;
         pTextElement = DocPtr->createElement("Texture");
         pElement->appendChild(pTextElement);
         XMLHelper::SetData(DocPtr,pTextElement,"File",m_Textures[i]);
      }
   }

   CSpriteData()
   {
      m_Name = _T("");
      m_FileName= _T("");
      m_fScale = 1.0f;
      m_zMeshRot = 0.0f;
      m_yMeshRot = 0.0f;


      m_fMaxSpeed= 0.0f;
      m_fMinSpeed= 0.0f;

      m_fTurningRate= 0.0f;
      m_fAcceleration= 0.0f;
      m_Drag= 0.0f;

      m_fMaxShields= 100.0f;
      m_fMaxDamage= 100.0f;
      m_fRegenRate= 1.0f;

      m_fMaxHight= 20.0f;
      m_fMinHight= 0.0f;

      m_DefaultWeapon = 0;

      m_bRestrictRot = FALSE;

      m_IsBuilding = FALSE;
      m_bCanBeUser = FALSE;

      m_Index = 0;
   }


   CString m_Name;
   CString m_FileName;
   FLOAT m_fScale;
   FLOAT m_zMeshRot;
   FLOAT m_yMeshRot;


   FLOAT m_fMaxSpeed;
   FLOAT m_fMinSpeed;

   FLOAT m_fTurningRate;
   FLOAT m_fAcceleration;
   FLOAT m_Drag;

   FLOAT m_fMaxShields;
   FLOAT m_fMaxDamage;
   FLOAT m_fRegenRate;

   FLOAT m_fMaxHight;
   FLOAT m_fMinHight;

   int m_DefaultWeapon;

   BOOL m_bRestrictRot;

   BOOL m_IsBuilding;
   BOOL m_bCanBeUser;

   std::vector<CString> m_Textures;

   int m_Index;
};

class CBeamData
{
public:
   CBeamData()
   {
   }

   virtual ~CBeamData()
   {
   }

   virtual void Load(MSXML2::IXMLDOMNodePtr& Item)
   {
      XMLHelper::GetData(Item,"Colour1",m_Colour1);
      XMLHelper::GetData(Item,"Colour2",m_Colour2);
      XMLHelper::GetData(Item,"Size",m_fSize);
      XMLHelper::GetData(Item,"Damage",m_fDamage);
      XMLHelper::GetData(Item,"Speed",m_fSpeed);
      XMLHelper::GetData(Item,"Port",m_Port);
      XMLHelper::GetData(Item,"Sound",m_SoundFile);
      XMLHelper::GetData(Item,"Gravity",m_fGravity);
      XMLHelper::GetData(Item,"Seek",m_bSeek);


   }
   virtual void Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement)
   {
      XMLHelper::SetData(DocPtr,pElement,"Colour1",m_Colour1);
      XMLHelper::SetData(DocPtr,pElement,"Colour2",m_Colour2);
      XMLHelper::SetData(DocPtr,pElement,"Size",m_fSize);
      XMLHelper::SetData(DocPtr,pElement,"Damage",m_fDamage);
      XMLHelper::SetData(DocPtr,pElement,"Speed",m_fSpeed);
      XMLHelper::SetData(DocPtr,pElement,"Port",m_Port);
      XMLHelper::SetData(DocPtr,pElement,"Sound",m_SoundFile);
      XMLHelper::SetData(DocPtr,pElement,"Gravity",m_fGravity);
      XMLHelper::SetData(DocPtr,pElement,"Seek",m_bSeek);


   }
protected:
   DWORD m_Colour1;
   DWORD m_Colour2;
   FLOAT m_fSize;
   FLOAT m_fDamage;
   FLOAT m_fSpeed;
   DWORD m_Port;
   FLOAT m_fGravity;
   BOOL m_bSeek;
   CString m_SoundFile;

};

#define NO_WEAPON_SEL -1

class CWeaponData
{
public:
   CWeaponData()
   {
      m_Name = "";
      m_RateOfFire = 1;
   }
   virtual ~CWeaponData()
   {
      for(UINT i=0;i<m_Beams.size();i++)
      {
         delete m_Beams[i];
      }
   }

   virtual void Load(MSXML2::IXMLDOMNodePtr& Item)
   {
      XMLHelper::GetData(Item,"RateOfFire",m_RateOfFire);
      XMLHelper::GetData(Item,"Name",m_Name);

      MSXML2::IXMLDOMNodeListPtr RowListPtr = Item->selectNodes("Beam");
      UINT nBeams = RowListPtr->length;

      if(nBeams > 0) // if there are any rows
      {
         for(UINT i=0;i<m_Beams.size();i++)
         {
            delete m_Beams[i];
         }
         m_Beams.clear();



         for(i=0;i<nBeams;i++) // for each row
         {
            // get the row
            MSXML2::IXMLDOMNodePtr RowItem = RowListPtr->item[i];
            CBeamData* pObj = CreateBeam();
            pObj->Load(RowItem);
            m_Beams.push_back(pObj);
         }


      }
   }

   virtual void Save(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pElement)
   {
      XMLHelper::SetData(DocPtr,pElement,"RateOfFire",m_RateOfFire);
      XMLHelper::SetData(DocPtr,pElement,"Name",m_Name);

      for(UINT i=0;i<m_Beams.size();i++)
      {
         MSXML2::IXMLDOMElementPtr pEventElement;

         pEventElement = DocPtr->createElement("Beam");
         pElement->appendChild(pEventElement);
         m_Beams[i]->Save(DocPtr,pEventElement);
      }
   }

   virtual CBeamData* CreateBeam()
   {
      return new CBeamData; 
   }

   virtual CBeamData* GetBeamData(DWORD beam)
   {
      if(beam < m_Beams.size())
         return m_Beams[beam];

      return NULL;
   }

   CString m_Name;
   FLOAT m_RateOfFire;
protected:
   std::vector<CBeamData*> m_Beams;
};

#endif