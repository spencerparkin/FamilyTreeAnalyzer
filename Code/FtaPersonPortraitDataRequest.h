// FtaPersonPortraitDataRequest.h

#pragma once

#include "FtaPersonInfoRequest.h"

class FtaPersonPortraitDataRequest : public FtaPersonInfoRequest
{
public:

	wxDECLARE_CLASS( FtaPersonPortraitDataRequest );

	FtaPersonPortraitDataRequest( const wxString& personId, ResponseProcessor* processor );
	virtual ~FtaPersonPortraitDataRequest( void );

	virtual bool FormulateRequest( void ) override;
	virtual bool MakeUrl( wxString& url ) override;
	virtual bool ProcessResponse( long& retryAfterSeconds ) override;
	virtual bool AccumulateInfoInCache( wxJSONValue& responseValue ) override;

	static size_t WriteImageDataFunction( void* buf, size_t size, size_t nitems, void* userPtr );
};

// FtaPersonPortraitDataRequest.h