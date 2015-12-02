// FtaPerson.cpp

#include "FtaPerson.h"
#include "FtaTreeCache.h"
#include "FtaMiscCache.h"
#include "FtaApp.h"

FtaPerson::FtaPerson( const wxString& personId )
{
	this->personId = personId;

	visitationData.key = -1;

	childrenIdSet = nullptr;
	spousesIdSet = nullptr;
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

	if( !spousesIdSet )
		return false;

	FtaPersonIdSet::iterator iter = spousesIdSet->begin();
	while( iter != spousesIdSet->end() )
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

	if( !childrenIdSet )
		return false;

	FtaPersonIdSet::iterator iter = childrenIdSet->begin();
	while( iter != childrenIdSet->end() )
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

	if( !spousesIdSet )
		return false;

	FtaPersonIdSet::iterator spouseIter = spousesIdSet->begin();
	while( spouseIter != spousesIdSet->end() )
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

		if( spouse->childrenIdSet )
		{
			FtaPersonIdSet::iterator childIter = spouse->childrenIdSet->begin();
			while( childIter != spouse->childrenIdSet->end() )
			{
				wxString childId = *childIter;
				FtaPerson* child = wxGetApp().GetTreeCache()->Lookup( childId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
				if( !child )
					return false;

				if( child->biologicalFatherId == personId || child->biologicalMotherId == personId )
					childSet->insert( child );

				childIter++;
			}
		}

		spouseIter++;
	}

	return false;
}

bool FtaPerson::SetImmediateAncestry( void )
{
	bool success = true;

	if( !wxGetApp().GetMiscCache()->LookupFather( personId, biologicalFatherId ) )
		success = false;
		
	if( !wxGetApp().GetMiscCache()->LookupMother( personId, biologicalMotherId ) )
		success = false;

	return success;
}

bool FtaPerson::SetImmediateDescendancy( void )
{
	bool success = true;

	if( !wxGetApp().GetMiscCache()->LookupChildren( personId, childrenIdSet ) )
		success = false;

	return success;
}

bool FtaPerson::SetSpouses( void )
{
	bool success = true;

	if( !wxGetApp().GetMiscCache()->LookupSpouses( personId, spousesIdSet ) )
		success = false;

	return success;
}

// FtaPerson.cpp