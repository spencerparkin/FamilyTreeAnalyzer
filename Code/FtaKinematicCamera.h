// FtaKinematicCamera.h

#pragma once

#include "FtaCamera.h"

class FtaKinematicCamera : public FtaCamera
{
public:

	FtaKinematicCamera( void );
	virtual ~FtaKinematicCamera( void );

	virtual void PreRender( GLenum renderMode ) override;
	virtual void PostRender( GLenum renderMode ) override;

	virtual bool Bind( void ) override;
	virtual bool Unbind( void ) override;

	virtual bool Animate( void ) override;

	void OnMouseLeftDown( wxMouseEvent& event );
	void OnKeyDown( wxKeyEvent& event );
	void OnCharHook( wxKeyEvent& event );

protected:

	//c3ga::vectorE3GA subject;
	c3ga::vectorE3GA velocity;
	double dragCoeficient;
	double mass;
};

// FtaKinematicCamera.h
