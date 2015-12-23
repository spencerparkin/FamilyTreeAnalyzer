// FtaLuaPanel.cpp

#include "FtaLuaPanel.h"
#include "FtaLuaUtil.h"
#include "FtaApp.h"
#include <wx/font.h>
#include <wx/fontdlg.h>
#include <wx/file.h>

wxIMPLEMENT_DYNAMIC_CLASS( FtaLuaPanel, FtaPanel );

FtaLuaPanel::FtaLuaPanel( void )
{
	textCtrl = nullptr;
	historyLocation = -1;
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

// TODO: Support line numbers and perhaps ctrl-G and ctrl-F.
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
	Bind( wxEVT_CHAR_HOOK, &FtaLuaPanel::OnCharHook, this );

	FileDropTarget* fileDropTarget = new FileDropTarget();
	fileDropTarget->textCtrl = textCtrl;
	textCtrl->SetDropTarget( fileDropTarget );

	return true;
}

void FtaLuaPanel::OnExecuteButtonPressed( wxCommandEvent& event )
{
	Execute();
}

bool FtaLuaPanel::Execute( void )
{
	lua_State* L = wxGetApp().GetLuaState();

	FtaLuaStackPopper stackPopper(L);

	wxString code = textCtrl->GetValue();

	int result = luaL_loadbuffer( L, code, code.Length(), "FtaChunk" );
	if( FtaLuaReportError( L, result ) )
		return false;

	result = lua_pcall( L, 0, 0, 0 );
	if( FtaLuaReportError( L, result ) )
		return false;

	codeHistory.Add( code );
	historyLocation = codeHistory.GetCount() - 1;

	textCtrl->SetText( "" );
	textCtrl->SetFocus();

	return true;
}

/*virtual*/ bool FtaLuaPanel::TimerUpdate( void )
{
	return true;
}

void FtaLuaPanel::OnCharHook( wxKeyEvent& event )
{
	if( !event.ControlDown() )
	{
		event.Skip();
		return;
	}

	// TODO: I need to fix this, but my mind is mush right now.
	switch( event.GetKeyCode() )
	{
		case WXK_UP:
		{
			if( historyLocation < ( signed )codeHistory.GetCount() - 1 )
				historyLocation++;
			break;
		}
		case WXK_DOWN:
		{
			if( historyLocation > 0 )
				historyLocation--;
			break;
		}
	}

	switch( event.GetKeyCode() )
	{
		case WXK_UP:
		case WXK_DOWN:
		{
			wxString code = codeHistory[ historyLocation ];
			textCtrl->SetText( code );
			break;
		}
		case WXK_RETURN:
		{
			Execute();
			break;
		}
	}
}

FtaLuaPanel::FileDropTarget::FileDropTarget( void )
{
	textCtrl = nullptr;
}

/*virtual*/ FtaLuaPanel::FileDropTarget::~FileDropTarget( void )
{
}

/*virtual*/ bool FtaLuaPanel::FileDropTarget::OnDropFiles( wxCoord x, wxCoord y, const wxArrayString& fileNames )
{
	if( !textCtrl )
		return false;

	for( unsigned int i = 0; i < fileNames.GetCount(); i++ )
	{
		wxString filePath = fileNames[i];
		wxFile file( filePath, wxFile::read );
		wxString code;
		if( !file.ReadAll( &code ) )
			continue;

		textCtrl->AppendText( "-- " + filePath + "\n\n" );
		textCtrl->AppendText( code );
		textCtrl->AppendText( "\n\n" );
	}

	return true;
}

// FtaLuaPanel.cpp