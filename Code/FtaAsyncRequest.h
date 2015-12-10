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
	virtual bool ProcessResponse( long& retryAfterSeconds );
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

	void SetRetryTime( long retryTimeSeconds ) { this->retryTimeSeconds = retryTimeSeconds; }
	int GetRetryTime( void ) const { return retryTimeSeconds; }

protected:

	int FindHeaderLine( const wxString& pattern );
	
	long retryTimeSeconds;
	wxString httpStatusCode;
	void* userData;
	ResponseProcessor* processor;
	wxString responseValueString;		// Typically JSON.
	wxArrayString headerArray;
	CURL* curlHandleEasy;
	CURLcode curlCode;
	curl_slist* headers;
};

// FtaAsyncRequest.h