// FtaClient.cpp

#include "FtaClient.h"
#include <wx/textdlg.h>
#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

FtaClient::FtaClient( void )
{
	curlHandle = nullptr;
}

/*virtual*/ FtaClient::~FtaClient( void )
{

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

	curl_easy_setopt( curlHandle, CURLOPT_DEBUGFUNCTION, &FtaClient::DebugFunction );
	curl_easy_setopt( curlHandle, CURLOPT_DEBUGDATA, this );
	curl_easy_setopt( curlHandle, CURLOPT_ERRORBUFFER, errorBuf );
	curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, &FtaClient::WriteFunction );
	curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, this );
	//curl_easy_setopt( curlHandle, CURLOPT_READFUNCTION, &FtaClient::ReadFunction );
	//curl_easy_setopt( curlHandle, CURLOPT_READDATA, this );

	return true;
}

bool FtaClient::Shutdown( void )
{
	if( curlHandle )
	{
		//...
	}

	curl_global_cleanup();
	return true;
}

bool FtaClient::Authenticate( void )
{
	bool success = false;
	curl_slist* header = nullptr;

	do
	{
		wxString notice =
			"FamilyTreeAnalyzer would like to know your basic FamilySearch profile information "
			"and access data about your ancestors from the FamilySearch family tree.  "
			"FamilyTreeAnalyzer will use this information in accordance with their respective terms "
			"of service and privacy policies.  May FamilyTreeAnalyzer proceed?";
		if( wxYES != wxMessageBox( notice, "Notice", wxYES_NO | wxICON_QUESTION ) )
			break;

		/*
		wxString userName = wxGetTextFromUser( "Enter username.", "Username", wxEmptyString, nullptr );
		if( userName.IsEmpty() )
			break;

		wxString passWord = wxGetPasswordFromUser( "Enter password.", "Password", wxEmptyString, nullptr );
		if( passWord.IsEmpty() )
			break;

		// See: https://familysearch.org/developers/docs/certification/authentication
		*/

		wxJSONValue blob;
		blob[ "grant_type " ] = wxT( "password" );
		blob[ "client_id" ] = wxT( "a02j000000BQrUOAA1" );		// This was registered at the FamilySearch developer site.
		blob[ "username" ] = wxT( "tuf000140222" );
		blob[ "password" ] = wxT( "1234pass" );

		wxJSONWriter writer;
		wxString jsonText;
		writer.Write( blob, jsonText );

		header = curl_slist_append( header, "Content-Type: text/json" );

		curl_easy_setopt( curlHandle, CURLOPT_POSTFIELDS, jsonText.c_str() );
		curl_easy_setopt( curlHandle, CURLOPT_POSTFIELDSIZE, jsonText.Length() );
		curl_easy_setopt( curlHandle, CURLOPT_URL, "https://sandbox.familysearch.org/cis-web/oauth2/v3/token" );

		writeString = "";

		CURLcode curlCode = curl_easy_perform( curlHandle );
		if( curlCode != CURLE_OK )
			break;

		if( writeString.IsEmpty() )
			break;

		wxJSONReader reader;
		if( 0 < reader.Parse( writeString, &blob ) )
			break;

		// TODO: We need to capture the access token.

		success = true;
	}
	while( false );

	if( header )
		curl_slist_free_all( header );

	return success;
}

/*static*/ size_t FtaClient::WriteFunction( void* buf, size_t size, size_t nitems, void* userPtr )
{
	FtaClient* client = ( FtaClient* )userPtr;
	size_t totalBytes = size * nitems;
	client->writeString.Append( ( const char* )buf, totalBytes );
	return size;
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

// FtaClient.cpp