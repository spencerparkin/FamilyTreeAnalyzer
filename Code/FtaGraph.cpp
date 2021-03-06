// FtaGraph.cpp

#include "FtaGraph.h"
#include "FtaTreeCache.h"
#include "FtaImageRequest.h"
#include "FtaClient.h"
#include "FtaCanvas.h"
#include "FtaApp.h"
#include "wx/listimpl.cpp"

WX_DEFINE_LIST( FtaGraphElementList );

FtaGraph::FtaGraph( void )
{
	layoutNeeded = true;
}

/*virtual*/ FtaGraph::~FtaGraph( void )
{
	DeleteGraphElementList();
}

void FtaGraph::DeleteGraphElementList( void )
{
	while( graphElementList.size() > 0 )
	{
		FtaGraphElementList::iterator iter = graphElementList.begin();
		FtaGraphElement* element = *iter;
		delete element;
		graphElementList.erase( iter );
	}
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
	if( !LayoutIfNeeded() )
		return false;

	FtaGraphElementList::iterator iter = graphElementList.begin();
	while( iter != graphElementList.end() )
	{
		FtaGraphElement* element = *iter;
		element->Draw( renderMode );
		iter++;
	}

	return false;
}

/*virtual*/ bool FtaGraph::CalcBoundingBox( FtaAxisAlignedBox& aab )
{
	if( !LayoutIfNeeded() )
		return false;

	aab.max.set( c3ga::vectorE3GA::coord_e1_e2_e3, -1.0, -1.0, -1.0 );
	aab.min.set( c3ga::vectorE3GA::coord_e1_e2_e3, 1.0, 1.0, 1.0 );

	FtaGraphElementList::iterator iter = graphElementList.begin();
	while( iter != graphElementList.end() )
	{
		FtaGraphElement* element = *iter;
		
		FtaAxisAlignedBox elementAab;
		if( !element->CalcBoundingBox( elementAab ) )
			return false;

		if( !aab.IsValid() )
			aab = elementAab;
		else
			aab.Combine( aab, elementAab );

		iter++;
	}

	return true;
}

bool FtaGraph::LayoutIfNeeded( void )
{
	if( layoutNeeded )
	{
		DeleteGraphElementList();

		if( !Layout() )
			return false;

		layoutNeeded = false;
	}
	
	return true;
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
	if( !PersonInGraphSet( personId ) )
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

bool FtaGraph::PersonInGraphSet( const wxString& personId )
{
	if( personIdSet.find( personId ) != personIdSet.end() )
		return true;

	return false;
}

/*virtual*/ bool FtaGraph::Bind( void )
{
	return true;
}

/*virtual*/ bool FtaGraph::Unbind( void )
{
	return true;
}

/*virtual*/ bool FtaGraph::Animate( void )
{
	bool animating = false;

	FtaGraphElementList::iterator iter = graphElementList.begin();
	while( iter != graphElementList.end() )
	{
		FtaGraphElement* element = *iter;
		if( element->Animate() )
			animating = true;
		iter++;
	}

	return animating;
}

FtaGraphElement::FtaGraphElement( FtaGraph* graph )
{
	this->graph = graph;
}

/*virtual*/ FtaGraphElement::~FtaGraphElement( void )
{
}

/*virtual*/ bool FtaGraphElement::ProcessResponse( FtaAsyncRequest* request, wxJSONValue* responseValue )
{
	return false;
}

FtaGraphNode::FtaGraphNode( FtaGraph* graph ) : FtaGraphElement( graph )
{
	textureRequestSignature = -1;
	texture = GL_INVALID_VALUE;
}

/*virtual*/ FtaGraphNode::~FtaGraphNode( void )
{
	if( textureRequestSignature >= 0 )
	{
		// Don't let the pending request list hang on to a stale pointer.
		FtaClient* client = wxGetApp().GetClient();
		if( client )
			client->CancelAllAsyncRequests( textureRequestSignature );
	}
}

/*virtual*/ bool FtaGraphNode::Animate( void )
{
	if( textureRequestSignature >= 0 )
	{
		FtaClient* client = wxGetApp().GetClient();
		client->ServiceAllAsyncRequests( false, textureRequestSignature );
		return true;
	}

	return false;
}

/*virtual*/ void FtaGraphNode::Draw( GLenum renderMode )
{
	FtaTreeCache* cache = wxGetApp().GetTreeCache();

	FtaPerson* person = cache->Lookup( personId, FtaTreeCache::FAIL_ON_CACHE_MISS );
	if( !person )
		return;

	wxString portraitUrl = person->GetPortraitUrl();
	if( !( portraitUrl.IsEmpty() || portraitUrl == "No Content" ) && texture == GL_INVALID_VALUE )
	{
		if( textureRequestSignature == -1 )
		{
			textureRequestSignature = FtaClient::NewSignature();
			FtaClient* client = wxGetApp().GetClient();
			bool added = client->AddAsyncRequest( new FtaImageRequest( portraitUrl, this, textureRequestSignature ) );
			wxASSERT( added );
		}
	}

	// TODO: Since the nodes have a 3D appearance, start using lighting?
	// TODO: Draw textured quad here.

	glColor3f( 0.9f, 0.9f, 0.9f );
	aab.Draw( false );

	FtaFontSystem* fontSystem = graph->canvas->GetFontSystem();

	fontSystem->SetLineHeight( aab.GetHeight() / 10.f );
	fontSystem->SetWordWrap( true );
	fontSystem->SetJustification( FtaFontSystem::JUSTIFY_CENTER );
	fontSystem->SetFont( "ChanticleerRomanNF.ttf" );
	fontSystem->SetLineWidth( aab.GetWidth() );
	fontSystem->SetBaseLineDelta( -fontSystem->GetLineHeight() * 1.5f );

	glPushAttrib( GL_LIGHTING_BIT | GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glPushMatrix();

	GLfloat x = aab.min.m_e1;
	GLfloat y = aab.min.m_e2;
	GLfloat z = aab.max.m_e3;

	GLfloat length;
	fontSystem->CalcTextLength( person->GetName(), length );
	while( length > fontSystem->GetLineWidth() )
	{
		y -= fontSystem->GetBaseLineDelta();
		length -= fontSystem->GetLineWidth();
	}

	glTranslatef( x, y, z );
	glColor3f( 0.f, 0.f, 0.f );

	fontSystem->DrawText( person->GetName(), true );

	glPopMatrix();
	glPopAttrib();
}

/*virtual*/ bool FtaGraphNode::ProcessResponse( FtaAsyncRequest* request, wxJSONValue* responseValue )
{
	if( !request->IsKindOf( &FtaImageRequest::ms_classInfo ) )
		return false;

	FtaImageRequest* imageRequest = ( FtaImageRequest* )request;
	texture = graph->canvas->GenerateTexture( *imageRequest->GetImage() );

	textureRequestSignature = -2;
	return true;
}

FtaGraphEdge::FtaGraphEdge( FtaGraph* graph ) : FtaGraphElement( graph )
{
	tailNode = nullptr;
	headNode = nullptr;
}

/*virtual*/ FtaGraphEdge::~FtaGraphEdge( void )
{
}

/*virtual*/ void FtaGraphEdge::Draw( GLenum renderMode )
{
	// TODO: We could do CGA-based edge-routing here using a divde-and-conqur algorithm.
	//       A derivative of this class, however, may use dot-generated edge-spline data, which would be better.
}

/*virtual*/ bool FtaGraphEdge::CalcBoundingBox( FtaAxisAlignedBox& aab ) const
{
	return false;
}

// FtaGraph.cpp