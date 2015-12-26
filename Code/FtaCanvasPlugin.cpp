// FtaCanvasPlugin.cpp

#include "FtaCanvasPlugin.h"

FtaCanvasPlugin::FtaCanvasPlugin( void )
{
	canvas = nullptr;
}

/*virtual*/ FtaCanvasPlugin::~FtaCanvasPlugin( void )
{
}

/*virtual*/ bool FtaCanvasPlugin::Animate( void )
{
	return false;
}

// FtaCanvasPlugin.cpp