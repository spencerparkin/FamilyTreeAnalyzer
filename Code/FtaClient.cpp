// FtaClient.cpp

#include "FtaClient.h"
#include "FtaTreeCache.h"
#include "FtaFrame.h"
#include "FtaAsyncRequest.h"
#include "FtaCurrentUserRequest.h"
#include "FtaApp.h"
#include <wx/textdlg.h>
#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>
#include <wx/utils.h>
#include <wx/progdlg.h>

FtaClient::FtaClient( void )
{
	curlHandleEasy = nullptr;
	curlHandleMulti = nullptr;

	privilegeFlags = 0;
}

/*virtual*/ FtaClient::~FtaClient( void )
{
	( void )Shutdown();
}

bool FtaClient::Initialize( void )
{
	CURLcode curlCode = curl_global_init( CURL_GLOBAL_ALL );
	if( curlCode != CURLE_OK )
		return false;

	curl_version_info_data* data = curl_version_info( CURLVERSION_NOW );
	data = nullptr;

	curlHandleEasy = curl_easy_init();
	if( !curlHandleEasy )
		return false;

	curlHandleMulti = curl_multi_init();
	if( !curlHandleMulti )
		return false;

	return true;
}

bool FtaClient::Shutdown( void )
{
	if( HasAccessToken() )
		( void )DeleteAccessToken();

	if( curlHandleEasy )
	{
		curl_easy_cleanup( curlHandleEasy );
		curlHandleEasy = nullptr;
	}

	CancelAllAsyncRequests();

	if( curlHandleMulti )
	{
		curl_multi_cleanup( curlHandleMulti );
		curlHandleMulti = nullptr;
	}

	curl_global_cleanup();
	return true;
}

bool FtaClient::Authenticate( void )
{
	bool success = false;
	curl_slist* headers = nullptr;

	do
	{
		if( HasAccessToken() )
			break;

		wxBusyCursor busyCursor;

		privilegeFlags = 0;

		curl_easy_reset( curlHandleEasy );

		// See https://familysearch.org/developers/docs/certification/authentication for more info.
#if !defined SANDBOX
		wxString notice =
			"FamilyTreeAnalyzer would like to know your basic FamilySearch profile information "
			"and access data about your ancestors from the FamilySearch family tree.  "
			"FamilyTreeAnalyzer will use this information in accordance with their respective terms "
			"of service and privacy policies.\n\nMay FamilyTreeAnalyzer proceed?";
		if( wxYES != wxMessageBox( notice, "Notice", wxYES_NO | wxICON_QUESTION ) )
			break;

		wxString userName = wxGetTextFromUser( "Enter username.", "Username", wxEmptyString, nullptr );
		if( userName.IsEmpty() )
			break;

		wxString passWord = wxGetPasswordFromUser( "Enter password.", "Password", wxEmptyString, nullptr );
		if( passWord.IsEmpty() )
			break;
#else
		wxString userName = "tuf000140222";
		wxString passWord = "1234pass";
#endif

		wxString postFields;
		postFields += "username=" + userName;
		postFields += "&grant_type=password";
		postFields += "&client_id=a02j000000BQrUOAA1";
		postFields += "&password=" + passWord;
		const char* postFieldsData = postFields.c_str();

		headers = curl_slist_append( headers, "Content-Type: application/x-www-form-urlencoded" );
		headers = curl_slist_append( headers, "Accept: application/json" );

		wxString responseString;

		curl_easy_setopt( curlHandleEasy, CURLOPT_HTTPHEADER, headers );
		curl_easy_setopt( curlHandleEasy, CURLOPT_POSTFIELDS, postFieldsData );
		curl_easy_setopt( curlHandleEasy, CURLOPT_URL, "https://sandbox.familysearch.org/cis-web/oauth2/v3/token" );
		curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEFUNCTION, &FtaClient::WriteFunction );
		curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEDATA, &responseString );

		CURLcode curlCode = curl_easy_perform( curlHandleEasy );
		if( ReportCurlError( curlCode ) )
			break;

		if( responseString.IsEmpty() )
			break;

		wxJSONReader reader;
		wxJSONValue responseValue;
		if( 0 < reader.Parse( responseString, &responseValue ) )
			break;

		wxJSONValue errorValue = responseValue[ "error" ];
		if( errorValue.IsString() )
		{
			wxJSONValue errorDescValue = responseValue[ "error_description" ];
			if( errorValue.IsString() )
				wxMessageBox( "Error: " + errorValue.AsString() + " -- " + errorDescValue.AsString(), "Error Response" );

			break;
		}

		wxJSONValue accessTokenValue = responseValue[ "access_token" ];
		if( accessTokenValue.IsNull() )
			break;

		accessToken = accessTokenValue.AsString();

		if( !HasAccessToken() )
			break;

		wxGetApp().GetFrame()->AddLogMessage( "Authenticated!" );

		AddAsyncRequest( new FtaCurrentUserRequest() );
		CompleteAllAsyncRequests( false );

		success = true;
	}
	while( false );

	if( headers )
		curl_slist_free_all( headers );

	return success;
}

