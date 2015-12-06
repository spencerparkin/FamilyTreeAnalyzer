// FtaPersonOrdinancesRequest.cpp

#include "FtaPersonOrdinancesRequest.h"

FtaPersonOrdinancesRequest::FtaPersonOrdinancesRequest( const wxString& personId, ResponseProcessor* processor ) : FtaPersonInfoRequest( personId, processor )
{
}

/*virtual*/ FtaPersonOrdinancesRequest::~FtaPersonOrdinancesRequest( void )
{
}

/*virtual*/ bool FtaPersonOrdinancesRequest::MakeUrl( wxString& url )
{
	if( !FtaPersonInfoRequest::MakeUrl( url ) )
		return false;

	// Uh...what about ordinances for a specific individual?
	url += "/platform/ordinances/ordinances";
	return true;
}

/*virtual*/ bool FtaPersonOrdinancesRequest::AccumulateInfoInCache( wxJSONValue& responseValue )
{
	return true;
}

// FtaPersonOrdinancesRequest.cpp