// FtaPrintFunction.cpp

#include "FtaPrintFunction.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaPrintFunction, FtaLuaFunction );

FtaPrintFunction::FtaPrintFunction( void )
{
}

/*virtual*/ FtaPrintFunction::~FtaPrintFunction( void )
{
}

/*virtual*/ int FtaPrintFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
		"Use the print() function to send a log message to the log in the form of the given string.  "
		"While the Lua panel is the primary source of input, the Log panel is "
		"the primary source of output." );
	return 0;
}

/*virtual*/ int FtaPrintFunction::Call( lua_State* L )
{
	wxString string = luaL_checkstring( L, 1 );
	if( string.IsEmpty() )
		return 0;

	wxGetApp().GetFrame()->AddLogMessage( string );
	return 0;
}

// FtaPrintFunction.cpp