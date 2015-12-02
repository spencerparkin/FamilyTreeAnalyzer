// FtaMiscCache.cpp

#include "FtaMiscCache.h"

FtaMiscCache::FtaMiscCache( void )
{
}

FtaMiscCache::~FtaMiscCache( void )
{
	( void )Wipe();
}

bool FtaMiscCache::Wipe( void )
{
	childToFatherMap.clear();
	childToMotherMap.clear();
	FtaDeleteRelationshipIdMap( parentToChildrenMap );
	FtaDeleteRelationshipIdMap( spouseToSpousesMap );
	return true;
}

bool FtaMiscCache::IsEmpty( void ) const
{
	if( childToFatherMap.size() > 0 )
		return false;

	if( childToMotherMap.size() > 0 )
		return false;

	if( parentToChildrenMap.size() > 0 )
		return false;

	if( spouseToSpousesMap.size() > 0 )
		return false;

	return true;
}

bool FtaMiscCache::ConsumeAncestry( const wxJSONValue& responseValue )
{
	bool success = false;

	do
	{
		wxJSONValue personsArrayValue = responseValue.Get( "persons", wxJSONValue() );
		if( !personsArrayValue.IsArray() )
			break;

		int size = personsArrayValue.Size();
		int i;
		for( i = 0; i < size; i++ )
		{
			wxJSONValue personValue = personsArrayValue[i];
			wxJSONValue ascendancyNumberValue = personValue[ "display" ][ "ascendancyNumber" ];
			wxJSONValue descendancyNumberValue = personValue[ "display" ][ "descendancyNumber" ];

			if( ascendancyNumberValue.IsString() )
			{
				wxString ascendancyNumberString = ascendancyNumberValue.AsString();
				long ascendancyNumber;
				if( !ascendancyNumberString.ToLong( &ascendancyNumber ) )
					break;

				wxJSONValue fatherValue = FindAscendancyNumber( 2 * ascendancyNumber, personsArrayValue );
				if( !fatherValue.IsNull() )
					CacheFather( personValue[ "id" ].AsString(), fatherValue[ "id" ].AsString() );

				wxJSONValue motherValue = FindAscendancyNumber( 2 * ascendancyNumber + 1, personsArrayValue );
				if( !motherValue.IsNull() )
					CacheMother( personValue[ "id" ].AsString(), motherValue[ "id" ].AsString() );
			}

			// Oddly, we find these types of numbers when we asked for ancestry, not descendancy.
			if( descendancyNumberValue.IsString() )
			{
				wxString descendancyNumberString = descendancyNumberValue.AsString();
				int j = descendancyNumberString.Find( "-S" );
				if( j >= 0 )
				{
					descendancyNumberString.Remove( j, j + 2 );
					long descendancyNumber;
					if( !descendancyNumberString.ToLong( &descendancyNumber ) )
						break;

					// Yes, pass in the descendancy number here as the ascendancy number.
					wxJSONValue spouseValue = FindAscendancyNumber( descendancyNumber, personsArrayValue );
					if( !spouseValue.IsNull() )
					{
						CacheSpouse( personValue[ "id" ].AsString(), spouseValue[ "id" ].AsString() );
						CacheSpouse( spouseValue[ "id" ].AsString(), personValue[ "id" ].AsString() );
					}
				}
			}
		}

		if( i < size )
			break;

		success = true;
	}
	while( false );

	return success;
}

wxJSONValue FtaMiscCache::FindAscendancyNumber( long givenAscendancyNumber, const wxJSONValue& personsArrayValue )
{
	int size = personsArrayValue.Size();
	for( int i = 0; i < size; i++ )
	{
		wxJSONValue personValue = ( *const_cast< wxJSONValue* >( &personsArrayValue ) )[i];
		long ascendancyNumber;
		if( personValue[ "display" ][ "ascendancyNumber" ].AsString().ToLong( &ascendancyNumber ) )
			if( ascendancyNumber == givenAscendancyNumber )
				return personValue;
	}

	return wxJSONValue();
}

bool FtaMiscCache::ConsumeDescendancy( const wxJSONValue& responseValue )
{
	return true;
}

void FtaMiscCache::CacheSpouse( const wxString& spouseId, const wxString& otherSpouseId )
{
	FtaOneToManyRelationshipIdMap::iterator iter = spouseToSpousesMap.find( spouseId );
	if( iter == spouseToSpousesMap.end() )
	{
		spouseToSpousesMap[ spouseId ] = new FtaPersonIdSet();
		iter = spouseToSpousesMap.find( spouseId );
	}

	FtaPersonIdSet* personIdSet = iter->second;
	personIdSet->insert( otherSpouseId );
}

void FtaMiscCache::CacheFather( const wxString& childId, const wxString& fatherId )
{
	childToFatherMap[ childId ] = fatherId;
}

void FtaMiscCache::CacheMother( const wxString& childId, const wxString& motherId )
{
	childToMotherMap[ childId ] = motherId;
}

bool FtaMiscCache::LookupFather( const wxString& childId, wxString& fatherId )
{
	FtaOneToOneRelationshipIdMap::iterator iter = childToFatherMap.find( childId );
	if( iter == childToFatherMap.end() )
		return false;

	fatherId = iter->second;
	return true;
}

bool FtaMiscCache::LookupMother( const wxString& childId, wxString& motherId )
{
	FtaOneToOneRelationshipIdMap::iterator iter = childToMotherMap.find( childId );
	if( iter == childToMotherMap.end() )
		return false;

	motherId = iter->second;
	return true;
}

bool FtaMiscCache::LookupChildren( const wxString& parentId, FtaPersonIdSet& childrenIdSet )
{
	FtaOneToManyRelationshipIdMap::iterator iter = parentToChildrenMap.find( parentId );
	if( iter == parentToChildrenMap.end() )
		return false;

	childrenIdSet = *iter->second;
	return true;
}

bool FtaMiscCache::LookupSpouses( const wxString& spouseId, FtaPersonIdSet& spousesIdSet )
{
	FtaOneToManyRelationshipIdMap::iterator iter = spouseToSpousesMap.find( spouseId );
	if( iter == spouseToSpousesMap.end() )
		return false;

	spousesIdSet = *iter->second;
	return true;
}

// FtaMiscCache.cpp