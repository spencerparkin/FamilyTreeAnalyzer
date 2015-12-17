// FtaClient.h

#pragma once

#include <wx/string.h>
#include <wx/jsonval.h>
#include <curl/curl.h>
#include "FtaContainers.h"
#include "FtaAsyncRequest.h"

class FtaClient : public FtaAsyncRequest::ResponseProcessor
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
	bool AsyncRequestsPending( void );

	virtual bool ProcessResponse( FtaAsyncRequest* request, wxJSONValue& responseValue ) override;

	const wxString& GetAccessToken( void ) { return accessToken; }

	static size_t WriteFunction( void* buf, size_t size, size_t nitems, void* userPtr );
	static size_t ReadFunction( void* buf, size_t size, size_t nitems, void* userPtr );
	static int DebugFunction( CURL* curlHandle, curl_infotype type, char* data, size_t size, void* userPtr );
	static size_t HeaderFunction( void* buf, size_t size, size_t nitems, void* userPtr );

	static bool ReportCurlError( CURLcode curlCode );
	static bool ReportCurlMultiError( CURLMcode curlmCode );

	enum PrivilegeFlag
	{
		PF_LDS_ORDINANCES			= 0x00000001,
	};

	void SetPrivilegeFlags( int privilegeFlags ) { this->privilegeFlags = privilegeFlags; }
	int GetPrivilegeFlags( void ) { return privilegeFlags; }

private:

	FtaAsyncRequestList::iterator FindAsyncRequest( CURL* curlHandleEasy );

	// TODO: Cache the personId of the person authenticated here.  May need to request it from service.
	//       See "read current user" request.
	CURL* curlHandleEasy;
	CURLM* curlHandleMulti;
	wxString accessToken;
	int privilegeFlags;
	char errorBuf[ CURL_ERROR_SIZE ];
	FtaAsyncRequestList asyncRequestList;
	FtaAsyncRequestList asyncRetryList;
};

// FtaClient.h
