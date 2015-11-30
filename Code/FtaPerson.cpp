// FtaPerson.cpp

#include "FtaPerson.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

FtaPerson::FtaPerson( const wxString& personId )
{
	this->personId = personId;
}

/*virtual*/ FtaPerson::~FtaPerson( void )
{
}

FtaPerson* FtaPerson::GetBiologicalFather( void )
{
	FtaPerson* biologicalFather = wxGetApp().GetTreeCache()->Lookup( biologicalFatherId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
	return biologicalFather;
}

FtaPerson* FtaPerson::GetBiologicalMother( void )
{
	FtaPerson* biologicalMother = wxGetApp().GetTreeCache()->Lookup( biologicalMotherId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
	return biologicalMother;
}

bool FtaPerson::GetBiologicalChildren( const FtaPerson* spouse, FtaPersonSet& children )
{
	return false;
}

bool FtaPerson::GetSpouses( FtaPersonSet& spouses )
{
	return false;
}

bool FtaPerson::SetImmediateAncestry( void )
{
	return false;
}

bool FtaPerson::SetImmediateDescendancy( void )
{
	return false;
}

bool FtaPerson::SetSpouses( void )
{
	return false;
}

// FtaPerson.cpp