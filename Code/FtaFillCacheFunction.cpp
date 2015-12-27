// FtaFillCacheFunction.cpp

#include "FtaFillCacheFunction.h"
#include "FtaFrame.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaFillCacheFunction, FtaLuaFunction );

FtaFillCacheFunction::FtaFillCacheFunction( void )
{
}

/*virtual*/ FtaFillCacheFunction::~FtaFillCacheFunction( void )
{
}

/*virtual*/ int FtaFillCacheFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
				"Use the fillCache() to populate the cache.  "
				"The first argument is the root person ID, and the second is the person count threshold." );
	return 0;
}

/*virtual*/ int FtaFillCacheFunction::Call( lua_State* L )
{
	if( !lua_isstring( L, -2 ) )
		return luaL_error( L, "Expected root person ID as first argument." );

	if( !lua_isinteger( L, -1 ) )
		return luaL_error( L, "Expected person count threshold as second argument." );

	wxString rootPersonId = luaL_checkstring( L, -2 );
	int personCountThreshold = luaL_checkinteger( L, -1 );

	wxGetApp().GetTreeCache()->Fill( rootPersonId, personCountThreshold );

	return 0;
}

// FtaFillCacheFunction.cpp