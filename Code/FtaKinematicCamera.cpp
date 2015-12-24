// FtaKinematicCamera.cpp

#include "FtaKinematicCamera.h"
#include "FtaVisualization.h"
#include "FtaCanvas.h"

FtaKinematicCamera::FtaKinematicCamera( void )
{
}

/*virtual*/ FtaKinematicCamera::~FtaKinematicCamera( void )
{
}

/*virtual*/ void FtaKinematicCamera::PreRender( GLenum renderMode, FtaCanvas* canvas )
{
	if( renderMode == GL_SELECT )
		PrepareHitBuffer();

	glClearColor( 1.f, 1.f, 1.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );

	SetupViewMatrices( renderMode, canvas );
}

/*virtual*/ void FtaKinematicCamera::PostRender( GLenum renderMode, FtaCanvas* canvas )
{
	glFlush();
	
	if( renderMode == GL_RENDER )
		canvas->SwapBuffers();

	if( renderMode == GL_SELECT )
		ProcessHitBuffer( canvas, true );
}

/*virtual*/ bool FtaKinematicCamera::Bind( void )
{
	return true;
}

/*virtual*/ bool FtaKinematicCamera::Unbind( void )
{
	return true;
}

// FtaKinematicCamera.cpp