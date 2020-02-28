//
// ROAM Simplistic Implementation
// All code copyright Bryan Turner (Jan, 2000)
//                    brturn@bellsouth.net
//
// Based on the Tread Marks engine by Longbow Digital Arts
//                               (www.LongbowDigitalArts.com)
// Much help and hints provided by Seumas McNally, LDA.
//
#include <windows.h>
#include <math.h>

#include "landscape.h"

// -------------------------------------------------------------------------------------------------
//	PATCH CLASS
// -------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------
// Split a single Triangle and link it into the mesh.
// Will correctly force-split diamonds.
//
void Patch::Split(TriTreeNode *tri)
{
	// We are already split, no need to do it again.
	if (tri->LeftChild)
		return;

	// If this triangle is not in a proper diamond, force split our base neighbor
	if ( tri->BaseNeighbor && (tri->BaseNeighbor->BaseNeighbor != tri) )
		Split(tri->BaseNeighbor);

	// Create children and link into mesh
	tri->LeftChild  = Landscape::AllocateTri();
	tri->RightChild = Landscape::AllocateTri();

	// If creation failed, just exit.
	if ( !tri->LeftChild )
		return;

	// Fill in the information we can get from the parent (neighbor pointers)
	tri->LeftChild->BaseNeighbor  = tri->LeftNeighbor;
	tri->LeftChild->LeftNeighbor  = tri->RightChild;

	tri->RightChild->BaseNeighbor  = tri->RightNeighbor;
	tri->RightChild->RightNeighbor = tri->LeftChild;

	// Link our Left Neighbor to the new children
	if (tri->LeftNeighbor != NULL)
	{
		if (tri->LeftNeighbor->BaseNeighbor == tri)
			tri->LeftNeighbor->BaseNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->LeftNeighbor == tri)
			tri->LeftNeighbor->LeftNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->RightNeighbor == tri)
			tri->LeftNeighbor->RightNeighbor = tri->LeftChild;
		else
			;// Illegal Left Neighbor!
	}

	// Link our Right Neighbor to the new children
	if (tri->RightNeighbor != NULL)
	{
		if (tri->RightNeighbor->BaseNeighbor == tri)
			tri->RightNeighbor->BaseNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->RightNeighbor == tri)
			tri->RightNeighbor->RightNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->LeftNeighbor == tri)
			tri->RightNeighbor->LeftNeighbor = tri->RightChild;
		else
			;// Illegal Right Neighbor!
	}

	// Link our Base Neighbor to the new children
	if (tri->BaseNeighbor != NULL)
	{
		if ( tri->BaseNeighbor->LeftChild )
		{
			tri->BaseNeighbor->LeftChild->RightNeighbor = tri->RightChild;
			tri->BaseNeighbor->RightChild->LeftNeighbor = tri->LeftChild;
			tri->LeftChild->RightNeighbor = tri->BaseNeighbor->RightChild;
			tri->RightChild->LeftNeighbor = tri->BaseNeighbor->LeftChild;
		}
		else
			Split( tri->BaseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
	}
	else
	{
		// An edge triangle, trivial case.
		tri->LeftChild->RightNeighbor = NULL;
		tri->RightChild->LeftNeighbor = NULL;
	}
}

// ---------------------------------------------------------------------
// Tessellate a Patch.
// Will continue to split until the variance metric is met.
//
void Patch::RecursTessellate( TriTreeNode *tri,
							 int leftX,  int leftY,
							 int rightX, int rightY,
							 int apexX,  int apexY,
							 int node , CD3DCamera * pCamera, 
							 float FrameVariance)
{
	float TriVariance;
	int centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
	int centerY = (leftY + rightY)>>1; // Compute Y coord...

	if ( node < (1<<VARIANCE_DEPTH) )
	{
		// Extremely slow distance metric (sqrt is used).
		// Replace this with a faster one!
		float distance = 1.0f + sqrtf( SQR((float)centerX - pCamera->GetEyePt().x) +
									   SQR((float)centerY - pCamera->GetEyePt().z) );
		
		// Egads!  A division too?  What's this world coming to!
		// This should also be replaced with a faster operation.
		TriVariance = ((float)m_CurrentVariance[node] * MAP_SIZE * 2)/distance;	// Take both distance and variance into consideration
	}

	if ( (node >= (1<<VARIANCE_DEPTH)) ||	// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
		 (TriVariance > FrameVariance))	// OR if we are not below the variance tree, test for variance.
	{
		Split(tri);														// Split this triangle.
		
		if (tri->LeftChild &&											// If this triangle was split, try to split it's children as well.
			((abs(leftX - rightX) >= 3) || (abs(leftY - rightY) >= 3)))	// Tessellate all the way down to one vertex per height field entry
		{
			RecursTessellate( tri->LeftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  ,pCamera, FrameVariance);
			RecursTessellate( tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) ,pCamera, FrameVariance);
		}
	}
}

