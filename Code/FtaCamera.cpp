// FtaCamera.cpp

#include "FtaCamera.h"
#include "FtaCanvas.h"
#include "FtaVisualization.h"
#include <gl/GLU.h>

FtaCamera::FtaCamera( void )
{
	hitBuffer = nullptr;
	hitBufferSize = 0;

	SetFovi( 60.f );

	eye.set( c3ga::vectorE3GA::coord_e1_e2_e3, 0.0, 0.0, 20.0 );
	up.set( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, 1.f, 0.f );
	target.set( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, 0.f, 0.0 );
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

	gluPerspective( fovi, aspectRatio, 0.1, 1000.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( eye.get_e1(), eye.get_e2(), eye.get_e3(),
				target.get_e1(), target.get_e2(), target.get_e3(),
				up.get_e1(), up.get_e2(), up.get_e3() );
}

// Build a right-handed system about the camera's eye with the -Z-axis facing target.
void FtaCamera::BuildCameraFrame( c3ga::vectorE3GA& xAxis, c3ga::vectorE3GA& yAxis, c3ga::vectorE3GA& zAxis )
{
	zAxis = c3ga::unit( eye - target );
	xAxis = c3ga::gp( c3ga::op( zAxis, up ), c3ga::I3 );
	xAxis = c3ga::unit( xAxis );
	yAxis = c3ga::gp( c3ga::op( xAxis, zAxis ), c3ga::I3 );
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

/*virtual*/ bool FtaCamera::GetHUDString( wxString& hudString )
{
	c3ga::vectorE3GA look = c3ga::unit( target - eye );
	hudString = wxString::Format( "eye: < %1.2f, %1.2f, %1.2f >, look: < %1.2f, %1.2f, %1.2f >",
										eye.m_e1, eye.m_e2, eye.m_e3,
										look.m_e1, look.m_e2, look.m_e3 );
	return true;
}

// FtaCamera.cpp