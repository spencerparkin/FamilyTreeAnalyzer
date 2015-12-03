// FtaTreeWalker.cpp

#include "FtaTreeWalker.h"
#include "FtaPerson.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

int FtaTreeWalker::key = -1;

FtaTreeWalker::FtaTreeWalker( void )
{
	maxAncestorGenerations = 0;
	maxDescendancyGenerations = 0;
	maxSpouseJumps = 0;
}

/*virtual*/ FtaTreeWalker::~FtaTreeWalker( void )
{
}

/*virtual*/ void FtaTreeWalker::PerformWalk( void )
{
	key++;

	FtaPerson* person = wxGetApp().GetTreeCache()->Lookup( rootPersonId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
	if( !person )
		return;

	person->visitationData.generation = 0;
	person->visitationData.spouseJumpCount = 0;

	FtaPersonList personQueue;
	personQueue.push_back( person );
	while( personQueue.size() > 0 )
	{
		FtaPersonList::iterator iter = personQueue.begin();
		person = *iter;
		personQueue.erase( iter );

		wxASSERT( person->visitationData.visitKey != key );

		person->visitationData.visitKey = key;

		VisitPerson( person );

		int generation = person->visitationData.generation;
		int spouseJumpCount = person->visitationData.spouseJumpCount;

		FtaPersonList candidateList;
		if( generation < maxAncestorGenerations )
		{
			FtaPerson* father = person->GetBiologicalFather();
			if( father )
			{
				father->visitationData.generation = generation + 1;
				candidateList.push_back( father );
			}

			FtaPerson* mother = person->GetBiologicalMother();
			if( mother )
			{
				mother->visitationData.generation = generation + 1;
				candidateList.push_back( mother );
			}
		}

		if( -generation < maxDescendancyGenerations )
		{
			FtaPersonSet childrenSet;
			( void )person->GetBiologicalChildren( childrenSet );

			FtaPersonSet::iterator childIter = childrenSet.begin();
			while( childIter != childrenSet.end() )
			{
				FtaPerson* child = *childIter;
				child->visitationData.generation = generation - 1;
				candidateList.push_back( child );
				childIter++;
			}
		}

		if( spouseJumpCount < maxSpouseJumps )
		{
			FtaPersonSet spousesSet;
			( void )person->GetSpouses( spousesSet );

			FtaPersonSet::iterator spouseIter = spousesSet.begin();
			while( spouseIter != spousesSet.end() )
			{
				FtaPerson* spouse = *spouseIter;
				spouse->visitationData.spouseJumpCount = spouseJumpCount + 1;
				candidateList.push_back( spouse );
				spouseIter++;
			}
		}

		// Enqueue all unvisited, unqueued and qualified adjacencies.
		FtaPersonList::iterator candidateIter = candidateList.begin();
		while( candidateIter != candidateList.end() )
		{
			FtaPerson* candidate = *candidateIter;
			if( candidate->visitationData.visitKey != key && candidate->visitationData.queueKey != key )
			{
				personQueue.push_back( candidate );
				candidate->visitationData.queueKey = key;
			}

			candidateIter++;
		}
	}
}

/*virtual*/ void FtaTreeWalker::VisitPerson( FtaPerson* person )
{
}

FtaTreeWalkAccumulator::FtaTreeWalkAccumulator( void )
{
}

/*virtual*/ FtaTreeWalkAccumulator::~FtaTreeWalkAccumulator( void )
{
}

/*virtual*/ void FtaTreeWalkAccumulator::PerformWalk( void )
{
	personSet.clear();

	FtaTreeWalker::PerformWalk();
}

/*virtual*/ void FtaTreeWalkAccumulator::VisitPerson( FtaPerson* person )
{
	personSet.insert( person );
}

// A note that relationship sequences can undergo compression...
// "my father's father's wife's daughter"
// "my father's mother's daughter"
// "my father's (half?) sister"
// "my aunt"

// FtaTreeWalker.cpp