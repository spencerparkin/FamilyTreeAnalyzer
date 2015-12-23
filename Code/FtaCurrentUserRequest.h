// FtaCurrentUserRequest.h

#pragma once

#include "FtaAsyncRequest.h"

class FtaCurrentUserRequest : public FtaAsyncRequest
{
public:

	wxDECLARE_CLASS( FtaCurrentUserRequest );

	FtaCurrentUserRequest( int signature = -1 );
	virtual ~FtaCurrentUserRequest( void );

	virtual bool ProcessJSONResponse( wxJSONValue& responseValue ) override;
	virtual bool MakeUrl( wxString& url ) override;
};

// FtaCurrentUserRequest.h