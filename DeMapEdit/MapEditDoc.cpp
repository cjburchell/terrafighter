// MapEditDoc.cpp : implementation of the CMapEditDoc class
//

#include "stdafx.h"
#include "MapEdit.h"
#include "Door.h"
#include "Chest.h"
#include "Monster.h"
#include "Murchent.h"
#include "Saver.h"
#include "KeySales.h"
#include "NPC.h"
#include "EndMonster.h"
#include "FireMonst.h"
#include "LightningMonst.h"
#include "Projectile.h"
#include "Healer.h"

#include "MapEditDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc

IMPLEMENT_DYNCREATE(CMapEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapEditDoc, CDocument)
	//{{AFX_MSG_MAP(CMapEditDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc construction/destruction

CMapEditDoc::CMapEditDoc()
{
    m_Icons.Create();
    for(int x=0;x<MAPX;x++)
	{
	  for(int y=0;y<MAPY;y++)
	  {
		m_pMap[y][x] = NULL;
	  }
	}
}

CMapEditDoc::~CMapEditDoc()
{
    DeleteMapObj();
}

BOOL CMapEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

    for(int x=0;x<MAPX;x++)
	{
	  for(int y=0;y<MAPY;y++)
	  {
		m_pMap[y][x] = NULL;
	  }
	}

    m_Size.cx = 0;
    m_Size.cy = 0;
    m_startpos = CPoint(0,0);
    m_Type = 0;
    m_ObjCount = CSize(0,0);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc serialization

void CMapEditDoc::Serialize(CArchive& ar)
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
// CMapEditDoc diagnostics

#ifdef _DEBUG
void CMapEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMapEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc commands

#define ICONSIZEB 5

BOOL CMapEditDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;


   FILE* mapfile;
   int count1;
   if((mapfile=fopen(lpszPathName,"r"))!=NULL)
   {
	    int sizey;
		int sizex;
	    fscanf(mapfile,"%d %d %d\n",&sizey,&sizex,&m_Type);
		m_Size.cx = sizex<<ICONSIZEB;
		m_Size.cy = sizey<<ICONSIZEB;
		fscanf(mapfile,"%d %d\n",&m_startpos.x,&m_startpos.y);
		m_startpos.x = m_startpos.x<<ICONSIZEB;
		m_startpos.y = m_startpos.y<<ICONSIZEB;

		for(int temp=0;temp<9;temp++)
			fscanf(mapfile,"%d %d %d\n",&inout[temp].x,&inout[temp].y,&inout[temp].map);

		char bkgnd[MAPX][MAPY];
		memset(bkgnd,m_Type,sizeof(bkgnd));
        for(count1=0;count1<sizey;count1++)
        {
            fgets(bkgnd[count1],sizex+1,mapfile);
            fgetc(mapfile);
        }
      fclose(mapfile);
	  DeleteMapObj();

      m_ObjCount = CSize(sizex,sizey);

	  for(int x=0;x<sizex;x++)
	  {
		  for(int y=0;y<sizey;y++)
		  {
			m_pMap[y][x] = new CMapObject(bkgnd[y][x]);
            m_pMap[y][x]->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB),m_Icons.GetImage(bkgnd[y][x],m_Type),m_Icons.GetItemText(bkgnd[y][x]));
			switch(bkgnd[y][x])
			{
			case 'd':
				{
					CDoor* pDoor = new CDoor();
					pDoor->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB),CSize(32,32));
					m_MapObj.AddHead(pDoor);
				}
				break;
			case '0':
				{
					CChest* pChest = new CChest();
					pChest->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB),CSize(32,32),TRUE);
					m_MapObj.AddHead(pChest);
				}
				break;
			case 'G':
				{
					CChest* pChest = new CChest();
					pChest->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB),CSize(32,32));
					m_MapObj.AddHead(pChest);
				}
				break;
			case 'k':
				{
					CChest* pChest = new CChest();
					pChest->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB),CSize(32,32),FALSE,TRUE);
					m_MapObj.AddHead(pChest);
				}
				break;
			}
		  }
	    }
        CString filename = lpszPathName;
        filename = filename.Right(filename.GetLength()-filename.ReverseFind('\\')-1);
        filename = filename.Right(filename.GetLength()-filename.ReverseFind('p')-1);
        LoadMonsters(atoi(filename));
   }
   else
      return FALSE;
	
	return TRUE;
}

