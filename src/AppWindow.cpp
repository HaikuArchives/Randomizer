//========================================================================
//	AppWindow.cpp
//
//	Copyright 2005-2009 Sergey Kazenyuk, All Rights Reserved.
//  Copyright 2018 Janus
//  Copyright 2019 Humdinger
//	Distributed under the terms of the MIT License.
//========================================================================
//	$Id: AppWindow.cpp 12 2009-02-02 08:51:09Z sergey.kazenyuk $
//	$Rev: 12 $
//	$Author: sergey.kazenyuk $
//	$Date: 2009-02-02 10:51:09 +0200 (Mon, 02 Feb 2009) $
//========================================================================

#include <Catalog.h>
#include <Clipboard.h>
#include <FindDirectory.h>
#include <IconUtils.h>
#include <LayoutBuilder.h>
#include <Path.h>
#include <Resources.h>
#include <Roster.h>
#include <SeparatorView.h>

#include <string>

#include "App.h"
#include "AppWindow.h"

#define PREFS_FILENAME "Randomizer_settings"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "AppWindow"

//====================================================================

void Generator(char* password, const int& num, const char* symbols);

//====================================================================

AppWindow::AppWindow(BRect frame)
	:
	BWindow(frame, B_TRANSLATE_SYSTEM_NAME(App_Name), B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	PassOut = new BTextControl("PassOut", "", NULL, NULL);
	PassOut->SetEnabled(false);

	PassLength = new BSpinner("PassLength", B_TRANSLATE("Password length:"),
		new BMessage(SEQ_LEN_MSG));
	PassLength->SetMinValue(1);
	PassLength->SetValue(10);

	UpperCaseCB = new BCheckBox(
		"UpperCaseCB", B_TRANSLATE("Upper case"), new BMessage(UCASE_CB_MSG));
	UpperCaseCB->SetValue(B_CONTROL_ON);

	LowerCaseCB = new BCheckBox(
		"LowerCaseCB", B_TRANSLATE("Lower case"), new BMessage(LCASE_CB_MSG));
	LowerCaseCB->SetValue(B_CONTROL_ON);

	NumCB = new BCheckBox(
		"NumCB", B_TRANSLATE("Numbers"), new BMessage(NUMBERS_CB_MSG));
	NumCB->SetValue(B_CONTROL_ON);

	SpecSymbCB = new BCheckBox(
		"SpecSymbCB", B_TRANSLATE("Symbols"), new BMessage(SPEC_SYMB_CB_MSG));

	// Custom symbols checkbox
	CustSymbCB = new BCheckBox(
		"CustSymbCB", B_TRANSLATE("Custom:"), new BMessage(CUST_SYMB_CB_MSG));

	// Custom symbols input field
	CustSymb
		= new BTextControl("CustSymb", "", NULL, new BMessage(CUST_SYMB_MSG));
	CustSymb->SetDivider(0);
	CustSymb->SetEnabled(false);

	GenerateBtn = new BButton(
		"GenBtn", B_TRANSLATE("Generate"), new BMessage(GEN_BTN_MSG));
	GenerateBtn->MakeDefault(true);

	font_height fh;
	PassOut->GetFontHeight(&fh);

	const float iconSize = ceilf(fh.ascent) - 2.0;

	CopyToClipboardBtn
		= new BButton("CopyToClipboardBtn", "", new BMessage(COPY_BTN_MSG));
	CopyToClipboardBtn->SetIcon(ResourceVectorToBitmap("CLIPBOARD", iconSize));
	CopyToClipboardBtn->SetToolTip(B_TRANSLATE("Copy to clipboard"));

	BSeparatorView* separatorPasswordView = new BSeparatorView(
		"generatedPassword", B_TRANSLATE("Generated password"), B_HORIZONTAL,
		B_FANCY_BORDER, BAlignment(B_ALIGN_CENTER, B_ALIGN_VERTICAL_CENTER));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(BuildMenuBar())
		.AddGrid(B_USE_DEFAULT_SPACING, 0, 1)
			.SetInsets(B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS,
				B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
			.Add(UpperCaseCB, 0, 0)
			.Add(LowerCaseCB, 0, 1)
			.Add(NumCB, 1, 0)
			.Add(SpecSymbCB, 1, 1)
			.Add(CustSymbCB, 0, 2)
			.Add(CustSymb, 1, 2)
			.End()
		.AddGroup(B_HORIZONTAL, 0)
			.SetInsets(
				B_USE_WINDOW_INSETS, 0, B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
			.Add(PassLength)
			.End()
		.AddStrut(B_USE_DEFAULT_SPACING)
		.Add(separatorPasswordView)
		.AddGroup(B_VERTICAL, B_USE_DEFAULT_SPACING)
			.SetInsets(
				B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS,
					B_USE_WINDOW_INSETS)
			.AddGroup(B_HORIZONTAL, 0)
				.Add(PassOut)
				.Add(CopyToClipboardBtn)
				.End()
			.AddGroup(B_HORIZONTAL)
				.AddGlue()
				.Add(GenerateBtn)
				.AddGlue()
				.End()
			.End();

	SpecSymbCB->SetToolTip("!@#$%^&*");
	CustSymbCB->SetToolTip(B_TRANSLATE("Custom set of characters"));

	UnarchivePreferences();

	GeneratePassword();
}

//--------------------------------------------------------------------

bool
AppWindow::QuitRequested()
{
	ArchivePreferences();
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

//--------------------------------------------------------------------

void
AppWindow::MessageReceived(BMessage* message)
{
#ifdef DEBUG
	printf("AppWindow::MessageReceived: \n");
	message->PrintToStream();
#endif
	switch (message->what) {
		case GEN_BTN_MSG:
		{
			GeneratePassword();
		} break;
		case COPY_BTN_MSG:
		{
			PassOut->TextView()->SelectAll();
			PassOut->TextView()->Copy(be_clipboard);
		} break;
		case CUST_SYMB_CB_MSG: // Custom symbols checkbox set/unset
			if (CustSymbCB->Value() == B_CONTROL_ON)
				CustSymb->SetEnabled(true);
			else
				CustSymb->SetEnabled(false);
			break;
		case B_ABOUT_REQUESTED:
			be_app->PostMessage(B_ABOUT_REQUESTED);
			break;
		default:
			BWindow::MessageReceived(message);
	}
}

//--------------------------------------------------------------------

BMenuBar*
AppWindow::BuildMenuBar()
{
	BMenuBar* menuBar = new BMenuBar("menubar");
	BMenu* menu = new BMenu(B_TRANSLATE("App"));

	menuBar->AddItem(menu);

	menu->AddItem(new BMenuItem(
		B_TRANSLATE("About Randomizer"), new BMessage(B_ABOUT_REQUESTED), 0, 0));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(
		B_TRANSLATE("Copy to clipboard"), new BMessage(COPY_BTN_MSG), 'C', 0));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(
		B_TRANSLATE("Quit"), new BMessage(B_QUIT_REQUESTED), 'Q', 0));

	return menuBar;
}

void
AppWindow::GeneratePassword()
{
	const char en_upsymbols[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char en_lowsymbols[] = "abcdefghijklmnopqrstuvwxyz";
	const char num_symbols[] = "0123456789";
	const char spec_symbols[] = "!@#$%^&*"; //<-------

	PassOut->SetText("");

	std::string symbols;
	if (UpperCaseCB->Value() == B_CONTROL_ON)
		symbols += en_upsymbols;
	if (LowerCaseCB->Value() == B_CONTROL_ON)
		symbols += en_lowsymbols;
	if (NumCB->Value() == B_CONTROL_ON)
		symbols += num_symbols;
	if (SpecSymbCB->Value() == B_CONTROL_ON)
		symbols += spec_symbols;
	if (CustSymbCB->Value() == B_CONTROL_ON)
		symbols += CustSymb->Text();

	int pass_length = PassLength->Value();
	char* password = new char[pass_length];

	Generator(password, pass_length, symbols.c_str());

	PassOut->SetText(password);
	delete[] password;
}

BBitmap*
AppWindow::ResourceVectorToBitmap(const char* resName, float iconSize)
{
	BResources res;
	size_t size;
	app_info appInfo;

	be_app->GetAppInfo(&appInfo);
	BFile appFile(&appInfo.ref, B_READ_ONLY);
	res.SetTo(&appFile);
	BBitmap* aBmp = NULL;
	const uint8* iconData
		= (const uint8*) res.LoadResource('VICN', resName, &size);

	if (size > 0) {
		aBmp = new BBitmap(BRect(0, 0, iconSize, iconSize), 0, B_RGBA32);
		status_t result = BIconUtils::GetVectorIcon(iconData, size, aBmp);
		if (result != B_OK) {
			delete aBmp;
			aBmp = NULL;
		}
	}
	return aBmp;
}

BFile
AppWindow::PrefsFile(int32 mode)
{
	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.SetTo(path.Path(), PREFS_FILENAME);

	return BFile(path.Path(), mode);
}

void
AppWindow::SavePreferences(BMessage& msg)
{
	BFile file = PrefsFile(B_WRITE_ONLY | B_CREATE_FILE);
	file.SetSize(0);
	msg.Flatten(&file);
}

void
AppWindow::LoadPreferences(BMessage& msg)
{
	BFile file = PrefsFile(B_READ_ONLY);
	msg.Unflatten(&file);
}

void
AppWindow::ArchivePreferences()
{
	BMessage message;
	message.AddInt32("PassLength", PassLength->Value());
	message.AddBool("UpperCaseCB", UpperCaseCB->Value() == B_CONTROL_ON);
	message.AddBool("LowerCaseCB", LowerCaseCB->Value() == B_CONTROL_ON);
	message.AddBool("NumCB", NumCB->Value() == B_CONTROL_ON);
	message.AddBool("SpecSymbCB", SpecSymbCB->Value() == B_CONTROL_ON);
	message.AddBool("CustSymbCB", CustSymbCB->Value() == B_CONTROL_ON);
	message.AddString("CustSymb", CustSymb->Text());
	SavePreferences(message);
}

void
AppWindow::UnarchivePreferences()
{
	BMessage message;
	LoadPreferences(message);

	int32 length;
	if (message.FindInt32("PassLength", &length) == B_OK)
		PassLength->SetValue(length);
	bool controlOn;
	if (message.FindBool("UpperCaseCB", &controlOn) == B_OK)
		UpperCaseCB->SetValue(controlOn ? B_CONTROL_ON : B_CONTROL_OFF);
	if (message.FindBool("LowerCaseCB", &controlOn) == B_OK)
		LowerCaseCB->SetValue(controlOn ? B_CONTROL_ON : B_CONTROL_OFF);
	if (message.FindBool("NumCB", &controlOn) == B_OK)
		NumCB->SetValue(controlOn ? B_CONTROL_ON : B_CONTROL_OFF);
	if (message.FindBool("SpecSymbCB", &controlOn) == B_OK)
		SpecSymbCB->SetValue(controlOn ? B_CONTROL_ON : B_CONTROL_OFF);
	if (message.FindBool("CustSymbCB", &controlOn) == B_OK) {
		CustSymbCB->SetValue(controlOn ? B_CONTROL_ON : B_CONTROL_OFF);
		CustSymb->SetEnabled(controlOn);
	}
	BString symbols;
	if (message.FindString("CustSymb", &symbols) == B_OK)
		CustSymb->SetText(symbols);
}
