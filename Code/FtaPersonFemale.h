// FtaPersonFemale.h

#pragma once

#include "FtaPerson.h"

class FtaPersonFemale : public FtaPerson
{
public:

	FtaPersonFemale( FtaTreeCache* cache );
	virtual ~FtaPersonFemale( void );

	static Gender GetStaticGender( void ) { return GENDER_FEMALE; }

	virtual Gender GetGender( void ) const override { return GENDER_FEMALE; }
	virtual void FindAllChildrenIds( FtaPersonIdSet& childrenIds, bool populateCacheAsNeeded = false ) const override;

	const wxString& GetHusbandId( void ) const { return husbandId; }
	void SetHusbandId( const wxString& husbandId ) { this->husbandId = husbandId; }

	const FtaPersonIdSet& GetChildrenIds( void ) const { return childrenIds; }
	void AddChildId( const wxString& childId );

private:

	wxString husbandId;
	FtaPersonIdSet childrenIds;
};

// FtaPersonFemale.h
