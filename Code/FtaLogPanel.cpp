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
	textCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE );

	wxFont monoFont;
	monoFont.SetFaceName( "Courier New" );
	monoFont.SetFamily( wxFONTFAMILY_MODERN );
	textCtrl->SetFont( monoFont );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( textCtrl, 1, wxALL | wxGROW, 0 );
	SetSizer( boxSizer );

	return true;
}

void FtaLogPanel::AddLogMessage( const wxString& message )
{
	textCtrl->AppendText( message + "\n" );
}

void FtaLogPanel::ClearLog( void )
{
	textCtrl->Clear();
}

bool FtaLogPanel::IsEmpty( void )
{
	return textCtrl->IsEmpty();
}

// FtaLogPanel.cpp