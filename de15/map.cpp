

#include "map.h"
#include <dxutil.h>
#include <d3dutil.h>
#include "DEApp.h"
#include <mmsystem.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <io.h>
//#include <afxwin.h>
//#include "dxutil.h"
//#include "types.h"
//#include "icons.h"
//#include "mouse.h"
//#include "items.h"
//#include <math.h>
//#include "SayDlg.h"
#include "Door.h"
#include "Chest.h"

#define FADE_TIME 500



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SIZING CPoint iconpos;\
   iconpos.x = m_mappos.x/ICONSIZE;\
   iconpos.y = m_mappos.y/ICONSIZE;\
   int xoffset = m_mappos.x%ICONSIZE;\
   if(m_mappos.x < 0)\
{\
   xoffset = ICONSIZE+xoffset;\
   iconpos.x --;\
}\
   int yoffset = m_mappos.y%ICONSIZE;\
   if(m_mappos.y < 0)\
{\
   yoffset = ICONSIZE+yoffset;\
   iconpos.y --;\
}\


void CGameMap::draw(C2DDraw* pDirDraw, CCursor* pCursor, CPlayer* player, CMonsters* monsters)
{
   if(m_bMapLoaded)
   {
      SIZING
         int starty = iconpos.y;

      int hight = pDirDraw->GetScreenHight();
      int width =pDirDraw->GetScreenWidth();

      CSize size = GetMapSize();

      for(int xpos = -xoffset;xpos<width;xpos+=ICONSIZE)
      {
         for(int ypos= -yoffset;ypos<hight;ypos+=ICONSIZE)
         {        
            if(iconpos.x<size.cx && iconpos.x>0 && iconpos.y<size.cy && iconpos.y>0)
            {
               if(m_pMap[iconpos.y][iconpos.x])
                  m_pMap[iconpos.y][iconpos.x]->Display(this,pDirDraw,pCursor);
            }
            iconpos.y++;
         }
         iconpos.x++;
         iconpos.y = starty;
      }

      // for each monster thats alive
      for(std::list <CDEStatic*>::iterator i = m_MapObj.begin(); i != m_MapObj.end(); i++)
      {
         CDEStatic* pObj = *i;
         if(pObj)
         {
            pObj->Display(this, pDirDraw, pCursor);
         }
      }

      m_icons.draw_player(pDirDraw,player->GetIcon(),m_playerpos.x,m_playerpos.y);
      player->DisplayPath(this,pDirDraw);
      monsters->display(this,pDirDraw,pCursor);
#ifdef _DEBUG
      deselect();
#endif
   }
   else
   {
      CRect rect(0,0,pDirDraw->GetScreenWidth(),pDirDraw->GetScreenHight());
      pDirDraw->Blt(0,0,&m_Startbmp,&rect);
   }
}

