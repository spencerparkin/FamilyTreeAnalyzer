// FtaGetAllCachedPersonsFunction.cpp

#include "FtaGetAllCachedPersonsFunction.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaGetAllCachedPersonsFunction, FtaLuaFunction );

FtaGetAllCachedPersonsFunction::FtaGetAllCachedPersonsFunction( void )
{
}

/*virtual*/ FtaGetAllCachedPersonsFunction::~FtaGetAllCachedPersonsFunction( void )
{
}

/*virtual*/ int FtaGetAllCachedPersonsFunction::Call( lua_State* L )
{
	lua_newtable(L);

	const FtaPersonMap& personMap = wxGetApp().GetTreeCache()->GetPersonMap();
	FtaPersonMap::const_iterator iter = personMap.begin();
	while( iter != personMap.end() )
	{
		wxString personId = iter->first;
		const FtaPerson* person = iter->second;

		lua_pushstring( L, personId );
		person->GetToLuaTable(L);
		lua_settable( L, -3 );
		
		iter++;
	}

	return 1;
}

// FtaGetAllCachedPersonsFunction.cpp