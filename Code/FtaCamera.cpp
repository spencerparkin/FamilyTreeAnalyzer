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

	xAxis.set( c3ga::vectorE3GA::coord_e1_e2_e3, 1.f, 0.f, 0.f );
	yAxis.set( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, 1.f, 0.f );
	zAxis.set( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, 0.f, 1.f );
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
	
	GLfloat viewMatrix[16];

	// That that the inverse is of the orignation sub-matrix is its transpose.

	viewMatrix[0] = xAxis.get_e1();
	viewMatrix[1] = yAxis.get_e1();
	viewMatrix[2] = zAxis.get_e1();
	viewMatrix[3] = 0.f;

	viewMatrix[4] = xAxis.get_e2();
	viewMatrix[5] = yAxis.get_e2();
	viewMatrix[6] = zAxis.get_e2();
	viewMatrix[7] = 0.f;

	viewMatrix[8] = xAxis.get_e3();
	viewMatrix[9] = yAxis.get_e3();
	viewMatrix[10] = zAxis.get_e3();
	viewMatrix[11] = 0.f;

	viewMatrix[12] = -eye.get_e1();
	viewMatrix[13] = -eye.get_e2();
	viewMatrix[14] = -eye.get_e3();
	viewMatrix[15] = 1.f;

	glMultMatrixf( viewMatrix );
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

// FtaCamera.cpp