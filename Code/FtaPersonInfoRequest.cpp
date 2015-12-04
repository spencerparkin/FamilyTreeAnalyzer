// FtaPersonInfoRequest.cpp

#include "FtaPersonInfoRequest.h"

FtaPersonInfoRequest::FtaPersonInfoRequest( const wxString& personId, ResponseProcessor* processor ) : FtaAsyncRequest( processor )
{
	this->personId = personId;
}

/*virtual*/ FtaPersonInfoRequest::~FtaPersonInfoRequest( void )
{
}

// FtaPersonRequest.cpp