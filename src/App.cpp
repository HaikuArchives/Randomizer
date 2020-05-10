//========================================================================
//	App.cpp
//
//	Copyright 2005-2009 Sergey Kazenyuk, All Rights Reserved.
//  Copyright 2019 Humdinger
//	Distributed under the terms of the MIT License.
//========================================================================
//	$Id: App.cpp 11 2009-02-01 16:13:11Z sergey.kazenyuk $
//	$Rev: 11 $
//	$Author: sergey.kazenyuk $
//	$Date: 2009-02-01 18:13:11 +0200 (Sun, 01 Feb 2009) $
//========================================================================

#include <iostream>

#include <AboutWindow.h>
#include <Alert.h>
#include <Catalog.h>

#include "App.h"
#include "AppWindow.h"
#include "Constants.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Application"

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
	BAboutWindow* about = new BAboutWindow(
		B_TRANSLATE_SYSTEM_NAME(App_Name), App_Sig);
	const char* extraCopyrights[] = {
		"2005-2009 Sergey S. Kazenyuk",
		"2018-2020 Janus, Humdinger",
		NULL
	};
	const char* authors[] = {
		B_TRANSLATE("Sergey S. Kazenyuk (original author)"),
		"Humdinger",
		"Janus",
		NULL
	};
	about->AddCopyright(2005, "Sergey S. Kazenyuk", extraCopyrights);
	about->AddAuthors(authors);
	about->Show();
}
