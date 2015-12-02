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

	bool ConsumePedigree( const wxJSONValue& responseValue );

	bool LookupFather( const wxString& childId, wxString& fatherId );
	bool LookupMother( const wxString& childId, wxString& motherId );
	bool LookupChildren( const wxString& parentId, FtaPersonIdSet*& childrenIdSet );
	bool LookupSpouses( const wxString& spouseId, FtaPersonIdSet*& spousesIdSet );

private:

	wxJSONValue FindNumber( long givenNumber, const wxString& type, const wxJSONValue& personsArrayValue );
	wxJSONValue FindNumberString( const wxString& givenNumberString, const wxString& type, wxJSONValue& personsArrayValue );
	void GatherNumbersWithPrefix( const wxString& prefix, const wxString& type, const wxJSONValue& personsArrayValue, FtaIndexArray& indexArray, bool excludeSpouses );

	void CacheSpouse( const wxString& spouseId, const wxString& otherSpouseId );
	void CacheFather( const wxString& childId, const wxString& fatherId );
	void CacheMother( const wxString& childId, const wxString& motherId );
	void CacheChild( const wxString& parentId, const wxString& childId );

	FtaOneToOneRelationshipIdMap childToFatherMap;
	FtaOneToOneRelationshipIdMap childToMotherMap;
	FtaOneToManyRelationshipIdMap parentToChildrenMap;
	FtaOneToManyRelationshipIdMap spouseToSpousesMap;
};

// FtaMiscCache.h