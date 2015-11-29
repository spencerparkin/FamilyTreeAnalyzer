// FtaPerson.h

#pragma once

#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/hashset.h>

class FtaTreeCache;

WX_DECLARE_HASH_SET( wxString, wxStringHash, wxStringEqual, FtaPersonIdSet );

class FtaPerson
{
public:

	enum Gender { GENDER_MALE, GENDER_FEMALE };

	FtaPerson( FtaTreeCache* cache );
	virtual ~FtaPerson( void );
	
	virtual Gender GetGender( void ) const = 0;
	virtual void FindAllChildrenIds( FtaPersonIdSet& childrenIds, bool populateCacheAsNeeded = false ) const = 0;

	template< typename FtaPersonType > FtaPersonType* Cast( bool assertCast = false )
	{
		if( FtaPersonType::GetStaticGender() == GetGender() )
			return( ( FtaPersonType* )this );
		wxASSERT( !assertCast );
		return nullptr;
	}

	const wxString& GetId( void ) const { return id; }
	void SetId( const wxString& id ) { this->id = id; }

	const wxString& GetMotherId( void ) const { return motherId; }
	void SetMotherId( const wxString& motherId ) { this->motherId = motherId; }

	wxString GetFatherId( bool populateCacheAsNeeded = false ) const;

protected:

	FtaTreeCache* cache;

	wxString id;
	wxString motherId;
	wxString fullName;
	wxString lifeSpan;
};

// FtaPerson.h
