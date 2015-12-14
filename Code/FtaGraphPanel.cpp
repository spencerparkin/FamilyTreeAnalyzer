// FtaGraphPanel.cpp

#include "FtaGraphPanel.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaGraphPanel, FtaPanel );

FtaGraphPanel::FtaGraphPanel( void )
{
}

/*virtual*/ FtaGraphPanel::~FtaGraphPanel( void )
{
}

/*virtual*/ bool FtaGraphPanel::GetPaneInfo( wxAuiPaneInfo& paneInfo )
{
	paneInfo.Caption( "Graph" );
	paneInfo.CenterPane();
	paneInfo.Name( "GraphPanel" );
	return true;
}

/*virtual*/ bool FtaGraphPanel::MakeControls( void )
{
	return true;
}

/*virtual*/ bool FtaGraphPanel::TimerUpdate( void )
{
	return true;
}

// FtaGraphPanel.cpp