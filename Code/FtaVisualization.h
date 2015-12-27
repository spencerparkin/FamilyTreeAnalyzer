// FtaVisualization.h

#pragma once

#include <wx/glcanvas.h>
#include "FtaCanvasPlugin.h"

// Derivatives of this class represent anything we would wish to view/visualize in the viz-panel.
class FtaVisualization : public FtaCanvasPlugin
{
public:

	FtaVisualization( void );
	virtual ~FtaVisualization( void );

	virtual bool Draw( GLenum renderMode ) = 0;
	virtual bool ProcessHitBuffer( GLuint* hitBuffer, GLuint hitBufferSize, GLuint hitCount );

	// TODO: Every visualization should be able to calculate and report back a minimum bounding box for it
	//       that a camera could use to view the whole thing.
};

// FtaVisualization.h