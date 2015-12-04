// FtaTreeCache.h

#pragma once

#include "FtaPerson.h"
#include "FtaContainers.h"
#include "FtaAsyncRequest.h"

class FtaTreeCache : public FtaAsyncRequest::ResponseProcessor
{
public:

	FtaTreeCache( void );
	~FtaTreeCache( void );

	bool Wipe( void );
	bool Fill( const wxString& rootPersonId, int personCount );
	bool IsEmpty( void ) const { return( personMap.size() == 0 ? true : false ); }

	virtual bool ProcessResponse( FtaAsyncRequest* request, wxJSONValue& responseValue ) override;

private:

	FtaPersonMap personMap;
};

// FtaTreeCache.h
