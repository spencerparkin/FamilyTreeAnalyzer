// FtaAsyncRequest.cpp

#include "FtaAsyncRequest.h"
#include "FtaClient.h"
#include <wx/jsonreader.h>

FtaAsyncRequest::FtaAsyncRequest( ResponseProcessor* processor )
{
	this->processor = processor;
	userData = nullptr;
	curlHandleEasy = curl_easy_init();
}

/*virtual*/ FtaAsyncRequest::~FtaAsyncRequest( void )
{
	if( curlHandleEasy )
	{
		curl_easy_cleanup( curlHandleEasy );
		curlHandleEasy = nullptr;
	}
}

/*virtual*/ bool FtaAsyncRequest::FormulateRequest( void )
{
#if defined SANDBOX
#else
#endif

	curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEFUNCTION, &FtaClient::WriteFunction );
	curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEDATA, &responseValueString );

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