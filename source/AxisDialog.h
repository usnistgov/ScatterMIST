//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: AxisDialog.h
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
#if !defined(AFX_AXISDIALOG_H__4CABBE74_F00A_4402_B353_F8E8D1DAC92F__INCLUDED_)
#define AFX_AXISDIALOG_H__4CABBE74_F00A_4402_B353_F8E8D1DAC92F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxisDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AxisDialog dialog

class AxisDialog : public CDialog
{
// Construction
public:
	AxisDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(AxisDialog)
	enum { IDD = IDD_AXISFORM };
	CButton	m_ylog;
	CButton	m_xlog;
	CComboBox	m_YAxisControl;
	CComboBox	m_XAxisControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AxisDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AxisDialog)
	afx_msg void OnSelchangeXAxisCombo();
	afx_msg void OnSelchangeYAxisCombo();
	afx_msg void OnXlog();
	afx_msg void OnYlog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISDIALOG_H__4CABBE74_F00A_4402_B353_F8E8D1DAC92F__INCLUDED_)
