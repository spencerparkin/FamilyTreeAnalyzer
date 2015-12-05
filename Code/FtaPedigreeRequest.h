// FtaPedigreeRequest.h

#pragma once

#include "FtaPersonInfoRequest.h"

class FtaPedigreeRequest : public FtaPersonInfoRequest
{
public:

	enum Type
	{
		TYPE_ANCESTRY,
		TYPE_DESCENDANCY,
	};

	FtaPedigreeRequest( const wxString& personId, Type type, ResponseProcessor* processor );
	virtual ~FtaPedigreeRequest( void );

	virtual bool FormulateRequest( void ) override;
	virtual bool MakeUrl( wxString& url ) override;

	virtual bool AccumulateInfoInCache( wxJSONValue& responseValue ) override;

private:

	wxJSONValue FindNumber( long givenNumber, const wxString& type, const wxJSONValue& personsArrayValue );
	wxJSONValue FindNumberString( const wxString& givenNumberString, const wxString& type, wxJSONValue& personsArrayValue );

	void GatherNumbersWithPrefix( const wxString& prefix, const wxString& type, const wxJSONValue& personsArrayValue, FtaOffsetArray& offsetArray, bool excludeSpouses );

	Type type;
};

// FtaPedigreeRequest.h