// FtaQueryUserForInteger.cpp

#include "FtaQueryUserForInteger.h"
#include "FtaFrame.h"
#include "FtaApp.h"
#include <wx/numdlg.h>

wxIMPLEMENT_DYNAMIC_CLASS( FtaQueryUserForInteger, FtaLuaFunction );

FtaQueryUserForInteger::FtaQueryUserForInteger( void )
{
}

/*virtual*/ FtaQueryUserForInteger::~FtaQueryUserForInteger( void )
{
}

/*virtual*/ int FtaQueryUserForInteger::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
		"Use the queryUserForInteger() function to ask the user for an integer in the given domain.  "
		"The function requires a table with fields \"question\", \"min\", \"max\" and \"default\".  "
		"The first of these is a string used to prompt the user, the second two are the domain of the input, "
		"and the last is default integer choice.  The function returns false if the user canceled.  "
		"If the user did not cancel, the field \"integer\" is added to the table and contains the user's answer.  "
		"Note that for now, the domain must be non-negative." );
	return 0;
}

/*virtual*/ int FtaQueryUserForInteger::Call( lua_State* L )
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

	int minInteger;
	lua_getfield( L, -1, "min" );
	if( lua_isinteger( L, -1 ) )
		minInteger = lua_tointeger( L, -1 );
	else
		minInteger = 0;
	lua_pop( L, 1 );

	int maxInteger;
	lua_getfield( L, -1, "max" );
	if( lua_isinteger( L, -1 ) )
		maxInteger = lua_tointeger( L, -1 );
	else
		maxInteger = minInteger;
	lua_pop( L, 1 );

	int defaultInteger;
	lua_getfield( L, -1, "default" );
	if( lua_isinteger( L, -1 ) )
		defaultInteger = lua_tointeger( L, -1 );
	else
		defaultInteger = ( minInteger + maxInteger ) / 2;
	lua_pop( L, 1 );

	if( minInteger > maxInteger )
		minInteger = maxInteger;

	if( defaultInteger < minInteger )
		defaultInteger = minInteger;
	if( defaultInteger > maxInteger )
		defaultInteger = maxInteger;

	int givenInteger = ( int )wxGetNumberFromUser( questionPrompt, "", "User Input", defaultInteger, minInteger, maxInteger, wxGetApp().GetFrame() );
	if( -1 == givenInteger )
	{
		lua_pushboolean( L, false );
		return 1;
	}

	lua_pushinteger( L, givenInteger );
	lua_setfield( L, -2, "integer" );

	lua_pushboolean( L, true );
	return 1;
}

// FtaQueryUserForInteger.cpp