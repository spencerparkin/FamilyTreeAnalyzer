// FtaClient.cpp

#include "FtaClient.h"
#include "FtaTreeCache.h"
#include "FtaFrame.h"
#include "FtaAsyncRequest.h"
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

		curl_easy_reset( curlHandleEasy );

		// See https://familysearch.org/developers/docs/certification/authentication for more info.
#if !defined SANDBOX
		wxString notice =
			"FamilyTreeAnalyzer would like to know your basic FamilySearch profile information "
			"and access data about your ancestors from the FamilySearch family tree.  "
			"FamilyTreeAnalyzer will use this information in accordance with their respective terms "
			"of service and privacy policies.  May FamilyTreeAnalyzer proceed?";
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

		if( wxGetApp().GetFrame() )
			wxGetApp().GetFrame()->AddLogMessage( "Logged out!" );

		success = true;
	}
	while( false );

	if( headers )
		curl_slist_free_all( headers );

	return success;
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
			
			int count = asyncRequestList.size() + asyncRetryList.size();
			progressDialog->Update( newValue, wxString::Format( "%d requests pending...", count ) );
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

	if( asyncRetryList.size() > 0 )
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

		CURLMcode curlmCode = curl_multi_remove_handle( curlHandleMulti, request->GetCurlHandle() );
		ReportCurlMultiError( curlmCode );
		wxASSERT( curlmCode == CURLM_OK );

		delete request;

		asyncRequestList.erase( iter );
	}

	while( asyncRetryList.size() > 0 )
	{
		FtaAsyncRequestList::iterator iter = asyncRetryList.begin();
		FtaAsyncRequest* request = *iter;

		delete request;

		asyncRetryList.erase( iter );
	}

	return true;
}

bool FtaClient::AddAsyncRequest( FtaAsyncRequest* request )
{
	if( !request->FormulateRequest() )
	{
		delete request;
		return false;
	}

	CURLMcode curlmCode = curl_multi_add_handle( curlHandleMulti, request->GetCurlHandle() );
	if( ReportCurlMultiError( curlmCode ) )
	{
		delete request;
		return false;
	}

	asyncRequestList.push_back( request );
	return true;
}

bool FtaClient::ServiceAllAsyncRequests( bool waitOnSockets )
{
	if( asyncRequestList.size() > 0 )
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
			request->SetCurlCode( curlMsg->data.result );

			long retryAfterSeconds = -1;
			if( curlMsg->data.result == CURLE_OK )
			{
				bool processed = request->ProcessResponse( retryAfterSeconds );
				wxASSERT( processed );
			}

			curlmCode = curl_multi_remove_handle( curlHandleMulti, request->GetCurlHandle() );
			ReportCurlMultiError( curlmCode );
			wxASSERT( curlmCode == CURLM_OK );

			asyncRequestList.erase( iter );

			if( retryAfterSeconds < 0 )
				delete request;
			else
			{
				long currentTimeSeconds = clock() / CLOCKS_PER_SEC;
				request->SetRetryTime( currentTimeSeconds + retryAfterSeconds );
				asyncRetryList.push_back( request );
			}
		}
	}

	FtaAsyncRequestList::iterator iter = asyncRetryList.begin();
	while( iter != asyncRetryList.end() )
	{
		FtaAsyncRequestList::iterator nextIter = iter;
		nextIter++;

		FtaAsyncRequest* request = *iter;
		long currentTimeSeconds = clock() / CLOCKS_PER_SEC;
		if( currentTimeSeconds >= request->GetRetryTime() )
		{
			asyncRetryList.erase( iter );

			bool added = AddAsyncRequest( request );
			wxASSERT( added );
		}

		iter = nextIter;
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

// FtaClient.cpp