int CGameMap::load(CString map,IDirect3DDevice9* device, CPlayer* player)
{
   FILE* mapfile;
   int count1;
   if((mapfile=fopen(map,"r"))!=NULL)
   {
      int sizey;
      int sizex;
      fscanf(mapfile,"%d %d %d\n",&sizey,&sizex,&type);
      m_Size.cx = sizex<<ICONSIZEB;
      m_Size.cy = sizey<<ICONSIZEB;
      fscanf(mapfile,"%d %d\n",&m_startpos.x,&m_startpos.y);
      m_startpos.x = m_startpos.x<<ICONSIZEB;
      m_startpos.y = m_startpos.y<<ICONSIZEB;

      for(int temp=0;temp<9;temp++)
         fscanf(mapfile,"%d %d %d\n",&inout[temp][xq],&inout[temp][yq],&inout[temp][2]);

      char bkgnd[MAPX][MAPY];
      memset(bkgnd,type,sizeof(bkgnd));
      for(count1=0;count1<sizey;count1++)
      {
         fgets(bkgnd[count1],sizex+1,mapfile);
         fgetc(mapfile);
      }
      fclose(mapfile);
      DeleteMapObj();

      for(int x=0;x<sizex;x++)
      {
         for(int y=0;y<sizey;y++)
         {
            m_pMap[y][x] = new CMapObject(bkgnd[y][x]);
            m_pMap[y][x]->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB), &m_icons,device,mapitem(bkgnd[y][x]));
            switch(bkgnd[y][x])
            {
            case 'd':
               {
                  CDoor* pDoor = new CDoor();
                  pDoor->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB),CSize(32,32),device);
                  m_MapObj.push_back(pDoor);
               }
               break;
            case '0':
               {
                  CChest* pChest = new CChest();
                  pChest->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB),CSize(32,32),player->level,device,TRUE);
                  m_MapObj.push_back(pChest);
               }
               break;
            case 'G':
               {
                  CChest* pChest = new CChest();
                  pChest->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB),CSize(32,32),player->level,device);
                  m_MapObj.push_back(pChest);
               }
               break;
            case 'k':
               {
                  CChest* pChest = new CChest();
                  pChest->Create(CPoint(x<<ICONSIZEB,y<<ICONSIZEB),CSize(32,32),player->level,device,FALSE,TRUE);
                  m_MapObj.push_back(pChest);
               }
               break;
            }
         }
      }
      player->StopSeekingMonster();

      m_bMapLoaded = TRUE;
   }
   else
      return(0);
   return(1);
}

void CGameMap::savemap(CString map)
{
   FILE* mapfile;
   int y,x;
   if((mapfile=fopen(map,"w"))!=NULL)
   {
      CSize msize = GetMapSize();
      fprintf(mapfile,"%d %d %d\n",msize.cy,msize.cx,type);
      fprintf(mapfile,"%d %d\n",m_startpos.x>>ICONSIZEB,m_startpos.y>>ICONSIZEB);
      for(int temp=0;temp<9;temp++)
         fprintf(mapfile,"%d %d %d\n",inout[temp][xq],inout[temp][yq],inout[temp][2]);
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
      sys_printf(__LOCATION__, 0, "could not open file %s",map);
   }
}

int CGameMap::mapitem(unsigned char value)
{
   int retval;
   switch(value)
   {
   case '.':
   case ' ':retval=0;break;
   case 'z':
   case 'w':retval=1;break;
   case 'm':retval=2;break;
   case '^':retval=3;break;
   case '*':retval=4;break;
   case 't':retval=5;break;
   case 'G':
   case '0':
   case 'k':
   case 'F':retval=6;break;
   case 'd':
   case 'b':retval=7;break;
   case 'g':retval=8;break;
   case 'f':retval=9;break;
   case '#':retval=10;break;
      //case 'G':
      //case 'k':retval=11;break;
      //case '0':retval=12;break;
   case 'B':retval=13;break;
   case '_':
   case '=':retval=14;break;
   case ':':
   case '|':retval=15;break;
   case 'I':
   case 'J':
   case 'K':
   case 'L':
   case 'M':
   case 'N':
   case 'O':retval=16;break;
   case 'P':
   case 'Q':
   case 'R':
   case 'S':retval=17;break;
   case 'T':
   case 'U':
   case 'V':
   case 'W':
   case 'X':
   case 'Y':
   case 'Z':retval=18;break;
      //case 'd':retval=19;break;
   case 'C':retval=20;break;
   case '1':
   case '2':
   case '3':
   case '4':retval=21;break;
   case '5':
   case '6':
   case '7':
   case '8':retval=22;break;
   case '9':retval=23;break;
   case 'D':retval=24;break;
   case '~':retval=25;break;
   case 'l':retval=26;break;
   case '-':retval=27;break;
   case '+':retval=28;break;
   case 'i':retval=29;break;
   case 'p': retval=30;break;
   case 'q': retval=31;break;
   case 'j': retval=32;break;
   case 's': retval=33;break;
   case 'n': retval=34;break;
   case '!': retval=35;break;
   case '"': retval=36;break;
   case '$': retval=37;break;
   case '%': retval=38;break;
   case '&': retval=39;break;
   case '`': retval=40;break;
   case ']': retval=41;break;
   case '[': retval=42;break;
   case '{': retval=43;break;
   case '}': retval=44;break;
   case '@': retval=45;break;
   case 128: 
   case 129:
   case 130:
   case 131:
   case 132:
   case 133:retval=46;break;
   case '<':retval=47;break;
   default:
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
      break;
   }
   return(retval);
}



