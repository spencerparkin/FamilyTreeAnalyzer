// FtaTreeCache.h

#pragma once

#include "FtaPerson.h"
#include "FtaContainers.h"
#include "FtaClient.h"

class FtaTreeCache : public FtaClient::ResponseProcessor
{
public:

	FtaTreeCache( void );
	~FtaTreeCache( void );

	bool Wipe( void );
	bool Fill( const wxString& rootPersonId, int personCount );
	bool IsEmpty( void ) const { return( personMap.size() == 0 ? true : false ); }

	virtual void ProcessResponse( const FtaClient::ResponseRequest& request, wxJSONValue& responseValue ) override;

private:

	FtaPersonMap personMap;
};

// FtaTreeCache.h
