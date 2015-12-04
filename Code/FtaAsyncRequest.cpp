// FtaAsyncRequest.cpp

#include "FtaAsyncRequest.h"
#include "FtaClient.h"
#include "FtaApp.h"
#include <wx/jsonreader.h>

FtaAsyncRequest::FtaAsyncRequest( ResponseProcessor* processor )
{
	this->processor = processor;
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

/*virtual*/ bool FtaAsyncRequest::ProcessResponse( void )
{
	if( !processor )
		return false;

	if( responseValueString.IsEmpty() )
		return false;

	wxJSONReader jsonReader;
	wxJSONValue responseValue;
	int errorCount = jsonReader.Parse( responseValueString, &responseValue );
	if( errorCount > 0 )
		return false;

	if( !processor->ProcessResponse( this, responseValue ) )
		return false;

	return true;
}

// FtaAsyncRequest.cpp