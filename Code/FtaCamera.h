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

	virtual bool GetHUDString( wxString& hudString ) override;

	GLuint* GetHitBuffer( void ) { return hitBuffer; }
	GLuint GetHitBufferSize( void ) const { return hitBufferSize; }

	void SetFovi( GLfloat fovi ) { this->fovi = fovi; }
	GLfloat GetFovi( void ) { return fovi; }

protected:

	void SetupViewMatrices( GLenum renderMode );
	void BuildCameraFrame( c3ga::vectorE3GA& xAxis, c3ga::vectorE3GA& yAxis, c3ga::vectorE3GA& zAxis );

	void PrepareHitBuffer( void );
	void ProcessHitBuffer( bool freeHitBuffer = true );

	GLfloat fovi;
	c3ga::vectorE3GA eye, up, target;

	GLuint* hitBuffer;
	GLuint hitBufferSize;
};

// FtaCamera.h
