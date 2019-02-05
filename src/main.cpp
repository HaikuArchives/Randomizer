//========================================================================
//	main.cpp
//
//	Copyright 2005, 2008 Sergey Kazenyuk, All Rights Reserved.
//	Distributed under the terms of the MIT License.
//========================================================================

#include "App.h"
#include <iostream>

using namespace std;

//====================================================================

int
main()
{
	App* theApp;
	theApp = new (App);
	theApp->Run();
	delete theApp;

	cout << "Good-bye!" << endl;
	return B_OK;
}
