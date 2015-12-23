// FtaGraphViz.h

#pragma once

#include "FtaGraph.h"

class FtaGraphViz : public FtaGraph
{
public:

	FtaGraphViz( void );
	virtual ~FtaGraphViz( void );

	virtual bool Layout( void ) override;
};

// FtaGraphViz.h
