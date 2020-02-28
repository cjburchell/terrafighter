
#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "Patch.h"

// ---------------------------------------------------------------------
// Various Pre-Defined map sizes & their #define counterparts:

#define MAP_256

#ifdef MAP_2048

// ------- 2048x2048 MAP -------
#define MAP_SIZE (2048)
#define NUM_PATCHES_PER_SIDE (32)

#else
#ifdef MAP_1024

// ------- 1024x1024 MAP -------
#define MAP_SIZE (1024)
#define NUM_PATCHES_PER_SIDE (16)

#else

#ifdef MAP_512

// ------- 1024x1024 MAP -------
#define MAP_SIZE (512)
#define NUM_PATCHES_PER_SIDE (8)

#else

// ------- 256x256 MAP -------
#define MAP_SIZE (256)
#define NUM_PATCHES_PER_SIDE (8)

#endif
#endif
#endif

// ---------------------------------------------------------------------
// Scale of the terrain ie: 1 unit of the height map == how many world units (meters)?
// 1.0f == 1 meter resolution
// 0.5f == 1/2 meter resolution
// 0.25f == 1/4 meter resolution
// etc..
#define MULT_SCALE (0.5f)

// How many TriTreeNodes should be allocated?
#define POOL_SIZE (25000)

// Some more definitions
#define PATCH_SIZE (MAP_SIZE/NUM_PATCHES_PER_SIDE)

#define SQR(x) ((x) * (x))
#define MAX(a,b) ((a < b) ? (b) : (a))
#define DEG2RAD(a) (((a) * M_PI) / 180.0f)
#define M_PI (3.14159265358979323846f)

// External variables and functions:

//
// Landscape Class
// Holds all the information to render an entire landscape.
//
class Landscape
{
protected:
	unsigned char *m_HeightMap;										// HeightMap of the Landscape
	DWORD* m_pIndexMap;
	Patch m_Patches[NUM_PATCHES_PER_SIDE][NUM_PATCHES_PER_SIDE];	// Array of patches

	static DWORD	m_NextTriNode;										// Index to next free TriTreeNode
	static TriTreeNode m_TriPool[POOL_SIZE];						// Pool of TriTree nodes for splitting

	static DWORD GetNextTriNode() { return m_NextTriNode; }
	static void SetNextTriNode( DWORD nNextNode ) { m_NextTriNode = nNextNode; }
	float m_FrameVariance;
	DWORD m_NumTrisRendered;

public:
	static TriTreeNode *AllocateTri();

	virtual void Init(unsigned char *hMap, DWORD* pIndexMap);
	virtual void Reset(CD3DCamera* pCamera);
	virtual void Tessellate(CD3DCamera* pCamera);
	virtual void Render(DWORD DesiredTris,DWORD &FacestoDraw,DWORD* pIndexBuffer);

};


#endif