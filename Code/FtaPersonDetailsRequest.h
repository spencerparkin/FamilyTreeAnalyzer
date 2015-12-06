// FtaPersonDetailsRequest.h

#pragma once

#include "FtaPersonInfoRequest.h"

class FtaPersonDetailsRequest : public FtaPersonInfoRequest
{
public:

	FtaPersonDetailsRequest( const wxString& personId, ResponseProcessor* processor );
	virtual ~FtaPersonDetailsRequest( void );

	virtual bool MakeUrl( wxString& url ) override;

	virtual bool AccumulateInfoInCache( wxJSONValue& responseValue ) override;
};

// FtaPersonDetailsRequest.h
