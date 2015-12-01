// FtaContainers.cpp

#include "FtaContainers.h"
#include "FtaPerson.h"
#include <wx/listimpl.cpp>

WX_DEFINE_LIST( JSONValueList );

void FtaDeletePersonMap( FtaPersonMap& personMap )
{
	while( personMap.size() > 0 )
	{
		FtaPersonMap::iterator iter = personMap.begin();
		FtaPerson* person = iter->second;
		delete person;
		personMap.erase( iter );
	}
}

void FtaDeleteRelationshipIdMap( FtaOneToManyRelationshipIdMap& relationshipMap )
{
	while( relationshipMap.size() > 0 )
	{
		FtaOneToManyRelationshipIdMap::iterator iter = relationshipMap.begin();
		FtaPersonIdSet* personIdSet = iter->second;
		delete personIdSet;
		relationshipMap.erase( iter );
	}
}

void FtaDeleteRelationshipMap( FtaOneToManyRelationshipMap& relationshipMap, bool deletePersons /*= false*/ )
{
	while( relationshipMap.size() > 0 )
	{
		FtaOneToManyRelationshipMap::iterator iter = relationshipMap.begin();
		FtaPersonSet* personSet = iter->second;
		if( deletePersons )
		{
			//...
		}
		delete personSet;
		relationshipMap.erase( iter );
	}
}

// FtaContainres.cpp