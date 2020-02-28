///////////////////////////////////////////////////////////////////////////////
///  XmlHelpers.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 4:41:02 PM by Christiaan Burchell
///
///  PURPOSE:  XML Helper functions
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

#ifndef __XMLHELPERS_H__
#define __XMLHELPERS_H__

#pragma warning( disable : 4100)
#pragma warning( disable : 4786)


namespace XMLHelper
{
   /////////////////////////////////////////////////
   ///
   /// NAME: GetData
   ///
   /// DESCRIPTION: Gets a Float from the xml file
   ///
   /// CREATED:     02/06/2004 4:41:40 PM
   ///
   /// PARAMETERS: 
   ///             MSXML2::IXMLDOMNodePtr& Item
   ///             const TCHAR* string
   ///             FLOAT& val
   ///             FLOAT defval = 0.0f
   ///
   /// RETURN: inline void 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   inline void GetData(MSXML2::IXMLDOMNodePtr& Item,const TCHAR* string,FLOAT& val,FLOAT defval = 0.0f) 
   {
      try
      {
         MSXML2::IXMLDOMNodePtr attb1 = Item->attributes->getNamedItem(string);
         if(attb1)
            val = (float)atof(attb1->text);
         else
            val = defval;
      }
      catch(...)
      {
         val = defval;
      }
   }



   /////////////////////////////////////////////////
   ///
   /// NAME: GetData
   ///
   /// DESCRIPTION: Gets a string from the xml file
   ///
   /// CREATED:     02/06/2004 4:41:51 PM
   ///
   /// PARAMETERS: 
   ///             MSXML2::IXMLDOMNodePtr& Item
   ///             const TCHAR* string
   ///             CString& val
   ///             CString defval = _T(""
   ///
   /// RETURN: inline void 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   inline void GetData(MSXML2::IXMLDOMNodePtr& Item, const TCHAR* string,CString& val,CString defval = _T("")) 
   {
      try
      {
         MSXML2::IXMLDOMNodePtr attb1 = Item->attributes->getNamedItem(string);
         if(attb1)
            val = CString((TCHAR*)attb1->text);
         else
            val = defval;
      }
      catch(...)
      {
         val = defval;
      }
   }



   /////////////////////////////////////////////////
   ///
   /// NAME: GetData
   ///
   /// DESCRIPTION: Gets an int from an xml file
   ///
   /// CREATED:     02/06/2004 4:42:11 PM
   ///
   /// PARAMETERS: 
   ///             MSXML2::IXMLDOMNodePtr& Item
   ///             const TCHAR* string
   ///             int& val
   ///             int defval = 0
   ///
   /// RETURN: inline void 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   inline void GetData(MSXML2::IXMLDOMNodePtr& Item, const TCHAR* string,int& val,int defval = 0) 
   {
      try
      {
         MSXML2::IXMLDOMNodePtr attb1 = Item->attributes->getNamedItem(string);
         if(attb1)
            val = atol(attb1->text);
         else
            val = defval;
      }
      catch(...)
      {
         val = defval;
      }
   }



   /////////////////////////////////////////////////
   ///
   /// NAME: GetData
   ///
   /// DESCRIPTION: Gets a DWORD from and xml file
   ///
   /// CREATED:     02/06/2004 4:42:23 PM
   ///
   /// PARAMETERS: 
   ///             MSXML2::IXMLDOMNodePtr& Item
   ///             const TCHAR* string
   ///             DWORD& val
   ///             DWORD defval = 0
   ///
   /// RETURN: inline void 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   inline void GetData(MSXML2::IXMLDOMNodePtr& Item, const TCHAR* string,DWORD& val,DWORD defval = 0) 
   {
      try
      {
         MSXML2::IXMLDOMNodePtr attb1 = Item->attributes->getNamedItem(string);
         if(attb1)
            val = atol(attb1->text);
         else
            val = defval;
      }
      catch(...)
      {
         val = defval;
      }
   }



