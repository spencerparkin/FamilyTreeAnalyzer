// FtaFontSystem.cpp

#include "FtaFontSystem.h"
#include "FtaApp.h"
#include FT_MODULE_H
#include FT_TRUETYPE_IDS_H

FtaFontSystem::FtaFontSystem( void )
{
	initialized = false;
}

/*virtual*/ FtaFontSystem::~FtaFontSystem( void )
{
	if( initialized )
		Finalize();

	WipeFontCache();
}

bool FtaFontSystem::Initialize( void )
{
	bool success = false;

	do
	{
		if( initialized )
			break;

		FT_Error error = FT_Init_FreeType( &library );
		if( error != FT_Err_Ok )
			break;

		initialized = true;

		success = true;
	}
	while( false );

	return success;
}

bool FtaFontSystem::Finalize( void )
{
	bool success = false;
	
	do
	{
		if( !initialized )
			break;

		FT_Error error = FT_Done_Library( library );
		if( error != FT_Err_Ok )
			break;

		initialized = false;

		success = true;
	}
	while( false );

	return success;
}

bool FtaFontSystem::DrawText( const wxString& text, const wxString& font, GLfloat wrapLength, Justification justification )
{
	bool success = false;

	do
	{
		if( !initialized )
			break;

		wxString key = MakeFontKey( font );

		FtaFont* cachedFont = nullptr;

		FtaFontMap::iterator iter = fontMap.find( key );
		if( iter == fontMap.end() )
		{
			cachedFont = new FtaFont( this );

			if( !cachedFont->Initialize( font ) )
			{
				delete cachedFont;
				break;
			}

			fontMap[ key ] = cachedFont;
		}

		if( !cachedFont )
			break;

		if( !cachedFont->DrawText( text, wrapLength, justification ) )
			break;

		success = true;
	}
	while( false );

	return success;
}

wxString FtaFontSystem::MakeFontKey( const wxString& font )
{
	wxString key = font;
	key.MakeLower();
	return key;
}

void FtaFontSystem::WipeFontCache( void )
{
	while( fontMap.size() > 0 )
	{
		FtaFontMap::iterator iter = fontMap.begin();
		FtaFont* font = iter->second;
		font->Finalize();
		delete font;
		fontMap.erase( iter );
	}
}

FtaFont::FtaFont( FtaFontSystem* fontSystem )
{
	initialized = false;
	this->fontSystem = fontSystem;
}

/*virtual*/ FtaFont::~FtaFont( void )
{
	if( initialized )
		Finalize();
}

/*virtual*/ bool FtaFont::Initialize( const wxString& font )
{
	bool success = false;
	FT_Face face;

	do
	{
		if( initialized )
			break;

		wxString fontFile = wxGetApp().GetResourceFolder() + "\\Fonts\\" + font;

		FT_Error error = FT_New_Face( fontSystem->GetLibrary(), fontFile, 0, &face );
		if( error != FT_Err_Ok )
			break;

		if( face->num_charmaps == 0 )
			break;

		// TODO: How do we know we have the right charmap for our purposes?
		if( face->charmap == nullptr )
		{
			error = FT_Set_Charmap( face, face->charmaps[0] );
			if( error != FT_Err_Ok )
				break;
		}

		// TODO: We only care about basic characters in ASCII, so we may need to do our own Unicode to ASCII conversion.
		FT_ULong charcode;
		FT_ULong maxCharcode = sizeof( char ) * 8;
		for( charcode = 0; charcode < maxCharcode; charcode++ )
		{
			FT_UInt glyph_index = FT_Get_Char_Index( face, charcode );
			if( glyph_index == 0 )
			{
				glyphArray.push_back( nullptr );
				continue;
			}

			error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
			if( error != FT_Err_Ok )
				break;

			FT_GlyphSlot& glyph = face->glyph;

			if( glyph->format != FT_GLYPH_FORMAT_BITMAP )
			{
				error = FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL );
				if( error != FT_Err_Ok )
					break;
			}

			FtaGlyph* cachedGlyph = new FtaGlyph();
			glyphArray.push_back( cachedGlyph );

			if( !cachedGlyph->Initialize( glyph ) )
				break;
		}

		if( charcode != maxCharcode )
			break;

		initialized = true;

		success = true;
	}
	while( false );

	if( face )
	{
		//...release face?...
	}

	return success;
}

/*virtual*/ bool FtaFont::Finalize( void )
{
	bool success = false;

	do
	{
		if( !initialized )
			break;
	
		for( int i = 0; i < ( signed )glyphArray.size(); i++ )
		{
			FtaGlyph* glyph = glyphArray[i];
			delete glyph;
		}

		glyphArray.Clear();

		initialized = false;

		success = true;
	}
	while( false );
	
	return success;
}

/*virtual*/ bool FtaFont::DrawText( const wxString& text, GLfloat wrapLength, FtaFontSystem::Justification justification )
{
	return true;
}

FtaGlyph::FtaGlyph( void )
{
	displayList = 0;
}

/*virtual*/ FtaGlyph::~FtaGlyph( void )
{
}

bool FtaGlyph::Initialize( FT_GlyphSlot& glyphSlot )
{
	bool success = false;

	do
	{
		displayList = glGenLists(1);
		if( displayList == 0 )
			break;

		//glyphSlot->format

		//...

		glNewList( displayList, GL_COMPILE );

		glEndList();

		success = true;
	}
	while( false );

	return success;
}

bool FtaGlyph::Finalize( void )
{
	// TODO: I suppose here we could delete the display list, but will the context be current and valid?

	return true;
}

// FtaFontSystem.cpp