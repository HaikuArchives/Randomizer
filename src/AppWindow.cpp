//========================================================================
//	AppWindow.cpp
//	
//	Copyright 2005-2009 Sergey Kazenyuk, All Rights Reserved.
//	Distributed under the terms of the MIT License.
//========================================================================
//	$Id: AppWindow.cpp 12 2009-02-02 08:51:09Z sergey.kazenyuk $
//	$Rev: 12 $
//	$Author: sergey.kazenyuk $
//	$Date: 2009-02-02 10:51:09 +0200 (Mon, 02 Feb 2009) $
//========================================================================

#include <Catalog.h>
#include <LayoutBuilder.h>

#include <string>

#include "AppWindow.h"
#include "App.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "AppWindow"

//====================================================================

void Generator(char* password, const int &num, const char* symbols);

//====================================================================

AppWindow::AppWindow(BRect frame)
	: BWindow(frame, B_TRANSLATE_SYSTEM_NAME(App_Name), B_TITLED_WINDOW,
		B_NOT_RESIZABLE|B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	MainView = new AppView(Bounds());

	PassOut = new BTextControl("PassOut", "", NULL, NULL);
	PassOut->SetEnabled(false);

	PassLength = new BSpinner("PassLength", B_TRANSLATE("Password length:"),
		new BMessage(SEQ_LEN_MSG));
	PassLength->SetMinValue(1);
	PassLength->SetValue(10);
	
	UpperCaseCB = new BCheckBox("UpperCaseCB", B_TRANSLATE("Upper case"),
		new BMessage(UCASE_CB_MSG));
	UpperCaseCB->SetValue(B_CONTROL_ON);

	LowerCaseCB = new BCheckBox( "LowerCaseCB", B_TRANSLATE("Lower case"),
		new BMessage(LCASE_CB_MSG));
	LowerCaseCB->SetValue(B_CONTROL_ON);
	
	NumCB = new BCheckBox("NumCB", B_TRANSLATE("Numbers"),
		new BMessage(NUMBERS_CB_MSG));
	NumCB->SetValue(B_CONTROL_ON);
	
	SpecSymbCB = new BCheckBox("SpecSymbCB", B_TRANSLATE("Symbols"),
		new BMessage(SPEC_SYMB_CB_MSG));
	
	//Custom symbols checkbox
	CustSymbCB = new BCheckBox("CustSymbCB", B_TRANSLATE("Custom:"),
		new BMessage(CUST_SYMB_CB_MSG));
	
	//Custom symbols input field
	CustSymb = new BTextControl("CustSymb", "", NULL,
		new BMessage(CUST_SYMB_MSG));
	CustSymb->SetDivider(0);
	CustSymb->SetEnabled(false);
			
	GenerateBtn = new BButton("GenBtn", B_TRANSLATE("Generate"),
		new BMessage(GEN_BTN_MSG));
	GenerateBtn->MakeDefault(true);
	GenerateBtn->ResizeToPreferred();

	BSeparatorView* separatorSettingsView = new BSeparatorView("settings", "Password settings",
		B_HORIZONTAL, B_FANCY_BORDER, BAlignment(B_ALIGN_LEFT,
											B_ALIGN_VERTICAL_CENTER));
	SeparatorPasswordView = new BSeparatorView("settings", B_TRANSLATE("Generated password"),
		B_HORIZONTAL, B_FANCY_BORDER, BAlignment(B_ALIGN_LEFT,
											B_ALIGN_VERTICAL_CENTER));
	SetupMenuBar();
	separatorSettingsView->Hide();
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_SMALL_SPACING)
		.Add(MenuBar)
		.Add(separatorSettingsView)
		.AddGrid()
			.SetInsets(B_USE_WINDOW_INSETS, 0, B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
			.Add(UpperCaseCB, 0, 0)
			.Add(LowerCaseCB, 0, 1)
			.Add(NumCB, 1, 0)
			.Add(SpecSymbCB, 1, 1)
			.Add(CustSymbCB, 0, 2)
			.Add(CustSymb, 1, 2)
			.Add(PassLength, 0, 3, 2)
		.End()
		.Add(SeparatorPasswordView)
		.AddGroup(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_INSETS, 0, B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
			.Add(PassOut)
			.AddGroup(B_HORIZONTAL)
				.AddGlue()
				.Add(GenerateBtn)
			.End()
		.End();

	PassOut->Hide();
	SeparatorPasswordView->Hide();

	// Is this useful?

	UpperCaseCB->SetToolTip("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	LowerCaseCB->SetToolTip("abcdefghijklmnopqrstuvwxyz");
	NumCB->SetToolTip("0123456789");
	SpecSymbCB->SetToolTip("!@#$%^&*");
	CustSymbCB->SetToolTip(B_TRANSLATE("Custom set of characters"));
}

//--------------------------------------------------------------------

bool AppWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

//--------------------------------------------------------------------

void AppWindow::MessageReceived(BMessage* message)
{
#ifdef DEBUG	
	printf("AppWindow::MessageReceived: \n"); 
	message->PrintToStream();
#endif
	switch(message->what)
	{
		case GEN_BTN_MSG:
		{
			const char en_upsymbols[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			const char en_lowsymbols[] = "abcdefghijklmnopqrstuvwxyz";
			const char num_symbols[] = "0123456789";
			const char spec_symbols[] = "!@#$%^&*";	//<-------
			
			PassOut->SetText("");
			
			string* symbols = new string;
			if (UpperCaseCB->Value() == B_CONTROL_ON)
				*symbols += en_upsymbols;
			if (LowerCaseCB->Value() == B_CONTROL_ON)
				*symbols += en_lowsymbols;
			if (NumCB->Value() == B_CONTROL_ON)
				*symbols += num_symbols;
			if (SpecSymbCB->Value() == B_CONTROL_ON)
				*symbols += spec_symbols;
			if (CustSymbCB->Value() == B_CONTROL_ON)
				*symbols += CustSymb->Text();
			int pass_length = PassLength->Value();
			char* password = new char [pass_length];			
			Generator(password, pass_length, symbols->c_str());
			delete symbols;
			PassOut->SetText(password);
			PassOut->Show();
			SeparatorPasswordView->Show();
			delete[] password;
		}
		break;
		case CUST_SYMB_CB_MSG:	//Custom symbols checkbox set/unset
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

void AppWindow::SetupMenuBar()
{
	MenuBar = new RandoMenuBar(BRect(0,0,0,0), "menubar",
		B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP, B_ITEMS_IN_ROW, true);
//	MainView->AddChild(MenuBar);
	
	FileMenu = new BMenu(B_TRANSLATE("App"));
	MenuBar->AddItem(FileMenu);
	
	AboutFileMenuItem = new BMenuItem(B_TRANSLATE("About"), new BMessage(B_ABOUT_REQUESTED),
		0, 0);
	FileMenu->AddItem(AboutFileMenuItem);
	
	FileMenu->AddSeparatorItem();
	
	QuitFileMenuItem = new BMenuItem(B_TRANSLATE("Quit"), new BMessage(B_QUIT_REQUESTED),
		'Q', 0);
	FileMenu->AddItem(QuitFileMenuItem);
}

