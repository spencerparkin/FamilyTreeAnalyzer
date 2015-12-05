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
	bool DumpInfo( void );

	InfoState GetInfoState( void ) const { return infoState; }
	void SetInfoState( InfoState infoState ) { this->infoState = infoState; }

	const wxString& GetPersonId( void ) const { return personId; }

	const wxString& GetMotherId( void ) const { return motherId; }
	void SetMotherId( const wxString& motherId ) { this->motherId = motherId; }

	const wxString& GetFatherId( void ) const { return fatherId; }
	void SetFatherId( const wxString& fatherId ) { this->fatherId = fatherId; }

	const FtaPersonIdSet& GetChildrenIdSet( void ) const { return childrenIdSet; }
	FtaPersonIdSet& GetChildrenIdSet( void ) { return childrenIdSet; }

	const FtaPersonIdSet& GetSpousesIdSet( void ) const { return spousesIdSet; }
	FtaPersonIdSet& GetSpousesIdSet( void ) { return spousesIdSet; }

	void GatherNearestRelations( FtaPersonList& personList );

private:

	InfoState infoState;
	wxString personId;
	wxString motherId, fatherId;
	FtaPersonIdSet childrenIdSet;
	FtaPersonIdSet spousesIdSet;
};

// FtaPerson.h
