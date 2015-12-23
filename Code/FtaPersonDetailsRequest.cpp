// FtaPersonDetailsRequest.cpp

#include "FtaPersonDetailsRequest.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

wxIMPLEMENT_CLASS( FtaPersonDetailsRequest, FtaPersonInfoRequest );

FtaPersonDetailsRequest::FtaPersonDetailsRequest( const wxString& personId, ResponseProcessor* processor, int signature /*= -1*/ ) : FtaPersonInfoRequest( personId, processor, signature )
{
}

/*virtual*/ FtaPersonDetailsRequest::~FtaPersonDetailsRequest( void )
{
}

/*virtual*/ bool FtaPersonDetailsRequest::MakeUrl( wxString& url )
{
	if( !FtaPersonInfoRequest::MakeUrl( url ) )
		return false;

	url += "/platform/tree/persons/" + personId;
	return true;
}

/*virtual*/ bool FtaPersonDetailsRequest::AccumulateInfoInCache( wxJSONValue& responseValue )
{
	bool success = false;

	do
	{
		FtaTreeCache* cache = wxGetApp().GetTreeCache();
		if( !cache )
			break;

		FtaPerson* person = cache->Lookup( personId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS );
		if( !person )
			break;

		wxJSONValue personsArrayValue = responseValue[ "persons" ];
		if( !personsArrayValue.IsArray() )
			break;

		wxJSONValue personValue = FindPersonId( personId, personsArrayValue );
		if( personValue.IsNull() )
			break;

		wxJSONValue displayValue = personValue[ "display" ];
		if( displayValue.IsNull() )
			break;

		person->SetName( displayValue[ "name" ].AsString() );
		person->SetBirthPlace( displayValue[ "birthPlace" ].AsString() );
		person->SetLifeSpan( displayValue[ "lifespan" ].AsString() );

		wxString genderValueString = displayValue[ "gender" ].AsString();
		if( genderValueString == "Female" )
			person->SetGender( FtaPerson::GENDER_FEMALE );
		else if( genderValueString == "Male" )
			person->SetGender( FtaPerson::GENDER_MALE );
		else
			person->SetGender( FtaPerson::GENDER_UNKNOWN );

		person->SetFlags( person->GetFlags() | FtaPerson::FLAG_PERSONAL_DETAILS );

		success = true;
	}
	while( false );

	return success;
}

// FtaPersonDetailsRequest.cpp