// FtaClient.cpp

#include "FtaClient.h"
#include "FtaTreeCache.h"
#include "FtaMiscCache.h"
#include "FtaApp.h"
#include <wx/textdlg.h>
#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>
#include <wx/utils.h>

FtaClient::FtaClient( void )
{
	curlHandle = nullptr;
}

/*virtual*/ FtaClient::~FtaClient( void )
{
	( void )Shutdown();
}

bool FtaClient::Initialize( void )
{
	CURLcode curlCode = curl_global_init( CURL_GLOBAL_ALL );
	if( curlCode != CURLE_OK )
		return false;

	curl_version_info_data* data = curl_version_info( CURLVERSION_NOW );
	data = nullptr;

	curlHandle = curl_easy_init();
	if( !curlHandle )
		return false;

	//curl_easy_setopt( curlHandle, CURLOPT_DEBUGFUNCTION, &FtaClient::DebugFunction );
	//curl_easy_setopt( curlHandle, CURLOPT_DEBUGDATA, this );
	//curl_easy_setopt( curlHandle, CURLOPT_ERRORBUFFER, errorBuf );
	//curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, &FtaClient::WriteFunction );
	//curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, this );
	//curl_easy_setopt( curlHandle, CURLOPT_READFUNCTION, &FtaClient::ReadFunction );
	//curl_easy_setopt( curlHandle, CURLOPT_READDATA, this );

	return true;
}

bool FtaClient::Shutdown( void )
{
	if( HasAccessToken() )
		( void )DeleteAccessToken();

	if( curlHandle )
	{
		curl_easy_cleanup( curlHandle );
		curlHandle = nullptr;
	}

	curl_global_cleanup();
	return true;
}

bool FtaClient::Authenticate( void )
{
	bool success = false;
	curl_slist* headers = nullptr;

	do
	{
		if( HasAccessToken() )
			break;

		wxBusyCursor busyCursor;

		curl_easy_reset( curlHandle );

		// See https://familysearch.org/developers/docs/certification/authentication for more info.
#if !defined SANDBOX
		wxString notice =
			"FamilyTreeAnalyzer would like to know your basic FamilySearch profile information "
			"and access data about your ancestors from the FamilySearch family tree.  "
			"FamilyTreeAnalyzer will use this information in accordance with their respective terms "
			"of service and privacy policies.  May FamilyTreeAnalyzer proceed?";
		if( wxYES != wxMessageBox( notice, "Notice", wxYES_NO | wxICON_QUESTION ) )
			break;

		wxString userName = wxGetTextFromUser( "Enter username.", "Username", wxEmptyString, nullptr );
		if( userName.IsEmpty() )
			break;

		wxString passWord = wxGetPasswordFromUser( "Enter password.", "Password", wxEmptyString, nullptr );
		if( passWord.IsEmpty() )
			break;
#else
		wxString userName = "tuf000140222";
		wxString passWord = "1234pass";
#endif

		wxString postFields;
		postFields += "username=" + userName;
		postFields += "&grant_type=password";
		postFields += "&client_id=a02j000000BQrUOAA1";
		postFields += "&password=" + passWord;
		const char* postFieldsData = postFields.c_str();

		headers = curl_slist_append( headers, "Content-Type: application/x-www-form-urlencoded" );
		headers = curl_slist_append( headers, "Accept: application/json" );

		curl_easy_setopt( curlHandle, CURLOPT_HTTPHEADER, headers );
		curl_easy_setopt( curlHandle, CURLOPT_POSTFIELDS, postFieldsData );
		curl_easy_setopt( curlHandle, CURLOPT_URL, "https://sandbox.familysearch.org/cis-web/oauth2/v3/token" );
		curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, &FtaClient::WriteFunction );
		curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, this );

		writeString = "";

		CURLcode curlCode = curl_easy_perform( curlHandle );
		if( curlCode != CURLE_OK )
			break;

		if( writeString.IsEmpty() )
			break;

		wxJSONReader reader;
		wxJSONValue responseValue;
		if( 0 < reader.Parse( writeString, &responseValue ) )
			break;

		wxJSONValue errorValue = responseValue[ "error" ];
		if( errorValue.IsString() )
		{
			wxJSONValue errorDescValue = responseValue[ "error_description" ];
			if( errorValue.IsString() )
				wxMessageBox( "Error: " + errorValue.AsString() + " -- " + errorDescValue.AsString(), "Error Response" );

			break;
		}

		wxJSONValue accessTokenValue = responseValue[ "access_token" ];
		if( accessTokenValue.IsNull() )
			break;

		accessToken = accessTokenValue.AsString();

		if( !HasAccessToken() )
			break;

		success = true;
	}
	while( false );

	if( headers )
		curl_slist_free_all( headers );

	return success;
}

