// FtaGraph.h

#pragma once

#include "FtaContainers.h"
#include "FtaVisualization.h"

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

	void AddPerson( const wxString& personId );
	void RemovePerson( const wxString& personId );

	bool CreateConnectedComponents( FtaPersonIdSetList& personIdSetList );
	bool DestroyConnectedComponents( FtaPersonIdSetList& personIdSetList );

protected:

	bool GraphPerson( const wxString& personId );

	virtual bool LayoutConnectedComponent( FtaPersonIdSet& connectedComponent );
	virtual bool PackConnectedComponents( FtaPersonIdSetList& personIdSetList );

	bool GenerateConnectedComponent( const wxString& personId, FtaPersonIdSet& remainingPersons, FtaPersonIdSet& connectedComponent );

	FtaPersonIdSet personIdSet;

	bool layoutNeeded;
};

// FtaGraph.h