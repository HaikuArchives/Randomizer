//========================================================================
//	App.cpp
//
//	Copyright 2005-2009 Sergey Kazenyuk, All Rights Reserved.
//	Distributed under the terms of the MIT License.
//========================================================================
//	$Id: App.cpp 11 2009-02-01 16:13:11Z sergey.kazenyuk $
//	$Rev: 11 $
//	$Author: sergey.kazenyuk $
//	$Date: 2009-02-01 18:13:11 +0200 (Sun, 01 Feb 2009) $
//========================================================================

#include <iostream>

#include <Alert.h>

#include "App.h"
#include "AppWindow.h"
#include "Constants.h"

//====================================================================

App::App()
	:
	BApplication(App_Sig)
{
}

//--------------------------------------------------------------------

void
App::ReadyToRun()
{
	BRect MainWindowRect;
	MainWindowRect.Set(50, 50, 255, 231);
	MainWindow = new AppWindow(MainWindowRect);
	MainWindow->Show();
}

//--------------------------------------------------------------------

void
App::AboutRequested()
{
#ifdef DEBUG
	std::cout << "App::AboutRequested Received!" << std::endl;
#endif
	BAlert* AboutAlert;
	AboutAlert = new BAlert("About " App_Name,
		App_Name "\n"
				 "Version " App_Version "\n"
				 "Compiled on: " __DATE__ " " __TIME__ "\n"
				 "\n"
				 "Project page:\n"
				 "\thttp://randomizer.googlecode.com/\n"
				 "You may also send your bug reports and comments to:\n"
				 "\tkazenyuk@gmail.com\n"
				 "\n"
				 "Copyright " B_UTF8_COPYRIGHT " 2005-2009 Sergey S. Kazenyuk\n"
				 "All rights reserved.\n"
				 "Distributed under the terms of the MIT License.",
		"Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_INFO_ALERT);
	AboutAlert->SetShortcut(0, B_ESCAPE);
	AboutAlert->Go();
}
