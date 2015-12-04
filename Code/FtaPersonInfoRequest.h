// FtaPersonInfoRequest.h

#pragma once

#include "FtaAsyncRequest.h"
#include "FtaContainers.h"

class FtaPersonInfoRequest : public FtaAsyncRequest
{
public:

	FtaPersonInfoRequest( const wxString& personId, ResponseProcessor* processor );
	virtual ~FtaPersonInfoRequest( void );

	virtual bool AccumulateInfoInCache( wxJSONValue& responseValue ) = 0;

	const wxString& GetPersonId( void ) const { return personId; }

protected:

	wxString personId;
};

// FtaPersonInfoRequest.h
