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

/*virtual*/ int FtaClearFunction::Call( lua_State* L )
{
	wxGetApp().GetFrame()->ClearLog();
	return 0;
}

// FtaClearFunction.cpp