// FtaVisualization.h

#pragma once

#include <wx/glcanvas.h>

// Derivatives of this class represent anything we would wish to view/visualize in the viz-panel.
class FtaVisualization
{
public:

	FtaVisualization( void );
	virtual ~FtaVisualization( void );

	virtual bool Draw( GLenum renderMode ) = 0;

	// TODO: Perhaps provide some virtual methods here to handle key-board input in the case that the viz is interactive.
	// TODO: Perhaps provide some virtual methods here to handle the processing of hit records for OpenGL selection.
};

// FtaVisualization.h