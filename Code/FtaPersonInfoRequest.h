// FtaPersonInfoRequest.h

#pragma once

#include "FtaAsyncRequest.h"
#include "FtaContainers.h"

class FtaPersonInfoRequest : public FtaAsyncRequest
{
public:

	wxDECLARE_ABSTRACT_CLASS( FtaPersonInfoRequest );

	FtaPersonInfoRequest( const wxString& personId, ResponseProcessor* processor, int signature = -1 );
	virtual ~FtaPersonInfoRequest( void );

	virtual bool AccumulateInfoInCache( wxJSONValue& responseValue ) = 0;
	virtual bool Matches( FtaAsyncRequest* request ) override;

	const wxString& GetPersonId( void ) const { return personId; }

protected:

	wxJSONValue FindPersonId( const wxString& personId, const wxJSONValue& personsArrayValue );
	wxJSONValue FindNumber( long givenNumber, const wxString& type, const wxJSONValue& personsArrayValue );
	wxJSONValue FindNumberString( const wxString& givenNumberString, const wxString& type, wxJSONValue& personsArrayValue );

	void GatherNumbersWithPrefix( const wxString& prefix, const wxString& type, const wxJSONValue& personsArrayValue, FtaOffsetArray& offsetArray, bool excludeSpouses );

	wxString personId;
};

// FtaPersonInfoRequest.h
