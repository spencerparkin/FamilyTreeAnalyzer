// FtaPerson.h

#pragma once

#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/hashset.h>
#include <wx/jsonval.h>

class FtaPerson;
class FtaTreeCache;

WX_DECLARE_HASH_SET( FtaPerson*, wxPointerHash, wxPointerEqual, FtaPersonSet );
WX_DECLARE_HASH_SET( wxString, wxStringHash, wxStringEqual, FtaPersonIdSet );

// To keep things simpler, a person appearing in the tree cache must have
// complete (as apposed to partial) information (i.e., all known information
// about the person must be present here.)
class FtaPerson
{
public:

	FtaPerson( const wxString& personId, FtaTreeCache* cache );
	virtual ~FtaPerson( void );

	const wxString& GetPersonId( void ) const { return personId; }

	FtaPerson* GetBiologicalFather( void );
	FtaPerson* GetBiologicalMother( void );

	// If no spouse is given here, all biological children are returned.
	bool GetBiologicalChildren( const FtaPerson* spouse, FtaPersonSet& children );

	// These are all those with whom this person had biological children, regardless of marriage.
	bool GetSpouses( FtaPersonSet& spouses );

	// Unfortunately, we're not making the most efficient use of the response data here
	// as familysearch.org may return to us much more information than we consume.
	// To keep things simpler, however, we focus on caching one and only one person at a time.
	bool SetImmediateAncestry( const wxJSONValue& responseValue );
	bool SetImmediateDescendancy( const wxJSONValue& responseValue );
	bool SetSpouses( const wxJSONValue& responseValue );

private:

	FtaTreeCache* cache;

	wxString personId;
	wxString biologicalFatherId;
	wxString biologicalMotherId;

	// TODO: Know spouses by id.  Know children by spouses by id.

	// TODO: Know ordinance information.
};

// FtaPerson.h
