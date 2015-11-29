// FtaPersonMale.cpp

#include "FtaPersonMale.h"
#include "FtaPersonFemale.h"
#include "FtaTreeCache.h"

FtaPersonMale::FtaPersonMale( FtaTreeCache* cache ) : FtaPerson( cache )
{
}

/*virtual*/ FtaPersonMale::~FtaPersonMale( void )
{
}

/*virtual*/ void FtaPersonMale::FindAllChildrenIds( FtaPersonIdSet& childrenIds, bool populateCacheAsNeeded /*= false*/ ) const
{
	childrenIds.clear();

	FtaPersonIdSet::const_iterator iter = wifeIds.begin();
	while( iter != wifeIds.end() )
	{
		wxString wifeId = *iter;

		FtaPersonFemale* wife = cache->LookupCast< FtaPersonFemale >( wifeId, populateCacheAsNeeded );
		if( wife )
		{
			FtaPersonIdSet childrenIdsForWife;
			wife->FindAllChildrenIds( childrenIdsForWife, populateCacheAsNeeded );

			// TODO: Concatinate here.
		}

		iter++;
	}
}

void FtaPersonMale::AddWifeId( const wxString& wifeId )
{
	FtaPersonIdSet::iterator iter = wifeIds.find( wifeId );
	if( iter == wifeIds.end() )
		wifeIds.insert( wifeId );
}

// FtaPersonMale.cpp