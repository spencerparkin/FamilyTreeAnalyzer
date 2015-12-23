// FtaPersonInfoRequest.cpp

#include "FtaPersonInfoRequest.h"

wxIMPLEMENT_ABSTRACT_CLASS( FtaPersonInfoRequest, FtaAsyncRequest );

FtaPersonInfoRequest::FtaPersonInfoRequest( const wxString& personId, ResponseProcessor* processor, int signature /*= -1*/ ) : FtaAsyncRequest( processor, signature )
{
	this->personId = personId;
}

/*virtual*/ FtaPersonInfoRequest::~FtaPersonInfoRequest( void )
{
}

/*virtual*/ bool FtaPersonInfoRequest::Matches( FtaAsyncRequest* request )
{
	if( !request->IsKindOf( &FtaPersonInfoRequest::ms_classInfo ) )
		return false;

	FtaPersonInfoRequest* personInfoRequest = ( FtaPersonInfoRequest* )request;
	if( personId != personInfoRequest->GetPersonId() )
		return false;

	if( GetClassInfo() != request->GetClassInfo() )
		return false;

	return true;
}

wxJSONValue FtaPersonInfoRequest::FindPersonId( const wxString& personId, const wxJSONValue& personsArrayValue )
{
	int size = personsArrayValue.Size();
	for( int i = 0; i < size; i++ )
	{
		wxJSONValue personValue = ( *const_cast< wxJSONValue* >( &personsArrayValue ) )[i];
		if( personValue[ "id" ].AsString() == personId )
			return personValue;
	}

	return wxJSONValue();
}

wxJSONValue FtaPersonInfoRequest::FindNumber( long givenNumber, const wxString& type, const wxJSONValue& personsArrayValue )
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

wxJSONValue FtaPersonInfoRequest::FindNumberString( const wxString& givenNumberString, const wxString& type, wxJSONValue& personsArrayValue )
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

void FtaPersonInfoRequest::GatherNumbersWithPrefix( const wxString& prefix, const wxString& type, const wxJSONValue& personsArrayValue, FtaOffsetArray& offsetArray, bool excludeSpouses )
{
	offsetArray.clear();

	int size = personsArrayValue.Size();
	for( int i = 0; i < size; i++ )
	{
		wxJSONValue personValue = ( *const_cast< wxJSONValue* >( &personsArrayValue ) )[i];
		wxString numberString = personValue[ "display" ][ type ].AsString();
		if( excludeSpouses && numberString.Find( "-S" ) >= 0 )
			continue;
		if( numberString.find( prefix ) == 0 )
			offsetArray.push_back(i);
	}
}

// FtaPersonRequest.cpp