#ifndef __NETMESSAGES_H__
#define __NETMESSAGES_H__

#include <DPlay8.h>

enum Sprite_State_e
{
   SPRITE_ALIVE = 0,
   SPRITE_DYING = 1,
   SPRITE_DEAD =2,
};

#define MAX_CHAT_STRING_LENGTH 255

#define GAME_MSGID_CHAT    1
#define GAME_MSGID_PLAYER_UPDATE   2
#define GAME_MSGID_WORLD_STAT    3
#define GAME_MSGID_SHOT_FIRED    4
#define GAME_MSGID_REQUEST_WORLD 5
#define GAME_MSGID_FULL_PLAYER_UPDATE 6
#define GAME_MSGID_PLAYER_EXIT 7
#define GAME_MSGID_REQUEST_FULL_PLAYER_UPDATE 8
#define GAME_MSGID_ADDFRAG_COUNT 9

// Change compiler pack alignment to be BYTE aligned, and pop the current value
#pragma pack( push, 1 )

struct GAMEMSG_GENERIC
{
   BYTE nType;
};

struct GAMEMSG_CHAT : public GAMEMSG_GENERIC
{
   char strChatString[MAX_CHAT_STRING_LENGTH];
};

struct GAMEMSG_PLAYER_UPDATE : public GAMEMSG_GENERIC
{
   Sprite_State_e m_SpriteState;
   FLOAT m_fShield_Str;
   FLOAT m_fDamage;
   D3DXVECTOR3 m_Speed;
   D3DXVECTOR3 m_Pos;
   FLOAT m_angleX;
   FLOAT m_angleY;
   FLOAT m_angleZ;
   BOOL m_bInvisable;
   BOOL m_bInvurnable;
};

struct GAMEMSG_FULL_PLAYER_UPDATE : public GAMEMSG_PLAYER_UPDATE
{
   WORD m_PlayerShipType;
   WORD m_TeamNo;
   BOOL m_bBot;
   DPNID	m_dpID;
   char m_Name[MAX_CHAT_STRING_LENGTH];
};

//-----------------------------------------------------------------------------
// Name: Tree
// Desc: Simple structure to hold data for rendering a tree
//-----------------------------------------------------------------------------



struct GAMEMSG_WORLD_STAT : public GAMEMSG_GENERIC
{
   DWORD MapNumber;
   DWORD m_NumbPlayers;
   GAMEMSG_FULL_PLAYER_UPDATE m_Players[10];
};

struct GAMEMSG_SHOT_FIRED : public GAMEMSG_GENERIC
{
   DWORD dwNumParticlesToEmit;
   D3DXCOLOR dwEmitColor;
   D3DXCOLOR dwFadeColor;
   D3DXVECTOR3 vPosition;
   D3DXVECTOR3 vEmitVel;
   FLOAT fPower;
   FLOAT fGravity;
   FLOAT fSize;
};

// Pop the old pack alignment
#pragma pack( pop )

#endif
