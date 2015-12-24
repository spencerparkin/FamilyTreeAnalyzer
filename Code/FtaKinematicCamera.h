// FtaKinematicCamera.h

#pragma once

#include "FtaCamera.h"

class FtaKinematicCamera : public FtaCamera
{
public:

	FtaKinematicCamera( void );
	virtual ~FtaKinematicCamera( void );

	virtual void PreRender( GLenum renderMode, FtaCanvas* canvas ) override;
	virtual void PostRender( GLenum renderMode, FtaCanvas* canvas ) override;

	virtual bool Bind( void ) override;
	virtual bool Unbind( void ) override;

protected:

	//c3ga::vectorE3GA subject;
	c3ga::vectorE3GA velocity, acceleration;
	//float dragCoeficient;
};

// FtaKinematicCamera.h
