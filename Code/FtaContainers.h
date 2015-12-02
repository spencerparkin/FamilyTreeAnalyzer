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

WX_DECLARE_STRING_HASH_MAP( FtaPerson*, FtaPersonMap );
WX_DECLARE_HASH_SET( FtaPerson*, wxPointerHash, wxPointerEqual, FtaPersonSet );
WX_DECLARE_HASH_SET( wxString, wxStringHash, wxStringEqual, FtaPersonIdSet );
WX_DECLARE_STRING_HASH_MAP( wxString, FtaOneToOneRelationshipIdMap );
WX_DECLARE_STRING_HASH_MAP( FtaPersonIdSet*, FtaOneToManyRelationshipIdMap );
WX_DECLARE_STRING_HASH_MAP( FtaPersonSet*, FtaOneToManyRelationshipMap );
WX_DECLARE_LIST( wxJSONValue, JSONValueList );
WX_DECLARE_LIST( FtaPerson, FtaPersonList );
WX_DEFINE_ARRAY_INT( int, FtaIndexArray );

void FtaDeletePersonMap( FtaPersonMap& personMap );
void FtaDeleteRelationshipIdMap( FtaOneToManyRelationshipIdMap& relationshipMap );
void FtaDeleteRelationshipMap( FtaOneToManyRelationshipMap& relationshipMap, bool deletePersons = false );

// FtaContainers.h