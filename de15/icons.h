#ifndef __ICONS_H
#define __ICONS_H
#define MAXX 30
#define MAXY 30
#include <2DDraw.h>
#include <Surface.h>
#include "types.h"
#define ICONSIZE 32
#define ICONSIZEB 5
#define NUMBICON 59
#define NUMPLAYERICONS 11
#define PLAYEROFFSET (NUMBICON-NUMPLAYERICONS)



/////////////////////////////////////////////////////////////////////////////
// CIcons fuctions
class CIcons
{
public:

   BOOL LoadFileList();
	//void putic(CSurface* Surface, int numb,int posx,int posy,int w = ICONSIZE, int h = ICONSIZE,int offx = 0,int offy = 0);
	void draw(C2DDraw* pDisplay,int numb,int posx,int posy,int w = ICONSIZE, int h = ICONSIZE,int offx = 0,int offy = 0);
	CIcons(IDirect3DDevice9* device);
	~CIcons(void);
	void draw_player(C2DDraw* pDisplay,int numb,int posx,int posy);
	CSurface* GetIcon(int type)
	{
		return m_pIcons[type];
	}
protected:

    int m_NumbIcon;
    CString* m_FileList;

	CSurface* m_pIcons[NUMBICON];
	int load(IDirect3DDevice9* device,CString filename,int numb);
	CSurface* m_pPlayerIcons[NUMPLAYERICONS];
};
#endif
/////////////////////////////////////////////////////////////////////////////
