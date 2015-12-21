// FtaHelpFunction.cpp

#include "FtaHelpFunction.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaHelpFunction, FtaLuaFunction );

FtaHelpFunction::FtaHelpFunction( void )
{
}

/*virtual*/ FtaHelpFunction::~FtaHelpFunction( void )
{
}

/*virtual*/ int FtaHelpFunction::Call( lua_State* L )
{
	if( !lua_iscfunction( L, -1 ) )
		return luaL_error( L, "Expected C function." );

	lua_CFunction function = lua_tocfunction( L, -1 );
	if( !FtaLuaFunction::IsEntryPoint( function ) )
		return luaL_error( L, "Help not available for the given C function." );

	FtaLuaFunction::ScopeFunctionHelp functionHelp;

	int result = lua_pcall( L, 0, 0, 0 );
	if( result != 0 )
		return luaL_error( L, "Lua error calling function for help." );

	return 0;
}

// FtaHelpFunction.cpp