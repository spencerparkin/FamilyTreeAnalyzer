// FtaKinematicCamera.cpp

#include "FtaKinematicCamera.h"
#include "FtaVisualization.h"
#include "FtaVizPanel.h"
#include "FtaCanvas.h"
#include "FtaFrame.h"
#include "FtaApp.h"

// TODO: You should be able to throw the camera into motion with a click and drag just as
//       you might do while swiping with your phone screen.

// TODO: Alternate click-&-drag should have you look in different directions.  This also
//       changes your panning plane.

FtaKinematicCamera::FtaKinematicCamera( void )
{
	dragCoeficient = 1.5;
	mass = 1.0;
}

/*virtual*/ FtaKinematicCamera::~FtaKinematicCamera( void )
{
}

/*virtual*/ void FtaKinematicCamera::PreRender( GLenum renderMode )
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

/*virtual*/ void FtaKinematicCamera::PostRender( GLenum renderMode )
{
	glFlush();
	
	if( renderMode == GL_RENDER )
		canvas->SwapBuffers();

	if( renderMode == GL_SELECT )
		ProcessHitBuffer( true );
}

/*virtual*/ bool FtaKinematicCamera::Bind( void )
{
	canvas->Bind( wxEVT_LEFT_DOWN, &FtaKinematicCamera::OnMouseLeftDown, this );
	canvas->Bind( wxEVT_KEY_DOWN, &FtaKinematicCamera::OnKeyDown, this );
	canvas->Bind( wxEVT_CHAR_HOOK, &FtaKinematicCamera::OnCharHook, this );

	return true;
}

/*virtual*/ bool FtaKinematicCamera::Unbind( void )
{
	canvas->Unbind( wxEVT_LEFT_DOWN, &FtaKinematicCamera::OnMouseLeftDown, this );
	canvas->Unbind( wxEVT_KEY_DOWN, &FtaKinematicCamera::OnKeyDown, this );
	canvas->Unbind( wxEVT_CHAR_HOOK, &FtaKinematicCamera::OnCharHook, this );

	return true;
}

/*virtual*/ bool FtaKinematicCamera::Animate( void )
{
	double speed = c3ga::norm( velocity );
	if( speed == 0.0 )
		return false;

	double deltaTime = 1.0 / canvas->GetFPS();
	eye += velocity * deltaTime;

	c3ga::vectorE3GA dragForce = -velocity * dragCoeficient;
	c3ga::vectorE3GA acceleration = dragForce * ( 1.0 / mass );
	velocity += acceleration * deltaTime;

	return true;
}

void FtaKinematicCamera::OnMouseLeftDown( wxMouseEvent& event )
{
	canvas->SetFocus();
}

void FtaKinematicCamera::OnKeyDown( wxKeyEvent& event )
{
}

void FtaKinematicCamera::OnCharHook( wxKeyEvent& event )
{
	c3ga::vectorE3GA moveDir( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, 0.f, 0.f );

	switch( event.GetKeyCode() )
	{
		case WXK_LEFT:
		{
			moveDir += c3ga::vectorE3GA( c3ga::vectorE3GA::coord_e1_e2_e3, -1.f, 0.f, 0.f );
			break;
		}
		case WXK_RIGHT:
		{
			moveDir += c3ga::vectorE3GA( c3ga::vectorE3GA::coord_e1_e2_e3, 1.f, 0.f, 0.f );
			break;
		}
		case WXK_UP:
		{
			if( event.ControlDown() )
				moveDir += c3ga::vectorE3GA( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, 0.f, -1.f );
			else
				moveDir += c3ga::vectorE3GA( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, 1.f, 0.f );
			break;
		}
		case WXK_DOWN:
		{
			if( event.ControlDown() )
				moveDir += c3ga::vectorE3GA( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, 0.f, 1.f );
			else
				moveDir += c3ga::vectorE3GA( c3ga::vectorE3GA::coord_e1_e2_e3, 0.f, -1.f, 0.f );
			break;
		}
	}

	double newtons = 40.0;
	c3ga::vectorE3GA force = moveDir * newtons;
	c3ga::vectorE3GA acceleration = force * ( 1.0 / mass );

	double deltaTime = 1.0 / canvas->GetFPS();
	velocity += acceleration * deltaTime;
}

// FtaKinematicCamera.cpp