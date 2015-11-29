// FtaPersonFemale.cpp

#include "FtaPersonFemale.h"

FtaPersonFemale::FtaPersonFemale( FtaTreeCache* cache ) : FtaPerson( cache )
{
}

/*virtual*/ FtaPersonFemale::~FtaPersonFemale( void )
{
}

/*virtual*/ void FtaPersonFemale::FindAllChildrenIds( FtaPersonIdSet& childrenIds, bool populateCacheAsNeeded /*= false*/ ) const
{
	childrenIds = this->childrenIds;
}

void FtaPersonFemale::AddChildId( const wxString& childId )
{
	FtaPersonIdSet::iterator iter = childrenIds.find( childId );
	if( iter == childrenIds.end() )
		childrenIds.insert( childId );
}

// FtaPersonFemale.cpp