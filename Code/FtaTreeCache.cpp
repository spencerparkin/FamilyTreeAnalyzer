// FtaTreeCache.cpp

#include "FtaTreeCache.h"
#include "FtaPerson.h"
#include "FtaClient.h"
#include "FtaApp.h"

FtaTreeCache::FtaTreeCache( void )
{
}

FtaTreeCache::~FtaTreeCache( void )
{
	Wipe();
}

FtaPerson* FtaTreeCache::Lookup( const wxString& id, bool populateCacheAsNeeded /*= false*/ )
{
	FtaPerson* person = nullptr;

	FtaPersonMap::iterator iter = personMap.find( id );
	if( iter != personMap.end() )
		person = iter->second;
	else if( populateCacheAsNeeded )
	{
		FtaClient* client = wxGetApp().GetClient();
		if( client )
		{
			// TODO: Use client to populate cache with the missing person.
		}
	}

	return person;
}

void FtaTreeCache::Wipe( void )
{
	while( personMap.size() > 0 )
	{
		FtaPersonMap::iterator iter = personMap.begin();
		FtaPerson* person = iter->second;
		delete person;
		personMap.erase( iter );
	}
}

// FtaTreeCache.cpp