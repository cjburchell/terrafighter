///////////////////////////////////////////////////////////////////////////////
///  TerraEdDoc.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:07:16 AM by Christiaan Burchell
///
///  PURPOSE:  implementation of the CTerraEdDoc class
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

#include "stdafx.h"
#include "TerraEd.h"
#include "TerraEdDoc.h"
#include "XmlHelpers.h"
#include "DeleteFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LEVEL_FLIE_NAME "Levels.xml"
#define OBJECTS_FLIE_NAME "Objects.xml"
#define SPRITES_FLIE_NAME "Sprites.xml"
#define WEAPONS_FLIE_NAME "Weapons.xml"

/////////////////////////////////////////////////////////////////////////////
// CTerraEdDoc

IMPLEMENT_DYNCREATE(CTerraEdDoc, CDocument)

BEGIN_MESSAGE_MAP(CTerraEdDoc, CDocument)
   //{{AFX_MSG_MAP(CTerraEdDoc)
   ON_COMMAND(ID_TOOLS_ADDFILE, OnToolsAddfile)
   ON_COMMAND(ID_TOOLS_REMOVEFILE, OnToolsRemovefile)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerraEdDoc construction/destruction



/////////////////////////////////////////////////
///
/// NAME: CTerraEdDoc
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 9:07:23 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CTerraEdDoc::CTerraEdDoc()
{
   // TODO: add one-time construction code here
}



/////////////////////////////////////////////////
///
/// NAME: ~CTerraEdDoc
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     03/06/2004 9:07:25 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CTerraEdDoc::~CTerraEdDoc()
{
   ClearLevelData();
   ClearPlanetObjectData();
   ClearSpriteData();
   for(UINT i=0;i<m_WeaponData.size();i++)
   {
      delete m_WeaponData[i];
   }
   m_WeaponData.clear();

   m_Zip.Close();
}



