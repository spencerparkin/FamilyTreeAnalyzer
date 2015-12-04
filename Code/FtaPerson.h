// FtaPerson.h

#pragma once

#include "FtaContainers.h"

class FtaPerson
{
public:

	FtaPerson( const wxString& personId );
	virtual ~FtaPerson( void );

	enum InfoState
	{
		INFO_UNKNOWN,
		INFO_REQUESTED,
		INFO_KNOWN,
	};

	bool IsInfoComplete( void );

	InfoState GetInfoState( void ) const { return infoState; }
	void SetInfoState( InfoState infoState ) { this->infoState = infoState; }

	const wxString& GetPersonId( void ) const { return personId; }

private:

	InfoState infoState;
	wxString personId;
	wxString motherId, fatherId;
	FtaPersonIdSet childrenIdSet;
	FtaPersonIdSet spousesIdSet;
};

// FtaPerson.h