CGameMap::CGameMap(IDirect3DDevice9* device, DWORD width, DWORD hight) : m_icons(device)
{
   m_bMapLoaded = FALSE;
   m_systime = timeGetTime();
   page=0;
   m_icons=NULL;
   refnum=0;
   edchar='.';


   SCREEN_TW = width >> ICONSIZE;
   SCREEN_TH = hight >> ICONSIZE;


   for(int x=0;x<MAPX;x++)
   {
      for(int y=0;y<MAPY;y++)
      {
         m_pMap[y][x] = NULL;
      }
   }


   m_Startbmp.Init("start1.bmp" ,width, hight, device, NULL);
}

CGameMap::~CGameMap()
{
   char file[20];

   for(int i=0;i<MAXMAPS;i++)
   {
      sprintf(file,"temp%d.map",i);
      remove(file);
      sprintf(file,"temp%d.fog",i);
      remove(file);
   }

   DeleteMapObj();
}



void CGameMap::gotopos(CPlayer* player, UINT xpos, UINT ypos)
{
   player->GotoPos(CPoint(xpos,ypos));
   //int offsety = pDirDraw->GetScreenHight()/2;
   //int offsetx = pDirDraw->GetScreenWidth()/2;
   //SetMapPos(xpos-offsetx,ypos-offsety,offsetx,offsety);
}

void CGameMap::SetMapVal(CPoint pos, char val)
{
   pos.y = pos.y>>ICONSIZEB;
   pos.x = pos.x>>ICONSIZEB;
   if(m_pMap[pos.y][pos.x])
      m_pMap[pos.y][pos.x]->SetType(val, &m_icons,mapitem(val));
}

/*void CGameMap::deselect(C2DDraw* pDirDraw )
{
static char saytable[MAXEDICONS][30]={
"Grass 1",
"grass 2",
"Water",
"mountan",
"swamp",
"desert",
"trees",
"Flore",
"brick path",
"grey brick path",
"Snow",
"Ship",
"Chest",
"Key in chest",
"open chest",
"Bridge",
"wall 1 hidden",
"wall 1 solid",
"wall 2 hidden",
"wall 2 solid",
"Town 6",
"Town 7",
"Town 8",
"Town 9",
"Town 10",
"Town 11",
"Town 12",
"shrine 13",
"shrine 14",
"shrine 15",
"shrine 16",
"cave 17",
"cave 18",
"cave 19",
"cave 20",
"cave 21",
"cave 22",
"cave 23",
"door",
"colum",
"up 1",
"up 2",
"up 3",
"up 4",
"down 5",
"down 6",
"down 7",
"down 8",
"warp 9",
"default exit from map",
"dark",
"water exit",
"ice wall1",
"ice wall2",
"yellow wall1",
"yellow wall2",
"snow flore",
"invisable wall",
"cave wall 1",
"cave wall 2",
"cave wall 3",
"cave wall 4",
"cave wall 5"
"cave wall 6",
"cave wall 7",
"cave wall 8",
"cave wall 9",
"cave wall 10",
"cave wall 11",
"cave wall 12",
"cave wall 13",
"cave wall 14",
"cave wall 15",
"sky",
"tower 24",
"tower 25",
"tower 26",
"tower 27",
"tower 28",
"tower 29",
"starts"};

static FLOAT fFPS      = 0.0f;
static FLOAT fLastTime = 0.0f;
static DWORD dwFrames  = 0L;

// Keep track of the time lapse and frame count
FLOAT fTime = timeGetTime() * 0.001f; // Get current time in seconds
++dwFrames;

// Update the frame rate once per second
if( fTime - fLastTime > 1.0f )
{
fFPS      = dwFrames / (fTime - fLastTime);
fLastTime = fTime;
dwFrames  = 0L;
}

SIZING

CRect r(2*FONT_SIZE_X,1*FONT_SIZE_Y,30*FONT_SIZE_X,7*FONT_SIZE_Y);
pDirDraw->DrawFilledRect(r,RGB(192,192,192));
//pDirDraw->outtext(2,1,"                       ");
//pDirDraw->outtext(2,2,"               x %3d   ",player->GetCenterPoint().x);
//pDirDraw->OutText(2,3,"               y %3d   ",player->GetCenterPoint().y);
//pDirDraw->outtext(2,4,"            %7.02f fps",fFPS);
//pDirDraw->outtext(2,5,"          Angle %7.02f",ToDeg(player->GetAngle()));  
}*/


