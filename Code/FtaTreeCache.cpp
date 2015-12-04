// FtaTreeCache.cpp

#include "FtaTreeCache.h"
#include "FtaPerson.h"
#include "FtaClient.h"
#include "FtaApp.h"
#include "FtaPedigreeRequest.h"

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

	FtaPersonMap::iterator iter = personMap.find( personId );
	if( iter == personMap.end() )
	{
		switch( disposition )
		{
			case ALLOCATE_ON_CACHE_MISS:
			{
				personMap[ personId ] = new FtaPerson( personId );
				break;
			}
			case POPULATE_ON_CACHE_MISS:
			{
				personCountThreshold = 0;
				if( RequestPerson( personId ) )
					wxGetApp().GetClient()->CompleteAllAsyncRequests();
				break;
			}
		}

		iter = personMap.find( personId );
	}

	if( iter != personMap.end() )
		person = iter->second;

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

	if( !client->CompleteAllAsyncRequests() )
		return false;

	return true;
}

/*virtual*/ bool FtaTreeCache::ProcessResponse( FtaAsyncRequest* request, wxJSONValue& responseValue )
{
	// TODO: Use RTTI here instead to be type-safe.
	FtaPersonInfoRequest* personInfoRequest = ( FtaPersonInfoRequest* )request;
	if( !personInfoRequest )
		return false;

	FtaPersonList adjacentPersonList;
	if( !personInfoRequest->AccumulateInfoInCache( responseValue, adjacentPersonList ) )
		return false;

	wxString personId = personInfoRequest->GetPersonId();
	FtaPerson* person = Lookup( personId, ALLOCATE_ON_CACHE_MISS );
	if( person->IsInfoComplete() )
		person->SetInfoState( FtaPerson::INFO_KNOWN );

	if( ( signed )personMap.size() < personCountThreshold )
	{
		FtaPersonList::iterator iter = adjacentPersonList.begin();
		while( iter != adjacentPersonList.end() )
		{
			FtaPerson* adjacentPerson = *iter;
			if( adjacentPerson->GetInfoState() == FtaPerson::INFO_UNKNOWN )
			{
				( void )RequestPerson( adjacentPerson->GetPersonId() );
			}

			iter++;
		}
	}

	return true;
}

bool FtaTreeCache::RequestPerson( const wxString& personId )
{
	FtaPerson* person = Lookup( personId, ALLOCATE_ON_CACHE_MISS );
	if( person->GetInfoState() != FtaPerson::INFO_UNKNOWN )
		return false;

	FtaClient* client = wxGetApp().GetClient();

	// If all of the following requests complete, then the person's information will become known.
	client->AddAsyncRequest( new FtaPedigreeRequest( personId, FtaPedigreeRequest::TYPE_ANCESTRY, this ) );
	client->AddAsyncRequest( new FtaPedigreeRequest( personId, FtaPedigreeRequest::TYPE_DESCENDANCY, this ) );
	//...

	person->SetInfoState( FtaPerson::INFO_REQUESTED );

	return true;
}

// FtaTreeCache.cpp