// FtaPedigreeRequest.cpp

#include "FtaPedigreeRequest.h"

FtaPedigreeRequest::FtaPedigreeRequest( const wxString& personId, Type type, ResponseProcessor* processor ) : FtaPersonInfoRequest( personId, processor )
{
	this->type = type;
}

/*virtual*/ FtaPedigreeRequest::~FtaPedigreeRequest( void )
{
}

/*virtual*/ bool FtaPedigreeRequest::FormulateRequest( void )
{
	if( !FtaAsyncRequest::FormulateRequest() )
		return false;

	return true;
}

/*virtual*/ bool FtaPedigreeRequest::MakeUrl( wxString& url )
{
	FtaAsyncRequest::MakeUrl( url );

	switch( type )
	{
		case TYPE_ANCESTRY:
		{
			url += "/platform/tree/ancestry?person=" + personId;
			break;
		}
		case TYPE_DESCENDANCY:
		{
			url += "/platform/tree/descendancy?person=" + personId;
			break;
		}
	}

	return true;
}

/*virtual*/ bool FtaPedigreeRequest::AccumulateInfoInCache( wxJSONValue& responseValue, FtaPersonList& adjacentPersonList )
{
	return true;
}

// FtaPedigreeRequest.cpp