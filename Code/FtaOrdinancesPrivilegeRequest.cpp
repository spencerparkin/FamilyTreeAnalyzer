// FtaOrdinancesPrivilegeRequest.cpp

#include "FtaOrdinancesPrivilegeRequest.h"
#include "FtaClient.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_CLASS( FtaOrdinancesPrivilegeRequest, FtaAsyncRequest );

FtaOrdinancesPrivilegeRequest::FtaOrdinancesPrivilegeRequest( ResponseProcessor* processor ) : FtaAsyncRequest( processor )
{
}

/*virtual*/ FtaOrdinancesPrivilegeRequest::~FtaOrdinancesPrivilegeRequest( void )
{
}

/*virtual*/ bool FtaOrdinancesPrivilegeRequest::MakeUrl( wxString& url )
{
	if( !FtaAsyncRequest::MakeUrl( url ) )
		return false;

	url += "/platform/ordinances/ordinances";
	return true;
}

/*virtual*/ bool FtaOrdinancesPrivilegeRequest::ProcessResponse( long& retryAfterSeconds )
{
	if( !FtaAsyncRequest::ProcessResponse( retryAfterSeconds ) )
		return false;

	FtaClient* client = wxGetApp().GetClient();
	FtaFrame* frame = wxGetApp().GetFrame();

	if( httpStatusCode.Find( "403" ) >= 0 )
		frame->AddLogMessage( "LDS ordinance access is expressly forbidden." );
	else if( httpStatusCode.Find( "200" ) >= 0 )
	{
		frame->AddLogMessage( "LDS ordinance access available." );

		wxString query = "FamilyTreeAnalyzer is provided to help you with family history research.  It is "
						"not a FamilySearch product.  While it can help you find ancestors who need temple "
						"ordinances, it does not replace the research that verifies the accuracy of information "
						"and that makes sure the ordinances have not already been done.  Please check the "
						"information you find for accuracy, attach relevant sources, and check for possible "
						"duplicates before you reserve ordinances.\n\nAccept LDS ordinance access privileges?";
		if( wxYES != wxMessageBox( query, "LDS ordinance access", wxYES_NO | wxICON_QUESTION ) )
			frame->AddLogMessage( "LDS ordinance access forgone." );
		else
		{
			frame->AddLogMessage( "LDS ordinance access granted." );
			client->SetPrivilegeFlags( client->GetPrivilegeFlags() | FtaClient::PF_LDS_ORDINANCES );
			// TODO: If I need to queue up another request here to get any additional information, then do it.
		}
	}
	else
		frame->AddLogMessage( "LDS ordinance access unavailable." );

	return true;
}

/*virtual*/ bool FtaOrdinancesPrivilegeRequest::ProcessJSONResponse( wxJSONValue& responseValue )
{
	// Cache away end-points found in JSON in the client?
	// Is there way I go for the ordinance API?
	return true;
}

// FtaOrdinancesPrivilegeRequest.cpp