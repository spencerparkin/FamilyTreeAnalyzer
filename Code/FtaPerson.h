// FtaPerson.h

#pragma once

#include "FtaContainers.h"
#include <lua.hpp>
#include <wx/glcanvas.h>
#include "c3ga/c3ga.h"

class FtaPerson
{
public:

	FtaPerson( const wxString& personId );
	virtual ~FtaPerson( void );

	enum InfoState
	{
		INFO_INCOMPLETE,
		INFO_REQUESTED,
		INFO_COMPLETE,		// This doesn't mean all info is known about the person; only that we sucked it all down from familysearch.org.
	};

	enum Flags
	{
		FLAG_ANCESTRY				= 0x00000001,
		FLAG_DESCENDANCY			= 0x00000002,
		FLAG_PERSONAL_DETAILS		= 0x00000004,
		FLAG_ORDINANCES				= 0x00000008,
		FLAG_PORTRAIT				= 0x00000010,
	};
	
	enum Gender
	{
		GENDER_UNKNOWN,
		GENDER_MALE,
		GENDER_FEMALE,
	};

	bool IsInfoComplete( void ) const;
	bool DumpInfo( void ) const;

	int GetFlags( void ) const { return flags; }
	void SetFlags( int flags ) { this->flags = flags; }

	const wxString& GetPersonId( void ) const { return personId; }

	const wxString& GetMotherId( void ) const { return motherId; }
	void SetMotherId( const wxString& motherId ) { this->motherId = motherId; }

	const wxString& GetFatherId( void ) const { return fatherId; }
	void SetFatherId( const wxString& fatherId ) { this->fatherId = fatherId; }

	const FtaPersonIdSet& GetChildrenIdSet( void ) const { return childrenIdSet; }
	FtaPersonIdSet& GetChildrenIdSet( void ) { return childrenIdSet; }

	const FtaPersonIdSet& GetSpousesIdSet( void ) const { return spousesIdSet; }
	FtaPersonIdSet& GetSpousesIdSet( void ) { return spousesIdSet; }

	Gender GetGender( void ) const { return gender; }
	void SetGender( Gender gender ) { this->gender = gender; }

	wxString GetGenderString( void ) const;

	const wxString& GetName( void ) const { return name; }
	void SetName( const wxString& name ) { this->name = name; }

	const wxString& GetLifeSpan( void ) const { return lifeSpan; }
	void SetLifeSpan( const wxString& lifeSpan ) { this->lifeSpan = lifeSpan; }

	const wxString& GetBirthPlace( void ) const { return birthPlace; }
	void SetBirthPlace( const wxString& birthPlace ) { this->birthPlace = birthPlace; }

	const wxString& GetPortraitUrl( void ) const { return portraitUrl; }
	void SetPortraitUrl( const wxString& portraitUrl ) { this->portraitUrl = portraitUrl; }

	void GatherNearestRelations( FtaPersonList& personList );

	bool SetFromLuaTable( lua_State* L, int idx );
	bool GetToLuaTable( lua_State* L ) const;

private:

	int flags;
	wxString personId;
	wxString motherId, fatherId;
	FtaPersonIdSet childrenIdSet;
	FtaPersonIdSet spousesIdSet;
	wxString name;
	Gender gender;
	wxString lifeSpan;
	wxString birthPlace;
	wxString portraitUrl;
};

// FtaPerson.h
