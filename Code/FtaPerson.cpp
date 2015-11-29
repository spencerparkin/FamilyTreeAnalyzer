// FtaPerson.cpp

#include "FtaPerson.h"
#include "FtaPersonMale.h"
#include "FtaPersonFemale.h"
#include "FtaTreeCache.h"

FtaPerson::FtaPerson( FtaTreeCache* cache )
{
	this->cache = cache;
}

/*virtual*/ FtaPerson::~FtaPerson( void )
{
}

wxString FtaPerson::GetFatherId( bool populateCacheAsNeeded /*= false*/ ) const
{
	wxString fatherId;

	FtaPersonFemale* mother = cache->LookupCast< FtaPersonFemale >( motherId, populateCacheAsNeeded );
	if( mother )
		fatherId = mother->GetHusbandId();

	return fatherId;
}

// FtaPerson.cpp