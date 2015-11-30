// FtaFrame.h

#pragma once

#include <wx/frame.h>

class FtaFrame : public wxFrame
{
public:

	FtaFrame( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	virtual ~FtaFrame( void );

private:

	enum
	{
		ID_AcquireAccessToken = wxID_HIGHEST,
		ID_PopulateTreeCacheAtPerson,
		ID_WipeAllCache,
		ID_Exit,
		ID_About,
	};

	void OnAcquireAccessToken( wxCommandEvent& event );
	void OnPopulateTreeCacheAtPerson( wxCommandEvent& event );
	void OnWipeAllCache( wxCommandEvent& event );
	void OnExit( wxCommandEvent& event );
	void OnAbout( wxCommandEvent& event );
	void OnUpdateMenuItemUI( wxUpdateUIEvent& event );
};

// FtaFrame.h
