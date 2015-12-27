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

	bool GenerateGraphForDrawing( Agraph_t* G );

	void ParseArray( wxArrayString& stringArray, const wxString& string, char delimeter );
};

// FtaGraphViz.h
