#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <afxwin.h>
#include "icons.h"
#include "dxutil.h"
#include "types.h"

#import "msxml3.dll"
using namespace MSXML2;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void CIcons::draw_player(C2DDraw* pDisplay,int numb,int posx,int posy)
{
   CRect rect(0,0,ICONSIZE,ICONSIZE);
   if(m_pPlayerIcons[numb])
      m_pPlayerIcons[numb]->Draw(pDisplay->GetTempDevice(),posx,posy,&rect);
}

/*void CIcons::putic(CSurface* Surface,int numb,int posx,int posy,int w, int h,int offx,int offy)
{
	CPoint point(posx,posy);
	CPoint rectPoint(numb<<ICONSIZEB,0);
	CSize  size(w,h);
	CRect rect(rectPoint,size);
	rect.OffsetRect(offx,offy);
	if(m_pIcons)
		Surface->Blt(point.x,point.y,m_pIcons,&rect);
}*/

void CIcons::draw(C2DDraw* pDisplay,int numb,int posx,int posy,int w, int h,int offx,int offy)
{
	// clip to the display
   int scrw = pDisplay->GetScreenWidth();
   int scrh = pDisplay->GetScreenHight();

	
	if(posx < 0)
	{
		w = w + posx;
		offx = -posx;
		posx = 0;
	}
	else if(posx+w > scrw)
	{
		w = scrw - posx;
	}

	if(posy < 0)
	{
		h = h + posy;
		offy = -posy;
		posy = 0;
	}
	else if(posy+h > scrh)
	{
		h = scrh - posy;
	}

	CPoint point(posx,posy);
	CPoint rectPoint(0,0);
	CSize  size(w,h);
	CRect rect(rectPoint,size);
	rect.OffsetRect(offx,offy);

   if(m_pIcons[numb])
      m_pIcons[numb]->Draw(pDisplay->GetTempDevice(),point.x,point.y,&rect);
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


int CIcons::load(IDirect3DDevice9* device,CString filename,int numb)
{
   if(m_pIcons[numb] != NULL)
      delete m_pIcons[numb];
   m_pIcons[numb] = new CSurface();
	if(FAILED(m_pIcons[numb]->Init(filename,ICONSIZE,ICONSIZE,device,NULL)))
	{
		sys_msg("can't make IconSurface");
		return FALSE;
	}
	if(numb>=PLAYEROFFSET && m_pIcons[numb])
	{
		m_pPlayerIcons[numb-PLAYEROFFSET] = m_pIcons[numb];
		m_pPlayerIcons[numb-PLAYEROFFSET]->SetColorKey(RGB(255,0,255));
	}

	return TRUE;
}
CIcons::CIcons(IDirect3DDevice9* device)
{
    m_FileList = NULL;
    LoadFileList();
													
	int pic;
	for(int i=0;i<NUMBICON;i++)
	{
		m_pIcons[i]=NULL;
	}
	for(i=0;i<NUMPLAYERICONS;i++)
	{
		m_pPlayerIcons[i] = NULL;
	}

	for(pic=0;pic<NUMBICON;pic++)
		load(device, m_FileList[pic], pic);
	
}
CIcons::~CIcons()
{
    if(m_FileList)
        delete [] m_FileList;
	for(int i=0;i<NUMBICON;i++)
	{
		SAFE_DELETE(m_pIcons[i]);
	}
}
