//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ViewRawMIST.h
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
#if !defined(AFX_VIEWRAWMIST_H__545B811B_69C0_4E83_AF96_914811E29ACB__INCLUDED_)
#define AFX_VIEWRAWMIST_H__545B811B_69C0_4E83_AF96_914811E29ACB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewRawMIST.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ViewRawMIST dialog

class ViewRawMIST : public CDialog
{
// Construction
public:
	ViewRawMIST(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ViewRawMIST)
	enum { IDD = IDD_RAWMIST };
	CString	m_Text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ViewRawMIST)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ViewRawMIST)
	afx_msg void OnCreateRawMISTDoc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWRAWMIST_H__545B811B_69C0_4E83_AF96_914811E29ACB__INCLUDED_)
