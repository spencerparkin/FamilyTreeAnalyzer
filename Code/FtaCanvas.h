// FtaCanvas.h

#pragma once

#include <wx/glcanvas.h>
#include "FtaVisualization.h"
#include "FtaCamera.h"

class FtaCanvas : public wxGLCanvas
{
public:

	FtaCanvas( wxWindow* parent );
	virtual ~FtaCanvas( void );

	void OnPaint( wxPaintEvent& event );
	void OnSize( wxSizeEvent& event );

	void PerformPick( void );

	GLuint GenerateTexture( const wxImage& image );
	void FreeTexture( GLuint texture );

	FtaVisualization* GetVisualization( void ) { return viz; }
	void SetVisualization( FtaVisualization* viz );

	FtaCamera* GetCamera( void ) { return camera; }
	void SetCamera( FtaCamera* camera );

private:

	void SwapPlugin( FtaCanvasPlugin*& curPlugin, FtaCanvasPlugin* newPlugin );

	void BindContext( void );

	wxGLContext* context;
	static int attributeList[];
	FtaVisualization* viz;
	FtaCamera* camera;
};

// FtaCanvas.h