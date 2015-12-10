// FtaPerson.cpp

#include "FtaPerson.h"
#include "FtaTreeCache.h"
#include "FtaFrame.h"
#include "FtaApp.h"

FtaPerson::FtaPerson( const wxString& personId )
{
	this->personId = personId;
	infoState = INFO_INCOMPLETE;
	flags = 0;
	gender = GENDER_UNKNOWN;
}

/*virtual*/ FtaPerson::~FtaPerson( void )
{
}

bool FtaPerson::IsInfoComplete( void )
{
	int infoCompleteMask = FLAG_ANCESTRY | FLAG_DESCENDANCY | FLAG_PERSONAL_DETAILS | FLAG_PORTRAIT;

	if( ( flags & infoCompleteMask ) != infoCompleteMask )
		return false;

	return true;
}

wxString FtaPerson::GetGenderString( void ) const
{
	switch( gender )
	{
		case GENDER_UNKNOWN:	return "Unknown";	break;
		case GENDER_MALE:		return "Male";		break;
		case GENDER_FEMALE:		return "Female";	break;
	}

	return "???";
}

bool FtaPerson::DumpInfo( void )
{
	FtaFrame* frame = wxGetApp().GetFrame();

	frame->AddLogMessage( "-------------------------------------" );
	frame->AddLogMessage( "Name: " + name );
	frame->AddLogMessage( "Gender: " + GetGenderString() );
	frame->AddLogMessage( "Life-span: " + lifeSpan );
	frame->AddLogMessage( "Birth-place: " + birthPlace );
	frame->AddLogMessage( "Person-ID: " + personId );
	frame->AddLogMessage( "Mother-ID: " + ( motherId.IsEmpty() ? wxString( "Unknown" ) : motherId ) );
	frame->AddLogMessage( "Father-ID: " + ( fatherId.IsEmpty() ? wxString( "Unknown" ) : fatherId ) );

	frame->AddLogMessage( wxString::Format( "%d child(ren)...", childrenIdSet.size() ) );
	int i = 0;
	FtaPersonIdSet::iterator iter = childrenIdSet.begin();
	while( iter != childrenIdSet.end() )
	{
			wxString childId = *iter;
		frame->AddLogMessage( wxString::Format( "%d: ", ++i ) + childId );
		iter++;
	}

	frame->AddLogMessage( wxString::Format( "%d spouse(s)...", spousesIdSet.size() ) );
	i = 0;
	iter = spousesIdSet.begin();
	while( iter != spousesIdSet.end() )
	{
		wxString spouseId = *iter;
		frame->AddLogMessage( wxString::Format( "%d: ", ++i ) + spouseId );
		iter++;
	}

	return true;
}

void FtaPerson::GatherNearestRelations( FtaPersonList& personList )
{
	personList.clear();

	FtaTreeCache* cache = wxGetApp().GetTreeCache();

	if( !motherId.IsEmpty() )
		personList.push_back( cache->Lookup( motherId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS ) );

	if( !fatherId.IsEmpty() )
		personList.push_back( cache->Lookup( fatherId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS ) );

	FtaPersonIdSet::iterator iter = childrenIdSet.begin();
	while( iter != childrenIdSet.end() )
	{
		wxString childId = *iter;
		if( !childId.IsEmpty() )
			personList.push_back( cache->Lookup( childId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS ) );
		iter++;
	}

	iter = spousesIdSet.begin();
	while( iter != spousesIdSet.end() )
	{
		wxString spouseId = *iter;
		if( !spouseId.IsEmpty() )
			personList.push_back( cache->Lookup( spouseId, FtaTreeCache::ALLOCATE_ON_CACHE_MISS ) );
		iter++;
	}
}

// FtaPerson.cpp