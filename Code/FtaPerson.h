// FtaPerson.h

#pragma once

#include "FtaContainers.h"

class FtaPerson
{
public:

	FtaPerson( const wxString& personId );
	virtual ~FtaPerson( void );

private:

	wxString personId;
	wxString motherId, fatherId;
	FtaPersonIdSet childrenIdSet;
	FtaPersonIdSet spousesIdSet;
};

// FtaPerson.h
