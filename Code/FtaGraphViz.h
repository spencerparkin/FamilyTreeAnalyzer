// FtaGraphViz.h

#pragma once

#include "FtaGraph.h"
#include "graphviz/cgraph.h"

class FtaGraphViz : public FtaGraph
{
public:

	FtaGraphViz( void );
	virtual ~FtaGraphViz( void );

	virtual bool Layout( void ) override;

private:

	bool GenerateNodes( Agraph_t* G );
	bool GenerateEdges( Agraph_t* G );

	bool PlacePersons( Agraph_t* G );
};

// FtaGraphViz.h
