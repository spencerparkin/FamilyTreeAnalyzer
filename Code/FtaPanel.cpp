// FtaPanel.cpp

#include "FtaPanel.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaPanel, wxPanel );

FtaPanel::FtaPanel( void )
{
}

/*virtual*/ FtaPanel::~FtaPanel( void )
{
}

/*virtual*/ bool FtaPanel::GetPaneInfo( wxAuiPaneInfo& paneInfo )
{
	return false;
}

/*virtual*/ bool FtaPanel::MakeControls( void )
{
	return false;
}

/*virtual*/ bool FtaPanel::TimerUpdate( void )
{
	return false;
}

// FtaPanel.cpp