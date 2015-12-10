// FtaPersonPortraitRequest.cpp

#include "FtaPersonPortraitRequest.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

FtaPersonPortraitRequest::FtaPersonPortraitRequest( const wxString& personId, ResponseProcessor* processor ) : FtaPersonInfoRequest( personId, processor )
{
}

/*virtual*/ FtaPersonPortraitRequest::~FtaPersonPortraitRequest( void )
{
}

/*virtual*/ bool FtaPersonPortraitRequest::MakeUrl( wxString& url )
{
	if( !FtaPersonInfoRequest::MakeUrl( url ) )
		return false;

	url += "/platform/tree/persons/" + personId + "/portrait";
	return true;
}

/*virtual*/ bool FtaPersonPortraitRequest::AccumulateInfoInCache( wxJSONValue& responseValue )
{
	FtaPerson* person = wxGetApp().GetTreeCache()->Lookup( personId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS );
	if( !person )
		return false;

	if( httpStatusCode.Find( "204" ) < 0 )
	{
		int i = FindHeaderLine( "Location:" );
		if( i < 0 )
			return false;

		wxString portraitUrl = headerArray[i];
		portraitUrl.Remove( 0, 10 );
		person->SetPortraitUrl( portraitUrl );
	}

	person->SetFlags( person->GetFlags() | FtaPerson::FLAG_PORTRAIT );

	return true;
}

// FtaPersonPortraitRequest.cpp