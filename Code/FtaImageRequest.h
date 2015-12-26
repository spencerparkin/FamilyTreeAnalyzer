// FtaImageRequest.h

#pragma once

#include "FtaAsyncRequest.h"
#include <wx/mstream.h>
#include <wx/glcanvas.h>

class FtaImageRequest : public FtaAsyncRequest
{
public:

	wxDECLARE_CLASS( FtaImageRequest );

	FtaImageRequest( const wxString& imageUrl, ResponseProcessor* processor, int signature );
	virtual ~FtaImageRequest( void );

	virtual bool FormulateRequest( void ) override;
	virtual bool MakeUrl( wxString& url ) override;
	virtual bool ProcessResponse( long& retryAfterSeconds ) override;

	static size_t WriteImageDataFunction( void* buf, size_t size, size_t nitems, void* userPtr );

	wxImage* GetImage( void ) { return image; }

private:

	wxString imageUrl;
	wxMemoryOutputStream memoryOutputStream;
	wxImage* image;
};

// FtaImageRequest.h