// ---------------------------------------------------------------------
// Render the tree.  Simple no-fan method.
//
void Patch::RecursRender( TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY , DWORD &NumTrisRendered,DWORD* pIndexBuffer, DWORD & index)
{
	if ( tri->LeftChild )					// All non-leaf nodes have both children, so just check for one
	{
		int centerX = (leftX + rightX)>>1;	// Compute X coordinate of center of Hypotenuse
		int centerY = (leftY + rightY)>>1;	// Compute Y coord...

		RecursRender( tri->LeftChild,  apexX,   apexY, leftX, leftY, centerX, centerY , NumTrisRendered,pIndexBuffer,index);
		RecursRender( tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY , NumTrisRendered,pIndexBuffer,index);
	}
	else									// A leaf node!  Output a triangle to be rendered.
	{
		// Actual number of rendered triangles...
		pIndexBuffer[index++] = m_pIndexMap[(leftY *MAP_SIZE)+leftX ];
		pIndexBuffer[index++] = m_pIndexMap[(rightY*MAP_SIZE)+rightX];
		pIndexBuffer[index++] = m_pIndexMap[(apexY *MAP_SIZE)+apexX ];
		NumTrisRendered++;

		//FLOAT leftZ  = m_HeightMap[(leftY *MAP_SIZE)+leftX ];
		//FLOAT rightZ = m_HeightMap[(rightY*MAP_SIZE)+rightX];
		//FLOAT apexZ  = m_HeightMap[(apexY *MAP_SIZE)+apexX ];

		// Perform lighting calculations if requested.
	/*	if (gDrawMode == DRAW_USE_LIGHTING)
		{
			float v[3][3];
			float out[3];
			
			// Create a vertex normal for this triangle.
			// NOTE: This is an extremely slow operation for illustration purposes only.
			//       You should use a texture map with the lighting pre-applied to the texture.
			v[0][0] = (FLOAT) leftX;
			v[0][1] = (FLOAT) leftZ;
			v[0][2] = (FLOAT) leftY;
			
			v[1][0] = (FLOAT) rightX;
			v[1][1] = (FLOAT) rightZ ;
			v[1][2] = (FLOAT) rightY;
			
			v[2][0] = (FLOAT) apexX;
			v[2][1] = (FLOAT) apexZ ;
			v[2][2] = (FLOAT) apexY;
			
			calcNormal( v, out );
			glNormal3fv( out );
		}

		// Perform polygon coloring based on a height sample
		float fColor = (60.0f + leftZ) / 256.0f;
		if ( fColor > 1.0f )  fColor = 1.0f;
		glColor3f( fColor, fColor, fColor );

		// Output the LEFT VERTEX for the triangle
		glVertex3f(		(FLOAT) leftX,
						(FLOAT) leftZ,
						(FLOAT) leftY );

		if ( gDrawMode == DRAW_USE_TEXTURE ||	// Gaurad shading based on height samples instead of light normal
			 gDrawMode == DRAW_USE_FILL_ONLY )
		{
			float fColor = (60.0f + rightZ) / 256.0f;
			if ( fColor > 1.0f )  fColor = 1.0f;
			glColor3f( fColor, fColor, fColor );
		}

		// Output the RIGHT VERTEX for the triangle
		glVertex3f(		(FLOAT) rightX,
						(FLOAT) rightZ,
						(FLOAT) rightY );


		if ( gDrawMode == DRAW_USE_TEXTURE ||	// Gaurad shading based on height samples instead of light normal
			 gDrawMode == DRAW_USE_FILL_ONLY )
		{
			float fColor = (60.0f + apexZ) / 256.0f;
			if ( fColor > 1.0f )  fColor = 1.0f;
			glColor3f( fColor, fColor, fColor );
		}

		// Output the APEX VERTEX for the triangle
		glVertex3f(		(FLOAT) apexX,
						(FLOAT) apexZ,
						(FLOAT) apexY );*/
	}
}