void CGameMap::load_new_map(CPlayer* player, int map, IDirect3DDevice9* device)
{
   CString filename;
   CString fogmap;
   filename.Format("temp%d.map\0",player->map);
   fogmap.Format("temp%d.fog\0",player->map);
   if((player->map==0)||(player->map>5))
   {
      savemap(filename);
      SaveFogMap(fogmap);
   }

   player->map = map;
   filename.Format("temp%d.map\0",map);
   if(_access(filename,0)== -1)
   {
      filename.Format("%s%d.map\0",player->name,map);
      fogmap.Format("%s%d.fog\0",player->name,map);
   }
   else
      fogmap.Format("temp%d.fog\0",map);


   if(load(filename,device,player)==0)
   {
      filename.Format("map%d.dat\0",map);
      load(filename,device,player);
   }
   else
   {
      LoadFogMap(fogmap);
   }
}

BOOL CGameMap::chkpos(CPlayer* player, CMonsters* monsters, IDirect3DDevice9* device)
{
   int bMoved=TRUE;
   int newmap;
   CPoint tmppos(player->GetCenterPoint());
   unsigned char choice=GetBkgrdItem(tmppos);
   switch(choice)
   {
   case 'I':
   case 'J':
   case 'K':
   case 'L':
   case 'M':
   case 'N':
   case 'O':
   case 'P':
   case 'Q':
   case 'R':
   case 'S':
   case 'T':
   case 'U':
   case 'V':
   case 'W':
   case 'X':
   case 'Y':
   case 'Z':
      if(choice != GetBkgrdItem(player->GetLastCenterPoint()))
      {
         FadeOut(FADE_TIME);
         if(player->ship==SHIP_IN)
            ship(tmppos);
         monsters->end();
         player->SetSavedPos();
         load_new_map(player,choice-'I'+6,device);
         gotopos(player,m_startpos.x,m_startpos.y);
         monsters->start(player,device,choice-'I'+6,NUMB_TOWN_NPC,FALSE);
         player->monster=TRUE;
         player->RandMonstOn=FALSE;
         ClearFog(player->GetCenterPoint());
         FadeIn(FADE_TIME);
      }
      break;
   case 128: 
   case 129:
   case 130:
   case 131:
   case 132:
   case 133:
      if(choice != GetBkgrdItem(player->GetLastCenterPoint()))
      {
         FadeOut(FADE_TIME);
         if(player->ship==SHIP_IN)
            ship(tmppos);
         monsters->end();
         player->SetSavedPos();
         load_new_map(player,(choice-128)+24,device);
         gotopos(player,m_startpos.x,m_startpos.y);
         monsters->start(player,device,choice-128+24,NUMB_TOWN_NPC,FALSE);
         player->monster=TRUE;
         player->RandMonstOn=FALSE;
         ClearFog(player->GetCenterPoint());
         FadeIn(FADE_TIME);
      }
      break;
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      if(choice != GetBkgrdItem(player->GetLastCenterPoint()))
      {
         FadeOut(FADE_TIME);
         if(player->ship==SHIP_IN)
            ship(tmppos);
         monsters->end();
         if(inout[choice - '1'][xq]==0 && inout[choice - '1'][yq]==0)
            gotopos(player, player->m_savepos.x,player->m_savepos.y);
         else
            gotopos(player, inout[choice - '1'][xq]<<ICONSIZEB,inout[choice - '1'][yq]<<ICONSIZEB);
         newmap = inout[choice - '1'][2];
         load_new_map(player, inout[choice - '1'][2],device);
         if(newmap!=0)
         {  
            monsters->start(player,device,newmap,NUMB_TOWN_NPC,FALSE);
            player->monster=TRUE;
            player->RandMonstOn=FALSE;
            //device->ResetPal();
         }
         else
         {
            player->RandMonstOn=TRUE;
            player->monster=FALSE;
         }
         ClearFog(player->GetCenterPoint());
         FadeIn(FADE_TIME);
      }
      break;
   case 'z':
      player->m_savepos.x+=ICONSIZE;
   case 'y':
      outside();
      break;
   case '#':ship(tmppos);break;
   case 0:
   case 10:
   case 'w':
      if(player->ship==SHIP_IN)
      {
         bMoved=TRUE;
      }
      else
      {
         player->MoveBack();
      }
      break;
   case '<':
   case ':':
   case '_':
   case ',':
   case 'H':
   case 'F':
   case 't':
   case 'k':
   case 'b':
   case 'g':
   case 'G':
   case '0':
   case '*':
   case 'f':
   case 'B':
   case ' ':
   case '.':
   case 'i':
      if(player->ship==SHIP_OUT)
      {
         bMoved=TRUE;
      }
      else
         ship(tmppos);
      break;
   default:
      player->MoveBack();
      break;
   }

   if(player->monster==TRUE && bMoved)
   { 
      if((monsters->checkpos(player,device,this))==1)
      {     
         if(monsters->GetNumb()==0 && m_MapObj.size() == 0 && player->RandMonstOn==TRUE)
            endfight();
      }
      else
         bMoved=FALSE;
   }

   return(bMoved);
}
void CGameMap::outside(CPlayer* player, CMonsters* monsters)
{
   FadeOut(FADE_TIME);
   load_new_map(0,pDirDraw);
   gotopos(player->m_savepos.x,player->m_savepos.y);
   if(player->monster==TRUE)
      monsters->end();
   player->RandMonstOn=TRUE;
   player->monster=FALSE;
   ClearFog(player->GetCenterPoint());
   FadeIn(FADE_TIME);
}

