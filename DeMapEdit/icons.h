#ifndef __ICONS_H
#define __ICONS_H

#define MAXX 30
#define MAXY 30
#define ICONSIZE 32
#define ICONSIZEB 5
//#define NUMBICON 59
#define NUMPLAYERICONS 11
#define PLAYEROFFSET (NUMBICON-NUMPLAYERICONS)

class CTile
{
public:
    int m_index;
    int m_icon;
    CString m_Text;
};

/////////////////////////////////////////////////////////////////////////////
// CIcons fuctions
class CIcons
{
    int m_NumbIcon;
    CString* m_FileList;
    CBitmap* m_pBitmap;
public:
    CMap<int,int,CTile,CTile> m_Tiles;
    BOOL SaveXML();
    BOOL LoadXML();
    BOOL LoadFileList();
    CImageList* CIcons::CreateImageList();
    CString GetItemText(unsigned char value);
    BOOL Create();
    BOOL DrawIcon(int icon, CDC* pDC, CPoint pos);
	CIcons();
	~CIcons();
    int GetImage(unsigned char value,char type);
};

#endif
/////////////////////////////////////////////////////////////////////////////