bool FtaClient::DeleteAccessToken( void )
{
	bool success = false;
	curl_slist* headers = nullptr;

	do
	{
		if( !HasAccessToken() || !curlHandle )
			break;

		curl_easy_reset( curlHandle );

		wxString authorization = "Authorization: Bearer " + accessToken;
		const char* authorizationData = authorization.c_str();

		headers = curl_slist_append( headers, authorizationData );

		curl_easy_setopt( curlHandle, CURLOPT_HTTPHEADER, headers );
		curl_easy_setopt( curlHandle, CURLOPT_POSTFIELDS, "POST /platform/logout" );
		curl_easy_setopt( curlHandle, CURLOPT_URL, "https://sandbox.familysearch.org" );

		CURLcode curlCode = curl_easy_perform( curlHandle );
		if( curlCode != CURLE_OK )
			break;

		accessToken = "";

		success = true;
	}
	while( false );

	if( headers )
		curl_slist_free_all( headers );

	return success;
}

/*static*/ size_t FtaClient::WriteFunction( void* buf, size_t size, size_t nitems, void* userPtr )
{
	FtaClient* client = ( FtaClient* )userPtr;
	size_t totalBytes = size * nitems;
	client->writeString.Append( ( const char* )buf, totalBytes );
	return totalBytes;
}

/*static*/ size_t FtaClient::ReadFunction( void* buf, size_t size, size_t nitems, void* userPtr )
{
	FtaClient* client = ( FtaClient* )userPtr;

	return 0;
}

/*static*/ int FtaClient::DebugFunction( CURL* curlHandle, curl_infotype type, char* data, size_t size, void* userPtr )
{
	FtaClient* client = ( FtaClient* )userPtr;

	return 0;
}

bool FtaClient::PopulateTreeCacheAt( const wxString& personId )
{
	bool success = false;

	do
	{
		FtaPerson* person = wxGetApp().GetTreeCache()->Lookup( personId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS );
		if( !person )
			break;

		if( !person->SetImmediateAncestry() )
		{
			if( !CacheAncestryFor( personId ) )
				break;

			if( !person->SetImmediateAncestry() )
				break;
		}

		if( !person->SetImmediateDescendancy() )
		{
			if( !CacheDescendancyFor( personId ) )
				break;

			if( !person->SetImmediateDescendancy() )
				break;
		}

		success = true;
	}
	while( false );

	if( !success )
		wxGetApp().GetTreeCache()->Wipe( personId );

	return success;
}

bool FtaClient::CacheAncestryFor( const wxString& personId )
{
	bool success = false;
	curl_slist* headers = nullptr;

	do
	{
		if( !HasAccessToken() )
			break;

		wxBusyCursor busyCursor;

		curl_easy_reset( curlHandle );

		wxString authorization = "Authorization: Bearer " + accessToken;
		const char* authorizationData = authorization.c_str();

		headers = curl_slist_append( headers, authorizationData );
		headers = curl_slist_append( headers, "Accept: application/x-fs-v1+json" );

		curl_easy_setopt( curlHandle, CURLOPT_HTTPHEADER, headers );
		curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, &FtaClient::WriteFunction );
		curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, this );

#if defined SANDBOX
		wxString url = "https://sandbox.familysearch.org";
#else
		wxString url = "https://www.familysearch.org";
#endif

		url += "/platform/tree/ancestry?person=" + personId;
		const char* urlData = url.c_str();

		curl_easy_setopt( curlHandle, CURLOPT_URL, urlData );

		writeString = "";

		CURLcode curlCode = curl_easy_perform( curlHandle );
		if( curlCode != CURLE_OK )
			break;

		if( writeString.IsEmpty() )
			break;

		wxJSONReader reader;
		wxJSONValue responseValue;
		if( 0 < reader.Parse( writeString, &responseValue ) )
			break;

		if( !wxGetApp().GetMiscCache()->ConsumeAncestry( responseValue ) )
			break;

		success = true;
	}
	while( false );

	if( headers )
		curl_slist_free_all( headers );

	return success;
}

bool FtaClient::CacheDescendancyFor( const wxString& personId )
{
	bool success = false;

	do
	{
		if( !HasAccessToken() )
			break;

		//...

		success = true;
	}
	while( false );

	return success;
}

// FtaClient.cpp