void CGameMap::ship(CPlayer* player, CPoint currentpoint)
{
   if(player->ship==SHIP_OUT)
   {
      player->ship=SHIP_IN;
      SetMapVal(currentpoint,'w');
      player->SetIcon(8); 
   }
   else
   {
      player->ship=SHIP_OUT;
      SetMapVal(player->GetLastCenterPoint(),'#');
      player->SetIcon(6);
   }
}

void CGameMap::startfight(CPlayer* player, CMonsters* monsters)
{
   FadeOut(FADE_TIME);
   UINT type;
   switch(mapitem(GetBkgrdItem(player->GetCenterPoint())))
   {
   case 0:type=1;break;
   case 1:type=2;break;
   case 4:type=3;break;
   case 5:type=5;break;
   case 9:type=4;break;
   default:type=5;break;
   }
   player->SetSavedPos();
   load_new_map(type,pDirDraw);
   gotopos(((rand()%19)+1)<<ICONSIZEB,((rand()%14)+1)<<ICONSIZEB);
   player->monster=TRUE;
   monsters->start(player,pDirDraw,type,0,TRUE);
   ClearFog(player->GetCenterPoint());
   FadeIn(FADE_TIME);
}

void CGameMap::endfight(CPlayer* player, CMonsters* monsters)
{
   FadeOut(FADE_TIME);
   gotopos(player->m_savepos.x,player->m_savepos.y);
   load_new_map(0,pDirDraw);
   player->monster=FALSE;
   monsters->end();
   ClearFog(player->GetCenterPoint());
   FadeIn(FADE_TIME);
}


