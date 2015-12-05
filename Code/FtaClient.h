// FtaClient.h

#pragma once

#include <wx/string.h>
#include <wx/jsonval.h>
#include <curl/curl.h>
#include "FtaContainers.h"

class FtaAsyncRequest;

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
	bool AddAsyncRequest( FtaAsyncRequest* request );
	bool ServiceAllAsyncRequests( bool waitOnSockets );
	bool CompleteAllAsyncRequests( bool showWorkingDialog );
	bool CancelAllAsyncRequests( void );
	bool AsyncRequestsPending( void ) { return( asyncRequestList.size() > 0 ? true : false ); }

	const wxString& GetAccessToken( void ) { return accessToken; }

	static size_t WriteFunction( void* buf, size_t size, size_t nitems, void* userPtr );
	static size_t ReadFunction( void* buf, size_t size, size_t nitems, void* userPtr );
	static int DebugFunction( CURL* curlHandle, curl_infotype type, char* data, size_t size, void* userPtr );

private:

	FtaAsyncRequestList::iterator FindAsyncRequest( CURL* curlHandleEasy );

	CURL* curlHandleEasy;
	CURLM* curlHandleMulti;
	wxString accessToken;
	char errorBuf[ CURL_ERROR_SIZE ];
	FtaAsyncRequestList asyncRequestList;
};

// FtaClient.h
