// FtaImageRequest.cpp

#include "FtaImageRequest.h"

wxIMPLEMENT_CLASS( FtaImageRequest, FtaAsyncRequest );

FtaImageRequest::FtaImageRequest( const wxString& imageUrl, ResponseProcessor* processor, int signature ) : FtaAsyncRequest( processor, signature )
{
	this->imageUrl = imageUrl;
	
	image = nullptr;
}

/*virtual*/ FtaImageRequest::~FtaImageRequest( void )
{
	delete image;
}

/*virtual*/ bool FtaImageRequest::FormulateRequest( void )
{
	if( !FtaAsyncRequest::FormulateRequest() )
		return false;

	curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEFUNCTION, &FtaImageRequest::WriteImageDataFunction );
	curl_easy_setopt( curlHandleEasy, CURLOPT_WRITEDATA, &memoryOutputStream );

	return true;
}

/*static*/ size_t FtaImageRequest::WriteImageDataFunction( void* buf, size_t size, size_t nitems, void* userPtr )
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

/*virtual*/ bool FtaImageRequest::ProcessResponse( long& retryAfterSeconds )
{
	if( !FtaAsyncRequest::ProcessResponse( retryAfterSeconds ) )
		return false;

	if( retryAfterSeconds >= 0 )
		return true;

	wxMemoryInputStream memoryInputStream( memoryOutputStream );
	image = new wxImage( memoryInputStream, wxBITMAP_TYPE_ANY );
	if( !image->IsOk() )
	{
		delete image;
		image = nullptr;
		return false;
	}

	if( processor )
		processor->ProcessResponse( this, nullptr );

	return true;
}

/*virtual*/ bool FtaImageRequest::MakeUrl( wxString& url )
{
	if( !FtaAsyncRequest::MakeUrl( url ) )
		return false;

	url += imageUrl;		// TODO: What if it's not a relative path?
	return true;
}

// FtaImageRequest.cpp