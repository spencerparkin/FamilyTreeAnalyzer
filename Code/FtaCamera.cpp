// FtaCamera.cpp

#include "FtaCamera.h"
#include "FtaCanvas.h"
#include "FtaVisualization.h"
#include <gl/GLU.h>

FtaCamera::FtaCamera( void )
{
	hitBuffer = nullptr;
	hitBufferSize = 0;

	eye.set( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, 0.f, 20.f );

	orient.set( c3ga::rotorE3GA::coord_scalar_e1e2_e2e3_e3e1, 1.0, 0.0, 0.0, 0.0 );
}

/*virtual*/ FtaCamera::~FtaCamera( void )
{
}

void FtaCamera::SetupViewMatrices( GLenum renderMode )
{
	GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );
	double aspectRatio = double( viewport[2] ) / double( viewport[3] );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	if( renderMode == GL_SELECT )
	{
		// TODO: Get mouse position from canvas.
		//gluPickMatrix( GLdouble( mousePos.x ), GLdouble( viewport[3] - mousePos.y - 1 ), 2.0, 2.0, viewport );
	}

	GLfloat foviAngle = 60.f;
	gluPerspective( foviAngle, aspectRatio, 0.1, 1000.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	c3ga::vectorE3GA up( c3ga::vectorE3GA::coord_e1_e2_e3, 0.0, 1.0, 0.0 );
	c3ga::vectorE3GA forward( c3ga::vectorE3GA::coord_e1_e2_e3, 0.0, 0.0, -1.0 );

	c3ga::applyUnitVersor( orient, up );
	c3ga::applyUnitVersor( orient, forward );

	c3ga::vectorE3GA target = eye + forward;

	gluLookAt( eye.get_e1(), eye.get_e2(), eye.get_e3(),
				target.get_e1(), target.get_e2(), target.get_e3(),
				up.get_e1(), up.get_e2(), up.get_e3() );
}

void FtaCamera::PrepareHitBuffer( void )
{
	wxASSERT( hitBuffer == nullptr );

	hitBufferSize = 512;
	hitBuffer = new GLuint[ hitBufferSize ];
	glSelectBuffer( hitBufferSize, hitBuffer );
	glRenderMode( GL_SELECT );
	glInitNames();
}

void FtaCamera::ProcessHitBuffer( bool freeHitBuffer /*= true*/ )
{
	GLuint hitCount = glRenderMode( GL_RENDER );

	FtaVisualization* viz = canvas->GetVisualization();
	if( viz )
	{
		if( viz->ProcessHitBuffer( hitBuffer, hitBufferSize, hitCount ) )
			canvas->Refresh();
	}

	if( freeHitBuffer )
	{
		delete[] hitBuffer;
		hitBuffer = nullptr;
		hitBufferSize = 0;
	}
}

/*virtual*/ bool FtaCamera::ViewBoundingBox( const FtaAxisAlignedBox& aab )
{
	// TODO: Write this after I've rewritten all the camera stuff/model.
	return false;
}

// FtaCamera.cpp