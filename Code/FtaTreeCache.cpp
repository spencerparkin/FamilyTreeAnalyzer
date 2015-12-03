// FtaTreeCache.cpp

#include "FtaTreeCache.h"
#include "FtaPerson.h"
#include "FtaClient.h"
#include "FtaApp.h"

FtaTreeCache::FtaTreeCache( void )
{
}

FtaTreeCache::~FtaTreeCache( void )
{
	Wipe();
}

bool FtaTreeCache::Wipe( void )
{
	FtaDeletePersonMap( personMap );
	return true;
}

bool FtaTreeCache::Fill( const wxString& rootPersonId, int personCount )
{
	// TODO: Use the curl multi interface to asynchronously build the tree in breadth-first fasion about the given person.

	return true;
}

/*virtual*/ void FtaTreeCache::ProcessResponse( const FtaClient::ResponseRequest& request, wxJSONValue& responseValue )
{
	// TODO: Build up the tree.  Make further requests here as needed.
}

// FtaTreeCache.cpp