//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ProgressBar.h
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
#if !defined(AFX_PROGRESSBAR_H__F9BDB6D8_6628_49B7_BD3D_8627892716CF__INCLUDED_)
#define AFX_PROGRESSBAR_H__F9BDB6D8_6628_49B7_BD3D_8627892716CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ProgressBar dialog

class ProgressBar : public CDialog
{
// Construction
public:
	ProgressBar(CWnd* pParent = NULL);   // standard constructor

	bool m_stop;

// Dialog Data
	//{{AFX_DATA(ProgressBar)
	enum { IDD = IDD_RUNNING };
	CProgressCtrl	m_progress;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ProgressBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ProgressBar)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSBAR_H__F9BDB6D8_6628_49B7_BD3D_8627892716CF__INCLUDED_)
