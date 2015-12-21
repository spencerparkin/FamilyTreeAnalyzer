// FtaGetPersonFunction.cpp

#include "FtaGetPersonFunction.h"
#include "FtaTreeCache.h"
#include "FtaClient.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaGetPersonFunction, FtaLuaFunction );

FtaGetPersonFunction::FtaGetPersonFunction( void )
{
}

/*virtual*/ FtaGetPersonFunction::~FtaGetPersonFunction( void )
{
}

/*virtual*/ int FtaGetPersonFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
		"Use the getPerson() function to "
		"get a person-info table for the given person.  "
		"The person is given by person-ID, which is a string.  "
		"If the desired person is not cached, this will cause FamilySearchAnalyzer to "
		"inquire FamilySearch.org about the person."  );
	return 0;
}

/*virtual*/ int FtaGetPersonFunction::Call( lua_State* L )
{
	wxString personId = luaL_checkstring( L, 1 );
	if( personId.IsEmpty() )
		personId = wxGetApp().GetClient()->GetCurrentUserPersonId();

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