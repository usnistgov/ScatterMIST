//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: WinMIST2.h
//**
//** Thomas A. Germer
//** Sensor Science Division, National Institute of Standards and Technology
//** 100 Bureau Dr. Stop 8443; Gaithersburg, MD 20899-8443
//** Phone: (301) 975-2876
//** Email: thomas.germer@nist.gov
//**
//** Version: 4.00 (January 2015) 
//**
//******************************************************************************
// WinMIST2.h : main header file for the WINMIST2 application
//

#if !defined(AFX_WINMIST2_H__9DA88712_9534_429E_B43C_661E1D7A6C94__INCLUDED_)
#define AFX_WINMIST2_H__9DA88712_9534_429E_B43C_661E1D7A6C94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2App:
// See WinMIST2.cpp for the implementation of this class
//

class CWinMIST2App : public CWinApp
{
public:
	CWinMIST2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinMIST2App)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//void OnFileNew();

// Implementation
	//{{AFX_MSG(CWinMIST2App)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnHelpMistweb();
	afx_msg void OnHelpScatmechweb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	public:
		CMultiDocTemplate* pTextResultsTemplate;
		CMultiDocTemplate* pTextListingTemplate;
		CMultiDocTemplate* pTextSamplesTemplate;
		CMultiDocTemplate* pPolarTemplate;
		CMultiDocTemplate* pGraphTemplate;
		CMultiDocTemplate* pWizardTemplate;
		CMultiDocTemplate* pWinMIST2DocTemplate;

};

extern CWinMIST2App theApp;

inline CWinMIST2App* GetApp() {return (CWinMIST2App*)(AfxGetApp());}

#include "TextListingView.h"
#include "TextResultsView.h"
#include "TextSamplesView.h"
#include "GraphResultsView.h"
#include "AxisDialog.h"

extern std::ostream sout;
extern std::ostream serr;
/*
extern CMultiDocTemplate* pTextResultsTemplate;
extern CMultiDocTemplate* pTextListingTemplate;
extern CMultiDocTemplate* pTextSamplesTemplate;
extern CMultiDocTemplate* pPolarTemplate;
extern CMultiDocTemplate* pGraphTemplate;
extern CMultiDocTemplate* pWizardTemplate;
extern CMultiDocTemplate* pWinMIST2DocTemplate;
*/
class scaler {
	public:
		scaler(double _y1,double _y2,double _x1,double _x2,bool _force=false) 
			: y1(_y1),y2(_y2),x1(_x1),x2(_x2),force(_force) 
		{
			sign = x1>x2;
			a=(y2-y1)/(x2-x1);
		}

		double operator()(double x) const {
			if (force) {
				if (sign) {
					if (x<x2) return y2;
					if (x>x1) return y1;
				} else {
					if (x>x2) return y2;
					if (x<x1) return y1;
				}
			}
			return y1+(x-x1)*a;
		}
	private:
		double x1,x2,y1,y2;
		double a;
		bool force;
		bool sign;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINMIST2_H__9DA88712_9534_429E_B43C_661E1D7A6C94__INCLUDED_)