BOOL CGameMap::CheckPos(CPoint cp)
{
   if(cp.x>GetSize().cx || cp.x<0 || cp.y>GetSize().cy || cp.y<0)
      return FALSE;
   cp.y = cp.y>>ICONSIZEB;
   cp.x = cp.x>>ICONSIZEB;
   return CheckPosMap(cp);
}

void CGameMap::SetMapPos(int x, int y, int playerx, int playery)
{
   m_mappos.x = x;
   m_mappos.y = y;
   playerpos.x = playerx;
   playerpos.y = playery;
}

unsigned char CGameMap::GetBkgrdItem(CPoint pos)
{
   pos.y = pos.y>>ICONSIZEB;
   pos.x = pos.x>>ICONSIZEB;
   CSize size = GetMapSize();

   if(pos.x>size.cx || pos.x<0 || pos.y>size.cy || pos.y<0)
      return type;
   if(m_pMap[pos.y][pos.x])
      return m_pMap[pos.y][pos.x]->GetMapType();
   return type;
}

float CGameMap::GetElapsedTime()
{
   return m_elapsed;
}

void CGameMap::UpdateTime()
{
   DWORD newtime=timeGetTime();
   m_elapsed = float(newtime - m_systime);
   m_systime=newtime;
   //TRACE2("Time %d Elapsed Time %f\n",m_systime,m_elapsed);
}

void CGameMap::ResetTime()
{
   m_systime = timeGetTime();
}

CPoint CGameMap::ScreenToMap(CPoint p)
{
   p.Offset(m_mappos);
   return p;
}

CPoint CGameMap::MapToScreen(CPoint p)
{
   CPoint np(p.x-m_mappos.x,p.y-m_mappos.y);
   return np;
}

#define ANGLE_STEP 6

BOOL CGameMap::FindPath(double &angle, CPoint &dest, CPoint src)
{
   double intangle = ToDeg(angle);
   double value = FindValueOfPath(angle,dest,src);
   double value2 = value;
   double angleinc = 0;
   while(value != 0 && value2 !=0)
   {
      angleinc += ANGLE_STEP;
      if(angleinc > 180.0)
         return FALSE;     

      // positive angle
      {
         double angletmp= intangle+angleinc;
         if(angletmp<0)
            angletmp += 360;

         double DestAngle = ToRad(angletmp);
         dest.x = cos(DestAngle) * value + src.x;
         dest.y = sin(DestAngle) * value + src.y;
         angle = DestAngle;
         value = FindValueOfPath(DestAngle,dest,src);
      }

      // negative angle
      if(value != 0)
      {
         double angletmp= intangle-angleinc;
         if(angletmp<0)
            angletmp += 360;

         double DestAngle = ToRad(angletmp);
         dest.x = cos(DestAngle) * value2 + src.x;
         dest.y = sin(DestAngle) * value2 + src.y;
         angle = DestAngle;
         value2 = FindValueOfPath(DestAngle,dest,src);
      }
   }
   return TRUE;

}

#define STEPSIZE 8.0

double CGameMap::FindValueOfPath(double angle, CPoint dest, CPoint src)
{
   double DestAngle = ToDeg(angle);
   double value = 0;
   BOOL bFoundBlocking = FALSE;
   if(DestAngle >=315 || DestAngle<45)
   {
      for(int x = src.x; x<dest.x; x+=STEPSIZE)
      {
         int y = (tan(angle) * (x - src.x)) + src.y;
         CPoint tmppoint(x,y);
         value+=STEPSIZE;
         if(CheckPos(tmppoint))
         {
            if(bFoundBlocking == TRUE)
               break;   
         }
         else
            bFoundBlocking = TRUE;

      }
   }
   else if(DestAngle >=135 && DestAngle<255)
   {  
      for(int x = src.x; x>dest.x; x-=STEPSIZE)
      {
         int y = (tan(angle) * (x - src.x)) + src.y;
         CPoint tmppoint(x,y);
         value+=STEPSIZE;
         if(CheckPos(tmppoint))
         {
            if(bFoundBlocking == TRUE)
               break;
         }
         else
            bFoundBlocking = TRUE;

      }
   }
   else if(DestAngle >=45 && DestAngle <135)
   {
      for(int y = src.y; y<dest.y; y+=STEPSIZE)
      {
         int x = ((y - src.y)/tan(angle)) + src.x;
         CPoint tmppoint(x,y);
         value+=STEPSIZE;
         if(CheckPos(tmppoint))
         {
            if(bFoundBlocking == TRUE)
               break;   
         }
         else
            bFoundBlocking = TRUE;

      }
   }
   else
   {
      for(int y = src.y; y>dest.y; y-=STEPSIZE)
      {
         int x = ((y - src.y)/tan(angle)) + src.x;
         CPoint tmppoint(x,y);
         value+=STEPSIZE;
         if(CheckPos(tmppoint))
         {
            if(bFoundBlocking == TRUE)
               break;   
         }
         else
            bFoundBlocking = TRUE;

      }
   }

   if(bFoundBlocking)
   {
      value = value * 1.13;
   }
   else
      value = 0;
   return value;
}

