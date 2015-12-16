// FtaContainers.h

#pragma once

#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/hashset.h>
#include <wx/hashmap.h>
#include <wx/dynarray.h>
#include <wx/list.h>
#include <wx/jsonval.h>

class FtaPerson;
class FtaAsyncRequest;

WX_DECLARE_STRING_HASH_MAP( FtaPerson*, FtaPersonMap );
WX_DECLARE_HASH_SET( FtaPerson*, wxPointerHash, wxPointerEqual, FtaPersonSet );
WX_DECLARE_HASH_SET( wxString, wxStringHash, wxStringEqual, FtaPersonIdSet );
WX_DECLARE_LIST( FtaPerson, FtaPersonList );
WX_DECLARE_LIST( FtaAsyncRequest, FtaAsyncRequestList );
WX_DECLARE_LIST( FtaPersonIdSet, FtaPersonIdSetList );
WX_DEFINE_ARRAY_INT( int, FtaOffsetArray );

void FtaDeletePersonMap( FtaPersonMap& personMap );

// FtaContainers.h