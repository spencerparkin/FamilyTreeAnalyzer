// FtaGraph.cpp

#include "FtaGraph.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

FtaGraph::FtaGraph( void )
{
	layoutNeeded = true;
}

/*virtual*/ FtaGraph::~FtaGraph( void )
{
}

/*virtual*/ bool FtaGraph::Layout( void )
{
	//...
	// TODO: Write default layout algorithm.

	return false;
}

/*virtual*/ bool FtaGraph::Draw( GLenum renderMode )
{
	if( layoutNeeded && !Layout() )
		return false;

	//...

	return false;
}

void FtaGraph::AddPerson( const wxString& personId )
{
	personIdSet.insert( personId );

	layoutNeeded = true;
}

void FtaGraph::RemovePerson( const wxString& personId )
{
	personIdSet.erase( personId );

	layoutNeeded = true;
}

bool FtaGraph::CreateConnectedComponents( FtaPersonIdSetList& personIdSetList )
{
	FtaPersonIdSet remainingPersons;

	FtaPersonIdSet::iterator iter = personIdSet.begin();
	while( iter != personIdSet.end() )
	{
		wxString personId = *iter;
		remainingPersons.insert( personId );
		iter++;
	}

	while( remainingPersons.size() > 0 )
	{
		iter = remainingPersons.begin();
		wxString personId = *iter;
		FtaPersonIdSet* connectedComponent = new FtaPersonIdSet;
		if( GenerateConnectedComponent( personId, remainingPersons, *connectedComponent ) )
			personIdSetList.push_back( connectedComponent );
		else
		{
			delete connectedComponent;
			return false;
		}
	}

	return true;
}

bool FtaGraph::GenerateConnectedComponent( const wxString& personId, FtaPersonIdSet& remainingPersons, FtaPersonIdSet& connectedComponent )
{
	// Do we want to graph this person?
	if( personIdSet.find( personId ) == personIdSet.end() )
		return true;

	// Have we already visited this person?
	if( connectedComponent.find( personId ) != connectedComponent.end() )
		return true;

	connectedComponent.insert( personId );
	remainingPersons.erase( personId );

	const FtaPerson* person = wxGetApp().GetTreeCache()->Lookup( personId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
	if( !person )
		return false;

	wxString motherId = person->GetMotherId();
	if( !GenerateConnectedComponent( motherId, remainingPersons, connectedComponent ) )
		return false;

	wxString fatherId = person->GetFatherId();
	if( !GenerateConnectedComponent( fatherId, remainingPersons, connectedComponent ) )
		return false;

	const FtaPersonIdSet& childrenIdSet = person->GetChildrenIdSet();
	for( FtaPersonIdSet::const_iterator iter = childrenIdSet.begin(); iter != childrenIdSet.end(); iter++ )
	{
		wxString childId = *iter;
		if( !GenerateConnectedComponent( childId, remainingPersons, connectedComponent ) )
			return false;
	}

	const FtaPersonIdSet& spousesIdSet = person->GetSpousesIdSet();
	for( FtaPersonIdSet::const_iterator iter = spousesIdSet.begin(); iter != spousesIdSet.end(); iter++ )
	{
		wxString spouseId = *iter;
		if( !GenerateConnectedComponent( spouseId, remainingPersons, connectedComponent ) )
			return false;
	}

	return true;
}

bool FtaGraph::DestroyConnectedComponents( FtaPersonIdSetList& personIdSetList )
{
	while( personIdSetList.size() > 0 )
	{
		FtaPersonIdSetList::iterator iter = personIdSetList.begin();
		FtaPersonIdSet* connectedComponent = *iter;
		delete connectedComponent;
		personIdSetList.erase( iter );
	}

	return true;
}

// FtaGraph.cpp