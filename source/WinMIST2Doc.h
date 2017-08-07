//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: WinMIST2Doc.h
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
// WinMIST2Doc.h : interface of the CWinMIST2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINMIST2DOC_H__B0F9F3C7_9435_49B3_B292_0A06F6C9C3C9__INCLUDED_)
#define AFX_WINMIST2DOC_H__B0F9F3C7_9435_49B3_B292_0A06F6C9C3C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include "mist0.h"
#include "ProgressBar.h"

class WinMIST_Notifier : public MIST_Notifier  {
public:

	WinMIST_Notifier(ProgressBar* _bar) : bar(_bar) {
		done=false;
		stop=false;
	}

	bool Stop() {
		MSG msg;
		while (::PeekMessage(&msg,HWND(*bar),0,0,PM_NOREMOVE)) theApp.PumpMessage( );
		if (bar) {
			if (bar->m_stop) stop=true;
		}
		return stop;
	}

	void SetProgress(double fraction) {
	    MSG msg;
		//while (::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) theApp.PumpMessage( );
		//while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) theApp.PumpMessage( );
		while (::PeekMessage(&msg,HWND(*bar),0,0,PM_NOREMOVE)) theApp.PumpMessage( );
		if (bar) {
			bar->m_progress.SetRange(0,100);
			bar->m_progress.SetPos(fraction*100);
			bar->m_progress.UpdateData(FALSE);
		}
	}

	void SetDone() {
		done=true;
	}

	ProgressBar* bar;
	bool done;
	bool stop;
};


class CWinMIST2Doc : public CDocument
{
protected: // create from serialization only
	CWinMIST2Doc();
	DECLARE_DYNCREATE(CWinMIST2Doc)


// Attributes
public:

	void Simulate(CString& commands);

	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinMIST2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL
	
	CView* GetTopView() {	
		POSITION pos = GetFirstViewPosition();
		if (pos != NULL) return (CView*)GetNextView(pos);
		else return NULL;
	}

// Implementation
public:
	virtual ~CWinMIST2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	WinMIST_Notifier* progress;

// Generated message map functions
protected:
	//{{AFX_MSG(CWinMIST2Doc)
	afx_msg void OnViewResultsText();
	afx_msg void OnViewSamplesText();
	afx_msg void OnViewListing();
	afx_msg void OnViewResultsGraph();
	afx_msg void OnViewSamplesGraph();
	afx_msg void OnSimulate();
	afx_msg void OnModelwizard();
	virtual void OnCancel();
	afx_msg void OnMist0();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	MIST_Calculation_Data data;
	/*
	std::string results;
	std::string samples;
	std::string corners;
	std::string listing;
	vvdouble xvalues;
	vvdouble yvalues;
	vstring xnames;
	vstring ynames;
	double startthetai;
	*/

public:
	//void LoadData(const std::string& samplestring);
	//std::vector<double> x,y,z,inten;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINMIST2DOC_H__B0F9F3C7_9435_49B3_B292_0A06F6C9C3C9__INCLUDED_)
