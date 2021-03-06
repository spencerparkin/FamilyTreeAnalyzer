// FtaAsyncRequest.h

#pragma once

#include <wx/object.h>
#include <wx/string.h>
#include <wx/jsonval.h>
#include <curl/curl.h>

class FtaAsyncRequest : public wxObject
{
public:

	wxDECLARE_ABSTRACT_CLASS( FtaAsyncRequest );

	class ResponseProcessor;

	FtaAsyncRequest( ResponseProcessor* processor = nullptr, int signature = -1 );
	virtual ~FtaAsyncRequest( void );

	virtual bool FormulateRequest( void );
	virtual bool ProcessResponse( long& retryAfterSeconds );
	virtual bool ProcessJSONResponse( wxJSONValue& responseValue ) { return true; }
	virtual bool MakeUrl( wxString& url );
	virtual bool Matches( FtaAsyncRequest* request ) { return false; }

	class ResponseProcessor
	{
	public:
		virtual bool ProcessResponse( FtaAsyncRequest* request, wxJSONValue* responseValue ) = 0;
	};

	CURL* GetCurlHandle( void ) { return curlHandleEasy; }

	void SetCurlCode( CURLcode curlCode ) { this->curlCode = curlCode; }
	CURLcode GetCurlCode( void ) { return curlCode; }

	void SetUserData( void* userData ) { this->userData = userData; }
	void* GetUserData( void ) { return userData; }

	void SetRetryTime( long retryTimeSeconds ) { this->retryTimeSeconds = retryTimeSeconds; }
	int GetRetryTime( void ) const { return retryTimeSeconds; }

	void SetSignature( int signature ) { this->signature = signature; }
	int GetSignature( void ) const { return signature; }

	enum State
	{
		STATE_NONE,
		STATE_PENDING,
		STATE_THROTTLED,
	};

	void SetState( State state ) { this->state = state; }
	State GetState( void ) const { return state; }

protected:

	// We might want to provide a version of this routine that uses regular expressions.
	int FindHeaderLine( const wxString& pattern, int requiredStartLocation = -1 );
	
	State state;
	int signature;
	long retryTimeSeconds;
	wxString httpStatusCode;
	void* userData;
	ResponseProcessor* processor;
	wxString responseValueString;		// Must be JSON if used.
	wxArrayString headerArray;
	CURL* curlHandleEasy;
	CURLcode curlCode;
	curl_slist* headers;
};

// FtaAsyncRequest.h