// FtaGraphPanel.h

#pragma once

#include "FtaPanel.h"

// TODO: Own a wxCanvas control.  Draw family tree using a spring-lattice method.
class FtaGraphPanel : public FtaPanel
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaGraphPanel );

	FtaGraphPanel( void );
	virtual ~FtaGraphPanel( void );

	virtual bool GetPaneInfo( wxAuiPaneInfo& paneInfo );
	virtual bool MakeControls( void );
	virtual bool TimerUpdate( void );
};

// FtaGraphPanel.h