bool FtaClient::DeleteAccessToken( void )
{
	bool success = false;
	curl_slist* headers = nullptr;

	do
	{
		if( !HasAccessToken() || !curlHandleEasy )
			break;

		privilegeFlags = 0;

		curl_easy_reset( curlHandleEasy );

		wxString authorization = "Authorization: Bearer " + accessToken;
		const char* authorizationData = authorization.c_str();

		headers = curl_slist_append( headers, authorizationData );

		curl_easy_setopt( curlHandleEasy, CURLOPT_HTTPHEADER, headers );
		curl_easy_setopt( curlHandleEasy, CURLOPT_POSTFIELDS, "POST /platform/logout" );
		curl_easy_setopt( curlHandleEasy, CURLOPT_URL, "https://sandbox.familysearch.org" );

		CURLcode curlCode = curl_easy_perform( curlHandleEasy );
		if( ReportCurlError( curlCode ) )
			break;

		accessToken = "";

		// In compliance with read certification, a log-out requires we purge the cache.
		if( !wxGetApp().GetTreeCache()->Wipe() )
			break;

		if( wxGetApp().GetFrame() )
			wxGetApp().GetFrame()->AddLogMessage( "Logged out!" );

		success = true;
	}
	while( false );

	if( headers )
		curl_slist_free_all( headers );

	return success;
}

/*virtual*/ bool FtaClient::ProcessResponse( FtaAsyncRequest* request, wxJSONValue& responseValue )
{
	return true;
}

/*static*/ bool FtaClient::ReportCurlError( CURLcode curlCode )
{
	if( curlCode == CURLE_OK )
		return false;

	wxString error = "Curl error unknown.";
	switch( curlCode )
	{
		case CURLE_COULDNT_RESOLVE_HOST:
		{
			error = "Failed to resolve host.";
			break;
		}
		case CURLE_URL_MALFORMAT:
		{
			error = "Malformed URL.";
			break;
		}
	}

	wxMessageDialog messageDialog( nullptr, error, "Curl Error", wxOK | wxCENTRE | wxICON_ERROR );
	messageDialog.ShowModal();
	return true;
}

/*static*/ bool FtaClient::ReportCurlMultiError( CURLMcode curlmCode )
{
	if( curlmCode == CURLM_OK )
		return false;

	wxString error = "Curl-multi error unknown.";
	switch( curlmCode )
	{
		default:
		{
			break;
		}
	}

	wxMessageDialog messageDialog( nullptr, error, "Curl Error", wxOK | wxCENTRE | wxICON_ERROR );
	messageDialog.ShowModal();
	return true;
}

/*static*/ size_t FtaClient::WriteFunction( void* buf, size_t size, size_t nitems, void* userPtr )
{
	wxString* responseString = ( wxString* )userPtr;
	size_t totalBytes = size * nitems;
	responseString->Append( ( const char* )buf, totalBytes );
	return totalBytes;
}

/*static*/ size_t FtaClient::ReadFunction( void* buf, size_t size, size_t nitems, void* userPtr )
{
	return 0;
}

/*static*/ int FtaClient::DebugFunction( CURL* curlHandleEasy, curl_infotype type, char* data, size_t size, void* userPtr )
{
	return 0;
}

/*static*/ size_t FtaClient::HeaderFunction( void* buf, size_t size, size_t nitems, void* userPtr )
{
	wxArrayString* arrayString = ( wxArrayString* )userPtr;
	wxString responseString;
	size_t totalBytes = size * nitems;
	responseString.Append( ( const char* )buf, totalBytes );
	arrayString->push_back( responseString );
	return totalBytes;
}

