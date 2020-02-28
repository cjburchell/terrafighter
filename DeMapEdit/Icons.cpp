#include "stdafx.h"
#include "icons.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#import "msxml3.dll"
using namespace MSXML2;

//#define MAXEDICONS 80

CString CIcons::GetItemText(unsigned char value)
{
    int index = value;
    CTile Item;
    if(m_Tiles.Lookup(index,Item))
    {
        return Item.m_Text;
    }
    else
        return "";
}

CImageList* CIcons::CreateImageList()
{
    CImageList* pImageList = new CImageList;
    pImageList->Create(32,32,ILC_COLOR32,0,256);
    for(int i=0; i<m_NumbIcon;i++)
    {
        pImageList->Add(&m_pBitmap[i],RGB(255,0,255));
    }
    return pImageList;
}

BOOL CIcons::SaveXML()
{
    try
    {
        IXMLDOMDocumentPtr docPtr;
        // create the xml document
        if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
            return FALSE;

        _variant_t varXml("Tiles.xml");
        _variant_t varOut((bool)TRUE);

        IXMLDOMElementPtr Root = docPtr->createElement("TileList");
        docPtr->appendChild(Root);

        long count = 0;
        for(int i=0;i<256;i++)
        {
            CString Text = GetItemText(i);
            if(!Text.IsEmpty())
            {
                IXMLDOMElementPtr ElementPtr = docPtr->createElement("Tile");
                ElementPtr->setAttribute("text",(LPCSTR)Text);
                ElementPtr->setAttribute("index",(long)i);
                ElementPtr->setAttribute("icon",(long)GetImage(i,0));
                Root->appendChild(ElementPtr);
                count++;
            }
        }

        Root->setAttribute("ElementCount",count);
        docPtr->save(varXml); // load the xml document
    }
    catch(...)
    {
        TRACE("Exception occurred CDropDownObjCtrl::Refresh");
        return FALSE;
    }
    return TRUE;
}

BOOL CIcons::LoadXML()
{
    try
    {
        IXMLDOMDocumentPtr docPtr;
        // create the xml document
        if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
            return FALSE;

        _variant_t varXml("Tiles.xml");
        _variant_t varOut((bool)TRUE);

        varOut = docPtr->load(varXml); // load the xml document

        m_Tiles.RemoveAll();

        if((bool)varOut == TRUE)
        {
            IXMLDOMNodePtr ElementCount_attb = docPtr->documentElement->attributes->getNamedItem("ElementCount");
            int ElementCount = atoi(ElementCount_attb->text);

            IXMLDOMNodeListPtr RowListPtr = docPtr->documentElement->selectNodes("Tile");
            ElementCount = RowListPtr->length;

            if(ElementCount > 0) // if there are any rows
            {
                for(int i=0;i<ElementCount;i++) // for each row
                {
                    // get the row
                    IXMLDOMNodePtr RowItem = RowListPtr->item[i];

                    IXMLDOMNodePtr text_attb = RowItem->attributes->getNamedItem("text");
                    CString text = CString((char*)text_attb->text);
                    IXMLDOMNodePtr index_attb = RowItem->attributes->getNamedItem("index");
                    int index = atoi(index_attb->text);
                    IXMLDOMNodePtr icon_attb = RowItem->attributes->getNamedItem("icon");
                    int icon = atoi(icon_attb->text);

                    m_Tiles[index].m_index = index;
                    m_Tiles[index].m_icon = icon;
                    m_Tiles[index].m_Text = text;

                }
            }
        }
    }
    catch(...)
    {
        TRACE("Exception occurred CDropDownObjCtrl::Refresh");
        return FALSE;
    }
    return TRUE;
}


BOOL CIcons::LoadFileList()
{
    try
    {
        IXMLDOMDocumentPtr docPtr;
        // create the xml document
        if( FAILED(docPtr.CreateInstance("msxml2.domdocument")))
            return FALSE;

        _variant_t varXml("Icons.xml");
        _variant_t varOut((bool)TRUE);

        varOut = docPtr->load(varXml); // load the xml document

        if((bool)varOut == TRUE)
        {
            IXMLDOMNodePtr ElementCount_attb = docPtr->documentElement->attributes->getNamedItem("IconNumber");
            m_NumbIcon = atoi(ElementCount_attb->text);

            IXMLDOMNodeListPtr RowListPtr = docPtr->documentElement->selectNodes("Icon");
            m_NumbIcon = RowListPtr->length;
            if(m_FileList)
                delete [] m_FileList;

            m_FileList = new CString[m_NumbIcon];

            if(m_NumbIcon > 0) // if there are any rows
            {
                for(int i=0;i<m_NumbIcon;i++) // for each row
                {
                    // get the row
                    IXMLDOMNodePtr RowItem = RowListPtr->item[i];

                    IXMLDOMNodePtr text_attb = RowItem->attributes->getNamedItem("FileName");
                    CString text = CString((char*)text_attb->text);
                    IXMLDOMNodePtr index_attb = RowItem->attributes->getNamedItem("Index");
                    int index = atoi(index_attb->text);
                 
                    m_FileList[index] = text;
                }
            }
        }
    }
    catch(...)
    {
        TRACE("Exception occurred CDropDownObjCtrl::Refresh");
        return FALSE;
    }
    return TRUE;
}


int CIcons::GetImage(unsigned char value,char type)
{
    int retval;
    int index = value;
    CTile Item;
    if(m_Tiles.Lookup(index,Item))
    {
        retval = Item.m_icon;
    }
    else
    {
        switch(type)
		{
			case 119:retval=1;break;
			case 42: retval=4;break;
			case 102:retval=9;break;
			case 116:retval=5;break;
			case 'D':retval=24;break;
			case 'i':retval=29;break;
			case '@':retval=45;break;
			default:retval=0;break;
		}
    }
	return retval;
}

BOOL CIcons::Create()
{
    if(!LoadFileList())
        return FALSE;
	
    if(m_FileList)
    {
        if(m_pBitmap)
            delete [] m_pBitmap;
        m_pBitmap = new CBitmap[m_NumbIcon];
	    for(int pic=0;pic<m_NumbIcon;pic++)
        {
	        
            HBITMAP hbitmap = (HBITMAP)LoadImage(::AfxGetInstanceHandle(),m_FileList[pic],IMAGE_BITMAP,ICONSIZE,ICONSIZE,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	        // normal tree images
            if(hbitmap)
            {
                m_pBitmap[pic].Attach(hbitmap);
            }
        }
    }

    if(!LoadXML())
        return FALSE;

    return TRUE;
}

BOOL CIcons::DrawIcon(int icon, CDC* pDC, CPoint pos)
{
   if(m_pBitmap[icon].GetSafeHandle())
   {
        BITMAP bmpInfo;
        m_pBitmap[icon].GetBitmap(&bmpInfo);

        CDC dcMemory;
        dcMemory.CreateCompatibleDC(pDC);

        // Select the bitmap into the in-memory DC
        CBitmap* pOldBitmap = dcMemory.SelectObject(&m_pBitmap[icon]);
       
        pDC->BitBlt(pos.x, pos.y, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 
        0, 0, SRCCOPY);

        dcMemory.SelectObject(pOldBitmap);
    } 
    return TRUE;
}


CIcons::CIcons()
{
    m_FileList = NULL;
    m_pBitmap = NULL;
}

CIcons::~CIcons()
{
    if(m_FileList)
        delete [] m_FileList;

    if(m_pBitmap)
        delete [] m_pBitmap;
}
