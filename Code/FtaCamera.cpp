// FtaCamera.cpp

#include "FtaCamera.h"
#include "FtaCanvas.h"
#include "FtaVisualization.h"
#include <gl/GLU.h>

FtaCamera::FtaCamera( void )
{
	hitBuffer = nullptr;
	hitBufferSize = 0;
}

/*virtual*/ FtaCamera::~FtaCamera( void )
{
}

void FtaCamera::SetupViewMatrices( GLenum renderMode, FtaCanvas* canvas )
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
	
	GLfloat viewMatrix[4][4];

	viewMatrix[0][0] = xAxis.get_e1();
	viewMatrix[1][0] = xAxis.get_e2();
	viewMatrix[2][0] = xAxis.get_e3();
	viewMatrix[3][0] = 0.f;

	viewMatrix[0][1] = yAxis.get_e1();
	viewMatrix[1][1] = yAxis.get_e2();
	viewMatrix[2][1] = yAxis.get_e3();
	viewMatrix[3][1] = 0.f;

	viewMatrix[0][2] = zAxis.get_e1();
	viewMatrix[1][2] = zAxis.get_e2();
	viewMatrix[2][2] = zAxis.get_e3();
	viewMatrix[3][2] = 0.f;

	viewMatrix[0][3] = eye.get_e1();
	viewMatrix[1][3] = eye.get_e2();
	viewMatrix[2][3] = eye.get_e3();
	viewMatrix[3][3] = 1.f;

	glLoadMatrixf( ( GLfloat* )viewMatrix );
}

void FtaCamera::PrepareHitBuffer( void )
{
	hitBufferSize = 512;
	hitBuffer = new GLuint[ hitBufferSize ];
	glSelectBuffer( hitBufferSize, hitBuffer );
	glRenderMode( GL_SELECT );
	glInitNames();
}

void FtaCamera::ProcessHitBuffer( FtaCanvas* canvas, bool freeHitBuffer /*= true*/ )
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