/////////////////////////////////////////////////
///
/// NAME: OnNewDocument
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Creates a new document
///
/// CREATED:     03/06/2004 9:07:37 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::OnNewDocument()
{
   if (!CDocument::OnNewDocument())
      return FALSE;

   // TODO: add reinitialization code here
   // (SDI documents will reuse this document)

   ClearLevelData();
   ClearPlanetObjectData();
   ClearSpriteData();
   for(UINT i=0;i<m_WeaponData.size();i++)
   {
      delete m_WeaponData[i];
   }
   m_WeaponData.clear();

   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: ClearLevelData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Clears the level data
///
/// CREATED:     03/06/2004 9:07:46 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdDoc::ClearLevelData()
{
   for(UINT i=0;i<m_Levels.size();i++)
   {
      delete m_Levels[i];
   }
   m_Levels.clear();
}



/////////////////////////////////////////////////////////////////////////////
// CTerraEdDoc serialization



/////////////////////////////////////////////////
///
/// NAME: Serialize
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Serializes the Object
///
/// CREATED:     03/06/2004 9:07:49 AM
///
/// PARAMETERS: 
///             CArchive& ar
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdDoc::Serialize(CArchive& ar)
{
   if (ar.IsStoring())
   {
      // TODO: add storing code here
   }
   else
   {
      // TODO: add loading code here
   }
}

/////////////////////////////////////////////////////////////////////////////
// CTerraEdDoc diagnostics

#ifdef _DEBUG


/////////////////////////////////////////////////
///
/// NAME: AssertValid
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Debug Function
///
/// CREATED:     03/06/2004 9:07:53 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdDoc::AssertValid() const
{
   CDocument::AssertValid();
}



/////////////////////////////////////////////////
///
/// NAME: Dump
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Debug Function
///
/// CREATED:     03/06/2004 9:07:54 AM
///
/// PARAMETERS: 
///             CDumpContext& dc
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdDoc::Dump(CDumpContext& dc) const
{
   CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTerraEdDoc commands



/////////////////////////////////////////////////
///
/// NAME: OnOpenDocument
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Opens a new document
///
/// CREATED:     03/06/2004 9:08:02 AM
///
/// PARAMETERS: 
///             LPCTSTR lpszPathName
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
   AfxGetApp()->WriteProfileString(_T("Settings"),_T("LastOpenFile"),lpszPathName);
   //if (!CDocument::OnOpenDocument(lpszPathName))
   //	return FALSE;

   //m_Zip.Open("Test2.zip");
   //m_Zip.AddNewFile("Test.txt");
   //m_Zip.Close();

   try
   {
      m_Zip.Open(lpszPathName);
   }
   catch(...)
   {
      CString str;
      str.Format("Unable To Open File %s",lpszPathName);
      MessageBox(NULL,str,"ERROR",0);
      return FALSE;
   }



   if(!LoadLevelData())
      return FALSE;
   if(!LoadSpriteData())
      return FALSE;
   if(!LoadPlanetObjectData())
      return FALSE;
   if(!LoadWeaponData())
      return FALSE;


   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: OnSaveDocument
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Saves the doument
///
/// CREATED:     03/06/2004 9:08:12 AM
///
/// PARAMETERS: 
///             LPCTSTR lpszPathName
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
   AfxGetApp()->WriteProfileString(_T("Settings"),_T("LastOpenFile"),lpszPathName);

   SaveLevelData();
   SavePlanetObjectData();
   SaveSpriteData();
   m_Zip.Close();
   m_Zip.Open(lpszPathName);

   return TRUE;//CDocument::OnSaveDocument(lpszPathName);
}



/////////////////////////////////////////////////
///
/// NAME: LoadPlanetObjectData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Loads the planet Object data
///
/// CREATED:     03/06/2004 9:10:32 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::LoadPlanetObjectData()
{
   try
   {
      MSXML2::IXMLDOMDocumentPtr docPtr;
      // create the xml document
      if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
         return FALSE;

      if(!m_Zip.ExtractFileTo(OBJECTS_FLIE_NAME,"temp.xml"))
      {
         AfxMessageBox("Unable to load Objects.xml");
         return FALSE;
      }


      _variant_t varOut((bool)TRUE);

      varOut = docPtr->load("temp.xml"); // load the xml document

      DeleteFile("temp.xml");

      if((bool)varOut == TRUE)
      {

         int ElementCount;
         MSXML2::IXMLDOMElementPtr docElementPtr = docPtr->documentElement;

         MSXML2::IXMLDOMNodeListPtr RowListPtr = docElementPtr->selectNodes("Object");
         ElementCount = RowListPtr->length;

         if(ElementCount > 0) // if there are any rows
         {

            ClearPlanetObjectData();

            for(int i=0;i<ElementCount;i++) // for each row
            {
               // get the row
               MSXML2::IXMLDOMNodePtr RowItem = RowListPtr->item[i];
               CPlanetObjectData Data;
               Data.Load(RowItem);

               CPlanetObjectDataEx* pData;
               switch(Data.m_Type)
               {
               case OBJECTTYPE_TREE:
                  pData = new CPlanetObjectTree();
                  break;
               case OBJECTTYPE_BUSH:
                  pData = new CPlanetObjectBush();
                  break;
               case OBJECTTYPE_MESH:
               case OBJECTTYPE_POWERUP:
               case OBJECTTYPE_WAYPOINT:
                  pData = new CPlanetObjectMesh();
                  break;
               }

               pData->Load(RowItem);
               pData->m_Index = i;
               m_PlanetObjectData.push_back(pData);

            }
         }
         else
            return FALSE;
      }
   }
   catch(...)
   {
      AfxMessageBox("Exception occurred LoadPlanetObjectData");
      return FALSE;
   }
   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: LoadSpriteData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Loads the sprite data
///
/// CREATED:     03/06/2004 9:10:22 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::LoadSpriteData()
{
   try
   {
      MSXML2::IXMLDOMDocumentPtr docPtr;
      // create the xml document
      if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
         return FALSE;

      if(!m_Zip.ExtractFileTo(SPRITES_FLIE_NAME,"temp.xml"))
      {
         AfxMessageBox("Unable to load Sprites.xml");
         return FALSE;
      }

      _variant_t varOut((bool)TRUE);

      varOut = docPtr->load("temp.xml"); // load the xml document

      DeleteFile("temp.xml");

      if((bool)varOut == TRUE)
      {

         int ElementCount;
         MSXML2::IXMLDOMElementPtr docElementPtr = docPtr->documentElement;
         MSXML2::IXMLDOMNodeListPtr RowListPtr = docElementPtr->selectNodes("Sprite");
         ElementCount = RowListPtr->length;

         if(ElementCount > 0) // if there are any rows
         {

            ClearSpriteData();

            for(int i=0;i<ElementCount;i++) // for each row
            {
               // get the row
               MSXML2::IXMLDOMNodePtr RowItem = RowListPtr->item[i];

               CSpriteDataEx* pData = new CSpriteDataEx;
               pData->Load(RowItem);
               pData->m_Index = i;
               m_SpriteData.push_back(pData);

            }
         }
         else
            return FALSE;
      }
   }
   catch(...)
   {
      AfxMessageBox("Exception occurred LoadShips");
      return FALSE;
   }
   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: ClearPlanetObjectData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Clears the planet data
///
/// CREATED:     03/06/2004 9:08:27 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdDoc::ClearPlanetObjectData()
{
   for(UINT i=0;i<m_PlanetObjectData.size();i++)
   {
      delete m_PlanetObjectData[i];
   }
   m_PlanetObjectData.clear();
}



/////////////////////////////////////////////////
///
/// NAME: ClearSpriteData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Clears the sprite data
///
/// CREATED:     03/06/2004 9:08:36 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdDoc::ClearSpriteData()
{
   for(UINT i=0;i<m_SpriteData.size();i++)
   {
      delete m_SpriteData[i];
   }
   m_SpriteData.clear();
}



/////////////////////////////////////////////////
///
/// NAME: SavePlanetObjectData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Saves the planet data
///
/// CREATED:     03/06/2004 9:10:08 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::SavePlanetObjectData()
{
   try 
   {
      MSXML2::IXMLDOMDocumentPtr	l_docPtr;
      MSXML2::IXMLDOMElementPtr  l_Root;
      l_docPtr.CreateInstance(_T("msxml2.domdocument"));

      l_Root = l_docPtr->createElement(_T("Objects"));
      l_docPtr->appendChild(l_Root);

      for(UINT i=0;i<m_PlanetObjectData.size();i++)
      {
         MSXML2::IXMLDOMElementPtr pElement;

         pElement = l_docPtr->createElement("Object");
         l_Root->appendChild(pElement);
         m_PlanetObjectData[i]->Save(l_docPtr,pElement);
      }

      l_docPtr->save(COleVariant(OBJECTS_FLIE_NAME));

      int index = m_Zip.FindFile(OBJECTS_FLIE_NAME);
      if( index != -1)
         m_Zip.DeleteFile(index);
      m_Zip.AddNewFile(OBJECTS_FLIE_NAME);
      DeleteFile(OBJECTS_FLIE_NAME);
   }
   catch (...)
   {
      sys_msg("Unexpected exception trying to save the XML file in CEMSOIMainFrame::OnFileOisaveworkspace\n");
      return FALSE;
   }	
   return TRUE;//CDocument::OnSaveDocument(lpszPathName);
}



/////////////////////////////////////////////////
///
/// NAME: SaveSpriteData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Saves the sprite data
///
/// CREATED:     03/06/2004 9:09:59 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::SaveSpriteData()
{
   try 
   {
      MSXML2::IXMLDOMDocumentPtr	l_docPtr;
      MSXML2::IXMLDOMElementPtr  l_Root;
      l_docPtr.CreateInstance(_T("msxml2.domdocument"));

      l_Root = l_docPtr->createElement(_T("Sprites"));
      l_docPtr->appendChild(l_Root);

      for(UINT i=0;i<m_SpriteData.size();i++)
      {
         MSXML2::IXMLDOMElementPtr pElement;

         pElement = l_docPtr->createElement("Sprite");
         l_Root->appendChild(pElement);
         m_SpriteData[i]->Save(l_docPtr,pElement);
      }

      l_docPtr->save(COleVariant(SPRITES_FLIE_NAME));

      int index = m_Zip.FindFile(SPRITES_FLIE_NAME);
      if( index != -1)
         m_Zip.DeleteFile(index);
      m_Zip.AddNewFile(SPRITES_FLIE_NAME);
      DeleteFile(SPRITES_FLIE_NAME);
   }
   catch (...)
   {
      sys_msg("Unexpected exception trying to save the XML file in CEMSOIMainFrame::OnFileOisaveworkspace\n");
      return FALSE;
   }	
   return TRUE;//CDocument::OnSaveDocument(lpszPathName);
}



/////////////////////////////////////////////////
///
/// NAME: SaveWeaponData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Saves the weapon data
///
/// CREATED:     03/06/2004 9:09:49 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::SaveWeaponData()
{
   try 
   {
      MSXML2::IXMLDOMDocumentPtr	l_docPtr;
      MSXML2::IXMLDOMElementPtr  l_Root;
      l_docPtr.CreateInstance(_T("msxml2.domdocument"));

      l_Root = l_docPtr->createElement(_T("Weapons"));
      l_docPtr->appendChild(l_Root);

      for(UINT i=0;i<m_WeaponData.size();i++)
      {
         MSXML2::IXMLDOMElementPtr pElement;

         pElement = l_docPtr->createElement("Weapon");
         l_Root->appendChild(pElement);
         m_WeaponData[i]->Save(l_docPtr,pElement);
      }

      l_docPtr->save(COleVariant(WEAPONS_FLIE_NAME));

      int index = m_Zip.FindFile(WEAPONS_FLIE_NAME);
      if( index != -1)
         m_Zip.DeleteFile(index);
      m_Zip.AddNewFile(WEAPONS_FLIE_NAME);
      DeleteFile(WEAPONS_FLIE_NAME);
   }
   catch (...)
   {
      sys_msg("Unexpected exception trying to save the XML file in CEMSOIMainFrame::OnFileOisaveworkspace\n");
      return FALSE;
   }	
   return TRUE;//CDocument::OnSaveDocument(lpszPathName);
}



/////////////////////////////////////////////////
///
/// NAME: LoadWeaponData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Loads the weapon data
///
/// CREATED:     03/06/2004 9:09:40 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::LoadWeaponData()
{
   try
   {
      MSXML2::IXMLDOMDocumentPtr docPtr;
      // create the xml document
      if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
         return FALSE;


      if(!m_Zip.ExtractFileTo(WEAPONS_FLIE_NAME,"temp.xml"))
      {
         AfxMessageBox("Unable to load Weapons.xml");
         return FALSE;
      }

      _variant_t varOut((bool)TRUE);

      varOut = docPtr->load("temp.xml"); // load the xml document

      DeleteFile("temp.xml");


      if((bool)varOut == TRUE)
      {
         MSXML2::IXMLDOMNodeListPtr RowListPtr = docPtr->documentElement->selectNodes("Weapon");
         int ElementCount = RowListPtr->length;

         if(ElementCount > 0) // if there are any rows
         {

            {
               for(UINT i=0;i<m_WeaponData.size();i++)
               {
                  delete m_WeaponData[i];
               }
               m_WeaponData.clear();
            }

            for(UINT i=0;i<ElementCount;i++) // for each row
            {
               // get the row
               MSXML2::IXMLDOMNodePtr RowItem = RowListPtr->item[i];
               CWeaponData* pWeapon = new CWeaponData;
               pWeapon->Load(RowItem);
               m_WeaponData.push_back(pWeapon);
            }
         }
         else
            return FALSE;
      }
   }
   catch(...)
   {
      sys_msg("Exception occurred LoadWeapons");
      return FALSE;
   }
   return TRUE;
}




/////////////////////////////////////////////////
///
/// NAME: LoadLevelData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Loads the level data
///
/// CREATED:     03/06/2004 9:09:31 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::LoadLevelData()
{
   try
   {
      MSXML2::IXMLDOMDocumentPtr docPtr;
      // create the xml document
      if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
         return FALSE;

      if(!m_Zip.ExtractFileTo(LEVEL_FLIE_NAME,"temp.xml"))
      {
         AfxMessageBox("Unable to load Levels.xml");
         return FALSE;
      }

      _variant_t varOut((bool)TRUE);

      varOut = docPtr->load("temp.xml"); // load the xml document

      DeleteFile("temp.xml");

      if((bool)varOut == TRUE)
      {
         int ElementCount;
         MSXML2::IXMLDOMElementPtr docElementPtr = docPtr->documentElement;
         MSXML2::IXMLDOMNodeListPtr RowListPtr = docElementPtr->selectNodes("Level");
         ElementCount = RowListPtr->length;

         if(ElementCount > 0) // if there are any rows
         {
            ClearLevelData();

            for(int i=0;i<ElementCount;i++) // for each row
            {
               // get the row
               MSXML2::IXMLDOMNodePtr RowItem = RowListPtr->item[i];
               CLevelData* pLevel = new CLevelData();
               pLevel->Load(RowItem);
               pLevel->SetIndex(m_Levels.size());
               m_Levels.push_back(pLevel);
            }
         }
         else
            return FALSE;
      }
   }
   catch(...)
   {
      AfxMessageBox("Exception occurred LoadShips");
      return FALSE;
   }
   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: SaveLevelData
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Saves the Level data
///
/// CREATED:     03/06/2004 9:09:23 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdDoc::SaveLevelData()
{
   try 
   {
      MSXML2::IXMLDOMDocumentPtr	l_docPtr;
      MSXML2::IXMLDOMElementPtr  l_Root;
      l_docPtr.CreateInstance(_T("msxml2.domdocument"));

      l_Root = l_docPtr->createElement(_T("Levels"));
      l_docPtr->appendChild(l_Root);

      for(UINT i=0;i<m_Levels.size();i++)
      {
         MSXML2::IXMLDOMElementPtr pElement;

         pElement = l_docPtr->createElement("Level");
         l_Root->appendChild(pElement);
         m_Levels[i]->Save(l_docPtr,pElement);
      }

      l_docPtr->save(COleVariant(LEVEL_FLIE_NAME));

      int index = m_Zip.FindFile(LEVEL_FLIE_NAME);
      if( index != -1)
         m_Zip.DeleteFile(index);
      m_Zip.AddNewFile(LEVEL_FLIE_NAME);
      DeleteFile(LEVEL_FLIE_NAME);
   }
   catch (...)
   {
      sys_msg("Unexpected exception trying to save the XML file in CEMSOIMainFrame::OnFileOisaveworkspace\n");
      return FALSE;
   }	
   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: OnToolsAddfile
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Adds a file to the arcive
///
/// CREATED:     03/06/2004 9:09:13 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdDoc::OnToolsAddfile() 
{
   static TCHAR BASED_CODE szFilter[] =  _T("All Files (*.*)|*.*||");
   CString FileName;

   CFileDialog dlg(TRUE,_T(""),FileName,OFN_HIDEREADONLY |OFN_NOCHANGEDIR,szFilter);
   if(dlg.DoModal() == IDOK)
   {
      BOOL bRemote = CopyFile(dlg.GetPathName(),dlg.GetFileName(),TRUE);

      int index = m_Zip.FindFile(dlg.GetFileName());
      if( index != -1)
         m_Zip.DeleteFile(index);
      m_Zip.AddNewFile(dlg.GetFileName());

      if(bRemote)
         DeleteFile(dlg.GetFileName());
   }

}



/////////////////////////////////////////////////
///
/// NAME: OnToolsRemovefile
///
/// CLASS: CTerraEdDoc
///
/// DESCRIPTION: Removes selected file from arcive
///
/// CREATED:     03/06/2004 9:09:03 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdDoc::OnToolsRemovefile() 
{
   CDeleteFileDlg dlg(&m_Zip);
   if(dlg.DoModal() == IDOK)
   {
      m_Zip.DeleteFiles(dlg.m_DelList);
   }
}
