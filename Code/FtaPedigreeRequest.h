// FtaPedigreeRequest.h

#pragma once

#include "FtaPersonInfoRequest.h"

class FtaPedigreeRequest : public FtaPersonInfoRequest
{
public:

	wxDECLARE_CLASS( FtaPedigreeRequest );

	enum Type
	{
		TYPE_ANCESTRY,
		TYPE_DESCENDANCY,
	};

	FtaPedigreeRequest( const wxString& personId, Type type, ResponseProcessor* processor );
	virtual ~FtaPedigreeRequest( void );

	virtual bool FormulateRequest( void ) override;
	virtual bool MakeUrl( wxString& url ) override;
	virtual bool Matches( FtaAsyncRequest* request ) override;

	virtual bool AccumulateInfoInCache( wxJSONValue& responseValue ) override;

private:

	Type type;
};

// FtaPedigreeRequest.h