// FtaBasicCamera.h

#pragma once

#include "FtaCamera.h"

class FtaBasicCamera : public FtaCamera
{
public:

	FtaBasicCamera( void );
	virtual ~FtaBasicCamera( void );

	virtual void PreRender( GLenum renderMode ) override;
	virtual void PostRender( GLenum renderMode ) override;

	virtual bool Bind( void ) override;
	virtual bool Unbind( void ) override;

	virtual bool Animate( void ) override;

	void OnMouseLeftDown( wxMouseEvent& event );
	void OnMouseLeftUp( wxMouseEvent& event );

protected:

	GLfloat moveUnitsPerSecond;
};

// FtaBasicCamera.h
