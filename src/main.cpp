//========================================================================
//	main.cpp
//	
//	Copyright 2005, 2008 Sergey Kazenyuk, All Rights Reserved.
//	Distributed under the terms of the MIT License.
//========================================================================

#include <iostream.h>
#include "App.h"

using namespace std;

//====================================================================

int main()
{
	App *theApp;
	theApp = new(App);
	theApp->Run();
	delete theApp;
	
	cout << "Good-bye!" << endl;
	return B_OK;
}