#define FOGRADIUS 5

void CGameMap::ClearFog(CPoint p)
{
   p.x = p.x>>ICONSIZEB;
   p.y = p.y>>ICONSIZEB;
   CSize size = GetMapSize();
   for(int y= -FOGRADIUS;y<=FOGRADIUS;y++)
   {
      int tmpy = p.y+y;
      if(tmpy < size.cy && tmpy>=0)
      {
         int fogxsize;
         if(y == -FOGRADIUS || y == FOGRADIUS)
            fogxsize = FOGRADIUS - 2;
         else if(y == -FOGRADIUS+1 || y == FOGRADIUS-1) 
            fogxsize = FOGRADIUS-1;
         else
            fogxsize = FOGRADIUS;

         for(int x= -fogxsize;x<=fogxsize;x++)
         {
            int tmpx = p.x+x;
            if(tmpx < size.cx && tmpx>=0)
            {
               if(m_pMap[tmpy][tmpx])
                  m_pMap[tmpy][tmpx]->Show(TRUE);
            }
         }
      }
   }
}

BOOL CGameMap::IsInFog(CPoint p)
{
   CSize size = GetMapSize();
   p.y = p.y>>ICONSIZEB;
   p.x = p.x>>ICONSIZEB;
   if(p.x>size.cx || p.x<0 || p.y>size.cy || p.y<0)
      return FALSE;
   if(m_pMap[p.y][p.x])
      return m_pMap[p.y][p.x]->IsShown()?FALSE:TRUE;
   return FALSE;
}

BOOL CGameMap::SaveFogMap(CString filename)
{
   FILE* mapfile;
   if((mapfile=fopen(filename,"w"))!=NULL)
   {
      for(int y=0;y<MAPY;y++)
      {
         for(int x=0;x<MAPX;x++)
         {
            if(m_pMap[y][x])
               fprintf(mapfile,"%c",m_pMap[y][x]->IsShown());
            else
               fprintf(mapfile,"%c",FALSE);
         }
      }
      fclose(mapfile);
      return TRUE;
   }
   else
   {
      sys_printf(__LOCATION__,0,"could not open file %s",filename);
      return FALSE;
   }
}

BOOL CGameMap::LoadFogMap(CString filename)
{
   FILE* mapfile;
   if((mapfile=fopen(filename,"r"))!=NULL)
   {
      for(int y=0;y<MAPY;y++)
      {
         for(int x=0;x<MAPX;x++)
         {
            char bfog;
            fscanf(mapfile,"%c",&bfog);
            if(m_pMap[y][x])
               m_pMap[y][x]->Show(bfog);
         }
      }
      fclose(mapfile);
   }
   else
      return FALSE;
   return TRUE;
}

