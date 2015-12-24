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
	bool success = false;
	FtaPersonIdSetList personIdSetList;

	do
	{
		if( !CreateConnectedComponents( personIdSetList ) )
			break;

		FtaPersonIdSetList::iterator iter = personIdSetList.begin();
		while( iter != personIdSetList.end() )
		{
			FtaPersonIdSet* connectedComponent = *iter;
			if( !LayoutConnectedComponent( *connectedComponent ) )
				break;

			iter++;
		}

		if( iter != personIdSetList.end() )
			break;

		if( PackConnectedComponents( personIdSetList ) )
			break;

		success = true;
	}
	while( false );

	DestroyConnectedComponents( personIdSetList );

	return success;
}

/*virtual*/ bool FtaGraph::LayoutConnectedComponent( FtaPersonIdSet& connectedComponent )
{
	// TODO: I'm thinking that the first thing I'll try is to simply place
	//       individuals at the same generation level on the same row and
	//       draw two arrows from every person to their parents.  How I sort
	//       each row will be based on the idea of minimizing how far the arrows
	//       have to go to reach the parents.

	return false;
}

/*virtual*/ bool FtaGraph::PackConnectedComponents( FtaPersonIdSetList& personIdSetList )
{
	// TODO: The simplest thing I can think of is simply to compute a bounding box
	//       for each component, and then array the boxes.

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
		personIdSetList.push_back( connectedComponent );
		if( !GenerateConnectedComponent( personId, remainingPersons, *connectedComponent ) )
		{
			DestroyConnectedComponents( personIdSetList );
			return false;
		}
	}

	return true;
}

bool FtaGraph::GenerateConnectedComponent( const wxString& personId, FtaPersonIdSet& remainingPersons, FtaPersonIdSet& connectedComponent )
{
	// Do we want to graph this person?
	if( !GraphPerson( personId ) )
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

bool FtaGraph::GraphPerson( const wxString& personId )
{
	if( personIdSet.find( personId ) != personIdSet.end() )
		return true;

	return false;
}

// FtaGraph.cpp