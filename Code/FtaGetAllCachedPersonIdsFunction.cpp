// FtaGetAllCachedPersonIdsFunction.cpp

#include "FtaGetAllCachedPersonIdsFunction.h"
#include "FtaTreeCache.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaGetAllCachedPersonIdsFunction, FtaLuaFunction );

FtaGetAllCachedPersonIdsFunction::FtaGetAllCachedPersonIdsFunction( void )
{
}

/*virtual*/ FtaGetAllCachedPersonIdsFunction::~FtaGetAllCachedPersonIdsFunction( void )
{
}

/*virtual*/ int FtaGetAllCachedPersonIdsFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
					"Use the getAllCachedPersonIds() function to "
					"get an array of strings, each being a person-ID.  "
					"This array will consist of all persons in the cache.  "
					"The cache is everything FamilyTreeAnalyzer has learned from FamilySearch.org "
					"about your family tree.  This cache is purged when you exit the program or logout." );
	return 0;
}

/*virtual*/ int FtaGetAllCachedPersonIdsFunction::Call( lua_State* L )
{
	lua_newtable(L);

	int i = 1;

	const FtaPersonMap& personMap = wxGetApp().GetTreeCache()->GetPersonMap();
	FtaPersonMap::const_iterator iter = personMap.begin();
	while( iter != personMap.end() )
	{
		wxString personId = iter->first;

		lua_pushinteger( L, i );
		lua_pushstring( L, personId );
		lua_settable( L, -3 );

		i++;
		iter++;
	}

	return 1;
}

// FtaGetAllCachedPersonIdsFunction.cpp