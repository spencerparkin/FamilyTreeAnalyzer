// FtaCamera.h

#pragma once

#include "FtaCanvasPlugin.h"
#include "FtaAxisAlignedBox.h"
#include "c3ga/c3ga.h"
#include <wx/glcanvas.h>

class FtaCanvas;

class FtaCamera : public FtaCanvasPlugin
{
public:

	FtaCamera( void );
	virtual ~FtaCamera( void );

	virtual void PreRender( GLenum renderMode ) = 0;
	virtual void PostRender( GLenum renderMode ) = 0;

	virtual bool ViewBoundingBox( const FtaAxisAlignedBox& aab );

	GLuint* GetHitBuffer( void ) { return hitBuffer; }
	GLuint GetHitBufferSize( void ) const { return hitBufferSize; }

protected:

	void SetupViewMatrices( GLenum renderMode );

	void PrepareHitBuffer( void );
	void ProcessHitBuffer( bool freeHitBuffer = true );

	// TODO: This is stupid and needs to be rewritten.
	c3ga::vectorE3GA eye;
	c3ga::rotorE3GA orient;

	GLuint* hitBuffer;
	GLuint hitBufferSize;
};

// FtaCamera.h
