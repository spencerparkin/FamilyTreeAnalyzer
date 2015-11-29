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

	FtaPerson* Lookup( const wxString& id, bool populateCacheAsNeeded = false );

	template< typename FtaPersonType > FtaPersonType* LookupCast( const wxString& id, bool populateCacheAsNeeded = false )
	{
		FtaPersonType* personType = nullptr;
		FtaPerson* person = Lookup( id, populateCacheAsNeeded );
		if( person )
			personType = person->Cast< FtaPersonType >( true );
		return personType;
	}

	bool IsEmpty( void ) const { return( personMap.size() == 0 ? true : false ); }
	void Wipe( void );

private:

	FtaPersonMap personMap;
};

// FtaTreeCache.h
