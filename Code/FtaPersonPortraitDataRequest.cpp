// FtaPersonPortraitDataRequest.cpp

#include "FtaPersonPortraitDataRequest.h"
#include "FtaTreeCache.h"
#include "FtaFrame.h"
#include "FtaVizPanel.h"
#include "FtaCanvas.h"
#include "FtaApp.h"

wxIMPLEMENT_CLASS( FtaPersonPortraitDataRequest, FtaPersonInfoRequest );

FtaPersonPortraitDataRequest::FtaPersonPortraitDataRequest( const wxString& personId, ResponseProcessor* processor, int signature ) : FtaPersonInfoRequest( personId, processor, signature )
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
	curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEDATA, &memoryOutputStream );

	return true;
}

/*static*/ size_t FtaPersonPortraitDataRequest::WriteImageDataFunction( void* buf, size_t size, size_t nitems, void* userPtr )
{
	wxMemoryOutputStream* memoryOutputStream = ( wxMemoryOutputStream* )userPtr;

	// We can't return from the callback until we've written everything.
	size_t totalBytes = size * nitems;
	size_t bytesLeft = totalBytes;
	char* byteBuffer = ( char* )buf;
	while( !memoryOutputStream->WriteAll( byteBuffer, bytesLeft ) )
	{
		size_t bytesWritten = memoryOutputStream->LastWrite();
		bytesLeft -= bytesWritten;
		byteBuffer += bytesWritten;
	}

	return totalBytes;
}

/*virtual*/ bool FtaPersonPortraitDataRequest::ProcessResponse( long& retryAfterSeconds )
{
	if( !FtaPersonInfoRequest::ProcessResponse( retryAfterSeconds ) )
		return false;

	if( retryAfterSeconds >= 0 )
		return true;

	wxMemoryInputStream memoryInputStream( memoryOutputStream );
	wxImage image( memoryInputStream, wxBITMAP_TYPE_ANY );
	if( !image.IsOk() )
		return false;

	FtaVizPanel* vizPanel = wxGetApp().GetFrame()->GetPanel< FtaVizPanel >();
	if( !vizPanel )
		return false;

	GLuint portraitTexture = vizPanel->GetCanvas()->GenerateTexture( image );
	if( portraitTexture == GL_INVALID_VALUE )
		return false;

	FtaPerson* person = wxGetApp().GetTreeCache()->Lookup( personId, FtaTreeCache::FAIL_ON_CACHE_MISS );
	if( !person )
		return false;

	if( !person->SetPortraitTexture( portraitTexture ) )
		return false;

	return true;
}

/*virtual*/ bool FtaPersonPortraitDataRequest::MakeUrl( wxString& url )
{
	FtaTreeCache* cache = wxGetApp().GetTreeCache();

	FtaPerson* person = cache->Lookup( personId, FtaTreeCache::FAIL_ON_CACHE_MISS );
	if( !person || ( person->GetFlags() & FtaPerson::FLAG_PORTRAIT ) == 0 )
		return false;

	if( person->GetPortraitUrl() == "No Content" )
		return false;

	url = person->GetPortraitUrl();
	if( url.IsEmpty() )
		return false;

	return true;
}

/*virtual*/ bool FtaPersonPortraitDataRequest::AccumulateInfoInCache( wxJSONValue& responseValue )
{
	return false;
}

// FtaPersonPortraitDataRequest.cpp