// FtaQueryUserForString.cpp

#include "FtaQueryUserForString.h"
#include "FtaFrame.h"
#include "FtaApp.h"
#include <wx/textdlg.h>

wxIMPLEMENT_DYNAMIC_CLASS( FtaQueryUserForString, FtaLuaFunction );

FtaQueryUserForString::FtaQueryUserForString( void )
{
}

/*virtual*/ FtaQueryUserForString::~FtaQueryUserForString( void )
{
}

/*virtual*/ int FtaQueryUserForString::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
		"Use the queryUserForString() function to ask the user for string input.  "
		"A single table argument is expected with the fields \"question\" and \"default\".  "
		"The first of these is used to prompt the user.  The second is a default string choice.  "
		"False is returned in the user canceled.  If the user did not cancel, then the field "
		"\"string\" is added to the given table and contains the user's answer." );
	return 0;
}

/*virtual*/ int FtaQueryUserForString::Call( lua_State* L )
{
	if( lua_gettop(L) < 1 )
		return luaL_error( L, "Expected an argument" );

	if( !lua_istable( L, -1 ) )
		return luaL_error( L, "Expected a table argument" );

	lua_getfield( L, -1, "question" );
	if( !lua_isstring( L, -1 ) )
		return luaL_error( L, "No question prompt given." );

	wxString questionPrompt = lua_tostring( L, -1 );
	lua_pop( L, 1 );

	wxString defaultString;
	lua_getfield( L, -1, "default" );
	if( lua_isstring( L, -1 ) )
		defaultString = lua_tostring( L, -1 );
	lua_pop( L, 1 );

	wxString chosenString = wxGetTextFromUser( questionPrompt, "User Input", defaultString, wxGetApp().GetFrame() );
	if( chosenString.IsEmpty() )
	{
		lua_pushboolean( L, false );
		return 1;
	}

	lua_pushstring( L, chosenString );
	lua_setfield( L, -2, "string" );

	lua_pushboolean( L, true );
	return 1;
}

// FtaQueryUserForString.cpp