// ---------------------------------------------------------------------
// Computes Variance over the entire tree.  Does not examine node relationships.
//
unsigned char Patch::RecursComputeVariance( int leftX,  int leftY,  unsigned char leftZ,
										    int rightX, int rightY, unsigned char rightZ,
											int apexX,  int apexY,  unsigned char apexZ,
											int node)
{
	//        /|\
	//      /  |  \
	//    /    |    \
	//  /      |      \
	//  ~~~~~~~*~~~~~~~  <-- Compute the X and Y coordinates of '*'
	//
	int centerX = (leftX + rightX) >>1;		// Compute X coordinate of center of Hypotenuse
	int centerY = (leftY + rightY) >>1;		// Compute Y coord...
	unsigned char myVariance;

	// Get the height value at the middle of the Hypotenuse
	unsigned char centerZ  = m_HeightMap[(centerY * MAP_SIZE) + centerX];

	// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
	// Use values passed on the stack instead of re-accessing the Height Field.
	myVariance = abs((int)centerZ - (((int)leftZ + (int)rightZ)>>1));

	// Since we're after speed and not perfect representations,
	//    only calculate variance down to an 8x8 block
	if ( (abs(leftX - rightX) >= 8) ||
		 (abs(leftY - rightY) >= 8) )
	{
		// Final Variance for this node is the max of it's own variance and that of it's children.
		myVariance = MAX( myVariance, RecursComputeVariance( apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    node<<1 ) );
		myVariance = MAX( myVariance, RecursComputeVariance( rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(node<<1)) );
	}

	// Store the final variance for this node.  Note Variance is never zero.
	if (node < (1<<VARIANCE_DEPTH))
		m_CurrentVariance[node] = 1 + myVariance;

	return myVariance;
}

// -------------------------------------------------------------------------------------------------
//	PATCH CLASS
// -------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------
// Initialize a patch.
//
void Patch::Init( int heightX, int heightY, int worldX, int worldY, unsigned char *hMap,DWORD* pIndexMap )
{
	// Clear all the relationships
	m_BaseLeft.RightNeighbor = m_BaseLeft.LeftNeighbor = m_BaseRight.RightNeighbor = m_BaseRight.LeftNeighbor =
	m_BaseLeft.LeftChild = m_BaseLeft.RightChild = m_BaseRight.LeftChild = m_BaseLeft.LeftChild = NULL;

	// Attach the two m_Base triangles together
	m_BaseLeft.BaseNeighbor = &m_BaseRight;
	m_BaseRight.BaseNeighbor = &m_BaseLeft;

	// Store Patch offsets for the world and heightmap.
	m_WorldX = worldX;
	m_WorldY = worldY;

	// Store pointer to first byte of the height data for this patch.
	m_HeightMap = &hMap[heightY * MAP_SIZE + heightX];
	m_pIndexMap = &pIndexMap[heightY * MAP_SIZE + heightX];
	// Initialize flags
	m_VarianceDirty = 1;
	m_isVisible = 0;
}

// ---------------------------------------------------------------------
// Reset the patch.
//
void Patch::Reset()
{
	// Assume patch is not visible.
	m_isVisible = 0;

	// Reset the important relationships
	m_BaseLeft.LeftChild = m_BaseLeft.RightChild = m_BaseRight.LeftChild = m_BaseLeft.LeftChild = NULL;

	// Attach the two m_Base triangles together
	m_BaseLeft.BaseNeighbor = &m_BaseRight;
	m_BaseRight.BaseNeighbor = &m_BaseLeft;

	// Clear the other relationships.
	m_BaseLeft.RightNeighbor = m_BaseLeft.LeftNeighbor = m_BaseRight.RightNeighbor = m_BaseRight.LeftNeighbor = NULL;
}

// ---------------------------------------------------------------------
// Compute the variance tree for each of the Binary Triangles in this patch.
//
void Patch::ComputeVariance()
{
	// Compute variance on each of the base triangles...

	m_CurrentVariance = m_VarianceLeft;
	RecursComputeVariance(	0,          PATCH_SIZE, m_HeightMap[PATCH_SIZE * MAP_SIZE],
							PATCH_SIZE, 0,          m_HeightMap[PATCH_SIZE],
							0,          0,          m_HeightMap[0],
							1);

	m_CurrentVariance = m_VarianceRight;
	RecursComputeVariance(	PATCH_SIZE, 0,          m_HeightMap[ PATCH_SIZE],
							0,          PATCH_SIZE, m_HeightMap[ PATCH_SIZE * MAP_SIZE],
							PATCH_SIZE, PATCH_SIZE, m_HeightMap[(PATCH_SIZE * MAP_SIZE) + PATCH_SIZE],
							1);

	// Clear the dirty flag for this patch
	m_VarianceDirty = 0;
}

