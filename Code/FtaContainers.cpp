// FtaContainers.cpp

#include "FtaContainers.h"
#include "FtaPerson.h"
#include "FtaAsyncRequest.h"
#include <wx/listimpl.cpp>

WX_DEFINE_LIST( FtaPersonList );
WX_DEFINE_LIST( FtaAsyncRequestList );

void FtaDeletePersonMap( FtaPersonMap& personMap )
{
	while( personMap.size() > 0 )
	{
		FtaPersonMap::iterator iter = personMap.begin();
		FtaPerson* person = iter->second;
		delete person;
		personMap.erase( iter );
	}
}

// FtaContainres.cpp