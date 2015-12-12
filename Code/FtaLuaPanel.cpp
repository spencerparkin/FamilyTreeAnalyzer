// FtaLuaPanel.cpp

#include "FtaLuaPanel.h"
#include "FtaLuaUtil.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaLuaPanel, FtaPanel );

FtaLuaPanel::FtaLuaPanel( void )
{
	textCtrl = nullptr;
}

/*virtual*/ FtaLuaPanel::~FtaLuaPanel( void )
{
}

/*virtual*/ bool FtaLuaPanel::GetPaneInfo( wxAuiPaneInfo& paneInfo )
{
	paneInfo.Caption( "Lua" );
	paneInfo.CenterPane();
	return true;
}

/*virtual*/ bool FtaLuaPanel::MakeControls( void )
{
	textCtrl = new wxStyledTextCtrl( this, wxID_ANY );
	textCtrl->StyleClearAll();
	textCtrl->SetLexer( wxSTC_LEX_LUA );

	wxButton* executeButton = new wxButton( this, wxID_ANY, "Execute" );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( textCtrl, 1, wxALL | wxGROW, 0 );
	boxSizer->Add( executeButton, 0, wxGROW );
	SetSizer( boxSizer );

	Bind( wxEVT_BUTTON, &FtaLuaPanel::OnExecuteButtonPressed, this );

	return true;
}

void FtaLuaPanel::OnExecuteButtonPressed( wxCommandEvent& event )
{
	lua_State* L = wxGetApp().GetLuaState();

	FtaLuaStackPopper stackPopper(L);

	wxString code = textCtrl->GetValue();

	int result = luaL_loadbuffer( L, code, code.Length(), "FtaChunk" );
	if( FtaLuaReportError( L, result ) )
		return;

	result = lua_pcall( L, 0, 0, 0 );
	if( FtaLuaReportError( L, result ) )
		return;

	textCtrl->SetFocus();
}

/*virtual*/ bool FtaLuaPanel::TimerUpdate( void )
{
	return true;
}

// FtaLuaPanel.cpp