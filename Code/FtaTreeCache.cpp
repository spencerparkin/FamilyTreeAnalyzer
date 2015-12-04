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
	FtaClient* client = wxGetApp().GetClient();

	// Make initial request...(request responses will triger subsequent requests)

	while( client->AsyncRequestsPending() )
		if( !client->ServiceAllAsyncRequests( true ) )
			return false;

	return true;
}

/*virtual*/ bool FtaTreeCache::ProcessResponse( FtaAsyncRequest* request, wxJSONValue& responseValue )
{
	return true;
}

// FtaTreeCache.cpp