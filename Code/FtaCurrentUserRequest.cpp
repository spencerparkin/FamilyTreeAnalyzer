// FtaCurrentUserRequest.cpp

#include "FtaCurrentUserRequest.h"
#include "FtaClient.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_CLASS( FtaCurrentUserRequest, FtaAsyncRequest );

FtaCurrentUserRequest::FtaCurrentUserRequest( int signature /*= -1*/ ) : FtaAsyncRequest( nullptr, signature )
{
}

/*virtual*/ FtaCurrentUserRequest::~FtaCurrentUserRequest( void )
{
}

/*virtual*/ bool FtaCurrentUserRequest::ProcessJSONResponse( wxJSONValue& responseValue )
{
	// Why an array?
	wxJSONValue usersArrayValue = responseValue[ "users" ];
	if( !usersArrayValue.IsArray() )
		return false;

	wxJSONValue userValue = usersArrayValue[0];
	if( userValue.IsNull() )
		return false;

	wxJSONValue personIdValue = userValue[ "personId" ];
	if( !personIdValue.IsString() )
		return false;

	wxString personId = personIdValue.AsString();
	wxGetApp().GetClient()->SetCurrentUserPersonId( personId );
	wxGetApp().GetFrame()->AddLogMessage( "Your person ID: " + personId );

	wxJSONValue displayNameValue = userValue[ "displayName" ];
	if( displayNameValue.IsString() )
	{
		wxString displayName = displayNameValue.AsString();
		wxGetApp().GetFrame()->AddLogMessage( "Hello, " + displayName + "!" );
	}

	return true;
}

/*virtual*/ bool FtaCurrentUserRequest::MakeUrl( wxString& url )
{
	if( !FtaAsyncRequest::MakeUrl( url ) )
		return false;

	url += "/platform/users/current";
	return true;
}

// FtaCurrentUserRequest.cpp