// ---------------------------------------------------------------------
// Discover the orientation of a triangle's points:
//
// Taken from "Programming Principles in Computer Graphics", L. Ammeraal (Wiley)
//
inline int orientation( int pX, int pY, int qX, int qY, int rX, int rY )
{
	int aX, aY, bX, bY;
	float d;

	aX = qX - pX;
	aY = qY - pY;

	bX = rX - pX;
	bY = rY - pY;

	d = (float)aX * (float)bY - (float)aY * (float)bX;
	return (d < 0) ? (-1) : (d > 0);
}

// ---------------------------------------------------------------------
// Set patch's visibility flag.
//
void Patch::SetVisibility( int eyeX, int eyeY, int leftX, int leftY, int rightX, int rightY )
{
	// Get patch's center point
	int patchCenterX = m_WorldX + PATCH_SIZE / 2;
	int patchCenterY = m_WorldY + PATCH_SIZE / 2;
	
	// Set visibility flag (orientation of both triangles must be counter clockwise)
	m_isVisible = (orientation( eyeX,  eyeY,  rightX, rightY, patchCenterX, patchCenterY ) < 0) &&
				  (orientation( leftX, leftY, eyeX,   eyeY,   patchCenterX, patchCenterY ) < 0);
}

// ---------------------------------------------------------------------
// Create an approximate mesh.
//
void Patch::Tessellate(CD3DCamera* pCamera,float FrameVariance)
{
	// Split each of the base triangles
	m_CurrentVariance = m_VarianceLeft;
	RecursTessellate (	&m_BaseLeft,
						m_WorldX,				m_WorldY+PATCH_SIZE,
						m_WorldX+PATCH_SIZE,	m_WorldY,
						m_WorldX,				m_WorldY,
						1, 
						pCamera,
						FrameVariance);
					
	m_CurrentVariance = m_VarianceRight;
	RecursTessellate(	&m_BaseRight,
						m_WorldX+PATCH_SIZE,	m_WorldY,
						m_WorldX,				m_WorldY+PATCH_SIZE,
						m_WorldX+PATCH_SIZE,	m_WorldY+PATCH_SIZE,
						1, 
						pCamera,
						FrameVariance );
}

// ---------------------------------------------------------------------
// Render the mesh.
//
void Patch::Render(DWORD &NumTrisRendered,DWORD* pIndexBuffer,DWORD & index)
{	
	RecursRender (	&m_BaseLeft,
		0,				PATCH_SIZE,
		PATCH_SIZE,		0,
		0,				0,
		NumTrisRendered,
		pIndexBuffer,index);
	
	RecursRender(	&m_BaseRight,
		PATCH_SIZE,		0,
		0,				PATCH_SIZE,
		PATCH_SIZE,		PATCH_SIZE,
		NumTrisRendered,
		pIndexBuffer,index);
}

// -------------------------------------------------------------------------------------------------
//	LANDSCAPE CLASS
// -------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------
// Definition of the static member variables
//
DWORD       Landscape::m_NextTriNode;
TriTreeNode Landscape::m_TriPool[POOL_SIZE];

// ---------------------------------------------------------------------
// Allocate a TriTreeNode from the pool.
//
TriTreeNode *Landscape::AllocateTri()
{
	TriTreeNode *pTri;

	// IF we've run out of TriTreeNodes, just return NULL (this is handled gracefully)
	if ( m_NextTriNode >= POOL_SIZE )
		return NULL;

	pTri = &(m_TriPool[m_NextTriNode++]);
	pTri->LeftChild = pTri->RightChild = NULL;

	return pTri;
}

// ---------------------------------------------------------------------
// Initialize all patches
//
void Landscape::Init(unsigned char *hMap,DWORD* pIndexMap)
{
	Patch *patch;
	int X, Y;

	// Store the Height Field array
	m_HeightMap = hMap;
	m_pIndexMap = pIndexMap;

	// Initialize all terrain patches
	for ( Y=0; Y < NUM_PATCHES_PER_SIDE; Y++)
		for ( X=0; X < NUM_PATCHES_PER_SIDE; X++ )
		{
			patch = &(m_Patches[Y][X]);
			patch->Init( X*PATCH_SIZE, Y*PATCH_SIZE, X*PATCH_SIZE, Y*PATCH_SIZE, hMap , pIndexMap);
			patch->ComputeVariance();
		}
}

