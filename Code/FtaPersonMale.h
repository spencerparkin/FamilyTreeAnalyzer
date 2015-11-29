// FtaPersonMale.h

#pragma once

#include "FtaPerson.h"

class FtaPersonMale : public FtaPerson
{
public:

	FtaPersonMale( FtaTreeCache* cache );
	virtual ~FtaPersonMale( void );

	static Gender GetStaticGender( void ) { return GENDER_MALE; }

	virtual Gender GetGender( void ) const override { return GENDER_MALE; }
	virtual void FindAllChildrenIds( FtaPersonIdSet& childrenIds, bool populateCacheAsNeeded = false ) const override;

	const FtaPersonIdSet& GetWifeIds( void ) const { return wifeIds; }
	void AddWifeId( const wxString& wifeId );

private:

	FtaPersonIdSet wifeIds;

	// TODO: Add male-specific attributes.  (e.g., priesthood ordinances.)
};

// FtaPersonMale.h
