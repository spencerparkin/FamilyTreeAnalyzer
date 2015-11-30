// FtaTreeCache.h

#pragma once

#include "FtaPerson.h"
#include <wx/hashmap.h>

WX_DECLARE_STRING_HASH_MAP( FtaPerson*, FtaPersonMap );

class FtaTreeCache
{
public:

	FtaTreeCache( void );
	~FtaTreeCache( void );

	enum LookupDisposition
	{
		FAIL_ON_CACHE_MISS,
		POPULATE_ON_CACHE_MISS,
		ALLOCATE_ON_CACHE_MISS,
	};

	FtaPerson* Lookup( const wxString& personId, LookupDisposition disposition );

	bool IsEmpty( void ) const { return( personMap.size() == 0 ? true : false ); }
	bool Wipe( void );
	bool Wipe( const wxString& personId );

private:

	FtaPersonMap personMap;
};

// FtaTreeCache.h