// ---------------------------------------------------------------------
// Reset all patches, recompute variance if needed
//
void Landscape::Reset(CD3DCamera* pCamera)
{
	//
	// Perform simple visibility culling on entire patches.
	//   - Define a triangle set back from the camera by one patch size, following
	//     the angle of the frustum.
	//   - A patch is visible if it's center point is included in the angle: Left,Eye,Right
	//   - This visibility test is only accurate if the camera cannot look up or down significantly.
	//
	const float PI_DIV_180 = M_PI / 180.0f;
	const float FOV_DIV_2 = pCamera->GetFOV()/2;

	int eyeX = (int)(pCamera->GetEyePt().x - PATCH_SIZE * sinf( pCamera->GetFOV() ));
	int eyeY = (int)(pCamera->GetEyePt().z + PATCH_SIZE * cosf( pCamera->GetFOV() ));

	int leftX  = (int)(eyeX + 100.0f * sinf( (pCamera->GetFOV()-FOV_DIV_2)));
	int leftY  = (int)(eyeY - 100.0f * cosf( (pCamera->GetFOV()-FOV_DIV_2)));

	int rightX = (int)(eyeX + 100.0f * sinf( (pCamera->GetFOV()+FOV_DIV_2)));
	int rightY = (int)(eyeY - 100.0f * cosf( (pCamera->GetFOV()+FOV_DIV_2)));

	int X, Y;
	Patch *patch;

	// Set the next free triangle pointer back to the beginning
	SetNextTriNode(0);

	// Reset rendered triangle count.
	m_NumTrisRendered = 0;

	// Go through the patches performing resets, compute variances, and linking.
	for ( Y=0; Y < NUM_PATCHES_PER_SIDE; Y++ )
		for ( X=0; X < NUM_PATCHES_PER_SIDE; X++)
		{
			patch = &(m_Patches[Y][X]);
			
			// Reset the patch
			patch->Reset();
			patch->SetVisibility( eyeX, eyeY, leftX, leftY, rightX, rightY );
			
			// Check to see if this patch has been deformed since last frame.
			// If so, recompute the varience tree for it.
			if ( patch->isDirty() )
				patch->ComputeVariance();

			if ( patch->isVisibile() )
			{
				// Link all the patches together.
				if ( X > 0 )
					patch->GetBaseLeft()->LeftNeighbor = m_Patches[Y][X-1].GetBaseRight();
				else
					patch->GetBaseLeft()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

				if ( X < (NUM_PATCHES_PER_SIDE-1) )
					patch->GetBaseRight()->LeftNeighbor = m_Patches[Y][X+1].GetBaseLeft();
				else
					patch->GetBaseRight()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

				if ( Y > 0 )
					patch->GetBaseLeft()->RightNeighbor = m_Patches[Y-1][X].GetBaseRight();
				else
					patch->GetBaseLeft()->RightNeighbor = NULL;		// Link to bordering Landscape here..

				if ( Y < (NUM_PATCHES_PER_SIDE-1) )
					patch->GetBaseRight()->RightNeighbor = m_Patches[Y+1][X].GetBaseLeft();
				else
					patch->GetBaseRight()->RightNeighbor = NULL;	// Link to bordering Landscape here..
			}
		}

}

// ---------------------------------------------------------------------
// Create an approximate mesh of the landscape.
//
void Landscape::Tessellate(CD3DCamera* pCamera)
{
	// Perform Tessellation
	int nCount;
	Patch *patch = &(m_Patches[0][0]);
	for (nCount=0; nCount < NUM_PATCHES_PER_SIDE*NUM_PATCHES_PER_SIDE; nCount++, patch++ )
	{
		if (patch->isVisibile())
			patch->Tessellate(pCamera,m_FrameVariance);
	}
}

// ---------------------------------------------------------------------
// Render each patch of the landscape & adjust the frame variance.
//
void Landscape::Render(DWORD DesiredTris,DWORD &FacestoDraw,DWORD* pIndexBuffer)
{
	int nCount;
	Patch *patch = &(m_Patches[0][0]);

	// Scale the terrain by the terrain scale specified at compile time.

	DWORD index = 0;
	for (nCount=0; nCount < NUM_PATCHES_PER_SIDE*NUM_PATCHES_PER_SIDE; nCount++, patch++ )
	{
		if (patch->isVisibile())
			patch->Render(m_NumTrisRendered,pIndexBuffer,index);
	}
	FacestoDraw = m_NumTrisRendered;

	// Check to see if we got close to the desired number of triangles.
	// Adjust the frame variance to a better value.
	if ( GetNextTriNode() != DesiredTris )
		m_FrameVariance += ((float)GetNextTriNode() - (float)DesiredTris) / (float)DesiredTris;

	// Bounds checking.
	if ( m_FrameVariance < 0 )
		m_FrameVariance = 0;
}




