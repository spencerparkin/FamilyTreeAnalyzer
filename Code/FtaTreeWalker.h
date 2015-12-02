// FtaTreeWalker.h

#pragma once

#include "FtaContainers.h"

class FtaPerson;

// This class walks a connected component of the family tree
// under a given set of contraints, so it's not necessarily a
// spanning tree of that component.  We do a breadth-first traversal.
class FtaTreeWalker
{
public:

	static int visitationKey;

	FtaTreeWalker( void );
	virtual ~FtaTreeWalker( void );

	virtual void PerformWalk( void );
	virtual void VisitPerson( FtaPerson* person );

	wxString rootPersonId;
	int maxAncestorGenerations;
	int maxDescendancyGenerations;
	int maxSpouseJumps;
};

class FtaTreeWalkAccumulator : public FtaTreeWalker
{
public:

	FtaTreeWalkAccumulator( void );
	virtual ~FtaTreeWalkAccumulator( void );

	virtual void PerformWalk( void ) override;
	virtual void VisitPerson( FtaPerson* person ) override;

	FtaPersonSet personSet;
};

// FtaTreeWalker.h