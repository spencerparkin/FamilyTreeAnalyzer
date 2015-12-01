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

bool FtaTreeCache::Wipe( void )
{
	FtaDeletePersonMap( personMap );
	return true;
}

bool FtaTreeCache::Wipe( const wxString& personId )
{
	FtaPersonMap::iterator iter = personMap.find( personId );
	if( iter == personMap.end() )
		return false;

	FtaPerson* person = iter->second;
	delete person;
	personMap.erase( iter );
	return true;
}

FtaPerson* FtaTreeCache::Lookup( const wxString& personId, LookupDisposition disposition )
{
	FtaPerson* person = nullptr;

	if( personId != "unknown" )
	{
		FtaPersonMap::iterator iter = personMap.find( personId );
		if( iter == personMap.end() )
		{
			switch( disposition )
			{
				case POPULATE_ON_CACHE_MISS:
				{
					wxGetApp().GetClient()->PopulateTreeCacheAt( personId );
					break;
				}
				case ALLOCATE_ON_CACHE_MISS:
				{
					person = new FtaPerson( personId );
					personMap[ personId ] = person;
					break;
				}
			}

			if( disposition != FAIL_ON_CACHE_MISS )
				iter = personMap.find( personId );
		}

		if( iter != personMap.end() )
			person = iter->second;
	}

	return person;
}

// FtaTreeCache.cpp