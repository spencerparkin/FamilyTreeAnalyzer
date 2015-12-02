// FtaPerson.h

#pragma once

#include "FtaContainers.h"

class FtaTreeCache;

// To keep things simpler, a person appearing in the tree cache must have
// complete (as apposed to partial) information (i.e., all known information
// about the person must be present here.)
class FtaPerson
{
public:

	FtaPerson( const wxString& personId );
	virtual ~FtaPerson( void );

	const wxString& GetPersonId( void ) const { return personId; }

	FtaPerson* GetBiologicalFather( void );
	FtaPerson* GetBiologicalMother( void );

	// These are all those with whom this person had biological children, regardless of marriage.
	// But it must also include those spouses with which this person may not have had any biological children.
	bool GetSpouses( FtaPersonSet& spousesSet );

	// These are the biological off-spring of this person.
	bool GetBiologicalChildren( FtaPersonSet& childrenSet );

	// For each spouse is given the set of children had with that spouse.
	bool GetBiologicalChildren( FtaOneToManyRelationshipMap& spouseToChildrenMap );

	// These pull from the miscellaneous cache.
	bool SetImmediateAncestry( void );
	bool SetImmediateDescendancy( void );
	bool SetSpouses( void );

	struct VisitationData
	{
		int key;
		int generation;
		int spouseJumpCount;
	};

	mutable VisitationData visitationData;

private:

	wxString personId;
	wxString biologicalFatherId;	// Point these into the misc. cache too?
	wxString biologicalMotherId;
	FtaPersonIdSet* childrenIdSet;	// Stale pointer warning: these point into the misc. cache.
	FtaPersonIdSet* spousesIdSet;

	// TODO: Know ordinance information.
};

// FtaPerson.h
