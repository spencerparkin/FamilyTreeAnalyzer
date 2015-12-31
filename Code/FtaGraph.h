// FtaGraph.h

#pragma once

#include "FtaContainers.h"
#include "FtaVisualization.h"
#include "FtaAsyncRequest.h"
#include "FtaAxisAlignedBox.h"
#include "c3ga/c3ga.h"

class FtaGraphElement;

WX_DECLARE_LIST( FtaGraphElement, FtaGraphElementList );

// Derivatives of this class implement various ways of laying out and drawing a graph of related persons.
class FtaGraph : public FtaVisualization
{
public:

	FtaGraph( void );
	virtual ~FtaGraph( void );

	virtual bool Layout( void );
	virtual bool Draw( GLenum renderMode ) override;
	virtual bool Bind( void ) override;
	virtual bool Unbind( void ) override;
	virtual bool Animate( void ) override;

	void AddPerson( const wxString& personId );
	void RemovePerson( const wxString& personId );

	bool CreateConnectedComponents( FtaPersonIdSetList& personIdSetList );
	bool DestroyConnectedComponents( FtaPersonIdSetList& personIdSetList );

protected:

	bool PersonInGraphSet( const wxString& personId );

	virtual bool LayoutConnectedComponent( FtaPersonIdSet& connectedComponent );
	virtual bool PackConnectedComponents( FtaPersonIdSetList& personIdSetList );

	bool GenerateConnectedComponent( const wxString& personId, FtaPersonIdSet& remainingPersons, FtaPersonIdSet& connectedComponent );

	void DeleteGraphElementList( void );

	FtaPersonIdSet personIdSet;
	FtaGraphElementList graphElementList;
	bool layoutNeeded;
};

class FtaGraphElement : public FtaAsyncRequest::ResponseProcessor
{
public:
	FtaGraphElement( FtaGraph* graph );
	virtual ~FtaGraphElement( void );

	virtual void Draw( GLenum renderMode ) = 0;
	virtual bool Animate( void ) { return false; }
	virtual bool ProcessResponse( FtaAsyncRequest* request, wxJSONValue* responseValue ) override;

protected:

	FtaGraph* graph;
};

class FtaGraphNode : public FtaGraphElement
{
public:
	FtaGraphNode( FtaGraph* graph );
	virtual ~FtaGraphNode( void );

	virtual void Draw( GLenum renderMode ) override;
	virtual bool Animate( void ) override;
	virtual bool ProcessResponse( FtaAsyncRequest* request, wxJSONValue* responseValue ) override;

	wxString personId;
	GLuint texture;
	int textureRequestSignature;
	FtaAxisAlignedBox aab;
};

class FtaGraphEdge : public FtaGraphElement
{
public:
	FtaGraphEdge( FtaGraph* graph );
	virtual ~FtaGraphEdge( void );

	virtual void Draw( GLenum renderMode ) override;

	FtaGraphNode* tailNode;
	FtaGraphNode* headNode;
	// TODO: Add spline data.  In general, we could use a poly-line that gets smooted out by splineage.
};

// FtaGraph.h