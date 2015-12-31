// FtaVisualization.cpp

#include "FtaVisualization.h"

FtaVisualization::FtaVisualization( void )
{
}

/*virtual*/ FtaVisualization::~FtaVisualization( void )
{
}

/*virtual*/ bool FtaVisualization::ProcessHitBuffer( GLuint* hitBuffer, GLuint hitBufferSize, GLuint hitCount )
{
	return false;
}

/*virtual*/ bool FtaVisualization::CalcBoundingBox( FtaAxisAlignedBox& aab )
{
	return false;
}

// FtaVisualization.cpp
