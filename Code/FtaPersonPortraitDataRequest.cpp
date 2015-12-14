// FtaPersonPortraitDataRequest.cpp

#include "FtaPersonPortraitDataRequest.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

wxIMPLEMENT_CLASS( FtaPersonPortraitDataRequest, FtaPersonInfoRequest );

FtaPersonPortraitDataRequest::FtaPersonPortraitDataRequest( const wxString& personId, ResponseProcessor* processor ) : FtaPersonInfoRequest( personId, processor )
{
}

/*virtual*/ FtaPersonPortraitDataRequest::~FtaPersonPortraitDataRequest( void )
{
}

/*virtual*/ bool FtaPersonPortraitDataRequest::FormulateRequest( void )
{
	if( !FtaPersonInfoRequest::FormulateRequest() )
		return false;

	curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEFUNCTION, &FtaPersonPortraitDataRequest::WriteImageDataFunction );
	curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEDATA, nullptr );		// TODO: Pass in buffer stream here.

	return true;
}

/*static*/ size_t FtaPersonPortraitDataRequest::WriteImageDataFunction( void* buf, size_t size, size_t nitems, void* userPtr )
{
	size_t totalBytes = size * nitems;
	return totalBytes;
}

/*virtual*/ bool FtaPersonPortraitDataRequest::ProcessResponse( long& retryAfterSeconds )
{
	if( !FtaPersonInfoRequest::ProcessResponse( retryAfterSeconds ) )
		return false;

	if( retryAfterSeconds >= 0 )
		return true;

	// TODO: Read our image data into a wxImage object and then save it on the FtaPerson class instance.

	return true;
}

/*virtual*/ bool FtaPersonPortraitDataRequest::MakeUrl( wxString& url )
{
	FtaTreeCache* cache = wxGetApp().GetTreeCache();

	FtaPerson* person = cache->Lookup( personId, FtaTreeCache::FAIL_ON_CACHE_MISS );
	if( !person || ( person->GetFlags() & FtaPerson::FLAG_PORTRAIT ) == 0 )
		return false;

	url = person->GetPortraitUrl();
	if( url.IsEmpty() )
		return false;

	// TODO: Glean from URL the image type so that we know how to read it.

	return true;
}

/*virtual*/ bool FtaPersonPortraitDataRequest::AccumulateInfoInCache( wxJSONValue& responseValue )
{
	return false;
}

// FtaPersonPortraitDataRequest.cpp