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

	enum LookupDisposition
	{
		FAIL_ON_CACHE_MISS,
		ALLOCATE_ON_CACHE_MISS,
		POPULATE_ON_CACHE_MISS,
	};

	FtaPerson* Lookup( const wxString& personId, LookupDisposition disposition );

	bool Dump( void );
	bool Wipe( void );
	bool Fill( const wxString& rootPersonId, int personCountThreshold );
	bool IsEmpty( void ) const { return( personMap.size() == 0 ? true : false ); }

	virtual bool ProcessResponse( FtaAsyncRequest* request, wxJSONValue& responseValue ) override;

	int GetPersonCount( void ) const { return personMap.size(); }
	const FtaPersonMap& GetPersonMap( void ) const { return personMap; }

private:

	bool RequestPerson( const wxString& personId );

	int personCountThreshold;
	FtaPersonMap personMap;
};

// FtaTreeCache.h
