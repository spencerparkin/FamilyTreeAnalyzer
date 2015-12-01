// FtaMiscCache.h

#pragma once

#include "FtaContainers.h"

class FtaMiscCache
{
public:

	FtaMiscCache( void );
	~FtaMiscCache( void );

	bool IsEmpty( void ) const;
	bool Wipe( void );

	bool ConsumeAncestry( const wxJSONValue& responseValue );
	bool ConsumeDescendancy( const wxJSONValue& responseValue );

	bool LookupFather( const wxString& childId, wxString& fatherId );
	bool LookupMother( const wxString& childId, wxString& motherId );
	bool LookupChildren( const wxString& parentId, FtaPersonIdSet& childrenIdSet );
	bool LookupSpouses( const wxString& spouseId, FtaPersonIdSet& spousesIdSet );

private:

	wxJSONValue FindAscendancyNumber( long givenAscendancyNumber, const wxJSONValue& personsArrayValue );

	void CacheSpouse( const wxString& spouseId, const wxString& otherSpouseId );
	void CacheFather( const wxString& childId, const wxString& fatherId );
	void CacheMother( const wxString& childId, const wxString& motherId );

	FtaOneToOneRelationshipIdMap childToFatherMap;
	FtaOneToOneRelationshipIdMap childToMotherMap;
	FtaOneToManyRelationshipIdMap parentToChildrenMap;
	FtaOneToManyRelationshipIdMap spouseToSpousesMap;
};

// FtaMiscCache.h