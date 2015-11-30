// FtaPerson.cpp

#include "FtaPerson.h"
#include "FtaTreeCache.h"

FtaPerson::FtaPerson( const wxString& personId, FtaTreeCache* cache )
{
	this->personId = personId;
	this->cache = cache;
}

/*virtual*/ FtaPerson::~FtaPerson( void )
{
}

FtaPerson* FtaPerson::GetBiologicalFather( void )
{
	FtaPerson* biologicalFather = cache->Lookup( biologicalFatherId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
	return biologicalFather;
}

FtaPerson* FtaPerson::GetBiologicalMother( void )
{
	FtaPerson* biologicalMother = cache->Lookup( biologicalMotherId, FtaTreeCache::POPULATE_ON_CACHE_MISS );
	return biologicalMother;
}

bool FtaPerson::GetBiologicalChildren( const FtaPerson* spouse, FtaPersonSet& children )
{
	return false;
}

bool FtaPerson::GetSpouses( FtaPersonSet& spouses )
{
	return false;
}

bool FtaPerson::SetImmediateAncestry( const wxJSONValue& responseValue )
{
	bool success = false;

	do
	{
		wxJSONValue personsArrayValue = responseValue.Get( "persons", wxJSONValue() );

		if( !personsArrayValue.IsArray() )
			return false;

		int i;
		for( i = 0; i < personsArrayValue.Size(); i++ )
		{
			wxJSONValue personValue = personsArrayValue[i];
			wxJSONValue displayValue = personValue[ "display" ];
			wxJSONValue ascendancyNumberValue = displayValue[ "ascendancyNumber" ];

			long ascendancyNumber = 0;
			if( !ascendancyNumberValue.AsString().ToCLong( &ascendancyNumber ) )
				break;
			
			if( ascendancyNumber == 2 )
				biologicalFatherId = personValue[ "id" ].AsString();
			else if( ascendancyNumber == 3 )
				biologicalMotherId = personValue[ "id" ].AsString();
		}

		if( i < personsArrayValue.Size() )
			break;

		if( biologicalFatherId.IsEmpty() || biologicalMotherId.IsEmpty() )
			break;

		success = true;
	}
	while( false );

	return success;
}

bool FtaPerson::SetImmediateDescendancy( const wxJSONValue& responseValue )
{
	return false;
}

bool FtaPerson::SetSpouses( const wxJSONValue& responseValue )
{
	return false;
}

// FtaPerson.cpp