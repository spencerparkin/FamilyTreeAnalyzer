// FtaPerson.cpp

#include "FtaPerson.h"

FtaPerson::FtaPerson( const wxString& personId )
{
	this->personId = personId;
	infoState = INFO_UNKNOWN;
}

/*virtual*/ FtaPerson::~FtaPerson( void )
{
}

bool FtaPerson::IsInfoComplete( void )
{
	if( motherId.IsEmpty() )
		return false;

	if( fatherId.IsEmpty() )
		return false;

	// As far as the children and spouses go, we really have no way
	// of knowing if the information is complete.  We assume here that
	// whatever response we get from a request to find out the children
	// or spouses gives us enough information.
	
	if( childrenIdSet.size() == 0 )
		return false;

	if( spousesIdSet.size() == 0 )
		return false;

	// TODO: Check other details too, such as ordinances, etc...

	return true;
}

// FtaPerson.cpp