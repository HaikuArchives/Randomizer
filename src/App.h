//========================================================================
//	App.h
//
//	Copyright 2005-2008 Sergey Kazenyuk, All Rights Reserved.
//	Distributed under the terms of the MIT License.
//========================================================================

#ifndef APP_H
#define APP_H

#include <Application.h>

#include "AppWindow.h"
#include "Constants.h"

//====================================================================

class App : public BApplication
{
public:
	App();
	virtual void ReadyToRun();
	virtual void AboutRequested();

private:
	AppWindow* MainWindow;
};

#endif
