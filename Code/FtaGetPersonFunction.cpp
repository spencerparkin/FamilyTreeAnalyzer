// FtaGetPersonFunction.cpp

#include "FtaGetPersonFunction.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaGetPersonFunction, FtaLuaFunction );

FtaGetPersonFunction::FtaGetPersonFunction( void )
{
}

/*virtual*/ FtaGetPersonFunction::~FtaGetPersonFunction( void )
{
}

/*virtual*/ int FtaGetPersonFunction::Call( lua_State* L )
{
	wxString personId = luaL_checkstring( L, 1 );
	if( personId.IsEmpty() )
	{
		// TODO: Here we should query for the ID of the person logged in.
	}

	FtaTreeCache* cache = wxGetApp().GetTreeCache();
	if( !cache )
		return 0;

	FtaPerson* person = cache->Lookup( personId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
	if( !person )
		return 0;

	if( !person->GetToLuaTable(L) )
		return 0;

	return 1;
}

// FtaGetPersonFunction.cpp