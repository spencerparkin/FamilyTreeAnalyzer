// FtaBasicCamera.cpp

#include "FtaBasicCamera.h"
#include "FtaVisualization.h"
#include "FtaVizPanel.h"
#include "FtaCanvas.h"
#include "FtaFrame.h"
#include "FtaApp.h"
#include <wx/utils.h>

FtaBasicCamera::FtaBasicCamera( void )
{
	moveUnitsPerSecond = 30.0;
}

/*virtual*/ FtaBasicCamera::~FtaBasicCamera( void )
{
}

/*virtual*/ void FtaBasicCamera::PreRender( GLenum renderMode )
{
	if( renderMode == GL_SELECT )
		PrepareHitBuffer();

	glClearColor( 1.f, 1.f, 1.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );

	SetupViewMatrices( renderMode );

	// TODO: Setup lighting.
}

/*virtual*/ void FtaBasicCamera::PostRender( GLenum renderMode )
{
	glFlush();
	
	if( renderMode == GL_RENDER )
		canvas->SwapBuffers();

	if( renderMode == GL_SELECT )
		ProcessHitBuffer( true );
}

/*virtual*/ bool FtaBasicCamera::Bind( void )
{
	canvas->Bind( wxEVT_LEFT_DOWN, &FtaBasicCamera::OnMouseLeftDown, this );
	canvas->Bind( wxEVT_LEFT_UP, &FtaBasicCamera::OnMouseLeftUp, this );
	//canvas->Bind( wxEVT_MOTION, &FtaBasicCamera::OnMouseMove, this );
	//canvas->Bind( wxEVT_MOUSE_CAPTURE_LOST, &FtaBasicCamera::OnMouseCaptureLost, this );
	//canvas->Bind( wxEVT_MOUSEWHEEL, &FtaBasicCamera::OnMouseWheel, this );

	return true;
}

/*virtual*/ bool FtaBasicCamera::Unbind( void )
{
	canvas->Unbind( wxEVT_LEFT_DOWN, &FtaBasicCamera::OnMouseLeftDown, this );
	canvas->Unbind( wxEVT_LEFT_UP, &FtaBasicCamera::OnMouseLeftUp, this );
	//canvas->Unbind( wxEVT_MOTION, &FtaBasicCamera::OnMouseMove, this );
	//canvas->Unbind( wxEVT_MOUSE_CAPTURE_LOST, &FtaBasicCamera::OnMouseCaptureLost, this );
	//canvas->Unbind( wxEVT_MOUSEWHEEL, &FtaBasicCamera::OnMouseWheel, this );

	return true;
}

/*virtual*/ bool FtaBasicCamera::Animate( void )
{
	if( !canvas->HasFocus() )
		return false;

	double framesPerSecond = canvas->GetFPS();
	if( framesPerSecond == 0.0 )
		return false;

	c3ga::vectorE3GA moveDir( c3ga::vectorE3GA::coord_e1_e2_e3, 0.0, 0.0, 0.0 );

	c3ga::vectorE3GA xAxis, yAxis, zAxis;
	BuildCameraFrame( xAxis, yAxis, zAxis );

	if( wxGetKeyState( WXK_UP ) )
	{
		if( wxGetKeyState( WXK_ALT ) )
			moveDir += -zAxis;
		else
			moveDir += yAxis;
	}

	if( wxGetKeyState( WXK_DOWN ) )
	{
		if( wxGetKeyState( WXK_ALT ) )
			moveDir += zAxis;
		else
			moveDir += -yAxis;
	}

	if( wxGetKeyState( WXK_LEFT ) )
		moveDir += -xAxis;

	if( wxGetKeyState( WXK_RIGHT ) )
		moveDir += xAxis;
	
	double moveDirNorm = c3ga::norm( moveDir );
	if( moveDirNorm == 0.0 )
		return false;
	
	moveDir = moveDir * ( 1.0 / moveDirNorm );

	double moveUnitsPerFrame = moveUnitsPerSecond / framesPerSecond;
		
	c3ga::vectorE3GA moveDelta = moveDir * moveUnitsPerFrame;

	double length = 0.0;
	if( wxGetKeyState( WXK_CONTROL ) )
		length = c3ga::norm( target - eye );

	eye += moveDelta;

	if( wxGetKeyState( WXK_CONTROL ) )
	{
		eye = target + c3ga::unit( eye - target ) * length;
		// TODO: Recompute up-vector.
	}
	else
	{
		target += moveDelta;
		target.m_e3 = 0.0;
	}

	return true;
}

void FtaBasicCamera::OnMouseLeftDown( wxMouseEvent& event )
{
	if( !canvas->HasFocus() )
		canvas->SetFocus();

	//...
}

void FtaBasicCamera::OnMouseLeftUp( wxMouseEvent& event )
{
	//...
}

// FtaBasicCamera.cpp