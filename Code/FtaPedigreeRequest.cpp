// FtaPedigreeRequest.cpp

#include "FtaPedigreeRequest.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

FtaPedigreeRequest::FtaPedigreeRequest( const wxString& personId, Type type, ResponseProcessor* processor ) : FtaPersonInfoRequest( personId, processor )
{
	this->type = type;
}

/*virtual*/ FtaPedigreeRequest::~FtaPedigreeRequest( void )
{
}

/*virtual*/ bool FtaPedigreeRequest::FormulateRequest( void )
{
	if( !FtaAsyncRequest::FormulateRequest() )
		return false;

	return true;
}

/*virtual*/ bool FtaPedigreeRequest::MakeUrl( wxString& url )
{
	if( !FtaAsyncRequest::MakeUrl( url ) )
		return false;

	switch( type )
	{
		case TYPE_ANCESTRY:
		{
			url += "/platform/tree/ancestry?person=" + personId;
			break;
		}
		case TYPE_DESCENDANCY:
		{
			url += "/platform/tree/descendancy?person=" + personId;
			break;
		}
	}

	return true;
}

/*virtual*/ bool FtaPedigreeRequest::AccumulateInfoInCache( wxJSONValue& responseValue )
{
	bool success = false;

	do
	{
		FtaTreeCache* cache = wxGetApp().GetTreeCache();
		if( !cache )
			break;

		wxJSONValue personsArrayValue = responseValue.Get( "persons", wxJSONValue() );
		if( !personsArrayValue.IsArray() )
			break;

		int size = personsArrayValue.Size();
		int i;
		for( i = 0; i < size; i++ )
		{
			wxJSONValue personValue = personsArrayValue[i];
			wxJSONValue personIdValue = personValue[ "id" ];
			if( !personIdValue.IsString() )
				break;

			wxString personId = personIdValue.AsString();
			FtaPerson* person = cache->Lookup( personId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS );
			if( !person )
				break;

			wxJSONValue ascendancyNumberValue = personValue[ "display" ][ "ascendancyNumber" ];
			if( ascendancyNumberValue.IsString() )
			{
				wxString ascendancyNumberString = ascendancyNumberValue.AsString();
				long ascendancyNumber;
				if( !ascendancyNumberString.ToLong( &ascendancyNumber ) )
					break;

				wxJSONValue fatherValue = FindNumber( 2 * ascendancyNumber, "ascendancyNumber", personsArrayValue );
				if( !fatherValue.IsNull() )
				{
					wxString fatherId = fatherValue[ "id" ].AsString();
					person->SetFatherId( fatherId );
				}

				wxJSONValue motherValue = FindNumber( 2 * ascendancyNumber + 1, "ascendancyNumber", personsArrayValue );
				if( !motherValue.IsNull() )
				{
					wxString motherId = motherValue[ "id" ].AsString();
					person->SetMotherId( motherId );
				}
			}

			wxJSONValue descendancyNumberValue = personValue[ "display" ][ "descendancyNumber" ];
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
						wxString spouseId = spouseValue[ "id" ].AsString();
						FtaPerson* spouse = cache->Lookup( spouseId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS );
						person->GetSpousesIdSet().insert( spouse->GetPersonId() );
						spouse->GetSpousesIdSet().insert( person->GetPersonId() );
					}
				}
				else
				{
					FtaOffsetArray offsetArray;
					GatherNumbersWithPrefix( descendancyNumberString, "descendancyNumber", personsArrayValue, offsetArray, true );

					for( j = 0; j < ( signed )offsetArray.size(); j++ )
					{
						int k = offsetArray[j];
						if( k == i )
							continue;

						wxJSONValue childValue = personsArrayValue[k];
						wxString childId = childValue[ "id" ].AsString();
						person->GetChildrenIdSet().insert( childId );
					}
				}
			}
		}

		if( i < size )
			break;

		FtaPerson* person = cache->Lookup( GetPersonId(), FtaTreeCache::ALLOCATE_ON_CACHE_MISS );
		if( !person )
			break;

		switch( type )
		{
			case TYPE_ANCESTRY:
			{
				person->SetFlags( person->GetFlags() | FtaPerson::FLAG_ANCESTRY );
				break;
			}
			case TYPE_DESCENDANCY:
			{
				person->SetFlags( person->GetFlags() | FtaPerson::FLAG_DESCENDANCY );
				break;
			}
		}

		success = true;
	}
	while( false );

	return success;
}

// FtaPedigreeRequest.cpp