//========================================================================
//	Constants.h
//	Miscellaneous constants and definitions
//	Copyright 2005-2008 Sergey Kazenyuk, All Rights Reserved.
//  Copyright 2018 Janus
//	Distributed under the terms of the MIT License.
//========================================================================

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define App_Name "Randomizer"
#define App_Sig "application/x-vnd.kazenyuk-randomizer"
#define App_Version "1.0.0b1"

//====================================================================
// Message commands (message->what)
//====================================================================
const uint32 GEN_BTN_MSG = 'Gen';
const uint32 COPY_BTN_MSG = 'CopC';
const uint32 SEQ_MSG = 'Sequ';
const uint32 SEQ_LEN_MSG = 'SNum';
const uint32 CUST_SYMB_MSG = 'CusS';

const uint32 UCASE_CB_MSG = 'UpCa';
const uint32 LCASE_CB_MSG = 'LoCa';
const uint32 NUMBERS_CB_MSG = 'NumS';
const uint32 SPEC_SYMB_CB_MSG = 'SpeS';
const uint32 CUST_SYMB_CB_MSG = 'CuSC';

#endif
