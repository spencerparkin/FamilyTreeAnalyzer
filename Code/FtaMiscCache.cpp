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

bool FtaMiscCache::ConsumePedigree( const wxJSONValue& responseValue )
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

				wxJSONValue fatherValue = FindNumber( 2 * ascendancyNumber, "ascendancyNumber", personsArrayValue );
				if( !fatherValue.IsNull() )
				{
					CacheFather( personValue[ "id" ].AsString(), fatherValue[ "id" ].AsString() );
					CacheChild( fatherValue[ "id" ].AsString(), personValue[ "id" ].AsString() );
				}

				wxJSONValue motherValue = FindNumber( 2 * ascendancyNumber + 1, "ascendancyNumber", personsArrayValue );
				if( !motherValue.IsNull() )
				{
					CacheMother( personValue[ "id" ].AsString(), motherValue[ "id" ].AsString() );
					CacheChild( motherValue[ "id" ].AsString(), personValue[ "id" ].AsString() );
				}
			}

			if( descendancyNumberValue.IsString() )
			{
				wxString descendancyNumberString = descendancyNumberValue.AsString();
				int j = descendancyNumberString.Find( "-S" );
				if( j >= 0 )
				{
					descendancyNumberString.Remove( j, j + 2 );
					
					wxJSONValue spouseValue = FindNumberString( descendancyNumberString, "descendancyNumber", personsArrayValue );
					if( !spouseValue.IsNull() )
					{
						CacheSpouse( personValue[ "id" ].AsString(), spouseValue[ "id" ].AsString() );
						CacheSpouse( spouseValue[ "id" ].AsString(), personValue[ "id" ].AsString() );
					}
				}
				else
				{
					FtaIndexArray indexArray;
					GatherNumbersWithPrefix( descendancyNumberString, "descendancyNumber", personsArrayValue, indexArray, true );

					for( j = 0; j < ( signed )indexArray.size(); j++ )
					{
						int k = indexArray[j];
						if( k == i )
							continue;

						wxJSONValue childValue = personsArrayValue[k];
						CacheChild( personValue[ "id" ].AsString(), childValue[ "id" ].AsString() );
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

wxJSONValue FtaMiscCache::FindNumber( long givenNumber, const wxString& type, const wxJSONValue& personsArrayValue )
{
	int size = personsArrayValue.Size();
	for( int i = 0; i < size; i++ )
	{
		wxJSONValue personValue = ( *const_cast< wxJSONValue* >( &personsArrayValue ) )[i];
		long number;
		if( personValue[ "display" ][ type ].AsString().ToLong( &number ) )
			if( number == givenNumber )
				return personValue;
	}

	return wxJSONValue();
}

wxJSONValue FtaMiscCache::FindNumberString( const wxString& givenNumberString, const wxString& type, wxJSONValue& personsArrayValue )
{
	int size = personsArrayValue.Size();
	for( int i = 0; i < size; i++ )
	{
		wxJSONValue personValue = ( *const_cast< wxJSONValue* >( &personsArrayValue ) )[i];
		wxString numberString = personValue[ "display" ][ type ].AsString();
		if( numberString == givenNumberString )
			return personValue;
	}

	return wxJSONValue();
}

void FtaMiscCache::GatherNumbersWithPrefix( const wxString& prefix, const wxString& type, const wxJSONValue& personsArrayValue, FtaIndexArray& indexArray, bool excludeSpouses )
{
	indexArray.clear();

	int size = personsArrayValue.Size();
	for( int i = 0; i < size; i++ )
	{
		wxJSONValue personValue = ( *const_cast< wxJSONValue* >( &personsArrayValue ) )[i];
		wxString numberString = personValue[ "display" ][ type ].AsString();
		if( excludeSpouses && numberString.Find( "-S" ) >= 0 )
			continue;
		if( numberString.find( prefix ) == 0 )
			indexArray.push_back(i);
	}
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

void FtaMiscCache::CacheChild( const wxString& parentId, const wxString& childId )
{
	FtaOneToManyRelationshipIdMap::iterator iter = parentToChildrenMap.find( parentId );
	if( iter == parentToChildrenMap.end() )
	{
		parentToChildrenMap[ parentId ] = new FtaPersonIdSet();
		iter = parentToChildrenMap.find( parentId );
	}

	FtaPersonIdSet* personIdSet = iter->second;
	personIdSet->insert( childId );
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

bool FtaMiscCache::LookupChildren( const wxString& parentId, FtaPersonIdSet*& childrenIdSet )
{
	FtaOneToManyRelationshipIdMap::iterator iter = parentToChildrenMap.find( parentId );
	if( iter == parentToChildrenMap.end() )
		return false;

	childrenIdSet = &( *iter->second );
	return true;
}

bool FtaMiscCache::LookupSpouses( const wxString& spouseId, FtaPersonIdSet*& spousesIdSet )
{
	FtaOneToManyRelationshipIdMap::iterator iter = spouseToSpousesMap.find( spouseId );
	if( iter == spouseToSpousesMap.end() )
		return false;

	spousesIdSet = &( *iter->second );
	return true;
}

// FtaMiscCache.cpp