   /////////////////////////////////////////////////
   ///
   /// NAME: GetData
   ///
   /// DESCRIPTION: Gets a WORD from an xml file
   ///
   /// CREATED:     02/06/2004 4:42:36 PM
   ///
   /// PARAMETERS: 
   ///             MSXML2::IXMLDOMNodePtr& Item
   ///             const TCHAR* string
   ///             WORD& val
   ///             WORD defval = 0
   ///
   /// RETURN: inline void 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   inline void GetData(MSXML2::IXMLDOMNodePtr& Item, const TCHAR* string,WORD& val,WORD defval = 0) 
   {
      try
      {
         MSXML2::IXMLDOMNodePtr attb1 = Item->attributes->getNamedItem(string);
         if(attb1)
            val = (WORD)atol(attb1->text);
         else
            val = defval;
      }
      catch(...)
      {
         val = defval;
      }
   }



   /////////////////////////////////////////////////
   ///
   /// NAME: GetData
   ///
   /// DESCRIPTION: gest a int from a xml file
   ///
   /// CREATED:     02/06/2004 4:42:56 PM
   ///
   /// PARAMETERS: 
   ///             MSXML2::IXMLDOMElementPtr& Item
   ///             const TCHAR* string
   ///             int& val
   ///             int defval = 0
   ///
   /// RETURN: inline void 
   ///
   /// SIDE EFFECTS: 
   ///
   /////////////////////////////////////////////////
   inline void GetData(MSXML2::IXMLDOMElementPtr& Item, const TCHAR* string,int& val,int defval = 0) 
   {
      try
      {
         MSXML2::IXMLDOMNodePtr attb1 = Item->attributes->getNamedItem(string);
         if(attb1)
            val = atol(attb1->text);
         else
            val = defval;
      }
      catch(...)
      {
         val = defval;
      }
   }

   ////////////////////////////////////////////////
   //
   // NAME: SetData
   //
   // DESCRIPTION: sets an float in an XML file
   //
   // CREATED:     1/16/2003 5:10:44 PM
   //
   // PARAMETERS: 
   //             MSXML2::IXMLDOMDocumentPtr& DocPtr
   //             MSXML2::IXMLDOMElementPtr& pNode
   //             const TCHAR* string
   //             double val
   //
   // RETURN: inline void 
   //
   // SIDE EFFECTS: 
   //
   ////////////////////////////////////////////////
   inline void SetData(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pNode,const TCHAR* string,double val) 
   {
      CString str;
      str.Format("%f",val);
      pNode->setAttribute(string,(const TCHAR*)str);
   }



   ////////////////////////////////////////////////
   //
   // NAME: SetData
   //
   // DESCRIPTION: sets an string in an XML file
   //
   // CREATED:     1/16/2003 5:11:00 PM
   //
   // PARAMETERS: 
   //             MSXML2::IXMLDOMDocumentPtr& DocPtr
   //             MSXML2::IXMLDOMElementPtr& pNode
   //             const TCHAR* string
   //             const TCHAR* val
   //
   // RETURN: inline void 
   //
   // SIDE EFFECTS: 
   //
   ////////////////////////////////////////////////
   inline void SetData(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pNode,const TCHAR* string,const TCHAR* val) 
   {
      pNode->setAttribute(string,val);
   }



   ////////////////////////////////////////////////
   //
   // NAME: SetData
   //
   // DESCRIPTION: sets an int in an XML file
   //
   // CREATED:     1/16/2003 5:11:08 PM
   //
   // PARAMETERS: 
   //             MSXML2::IXMLDOMDocumentPtr& DocPtr
   //             MSXML2::IXMLDOMElementPtr& pNode
   //             const TCHAR* string
   //             DWORD val
   //
   // RETURN: inline void 
   //
   // SIDE EFFECTS: 
   //
   ////////////////////////////////////////////////
   inline void SetData(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pNode,const TCHAR* string, DWORD val) 
   {
      pNode->setAttribute(string,(long)val);
   }

   inline void SetData(MSXML2::IXMLDOMDocumentPtr& DocPtr,MSXML2::IXMLDOMElementPtr& pNode,const TCHAR* string, int val) 
   {
      pNode->setAttribute(string,(long)val);
   }

};


#endif // __XMLHELPERS_H__