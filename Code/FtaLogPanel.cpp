// FtaLogPanel.cpp

#include "FtaLogPanel.h"
#include <wx/sizer.h>

wxIMPLEMENT_DYNAMIC_CLASS( FtaLogPanel, FtaPanel );

FtaLogPanel::FtaLogPanel( void )
{
	textCtrl = nullptr;
}

/*virtual*/ FtaLogPanel::~FtaLogPanel( void )
{
}

/*virtual*/ bool FtaLogPanel::GetPaneInfo( wxAuiPaneInfo& paneInfo )
{
	paneInfo.Caption( "Log" );
	paneInfo.Dockable();
	paneInfo.BestSize( 300, 300 );
	return true;
}

/*virtual*/ bool FtaLogPanel::MakeControls( void )
{
	// TODO: Make it read-only.
	textCtrl = new wxStyledTextCtrl( this, wxID_ANY );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( textCtrl, 1, wxALL | wxGROW, 0 );
	SetSizer( boxSizer );

	return true;
}

void FtaLogPanel::AddLogMessage( const wxString& message )
{
	textCtrl->AddText( message + "\n" );
}

void FtaLogPanel::ClearLog( void )
{
	textCtrl->SetText( "" );
}

bool FtaLogPanel::IsEmpty( void )
{
	return textCtrl->IsEmpty();
}

// FtaLogPanel.cpp