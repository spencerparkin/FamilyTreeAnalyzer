// FtaGraphViz.cpp

#include "FtaGraphViz.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

FtaGraphViz::FtaGraphViz( void )
{
}

/*virtual*/ FtaGraphViz::~FtaGraphViz( void )
{
}

// TODO: We may want to provide an option that puts spouses in their own sub-graph/cluster.
//       This should have the effect of grouping them together.
/*virtual*/ bool FtaGraphViz::Layout( void )
{
	bool success = false;
	Agraph_t* G = nullptr;

	do
	{
		G = agopen( "FtaGraphViz", Agstrictdirected, &AgDefaultDisc );
		if( !G )
			break;

		// TODO: Setup graph attributes here.

		if( !GenerateNodes(G) )
			break;

		if( !GenerateEdges(G) )
			break;

		//...

		// TODO: We may want to write out the DOT language here for debugging purposes.

		// TODO: Layout and render graph; capture positional information.

		success = true;
	}
	while( false );

	if(G)
	{
		agclose(G);
		G = nullptr;
	}

	return success;
}

bool FtaGraphViz::GenerateNodes( Agraph_t* G )
{
	FtaPersonIdSet::iterator iter = personIdSet.begin();
	while( iter != personIdSet.end() )
	{
		wxString personId = *iter;

		Agnode_t* personNode = agnode( G, ( char* )( const char* )personId, 1 );
		if( !personNode )
			return false;

		//...

		iter++;
	}

	return true;
}

bool FtaGraphViz::GenerateEdges( Agraph_t* G )
{
	FtaPersonIdSet::iterator iter = personIdSet.begin();
	while( iter != personIdSet.end() )
	{
		wxString personId = *iter;

		Agnode_t* personNode = agnode( G, ( char* )( const char* )personId, 0 );
		if( !personNode )
			return false;

		FtaPerson* person = wxGetApp().GetTreeCache()->Lookup( personId, FtaTreeCache::FAIL_ON_CACHE_MISS );
		if( !person )
			return false;

		if( GraphPerson( person->GetFatherId() ) )
		{
			Agnode_t* fatherNode = agnode( G, ( char* )( const char* )person->GetFatherId(), 0 );
			if( !fatherNode )
				return false;

			Agedge_t* edge = agedge( G, personNode, fatherNode, nullptr, 0 );
			if( !edge )
			{
				edge = agedge( G, personNode, fatherNode, nullptr, 1 );

				//...
			}
		}

		if( GraphPerson( person->GetMotherId() ) )
		{
			Agnode_t* motherNode = agnode( G, ( char* )( const char* )person->GetMotherId(), 0 );
			if( !motherNode )
				return false;

			Agedge_t* edge = agedge( G, personNode, motherNode, nullptr, 0 );
			if( !edge )
			{
				edge = agedge( G, personNode, motherNode, nullptr, 1 );

				//...
			}
		}

		const FtaPersonIdSet& spouseIdSet = person->GetSpousesIdSet();
		FtaPersonIdSet::const_iterator spouseIter = spouseIdSet.begin();
		while( spouseIter != spouseIdSet.end() )
		{
			wxString spouseId = *spouseIter;

			if( GraphPerson( spouseId ) )
			{
				Agnode_t* spouseNode = agnode( G, ( char* )( const char* )spouseId, 0 );
				if( !spouseNode )
					return false;

				Agedge_t* edge = agedge( G, personNode, spouseNode, nullptr, 0 );
				if( !edge )
				{
					edge = agedge( G, personNode, spouseNode, nullptr, 1 );

					//...
				}
			}

			spouseIter++;
		}

		iter++;
	}

	return true;
}

// FtaGraphViz.cpp