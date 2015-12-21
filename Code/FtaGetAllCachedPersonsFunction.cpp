// FtaGetAllCachedPersonsFunction.cpp

#include "FtaGetAllCachedPersonsFunction.h"
#include "FtaTreeCache.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaGetAllCachedPersonsFunction, FtaLuaFunction );

FtaGetAllCachedPersonsFunction::FtaGetAllCachedPersonsFunction( void )
{
}

/*virtual*/ FtaGetAllCachedPersonsFunction::~FtaGetAllCachedPersonsFunction( void )
{
}

/*virtual*/ int FtaGetAllCachedPersonsFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
		"Use the getAllCachedPersons() function to "
		"get a map from person-IDs to person info-tables."
		"This map will consist of all persons in the cache.  "
		"The cache is everything FamilyTreeAnalyzer has learned from FamilySearch.org "
		"about your family tree.  This cache is purged when you exit the program or logout." );
	return 0;
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