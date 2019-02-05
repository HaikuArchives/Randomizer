//========================================================================
//	Generator.cpp
//
//	Copyright 2005-2008 Sergey Kazenyuk, All Rights Reserved.
//	Distributed under the terms of the MIT License.
//========================================================================

#include <OS.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

//====================================================================

void
Generator(char* password, const int& num, const char* symbols)
{
	if (!strlen(symbols)) {
		password[0] = '\0';
		return;
	}
	cout << "Random sequence length: " << num << endl;
	cout << "Symbols: " << symbols << endl;
	srand(real_time_clock_usecs() + real_time_clock());
	for (int i = 0; i < num; i++)
		password[i] = symbols[rand() % strlen(symbols)];
	password[num] = '\0';
	cout << "Random sequence: " << password << endl;
}
