// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "Sprite.h"
#include "icons.h"
#include "map.h"
#include <d3dutil.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite() : CDEObject()
{
	m_bPlayer = FALSE;
	m_move_state = STOPPED;
	m_Angle = 0;
	m_Path = NULL;
}

CSprite::~CSprite()
{
	if(m_Path)
	{
		m_Path->FreeSolutionNodes();
		delete m_Path;
	}
}

void CSprite::MoveBack()
{
	m_pos.x = m_lastpos.x;
	m_pos.y = m_lastpos.y;
}

void CSprite::GotoPos(CPoint p)
{
	m_lastpos = p;
	CDEObject::GotoPos(p);
}

BOOL CSprite::IsPlayer()
{
	return m_bPlayer;
}

CPoint CSprite::GetLastCenterPoint()
{
	CRect rect(CPoint(0,0),m_size);
	rect.OffsetRect(m_lastpos);
	return rect.CenterPoint();
}

void CSprite::DisplayPath(IGameMap *pMap, C2DDraw *pDisplay)
{
	if(m_move_state != STOPPED)
	{
#ifdef _DEBUG
		//m_Path.DisplayPath(pDisplay,pMap);
		//pDisplay->DrawLine(pMap->MapToScreen(GetCenterPoint()),pMap->MapToScreen(m_WayPoint),RGB(255,0,0));
		pDisplay->DrawLine(pMap->MapToScreen(GetCenterPoint()),pMap->MapToScreen(m_GotoPos),RGB(255,0,0));
#endif
	}
}

void CSprite::SeekPoint(CPoint p, IGameMap *pMap, BOOL bUseAStar)
{
	CGameMap::SnapToGrid(p);
	BOOL bIsValidPoint = TRUE;
	if(pMap)
		bIsValidPoint = pMap->CheckPos(p);
	if(bIsValidPoint)
	{
		
		double x = p.x - GetCenterPoint().x;
		double y = p.y - GetCenterPoint().y;
		if(x != 0)
			m_Angle = atan2(y,x);
		else if(y > 0)
			m_Angle = ToRad(90);
		else
			m_Angle = ToRad(270);
		//find best angle!	
		m_GotoPos = p;
		m_NextPos = p;
		
		if(pMap)
		{
			// look to see if there is anything in the path
			if(!CanSee(p,pMap))
			{
				if(bUseAStar)
				{
					// do an A* path Finding
					CPoint start = GetCenterPoint();
					CGameMap::SnapToGrid(start);
					MapSearchNode nodeStart(start);
					// Define the goal state
					MapSearchNode nodeEnd(p);
					
					if(m_Path)
					{
						m_Path->FreeSolutionNodes();
						delete m_Path;
					}

					m_Path = new AStarSearch<MapSearchNode>(50000);
					m_Path->SetStartAndGoalStates(nodeStart, nodeEnd);
					UINT SearchState;
					//UINT SearchSteps = 0;
					do
					{
						SearchState = m_Path->SearchStep();
						//SearchSteps++;
					}
					while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING && m_Path->GetStepCount()<1500);

					TRACE1("Steps %d\n",m_Path->GetStepCount());

					if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED )
					{
						m_Path->GetSolutionStart();
						MapSearchNode* node = m_Path->GetSolutionNext();
						m_NextPos = node->GetPos();
						m_move_state = MOVING;
					}
					else
						m_move_state = STOPPED;
				}
				else
					m_move_state = STOPPED;
			}
			else
				m_move_state = MOVING;
		}
		else
		{
		
			m_move_state = MOVING;
		}
	}
	//TRACE1("Seek Point %f angle\n",ToDeg(m_Angle));
}

void CSprite::MoveToGoTo(double distance, IGameMap* pMap)
{
	if(pMap)
	{
		int val = GetNextPos();
		
		if(val == 1)
		{
			double x = m_NextPos.x - GetCenterPoint().x;
			double y = m_NextPos.y - GetCenterPoint().y;
			if(x != 0)
				m_Angle = atan2(y,x);
			else if(y > 0)
				m_Angle = ToRad(0);
			else
				m_Angle = ToRad(180);
		}
		else if(val == 0)
			m_move_state = STOPPED;

		{
			CSize size = m_GotoPos - GetCenterPoint();
			size.cx = abs(size.cx);
			size.cy = abs(size.cy);
			if(size.cx < 5 && size.cy < 5)
			{
				m_move_state = STOPPED;
			}
		}
	}
	double x  = distance * cos(m_Angle);
	double y  = distance * sin(m_Angle);
	m_pos.x+=x;
	m_pos.y+=y;
}

double CSprite::GetAngle()
{
	return m_Angle;
}

CString CSprite::GetStateStr()
{
	switch(m_move_state)
	{
	case MOVING:
		return "MOVING";
	case SEARCHING:
		return "SEARCHING";
	case STOPPED:
		return "STOPPED";
	case SEEKMONSTER:
		return "SEEKMONSTER";
	case USEOBJECT:
		return "USEOBJECT";
	}
	return "";
}

int CSprite::GetNextPos()
{
	if(m_NextPos != m_GotoPos)
	{
		CSize size = m_NextPos - GetCenterPoint();
		size.cx = abs(size.cx);
		size.cy = abs(size.cy);
		if(size.cx < 5 && size.cy < 5)
		{
			MapSearchNode* node = m_Path->GetSolutionNext();
			if(node)
			{
				m_NextPos = node->GetPos();
				return 1;
			}
			else
			{
			
				return 0;
			}
		}
		return 1;
	}
	return 1;
}

BOOL CSprite::CanSee(CPoint p, IGameMap *pMap)
{
	double x = p.x - GetCenterPoint().x;
	double y = p.y - GetCenterPoint().y;
	double angle;
	if(x != 0)
		angle = atan2(y,x);
	else if(y > 0)
		angle = ToRad(0);
	else
		angle = ToRad(180);
	return pMap->FindValueOfPath(angle,p,GetCenterPoint())== 0?TRUE:FALSE;
}