void CGameMap::DeleteMapObj()
{
   CDEStatic* pObj;
   while(m_MapObj.size() != 0)
   {
      pObj = m_MapObj.front();
      m_MapObj.pop_front();
      SAFE_DELETE(pObj);
   }

   player->StopSeekingMonster();

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

BOOL CGameMap::KillObject(CDEStatic *pObj)
{
   m_MapObj.remove(pObj);
   delete pObj;
   return TRUE;
}

CDEStatic* CGameMap::GetObjAtPos(CPoint p)
{
   // for each monster thats alive
   for(std::list <CDEStatic*>::iterator i = m_MapObj.begin(); i != m_MapObj.end(); i++)
   {
      CDEStatic* pObj = *i;
      if(pObj)
      {
         if(pObj->GetRect().PtInRect(p))
            return pObj;
      }
   }
   return NULL;
}

BOOL CGameMap::CheckPosMap(CPoint cp)
{
   CSize size = GetMapSize();

   if(cp.x>size.cx || cp.x<0 || cp.y>size.cy || cp.y<0 || m_pMap[cp.y][cp.x] == NULL)
      return FALSE;
   unsigned char val = m_pMap[cp.y][cp.x]->GetMapType(TRUE);
   switch(val)
   {
   case 0:
   case 10:
   case 'w':
      if(player->ship!=SHIP_IN)
         return FALSE;
      break;
   case 'd':
   case 'I':
   case 'J':
   case 'K':
   case 'L':
   case 'M':
   case 'N':
   case 'O':
   case 'P':
   case 'Q':
   case 'R':
   case 'S':
   case 'T':
   case 'U':
   case 'V':
   case 'W':
   case 'X':
   case 'Y':
   case 'Z':
   case 127:
   case 128: 
   case 129:
   case 130:
   case 131:
   case 132:
   case 133:
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
   case 'z':
   case 'y':
   case '#':
   case '<':
   case ':':
   case '_':
   case ',':
   case 'H':
   case 'F':
   case 't':
   case 'k':
   case 'b':
   case 'g':
   case 'G':
   case '0':
   case '*':
   case 'f':
   case 'B':
   case ' ':
   case '.':
   case 'i':
      break;
   default:
      return FALSE;
      break;
   }
   return TRUE;
}

BOOL CGameMap::CreateChest(CPoint p, int gold)
{
   CChest* pChest = new CChest();
   pChest->Create(p,CSize(32,32),player,pDirDraw,FALSE,FALSE,TRUE);
   pChest->m_gold = gold;
   m_MapObj.push_back(pChest);
   return TRUE;
}

void CGameMap::FadeIn(int time)
{
   /*UpdateTime();
   FLOAT alpha = 255.0f;
   while(alpha>0.0f && GetInputObj()->IsButtonPressEmpty())
   {
   if(!LookForMSG())
   {
   return;
   }
   pDirDraw->Clear(RGB(0,0,0));
   pDirDraw->StartRender();
   setscr();
   CRect rect(0,0,pDirDraw->GetScreenWidth(),pDirDraw->GetScreenHight());
   pDirDraw->DrawBlendRect(rect,D3DCOLOR_ARGB(((BYTE)alpha),0,0,0));
   pDirDraw->Present();
   UpdateTime();
   FLOAT inc = ((FLOAT)m_elapsed/(FLOAT)time)*255.0f;
   alpha-= inc;
   }*/
}

void CGameMap::FadeOut(int time)
{
   /*UpdateTime();
   FLOAT alpha = 0.0f;
   while(alpha<255.0f && GetInputObj()->IsButtonPressEmpty())
   {
   if(!LookForMSG())
   {
   return;
   }
   pDirDraw->Clear(RGB(0,0,0));
   pDirDraw->StartRender();
   setscr();
   CRect rect(0,0,pDirDraw->GetScreenWidth(),pDirDraw->GetScreenHight());
   pDirDraw->DrawBlendRect(rect,D3DCOLOR_ARGB(((BYTE)alpha),0,0,0));
   pDirDraw->Present();
   UpdateTime();
   FLOAT inc = ((FLOAT)m_elapsed/(FLOAT)time)*255.0f;
   alpha+= inc;
   }*/
}

void CGameMap::GoToPos(CPoint point)
{
   gotopos(point.x,point.y);
}
