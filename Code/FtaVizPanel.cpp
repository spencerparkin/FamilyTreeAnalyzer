// FtaVizPanel.cpp

#include "FtaVizPanel.h"
#include "FtaCanvas.h"
#include <wx/sizer.h>

wxIMPLEMENT_DYNAMIC_CLASS( FtaVizPanel, FtaPanel );

FtaVizPanel::FtaVizPanel( void )
{
	canvas = nullptr;
}

/*virtual*/ FtaVizPanel::~FtaVizPanel( void )
{
}

/*virtual*/ bool FtaVizPanel::GetPaneInfo( wxAuiPaneInfo& paneInfo )
{
	paneInfo.Caption( "Visualization" );
	paneInfo.CenterPane();
	paneInfo.Name( "VizPanel" );
	return true;
}

/*virtual*/ bool FtaVizPanel::MakeControls( void )
{
	canvas = new FtaCanvas( this );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( canvas, 1, wxALL | wxGROW, 0 );
	SetSizer( boxSizer );

	return true;
}

/*virtual*/ bool FtaVizPanel::TimerUpdate( void )
{
	FtaVisualization* viz = canvas->GetVisualization();
	if( viz && viz->Animate() )
		canvas->Refresh();

	return true;
}

// FtaVizPanel.cpp