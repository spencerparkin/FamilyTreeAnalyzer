// FtaLuaPanel.cpp

#include "FtaLuaPanel.h"
#include "FtaLuaUtil.h"
#include "FtaApp.h"
#include <wx/font.h>
#include <wx/fontdlg.h>

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
	paneInfo.Dockable();
	paneInfo.Name( "LuaPanel" );
	return true;
}

/*virtual*/ bool FtaLuaPanel::MakeControls( void )
{
	textCtrl = new wxStyledTextCtrl( this, wxID_ANY );
	textCtrl->StyleClearAll();
	textCtrl->SetLexer( wxSTC_LEX_LUA );
	
	wxFont monoFont;
	monoFont.SetFaceName( "Courier New" );
	monoFont.SetFamily( wxFONTFAMILY_MODERN );
	
	wxFont monoFontBold;
	monoFontBold.SetFaceName( "Courier New" );
	monoFontBold.SetFamily( wxFONTFAMILY_MODERN );
	monoFontBold.MakeBold();

	textCtrl->StyleSetFont( wxSTC_LUA_DEFAULT, monoFont );
	textCtrl->StyleSetFont( wxSTC_LUA_COMMENT, monoFontBold );
	textCtrl->StyleSetFont( wxSTC_LUA_COMMENTLINE, monoFontBold );
	textCtrl->StyleSetFont( wxSTC_LUA_COMMENTDOC, monoFontBold );
	textCtrl->StyleSetFont( wxSTC_LUA_NUMBER, monoFont );
	textCtrl->StyleSetFont( wxSTC_LUA_WORD, monoFontBold );
	textCtrl->StyleSetFont( wxSTC_LUA_STRING, monoFont );
	textCtrl->StyleSetFont( wxSTC_LUA_CHARACTER, monoFont );
	textCtrl->StyleSetFont( wxSTC_LUA_LITERALSTRING, monoFont );
	textCtrl->StyleSetFont( wxSTC_LUA_PREPROCESSOR, monoFont );
	textCtrl->StyleSetFont( wxSTC_LUA_OPERATOR, monoFont );
	textCtrl->StyleSetFont( wxSTC_LUA_IDENTIFIER, monoFont );
	textCtrl->StyleSetFont( wxSTC_LUA_STRINGEOL, monoFont );
	textCtrl->StyleSetFont( wxSTC_LUA_LABEL, monoFont );

	textCtrl->StyleSetForeground( wxSTC_LUA_DEFAULT, wxColour( 0, 0, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_COMMENT, wxColour( 0, 128, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_COMMENTLINE, wxColour( 0, 128, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_COMMENTDOC, wxColour( 0, 128, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_NUMBER, wxColour( 0, 0, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_WORD, wxColour( 0, 0, 255 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_STRING, wxColour( 128, 0, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_CHARACTER, wxColour( 128, 0, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_LITERALSTRING, wxColour( 128, 0, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_PREPROCESSOR, wxColour( 0, 0, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_OPERATOR, wxColour( 0, 0, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_IDENTIFIER, wxColour( 0, 0, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_STRINGEOL, wxColour( 0, 0, 0 ) );
	textCtrl->StyleSetForeground( wxSTC_LUA_LABEL, wxColour( 0, 0, 0 ) );

	textCtrl->SetKeyWords( 0, "and break do else elseif end false for function if in local nil not or repeat return then true until while require" );

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