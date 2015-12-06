// FtaClient.cpp

#include "FtaClient.h"
#include "FtaTreeCache.h"
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
		if( curlCode != CURLE_OK )
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
		if( curlCode != CURLE_OK )
			break;

		accessToken = "";

		success = true;
	}
	while( false );

	if( headers )
		curl_slist_free_all( headers );

	return success;
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

bool FtaClient::CompleteAllAsyncRequests( bool showWorkingDialog )
{
	bool success = true;

	wxGenericProgressDialog* progressDialog = nullptr;
	wxBusyCursor* busyCursor = nullptr;

	if( showWorkingDialog )
		progressDialog = new wxGenericProgressDialog( "Requests Pending...", "Working...", asyncRequestList.size(), nullptr, wxPD_APP_MODAL | wxPD_CAN_ABORT );
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

			// We can't set the range as the true high water-mark, because the progress dialog
			// goes into a blocking modal message pump loop thinger if we let it hit 100%.
			if( ( signed )asyncRequestList.size() >= progressDialog->GetRange() )
				progressDialog->SetRange( asyncRequestList.size() + 1 );

			progressDialog->Update( asyncRequestList.size(), wxString::Format( "%d requests pending...", asyncRequestList.size() ) );
		}
	}

	if( AsyncRequestsPending() )
		CancelAllAsyncRequests();

	delete progressDialog;
	delete busyCursor;

	return success;
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
		wxASSERT( curlmCode == CURLM_OK );

		delete request;

		asyncRequestList.erase( iter );
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
	if( curlmCode != CURLM_OK )
	{
		delete request;
		return false;
	}

	asyncRequestList.push_back( request );
	return true;
}

bool FtaClient::ServiceAllAsyncRequests( bool waitOnSockets )
{
	CURLMcode curlmCode;

	if( waitOnSockets )
	{
		long waitTimeMilliseconds;
		curlmCode = curl_multi_timeout( curlHandleMulti, &waitTimeMilliseconds );
		if( curlmCode != CURLM_OK )
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
			if( curlmCode != CURLM_OK )
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
	if( curlmCode != CURLM_OK )
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

		if( curlMsg->data.result == CURLE_OK )
		{
			bool processed = request->ProcessResponse();
			wxASSERT( processed );
		}

		curlmCode = curl_multi_remove_handle( curlHandleMulti, request->GetCurlHandle() );
		wxASSERT( curlmCode == CURLM_OK );

		delete request;
		asyncRequestList.erase( iter );
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