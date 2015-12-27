// FtaGraphViz.cpp

#include "FtaGraphViz.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"
#include "graphviz/gvc.h"

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
	GVC_t* gvc = nullptr;
	char* renderBuffer = nullptr;
	FILE* fp = nullptr;

	do
	{
		G = agopen( "FtaGraphViz", Agstrictdirected, &AgDefaultDisc );
		if( !G )
			break;

		agsafeset( G, "rankdir", "TB", "LR" );
		agsafeset( G, "lwidth", "100", "100" );
		agsafeset( G, "lheight", "100", "100" );

		if( !GenerateNodes(G) )
			break;

		if( !GenerateEdges(G) )
			break;

		gvc = gvContext();
		if( !gvc )
			break;

		int result = gvLayout( gvc, G, "dot" );
		if( result != 0 )
			break;

		// This is supposed to put the layout data in the attribute data.
		result = gvRender( gvc, G, "dot", nullptr );
		if( result != 0 )
			break;

		if( !GenerateGraphForDrawing( G ) )
			break;

#if 1
		unsigned int renderBufferLength = 0;
		result = gvRenderData( gvc, G, "dot", &renderBuffer, &renderBufferLength );
		if( result != 0 )
			break;

		fp = fopen( "test.dot", "w" );
		if( !fp )
			break;

		fwrite( renderBuffer, renderBufferLength, 1, fp );
#endif

		success = true;
	}
	while( false );

	if( G && gvc )
		gvFreeLayout( gvc, G );

	if( gvc )
		gvFreeContext( gvc );

	if(G)
		agclose(G);

	if( renderBuffer )
		gvFreeRenderData( renderBuffer );
	
	if( fp )
		fclose( fp );

	return success;
}

bool FtaGraphViz::GenerateNodes( Agraph_t* G )
{
	FtaPersonIdSet::iterator iter = personIdSet.begin();
	while( iter != personIdSet.end() )
	{
		wxString personId = *iter;

		FtaPerson* person = wxGetApp().GetTreeCache()->Lookup( personId, FtaTreeCache::FAIL_ON_CACHE_MISS );
		if( !person )
			return false;

		Agnode_t* personNode = agnode( G, ( char* )( const char* )personId, 1 );
		if( !personNode )
			return false;

		if( person->GetGender() == FtaPerson::GENDER_MALE )
			agsafeset( personNode, "color", "blue", "yellow" );
		else
			agsafeset( personNode, "color", "red", "yellow" );

		agsafeset( personNode, "shape", "record", "record" );
		agsafeset( personNode, "width", "2.0", "1.0" );
		agsafeset( personNode, "height", "2.0", "1.0" );

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

		if( PersonInGraphSet( person->GetFatherId() ) )
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

		if( PersonInGraphSet( person->GetMotherId() ) )
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

			if( PersonInGraphSet( spouseId ) )
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

bool FtaGraphViz::GenerateGraphForDrawing( Agraph_t* G )
{
	double scale = 0.1;

	FtaPersonIdSet::iterator iter = personIdSet.begin();
	while( iter != personIdSet.end() )
	{
		wxString personId = *iter;

		FtaPerson* person = wxGetApp().GetTreeCache()->Lookup( personId, FtaTreeCache::FAIL_ON_CACHE_MISS );
		if( !person )
			return false;

		Agnode_t* personNode = agnode( G, ( char* )( const char* )personId, 0 );
		if( !personNode )
			return false;

		FtaGraphNode* graphNode = new FtaGraphNode( this );
		graphNode->personId = personId;

		wxString rectString = agget( personNode, "rects" );

		wxArrayString stringArray;
		ParseArray( stringArray, rectString, ',' );
		wxASSERT( stringArray.Count() == 4 );
		stringArray[0].ToDouble( &graphNode->minRect.m_e1 );
		stringArray[1].ToDouble( &graphNode->minRect.m_e2 );
		stringArray[2].ToDouble( &graphNode->maxRect.m_e1 );
		stringArray[3].ToDouble( &graphNode->maxRect.m_e2 );
		
		// TODO: Why can't I tell GraphViz to scale it down?
		graphNode->minRect *= scale;
		graphNode->maxRect *= scale;

		graphElementList.push_back( graphNode );

		iter++;
	}

	// TODO: Generate edges for drawing here.

	return true;
}

void FtaGraphViz::ParseArray( wxArrayString& stringArray, const wxString& string, char delimeter )
{
	stringArray.clear();

	int i = 0, j;

	do
	{
		wxString subString;
		j = string.find( delimeter, i );
		if( j > 0 )
		{
			subString = string.SubString( i, j );
			subString.Truncate( subString.Length() - 1 );
		}
		else
			subString = string.SubString( i, string.Length() );

		stringArray.Add( subString );

		i = j + 1;
	}
	while( i > 0 );
}

// FtaGraphViz.cpp