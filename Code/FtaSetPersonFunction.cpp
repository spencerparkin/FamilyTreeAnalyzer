// FtaSetPersonFunction.cpp

#include "FtaSetPersonFunction.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaSetPersonFunction, FtaLuaFunction );

FtaSetPersonFunction::FtaSetPersonFunction( void )
{
}

/*virtual*/ FtaSetPersonFunction::~FtaSetPersonFunction( void )
{
}

/*virtual*/ int FtaSetPersonFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
			"The setPerson() function is stubbed in for completeness, but not yet implemented.  "
			"When it is implemented, it will always require user intervention and verification before "
			"possibly doing any up-load request to FamilySearch.org.  Lua scripts, like any program, have "
			"the potential to make many mistakes in a very short period of time." );
	return 0;
}

/*virtual*/ int FtaSetPersonFunction::Call( lua_State* L )
{
	// TODO: Here we write to the cache, but we don't push the cache up to www.familysearch.org.
	//       That would have to be a special operation in and of itself.  And I doubt the maintainers
	//       of familysearch.org would find it a good idea to expose write access to random Lua
	//       scripts that people write to manipluate tree data.
	return 0;
}

// FtaSetPersonFunction.cpp