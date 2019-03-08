
// FiveInRow.h : FiveInRow main head file of the application
//
#pragma once

#ifndef __AFXWIN_H__
    #error "Include \"stdafx.h\" before including this file to generate PCH file"
#endif

#include "resource.h"       // Main symbol


// CFiveInRowApp:
//
//

class CFiveInRowApp : public CWinApp
{
public:
	CFiveInRowApp();


// Override
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implement

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFiveInRowApp theApp;
