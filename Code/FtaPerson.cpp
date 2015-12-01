// FtaPerson.cpp

#include "FtaPerson.h"
#include "FtaTreeCache.h"
#include "FtaMiscCache.h"
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

bool FtaPerson::GetSpouses( FtaPersonSet& spousesSet )
{
	spousesSet.clear();

	FtaPersonIdSet::iterator iter = spousesIdSet.begin();
	while( iter != spousesIdSet.end() )
	{
		wxString spouseId = *iter;
		FtaPerson* spouse = wxGetApp().GetTreeCache()->Lookup( spouseId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
		if( !spouse )
			return false;

		spousesSet.insert( spouse );
		iter++;
	}

	return true;
}

bool FtaPerson::GetBiologicalChildren( FtaPersonSet& childrenSet )
{
	childrenSet.clear();

	FtaPersonIdSet::iterator iter = childrenIdSet.begin();
	while( iter != childrenIdSet.end() )
	{
		wxString childId = *iter;
		FtaPerson* child = wxGetApp().GetTreeCache()->Lookup( childId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
		if( !child )
			return false;

		childrenSet.insert( child );
		iter++;
	}

	return true;
}

bool FtaPerson::GetBiologicalChildren( FtaOneToManyRelationshipMap& spouseToChildrenMap )
{
	FtaDeleteRelationshipMap( spouseToChildrenMap );

	FtaPersonIdSet::iterator spouseIter = spousesIdSet.begin();
	while( spouseIter != spousesIdSet.end() )
	{
		wxString spouseId = *spouseIter;
		FtaPerson* spouse = wxGetApp().GetTreeCache()->Lookup( spouseId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
		if( !spouse )
			return false;

		FtaPersonSet* childSet = nullptr;
		FtaOneToManyRelationshipMap::iterator iter = spouseToChildrenMap.find( spouseId );
		if( iter != spouseToChildrenMap.end() )
			childSet = iter->second;
		else
		{
			childSet = new FtaPersonSet;
			spouseToChildrenMap[ spouseId ] = childSet;
		}

		FtaPersonIdSet::iterator childIter = spouse->childrenIdSet.begin();
		while( childIter != spouse->childrenIdSet.end() )
		{
			wxString childId = *childIter;
			FtaPerson* child = wxGetApp().GetTreeCache()->Lookup( childId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
			if( !child )
				return false;

			if( child->biologicalFatherId == personId || child->biologicalMotherId == personId )
				childSet->insert( child );

			childIter++;
		}

		spouseIter++;
	}

	return false;
}

bool FtaPerson::SetImmediateAncestry( void )
{
	if( !wxGetApp().GetMiscCache()->LookupFather( personId, biologicalFatherId ) )
		return false;
		
	if( !wxGetApp().GetMiscCache()->LookupMother( personId, biologicalMotherId ) )
		return false;

	return true;
}

bool FtaPerson::SetImmediateDescendancy( void )
{
	if( !wxGetApp().GetMiscCache()->LookupChildren( personId, childrenIdSet ) )
		return false;

	return true;
}

bool FtaPerson::SetSpouses( void )
{
	if( !wxGetApp().GetMiscCache()->LookupSpouses( personId, spousesIdSet ) )
		return false;

	return true;
}

// FtaPerson.cpp