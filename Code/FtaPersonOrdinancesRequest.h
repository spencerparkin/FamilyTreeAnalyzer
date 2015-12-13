// FtaPersonOrdinancesRequest.h

#pragma once

#include "FtaPersonInfoRequest.h"

class FtaPersonOrdinancesRequest : public FtaPersonInfoRequest
{
public:

	wxDECLARE_CLASS( FtaPersonOrdinancesRequest );

	FtaPersonOrdinancesRequest( const wxString& personId, ResponseProcessor* processor );
	virtual ~FtaPersonOrdinancesRequest( void );

	virtual bool MakeUrl( wxString& url ) override;

	virtual bool AccumulateInfoInCache( wxJSONValue& responseValue ) override;
};

// FtaPersonOrdinancesRequest.h