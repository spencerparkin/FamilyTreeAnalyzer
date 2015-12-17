// FtaAsyncRequest.cpp

#include "FtaAsyncRequest.h"
#include "FtaClient.h"
#include "FtaApp.h"
#include <wx/jsonreader.h>

wxIMPLEMENT_ABSTRACT_CLASS( FtaAsyncRequest, wxObject );

FtaAsyncRequest::FtaAsyncRequest( ResponseProcessor* processor )
{
	this->processor = processor;
	retryTimeSeconds = 0;
	userData = nullptr;
	headers = nullptr;
	curlHandleEasy = curl_easy_init();	// TODO: Alloc from pool of existing handles instead?
}

/*virtual*/ FtaAsyncRequest::~FtaAsyncRequest( void )
{
	if( headers )
		curl_slist_free_all( headers );

	if( curlHandleEasy )
	{
		// TODO: Should/can the curl-handle be recycled for some other request instance?
		//       Would this be a good performance improvement?
		curl_easy_cleanup( curlHandleEasy );
		curlHandleEasy = nullptr;
	}
}

/*virtual*/ bool FtaAsyncRequest::FormulateRequest( void )
{
	wxString accessToken = wxGetApp().GetClient()->GetAccessToken();
	if( accessToken.IsEmpty() )
		return false;

	curl_easy_reset( curlHandleEasy );

	wxString authorization = "Authorization: Bearer " + accessToken;
	const char* authorizationData = authorization.c_str();

	wxASSERT( headers == nullptr );
	headers = curl_slist_append( headers, authorizationData );
	headers = curl_slist_append( headers, "Accept: application/x-fs-v1+json" );

	curl_easy_setopt( curlHandleEasy, CURLOPT_HTTPHEADER, headers );

	wxString url;
	if( MakeUrl( url ) )
	{
		const char* urlData = url.c_str();
		curl_easy_setopt( curlHandleEasy, CURLOPT_URL, urlData );
	}

	curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEFUNCTION, &FtaClient::WriteFunction );
	curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEDATA, &responseValueString );

	curl_easy_setopt( curlHandleEasy, CURLOPT_HEADERFUNCTION, &FtaClient::HeaderFunction );
	curl_easy_setopt( curlHandleEasy, CURLOPT_HEADERDATA, &headerArray );

	return true;
}

/*virtual*/ bool FtaAsyncRequest::MakeUrl( wxString& url )
{
#if defined SANDBOX
	url = "https://sandbox.familysearch.org";
#else
	url = "https://www.familysearch.org";
#endif
	return true;
}

int FtaAsyncRequest::FindHeaderLine( const wxString& pattern, int requiredStartLocation /*= -1*/ )
{
	for( int i = 0; i < ( signed )headerArray.GetCount(); i++ )
	{
		int startLocation = headerArray[i].Find( pattern );
		if( startLocation >= 0 )
			if( requiredStartLocation == -1 || startLocation == requiredStartLocation )
				return i;
	}

	return -1;
}

/*virtual*/ bool FtaAsyncRequest::ProcessResponse( long& retryAfterSeconds )
{
	retryAfterSeconds = -1;

	int i = FindHeaderLine( "HTTP/" );
	if( i < 0 )
		return false;

	httpStatusCode = headerArray[i];
	if( httpStatusCode.Find( "429" ) >= 0 )
	{
		i = FindHeaderLine( "Retry-After:" );
		if( i < 0 )
			return false;

		wxString retryString = headerArray[i];
		retryString.Remove( 0, 13 );
		if( !retryString.ToLong( &retryAfterSeconds ) )
			return false;

		return true;
	}

	if( !responseValueString.IsEmpty() )
	{
		wxJSONReader jsonReader;
		wxJSONValue responseValue;
		int errorCount = jsonReader.Parse( responseValueString, &responseValue );
		if( errorCount > 0 )
			return false;

		if( processor )
		{
			if( !processor->ProcessResponse( this, responseValue ) )
				return false;
		}
		else if( !ProcessJSONResponse( responseValue ) )
			return false;
	}

	return true;
}

// FtaAsyncRequest.cpp