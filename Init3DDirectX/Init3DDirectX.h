// Init3DDirectX.h : main header file for the Init3DDirectX application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// определим удобный макрос для освобождения COM-объектов
#define RELEASE(obj) if(obj != NULL) {obj->Release(); obj = NULL;}


// CInit3DDirectXApp:
// See Init3DDirectX.cpp for the implementation of this class
//

class CInit3DDirectXApp : public CWinApp
{
public:
	CInit3DDirectXApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
};

extern CInit3DDirectXApp theApp;