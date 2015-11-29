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

	//...

	bool IsEmpty( void ) const { return( personMap.size() == 0 ? true : false ); }
	void Wipe( void );

private:

	FtaPersonMap personMap;
};

// FtaTreeCache.h
