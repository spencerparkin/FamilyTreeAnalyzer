// FtaClearFunction.cpp

#include "FtaClearFunction.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaClearFunction, FtaLuaFunction );

FtaClearFunction::FtaClearFunction( void )
{
}

/*virtual*/ FtaClearFunction::~FtaClearFunction( void )
{
}

/*virtual*/ int FtaClearFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage( "Use the clear() function to clear all log messages.  It takes no arguments and returns nothing." );
	return 0;
}

/*virtual*/ int FtaClearFunction::Call( lua_State* L )
{
	wxGetApp().GetFrame()->ClearLog();
	return 0;
}

// FtaClearFunction.cpp