BOOL CMapEditDoc::LoadMonsters(int set)
{
   char files[20];
   sprintf(files,"monstset%d.dat",set);
   char bmp_file[20];
   int count;
   int hp1 = 0;
   int constpoint = 0;
   char line[201];
   int ha;
   UINT xp;
   UINT gold;
   int size;
   CPoint pos;
   FILE* data;
   TYPE_NPC npctype;
   int movement; 
   char tmpname[500];
   if((data=fopen(files,"r"))!=NULL)
   {
      for(count=0;count<MAX_RANDOM_MONST;count++)
      {
		  if(fgets(line,200,data)==NULL)
		  {
			  fclose(data);
			  return TRUE;
		  }

		
		 sscanf(line,"%d %d %d %U %s %s %U %d %d %d %d %d",
			 &hp1,
			 &constpoint,
			 &ha,
			 &xp,
			 bmp_file,
			 tmpname,
			 &gold,
			 &size,
			 &pos.x,
			 &pos.y,
			 &movement,
			 &npctype
			 );
		 pos.x <<=ICONSIZEB;
		 pos.y <<=ICONSIZEB;
         CMonster* pMonster;
	      switch(npctype)
	      { 
		    case HEAL:
			    pMonster = new CHealer();
			    break;
		    case MURCH:
			    pMonster = new CMurchent();
			    break;
		    case SAVE:
			    pMonster = new CSaver();
			    break;
		    case KEY:
			    pMonster = new CKeySales();
			    break;
		    case NPC:
			    pMonster = new CNPC();
			    break;
		    case ENDMON:
			    pMonster = new CEndMonster();
			    break;
		    case FIRE:
			    pMonster = new CFireMonst();
			    break;
		    case LIT:
			    pMonster = new CLightningMonst();
			    break;
		    case PRO:
			    pMonster = new CProjectile(NULL);
			    break;
		    default:
			    pMonster = new CMonster();
			    break;
	      }
	      CSize s(size<<ICONSIZEB,size<<ICONSIZEB);
	      pMonster->Create(
		         hp1,
			     constpoint,
			     ha,
			     xp,
			     bmp_file,
			     tmpname,
			     gold,
			     s,
			     pos,
			     movement,
			     npctype);
          m_pMonst.AddHead(pMonster);
      }
      fclose(data);
	  return TRUE;	  
   }
   return FALSE;
}


void CMapEditDoc::DeleteMapObj()
{
    CDEStatic* pObj;
	while(m_MapObj.GetCount() != 0)
	{
		pObj = m_MapObj.RemoveHead();
		if(pObj)
            delete pObj;
	}

    CMonster* pMonster;
	while(m_pMonst.GetCount() != 0)
	{
		pMonster = m_pMonst.RemoveHead();
        if(pMonster)
		    delete pMonster;
	}

	for(int x=0;x<MAPX;x++)
	{
	  for(int y=0;y<MAPY;y++)
	  {
		if(m_pMap[y][x])
			delete m_pMap[y][x];
		m_pMap[y][x] = NULL;
	  }
	}
}

BOOL CMapEditDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
   FILE* mapfile;
   int y,x;
   if((mapfile=fopen(lpszPathName,"w"))!=NULL)
   {
	  CSize msize = GetSize();
      msize.cx = msize.cx/32;
      msize.cy = msize.cy/32;
      fprintf(mapfile,"%d %d %d\n",msize.cy,msize.cx,m_Type);
	  fprintf(mapfile,"%d %d\n",m_startpos.x>>ICONSIZEB,m_startpos.y>>ICONSIZEB);
	  for(int temp=0;temp<9;temp++)
		fprintf(mapfile,"%d %d %d\n",inout[temp].x,inout[temp].y,inout[temp].map);
      for(y=0;y<msize.cy;y++)
      {
         for(x=0;x<msize.cx;x++)
		 {
			if(m_pMap[y][x])
				fprintf(mapfile,"%c", m_pMap[y][x]->GetMapType(TRUE));
			else
				fprintf(mapfile," ");
		 }
         fprintf(mapfile,"\n");
      }
      fclose(mapfile);
   }
   else
   {
      CString msg;
	  msg.Format("Could not open file %s",lpszPathName);
	  AfxMessageBox(msg);
      return FALSE;
   }
	
	return TRUE;//CDocument::OnSaveDocument(lpszPathName);
}