bool FtaClient::CompleteAllAsyncRequests( bool showWorkingDialog )
{
	bool success = true;

	wxGenericProgressDialog* progressDialog = nullptr;
	wxBusyCursor* busyCursor = nullptr;

	if( showWorkingDialog )
		progressDialog = new wxGenericProgressDialog( "Requests Pending...", "Working...", 100, nullptr, wxPD_APP_MODAL | wxPD_CAN_ABORT );
	else
		busyCursor = new wxBusyCursor();

	while( AsyncRequestsPending() )
	{
		if( !ServiceAllAsyncRequests( true ) )
		{
			success = false;
			break;
		}

		if( progressDialog )
		{
			if( progressDialog->WasCancelled() )
				break;

			int newValue = progressDialog->GetValue() + 1;
			if( newValue == progressDialog->GetRange() )
				newValue = 1;
			
			progressDialog->Update( newValue, wxString::Format( "%d requests pending...", asyncRequestList.size() ) );
		}
	}

	if( AsyncRequestsPending() )
		CancelAllAsyncRequests();

	delete progressDialog;
	delete busyCursor;

	return success;
}

bool FtaClient::AsyncRequestsPending( void )
{
	if( asyncRequestList.size() > 0 )
		return true;

	return false;
}

bool FtaClient::CancelAllAsyncRequests( void )
{
	if( !curlHandleMulti )
		return false;

	while( asyncRequestList.size() > 0 )
	{
		FtaAsyncRequestList::iterator iter = asyncRequestList.begin();
		FtaAsyncRequest* request = *iter;

		bool changed = ChangeRequestState( request, FtaAsyncRequest::STATE_NONE );
		wxASSERT( changed );

		delete request;

		asyncRequestList.erase( iter );
	}

	return true;
}

bool FtaClient::ChangeRequestState( FtaAsyncRequest* request, FtaAsyncRequest::State newState )
{
	if( request->GetState() == newState )
		return false;
	
	FtaAsyncRequest::State oldState = request->GetState();

	switch( oldState )
	{
		case FtaAsyncRequest::STATE_PENDING:
		{
			CURLMcode curlmCode = curl_multi_remove_handle( curlHandleMulti, request->GetCurlHandle() );
			if( ReportCurlMultiError( curlmCode ) )
				return false;

			break;
		}
	}

	switch( newState )
	{
		case FtaAsyncRequest::STATE_PENDING:
		{
			if( !request->FormulateRequest() )
				return false;

			CURLMcode curlmCode = curl_multi_add_handle( curlHandleMulti, request->GetCurlHandle() );
			if( ReportCurlMultiError( curlmCode ) )
				return false;

			break;
		}
	}

	request->SetState( newState );

	return true;
}

bool FtaClient::AddAsyncRequest( FtaAsyncRequest* request, bool rejectIfAlreadyQueued /*= false*/, bool deleteIfNotAdded /*= true*/ )
{
	bool success = false;
	bool alreadyQueued = false;
	bool pointerMatch = false;

	do
	{
		FtaAsyncRequestList::iterator iter = FindAsyncRequest( request, pointerMatch );
		if( iter != asyncRequestList.end() )
			alreadyQueued = true;

		if( rejectIfAlreadyQueued && alreadyQueued )
			break;

		if( !ChangeRequestState( request, FtaAsyncRequest::STATE_PENDING ) )
			break;

		if( !pointerMatch )
			asyncRequestList.push_back( request );

		success = true;
	}
	while( false );

	if( !success && deleteIfNotAdded )
	{
		// We wouldn't want to delete a request that's in our list; thereby leaving us with a stale pointer.
		wxASSERT( pointerMatch == false );
		wxASSERT( request->GetState() == FtaAsyncRequest::STATE_NONE );
		delete request;
	}

	return success;
}

