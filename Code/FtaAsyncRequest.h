// FtaAsyncRequest.h

#pragma once

#include <wx/string.h>
#include <wx/jsonval.h>
#include <curl/curl.h>

class FtaAsyncRequest
{
public:

	class ResponseProcessor;

	FtaAsyncRequest( ResponseProcessor* processor );
	virtual ~FtaAsyncRequest( void );

	virtual bool FormulateRequest( void );
	virtual bool ProcessResponse( void );
	virtual bool MakeUrl( wxString& url );

	class ResponseProcessor
	{
	public:
		virtual bool ProcessResponse( FtaAsyncRequest* request, wxJSONValue& responseValue ) = 0;
	};

	CURL* GetCurlHandle( void ) { return curlHandleEasy; }

	void SetCurlCode( CURLcode curlCode ) { this->curlCode = curlCode; }
	CURLcode GetCurlCode( void ) { return curlCode; }

	void SetUserData( void* userData ) { this->userData = userData; }
	void* GetUserData( void ) { return userData; }

protected:

	void* userData;
	ResponseProcessor* processor;
	wxString responseValueString;		// Typically JSON.
	CURL* curlHandleEasy;
	CURLcode curlCode;
	curl_slist* headers;
};

// FtaAsyncRequest.h