// FtaPerson.cpp

#include "FtaPerson.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

FtaPerson::FtaPerson( const wxString& personId )
{
	this->personId = personId;
	infoState = INFO_UNKNOWN;
}

/*virtual*/ FtaPerson::~FtaPerson( void )
{
}

bool FtaPerson::IsInfoComplete( void )
{
	if( motherId.IsEmpty() )
		return false;

	if( fatherId.IsEmpty() )
		return false;

	// As far as the children and spouses go, we really have no way
	// of knowing if the information is complete.  We assume here that
	// whatever response we get from a request to find out the children
	// or spouses gives us enough information.
	
	if( childrenIdSet.size() == 0 )
		return false;

	if( spousesIdSet.size() == 0 )
		return false;

	// TODO: Check other details too, such as ordinances, etc...

	return true;
}

void FtaPerson::GatherNearestRelations( FtaPersonList& personList )
{
	personList.clear();

	FtaTreeCache* cache = wxGetApp().GetTreeCache();

	if( !motherId.IsEmpty() )
		personList.push_back( cache->Lookup( motherId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS ) );

	if( !fatherId.IsEmpty() )
		personList.push_back( cache->Lookup( fatherId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS ) );

	FtaPersonIdSet::iterator iter = childrenIdSet.begin();
	while( iter != childrenIdSet.end() )
	{
		wxString childId = *iter;
		if( !childId.IsEmpty() )
			personList.push_back( cache->Lookup( childId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS ) );
		iter++;
	}

	iter = spousesIdSet.begin();
	while( iter != spousesIdSet.end() )
	{
		wxString spouseId = *iter;
		if( !spouseId.IsEmpty() )
			personList.push_back( cache->Lookup( spouseId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS ) );
		iter++;
	}
}

// FtaPerson.cpp