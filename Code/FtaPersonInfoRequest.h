// FtaPersonInfoRequest.h

#pragma once

#include "FtaAsyncRequest.h"
#include "FtaContainers.h"

class FtaPersonInfoRequest : public FtaAsyncRequest
{
public:

	FtaPersonInfoRequest( const wxString& personId, ResponseProcessor* processor );
	virtual ~FtaPersonInfoRequest( void );

	// While also updating the cache, here a list of nearest relations should be returned.
	virtual bool AccumulateInfoInCache( wxJSONValue& responseValue, FtaPersonList& adjacentPersonList ) = 0;

	const wxString& GetPersonId( void ) const { return personId; }

protected:

	wxString personId;
};

// FtaPersonInfoRequest.h
