// FtaTreeCache.cpp

#include "FtaTreeCache.h"
#include "FtaPerson.h"
#include "FtaFrame.h"
#include "FtaClient.h"
#include "FtaApp.h"
#include "FtaPedigreeRequest.h"
#include "FtaPersonDetailsRequest.h"
#include "FtaPersonPortraitRequest.h"
#include "FtaPersonOrdinancesRequest.h"

FtaTreeCache::FtaTreeCache( void )
{
	personCountThreshold = 0;
}

FtaTreeCache::~FtaTreeCache( void )
{
	Wipe();
}

FtaPerson* FtaTreeCache::Lookup( const wxString& personId, LookupDisposition disposition )
{
	FtaPerson* person = nullptr;

	if( !personId.IsEmpty() )
	{
		FtaPersonMap::iterator iter = personMap.find( personId );
		if( iter == personMap.end() )
		{
			switch( disposition )
			{
				case ALLOCATE_ON_CACHE_MISS:
				{
					// A weakness here is that we may be allocating a non-existent, non-sensical person.
					personMap[ personId ] = new FtaPerson( personId );
					break;
				}
				case POPULATE_ON_CACHE_MISS:
				{
					personCountThreshold = 0;
					int signature = FtaClient::NewSignature();
					if( RequestPerson( personId, signature ) )
						wxGetApp().GetClient()->CompleteAllAsyncRequests( false, signature );
					break;
				}
			}

			iter = personMap.find( personId );
		}

		if( iter != personMap.end() )
			person = iter->second;
	}

	if( person && !person->IsInfoComplete() && disposition == POPULATE_ON_CACHE_MISS )
	{
		int signature = FtaClient::NewSignature();
		if( RequestPerson( personId, signature ) )
			wxGetApp().GetClient()->CompleteAllAsyncRequests( false, signature );
	}

	return person;
}

bool FtaTreeCache::Wipe( void )
{
	FtaDeletePersonMap( personMap );
	return true;
}

bool FtaTreeCache::Fill( const wxString& rootPersonId, int personCountThreshold )
{
	FtaClient* client = wxGetApp().GetClient();
	if( !client->HasAccessToken() )
		return false;

	this->personCountThreshold = personCountThreshold;
	
	if( !RequestPerson( rootPersonId ) )
		return false;

	if( !client->CompleteAllAsyncRequests( true ) )
		return false;

	return true;
}

bool FtaTreeCache::Dump( void )
{
	wxGetApp().GetFrame()->AddLogMessage( wxString::Format( "%d person(s) in the cache...", personMap.size() ) );

	FtaPersonMap::iterator iter = personMap.begin();
	while( iter != personMap.end() )
	{
		FtaPerson* person = iter->second;
		person->DumpInfo();
		iter++;
	}

	return true;
}

/*virtual*/ bool FtaTreeCache::ProcessResponse( FtaAsyncRequest* request, wxJSONValue& responseValue )
{
	if( !request->IsKindOf( &FtaPersonInfoRequest::ms_classInfo ) )
		return false;

	FtaPersonInfoRequest* personInfoRequest = ( FtaPersonInfoRequest* )request;

	wxString personId = personInfoRequest->GetPersonId();
	FtaPerson* person = Lookup( personId, ALLOCATE_ON_CACHE_MISS );

	if( !personInfoRequest->AccumulateInfoInCache( responseValue ) )
		return false;

	if( ( signed )personMap.size() < personCountThreshold )
	{
		FtaPersonList adjacentPersonList;
		person->GatherNearestRelations( adjacentPersonList );

		FtaPersonList::iterator iter = adjacentPersonList.begin();
		while( iter != adjacentPersonList.end() )
		{
			FtaPerson* adjacentPerson = *iter;
			if( !adjacentPerson->IsInfoComplete() )
			{
				( void )RequestPerson( adjacentPerson->GetPersonId() );
			}

			iter++;
		}
	}

	return true;
}

bool FtaTreeCache::RequestPerson( const wxString& personId, int signature /*= -1*/ )
{
	FtaPerson* person = Lookup( personId, ALLOCATE_ON_CACHE_MISS );
	if( !person || person->IsInfoComplete() )
		return false;

	FtaClient* client = wxGetApp().GetClient();

	// If all of the following requests complete, then the person's information on www.familysearch.com will become fully known.
	// Which, of course, is not to say that we know everything about the person.

	if( ( person->GetFlags() & FtaPerson::FLAG_ANCESTRY ) == 0 )
		client->AddAsyncRequest( new FtaPedigreeRequest( personId, FtaPedigreeRequest::TYPE_ANCESTRY, this, signature ), true );

	if( ( person->GetFlags() & FtaPerson::FLAG_DESCENDANCY ) == 0 )
		client->AddAsyncRequest( new FtaPedigreeRequest( personId, FtaPedigreeRequest::TYPE_DESCENDANCY, this, signature ), true );

	if( ( person->GetFlags() & FtaPerson::FLAG_PERSONAL_DETAILS ) == 0 )
		client->AddAsyncRequest( new FtaPersonDetailsRequest( personId, this, signature ), true );

	if( ( person->GetFlags() & FtaPerson::FLAG_PORTRAIT ) == 0 )
		client->AddAsyncRequest( new FtaPersonPortraitRequest( personId, this, signature ), true );

	if( ( client->GetPrivilegeFlags() & FtaClient::PF_LDS_ORDINANCES ) != 0 )
	{
		// This program is far away from ever becoming LDS ordinance access certified.
		//if( ( person->GetFlags() & FtaPerson::FLAG_ORDINANCES ) == 0 )
		//	client->AddAsyncRequest( new FtaPersonOrdinancesRequest( personId, this, signature ), true );
	}

	return true;
}

// FtaTreeCache.cpp