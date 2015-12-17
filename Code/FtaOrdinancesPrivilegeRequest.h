// FtaOrdinancesPrivilegeRequest.h

#pragma once

#include "FtaAsyncRequest.h"

class FtaOrdinancesPrivilegeRequest : public FtaAsyncRequest
{
public:

	wxDECLARE_CLASS( FtaOrdinancesPrivilegeRequest );

	FtaOrdinancesPrivilegeRequest( ResponseProcessor* processor );
	virtual ~FtaOrdinancesPrivilegeRequest( void );

	virtual bool ProcessResponse( long& retryAfterSeconds ) override;
	virtual bool MakeUrl( wxString& url ) override;
	virtual bool ProcessJSONResponse( wxJSONValue& responseValue ) override;
};

// FtaOrdinancesPrivilegeRequest.h