// FtaPersonPortraitRequest.cpp

#include "FtaPersonPortraitRequest.h"

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
	// Hmmm...I don't thing we're going to get image data from JSON.  Figure it out.
	// We have to somehow get the redirect location and then get the image from there.
	return false;
}

// FtaPersonPortraitRequest.cpp