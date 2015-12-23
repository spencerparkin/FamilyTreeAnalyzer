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
	bool Authenticate( wxString& userName );
	bool DeleteAccessToken( void );
	bool HasAccessToken( void ) { return !accessToken.IsEmpty(); }
	bool AddAsyncRequest( FtaAsyncRequest* request, bool rejectIfAlreadyQueued = false, bool deleteIfNotAdded = true );
	bool ServiceAllAsyncRequests( bool waitOnSockets, int signature = -1 );
	bool CompleteAllAsyncRequests( bool showWorkingDialog, int signature = -1 );
	bool CancelAllAsyncRequests( int signature = -1 );
	bool AsyncRequestsPending( int signature = -1 );

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

	void SetCurrentUserPersonId( const wxString& personId ) { currentUserPersonId = personId; }
	const wxString& GetCurrentUserPersonId( void ) const { return currentUserPersonId; }

	static int NewSignature( void ) { return newSignature++; }

private:

	bool ChangeRequestState( FtaAsyncRequest* request, FtaAsyncRequest::State newState );

	FtaAsyncRequestList::iterator FindAsyncRequest( CURL* curlHandleEasy );
	FtaAsyncRequestList::iterator FindAsyncRequest( FtaAsyncRequest* request, bool& pointerMatch, bool performLogicalMatch = true );

	wxString currentUserPersonId;
	CURL* curlHandleEasy;
	CURLM* curlHandleMulti;
	wxString accessToken;
	int privilegeFlags;
	char errorBuf[ CURL_ERROR_SIZE ];
	FtaAsyncRequestList asyncRequestList;
	static int newSignature;
};

// FtaClient.h
