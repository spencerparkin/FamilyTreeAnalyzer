// FtaPersonPortraitRequest.cpp

#include "FtaPersonPortraitRequest.h"
#include "FtaTreeCache.h"
#include "FtaApp.h"

wxIMPLEMENT_CLASS( FtaPersonPortraitRequest, FtaPersonInfoRequest );

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

/*virtual*/ bool FtaPersonPortraitRequest::ProcessResponse( long& retryAfterSeconds )
{
	if( !FtaPersonInfoRequest::ProcessResponse( retryAfterSeconds ) )
		return false;

	FtaPerson* person = wxGetApp().GetTreeCache()->Lookup( personId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS );
	if( !person )
		return false;

	if( httpStatusCode.Find( "204" ) >= 0 )
		person->SetPortraitUrl( "No Content" );
	else
	{
		int i = FindHeaderLine( "Location:", 0 );
		if( i < 0 )
			return false;

		wxString portraitUrl = headerArray[i];
		i = portraitUrl.Find( "http" );
		if( i < 0 )
			return false;

		portraitUrl.Remove( 0, i );

		i = portraitUrl.Find( "\n" );
		if( i >= 0 )
			portraitUrl.Remove( i, 1 );

		i = portraitUrl.Find( "\r" );
		if( i >= 0 )
			portraitUrl.Remove( i, 1 );

		person->SetPortraitUrl( portraitUrl );
	}

	person->SetFlags( person->GetFlags() | FtaPerson::FLAG_PORTRAIT );

	return true;
}

/*virtual*/ bool FtaPersonPortraitRequest::AccumulateInfoInCache( wxJSONValue& responseValue )
{
	return true;
}

// FtaPersonPortraitRequest.cpp