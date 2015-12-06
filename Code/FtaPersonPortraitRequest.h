// FtaPersonPortraitRequest.h

#pragma once

#include "FtaPersonInfoRequest.h"

class FtaPersonPortraitRequest : public FtaPersonInfoRequest
{
public:

	FtaPersonPortraitRequest( const wxString& personId, ResponseProcessor* processor );
	virtual ~FtaPersonPortraitRequest( void );

	virtual bool MakeUrl( wxString& url ) override;

	virtual bool AccumulateInfoInCache( wxJSONValue& responseValue ) override;
};

// FtaPersonPortraitRequest.h