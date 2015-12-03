// FtaClient.h

#pragma once

#include <wx/string.h>
#include <wx/jsonval.h>
#include <curl/curl.h>

class FtaClient
{
public:

	FtaClient( void );
	virtual ~FtaClient( void );

	bool Initialize( void );
	bool Shutdown( void );
	bool Authenticate( void );
	bool DeleteAccessToken( void );
	bool HasAccessToken( void ) { return !accessToken.IsEmpty(); }

	struct ResponseRequest
	{
		enum Type
		{
			TYPE_ANCESTRY,
			TYPE_DESCENDANCY,
			TYPE_SPOUSES,
			TYPE_PERSONAL_DETAILS,
		};

		Type type;
		wxString personId;
	};

	class ResponseProcessor
	{
	public:
		virtual void ProcessResponse( const ResponseRequest& request, wxJSONValue& responseValue ) = 0;
	};
	
	bool MakeAsyncRequest( const ResponseRequest& request, ResponseProcessor* processor );

private:

	static int DebugFunction( CURL* curlHandle, curl_infotype type, char* data, size_t size, void* userPtr );
	static size_t WriteFunction( void* buf, size_t size, size_t nitems, void* userPtr );
	static size_t ReadFunction( void* buf, size_t size, size_t nitems, void* userPtr );

	CURL* curlHandleEasy;
	wxString accessToken;
	char errorBuf[ CURL_ERROR_SIZE ];
	wxString writeString;
	wxString readString;
};

// FtaClient.h
