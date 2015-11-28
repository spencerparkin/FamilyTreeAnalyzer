// FtaClient.h

#pragma once

#include <wx/string.h>
#include <curl/curl.h>

class FtaClient
{
public:

	FtaClient( void );
	virtual ~FtaClient( void );

	bool Initialize( void );
	bool Shutdown( void );
	bool Authenticate( void );
	bool HasAccessToken( void ) { return !accessToken.IsEmpty(); }

private:

	static int DebugFunction( CURL* curlHandle, curl_infotype type, char* data, size_t size, void* userPtr );
	static size_t WriteFunction( void* buf, size_t size, size_t nitems, void* userPtr );
	static size_t ReadFunction( void* buf, size_t size, size_t nitems, void* userPtr );

	CURL* curlHandle;
	wxString accessToken;
	char errorBuf[ CURL_ERROR_SIZE ];
	wxString writeString;
	wxString readString;
};

// FtaClient.h
