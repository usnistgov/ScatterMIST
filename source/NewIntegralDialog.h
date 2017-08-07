//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: NewIntegralDialog.h
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
#if !defined(AFX_NEWINTEGRALDIALOG_H__9CC4FC98_E719_4973_86B7_D565D583BF0E__INCLUDED_)
#define AFX_NEWINTEGRALDIALOG_H__9CC4FC98_E719_4973_86B7_D565D583BF0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewIntegralDialog.h : header file
//

#include <list>
#include "parameterwizard.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// NewIntegralDialog dialog

class NewIntegralDialog : public CDialog
{
// Construction
public:
	NewIntegralDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewIntegralDialog)
	enum { IDD = IDD_NEWINTEGRALDIALOG };
	//}}AFX_DATA

	char type;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewIntegralDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewIntegralDialog)
	afx_msg void OnCircle();
	afx_msg void OnHemi();
	afx_msg void OnPolygon();
	afx_msg void OnDifferential();
	afx_msg void OnAnnulus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSehemi();
};

/////////////////////////////////////////////////////////////////////////////
// NewCircleIntegralDialog dialog

class NewCircleIntegralDialog : public CDialog
{
// Construction
public:
	NewCircleIntegralDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewCircleIntegralDialog)
	enum { IDD = IDD_NEWCIRCLE };
	CString	m_theta;
	CString	m_phi;
	CString	m_alpha;
	CString	m_pol;
	CString	m_name;
	CString m_incpol;
	CString	m_Comment;
	//}}AFX_DATA

	CircleIntegral result;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewCircleIntegralDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewCircleIntegralDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};
/////////////////////////////////////////////////////////////////////////////
// NewPolygonDialog dialog

class NewPolygonDialog : public CDialog
{
// Construction
public:
	NewPolygonDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewPolygonDialog)
	enum { IDD = IDD_NEWPOLYGON };
	CButton	m_add;
	CButton	m_moveup;
	CButton	m_movedown;
	CButton	m_delete;
	CButton	m_edit;
	CListCtrl	m_vertices;
	CString	m_phi;
	CString	m_pol;
	CString	m_theta;
	CString	m_newtheta;
	CString	m_newphi;
	CString	m_name;
	CString	m_Comment;
	CString m_incpol;
	//}}AFX_DATA

	PolygonIntegral result;

	bool dragging;
	int dragitem;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewPolygonDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void EnableEditButton(BOOL state) {m_edit.EnableWindow(state);}
	void EnableAddButton(BOOL state) {m_add.EnableWindow(state);}
	void EnableDeleteButton(BOOL state) {m_delete.EnableWindow(state);}
	void EnableMoveButtons(BOOL state) {m_moveup.EnableWindow(state);m_movedown.EnableWindow(state);}

	// Generated message map functions
	//{{AFX_MSG(NewPolygonDialog)
	afx_msg void OnAdd();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnClickVertices(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};
/////////////////////////////////////////////////////////////////////////////
// NewHemisphere dialog

class NewHemisphere : public CDialog
{
// Construction
public:
	NewHemisphere(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewHemisphere)
	enum { IDD = IDD_NEWHEMI };
	CString	m_name;
	CString	m_Comment;
	CString m_incpol;
	//}}AFX_DATA

	HemiIntegral result;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewHemisphere)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewHemisphere)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// OutputDialog dialog

class OutputDialog : public CDialog
{
// Construction
public:
	OutputDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(OutputDialog)
	enum { IDD = IDD_OUTPUTDIALOG };
	CString	m_value;
	CString	m_Comment;
	CString	m_label;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OutputDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OutputDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// NewDifferentialDialog dialog

class NewDifferentialDialog : public CDialog
{
// Construction
public:
	NewDifferentialDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewDifferentialDialog)
	enum { IDD = IDD_NEWDIFFERENTIAL };
	CString	m_name;
	CString	m_phi;
	CString	m_theta;
	CString	m_pol;
	CString	m_Comment;
	CString	m_incpol;
	//}}AFX_DATA

	DifferentialIntegral result;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewDifferentialDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewDifferentialDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWINTEGRALDIALOG_H__9CC4FC98_E719_4973_86B7_D565D583BF0E__INCLUDED_)
#pragma once


// NewSEHemisphere dialog

class NewAnnulus : public CDialog
{
	DECLARE_DYNAMIC(NewAnnulus)

public:
	NewAnnulus(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewAnnulus();

// Dialog Data
	enum { IDD = IDD_NEWANNULUS };

	AnnulusIntegral result;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_inpol;
	CString m_pol;
	CString m_comment;
	CString m_outerpolar;
	CString m_outerazimuthal;
	CString m_outeralpha;
	CString m_innerpolar;
	CString m_innerazimuthal;
	CString m_inneralpha;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
