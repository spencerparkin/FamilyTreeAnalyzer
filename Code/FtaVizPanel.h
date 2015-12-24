// FtaVizPanel.h

#pragma once

#include "FtaPanel.h"
#include "FtaVisualization.h"
#include "FtaCamera.h"
#include <wx/glcanvas.h>

class FtaCanvas;

class FtaVizPanel : public FtaPanel
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaVizPanel );

	FtaVizPanel( void );
	virtual ~FtaVizPanel( void );

	virtual bool GetPaneInfo( wxAuiPaneInfo& paneInfo );
	virtual bool MakeControls( void );
	virtual bool TimerUpdate( void );

	FtaCanvas* GetCanvas( void ) { return canvas; }

private:

	FtaCanvas* canvas;
};

// FtaVizPanel.h