bool FtaClient::ServiceAllAsyncRequests( bool waitOnSockets )
{
	CURLMcode curlmCode;

	if( waitOnSockets )
	{
		long waitTimeMilliseconds;
		curlmCode = curl_multi_timeout( curlHandleMulti, &waitTimeMilliseconds );
		if( ReportCurlMultiError( curlmCode ) )
			return false;

		if( waitTimeMilliseconds > 0 )
		{
			long waitTimeMicroseconds = waitTimeMilliseconds * 1000;
			timeval waitTime;
			waitTime.tv_sec = 0;
			waitTime.tv_usec = waitTimeMicroseconds;

			fd_set fdread, fdwrite, fdexcep;
			FD_ZERO( &fdread );
			FD_ZERO( &fdwrite );
			FD_ZERO( &fdexcep );

			int maxfd = -1;
			curlmCode = curl_multi_fdset( curlHandleMulti, &fdread, &fdwrite, &fdexcep, &maxfd );
			if( ReportCurlMultiError( curlmCode ) )
				return false;

			if( fdread.fd_count > 0 || fdwrite.fd_count > 0 )
			{
				if( SOCKET_ERROR == select( maxfd + 1, &fdread, &fdwrite, &fdexcep, &waitTime ) )
				{
					int error = WSAGetLastError();
					return false;
				}
			}
		}
	}

	// STPTODO: Does this handle the vacuous case?  We may have pending requests, but they're all being throttled.
	int runningHandles = 0;
	curlmCode = curl_multi_perform( curlHandleMulti, &runningHandles );
	if( ReportCurlMultiError( curlmCode ) )
		return false;

	struct CURLMsg* curlMsg = nullptr;
	while( true )
	{
		int queuedMessageCount = 0;
		curlMsg = curl_multi_info_read( curlHandleMulti, &queuedMessageCount );
		if( !curlMsg )
			break;

		if( curlMsg->msg != CURLMSG_DONE )
			continue;
		
		FtaAsyncRequestList::iterator iter = FindAsyncRequest( curlMsg->easy_handle );
		wxASSERT( iter != asyncRequestList.end() );
		
		FtaAsyncRequest* request = *iter;
		wxASSERT( request->GetState() == FtaAsyncRequest::STATE_PENDING );

		request->SetCurlCode( curlMsg->data.result );
		ReportCurlError( request->GetCurlCode() );

		long retryAfterSeconds = -1;
		if( curlMsg->data.result == CURLE_OK )
		{
			bool processed = request->ProcessResponse( retryAfterSeconds );
			wxASSERT( processed );
		}

		bool changed = ChangeRequestState( request, FtaAsyncRequest::STATE_NONE );
		wxASSERT( changed );

		if( retryAfterSeconds < 0 )
		{
			asyncRequestList.erase( iter );
			delete request;
		}
		else
		{
			long currentTimeSeconds = clock() / CLOCKS_PER_SEC;
			request->SetRetryTime( currentTimeSeconds + retryAfterSeconds );
			ChangeRequestState( request, FtaAsyncRequest::STATE_THROTTLED );
		}
	}

	FtaAsyncRequestList::iterator iter = asyncRequestList.begin();
	while( iter != asyncRequestList.end() )
	{
		FtaAsyncRequest* request = *iter;

		if( request->GetState() == FtaAsyncRequest::STATE_THROTTLED )
		{
			long currentTimeSeconds = clock() / CLOCKS_PER_SEC;
			if( currentTimeSeconds >= request->GetRetryTime() )
			{
				bool changed = ChangeRequestState( request, FtaAsyncRequest::STATE_PENDING );
				wxASSERT( changed );
			}
		}

		iter++;
	}

	return true;
}

FtaAsyncRequestList::iterator FtaClient::FindAsyncRequest( CURL* curlHandleEasy )
{
	FtaAsyncRequestList::iterator iter = asyncRequestList.begin();
	while( iter != asyncRequestList.end() )
	{
		FtaAsyncRequest* request = *iter;
		if( request->GetCurlHandle() == curlHandleEasy )
			break;

		iter++;
	}

	return iter;
}

FtaAsyncRequestList::iterator FtaClient::FindAsyncRequest( FtaAsyncRequest* request, bool& pointerMatch, bool performLogicalMatch /*= true*/ )
{
	pointerMatch = false;

	FtaAsyncRequestList::iterator iter = asyncRequestList.begin();
	while( iter != asyncRequestList.end() )
	{
		FtaAsyncRequest* pendingRequest = *iter;
		if( pendingRequest == request )
		{
			pointerMatch = true;
			break;
		}

		if( performLogicalMatch && pendingRequest->Matches( request ) )
			break;

		iter++;
	}

	return iter;
}

// FtaClient.cpp