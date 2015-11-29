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