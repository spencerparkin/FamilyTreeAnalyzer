// FtaPersonOrdinancesRequest.cpp

#include "FtaPersonOrdinancesRequest.h"
#include "FtaClient.h"
#include "FtaApp.h"

wxIMPLEMENT_CLASS( FtaPersonOrdinancesRequest, FtaPersonInfoRequest );

FtaPersonOrdinancesRequest::FtaPersonOrdinancesRequest( const wxString& personId, ResponseProcessor* processor, int signature /*= -1*/ ) : FtaPersonInfoRequest( personId, processor, signature )
{
}

/*virtual*/ FtaPersonOrdinancesRequest::~FtaPersonOrdinancesRequest( void )
{
}

/*virtual*/ bool FtaPersonOrdinancesRequest::MakeUrl( wxString& url )
{
	FtaClient* client = wxGetApp().GetClient();
	if( ( client->GetPrivilegeFlags() & FtaClient::PF_LDS_ORDINANCES ) == 0 )
		return false;

	// Until this program gets certified, I don't have access to the ordinances end-points.
	return false;
}

/*virtual*/ bool FtaPersonOrdinancesRequest::AccumulateInfoInCache( wxJSONValue& responseValue )
{
	// TODO: Read ordinances info into the person's record in the cache.
	return true;
}

// FtaPersonOrdinancesRequest.cpp