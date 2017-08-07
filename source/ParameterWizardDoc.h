//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ParameterWizardDoc.h
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
#if !defined(AFX_PARAMETERWIZARDDOC_H__B23B9770_1371_4268_B51E_C2EF814BCBD1__INCLUDED_)
#define AFX_PARAMETERWIZARDDOC_H__B23B9770_1371_4268_B51E_C2EF814BCBD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParameterWizardDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ParameterWizardDoc document

class ParameterWizardDoc : public CWinMIST2Doc
{
protected:
	ParameterWizardDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ParameterWizardDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ParameterWizardDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ParameterWizardDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(ParameterWizardDoc)
	afx_msg void OnSimulate();
	afx_msg void OnViewRawMISTText();
	afx_msg void OnPurge();
	afx_msg void OnMist0();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// RawMISTDoc document

class RawMISTDoc : public CWinMIST2Doc
{
protected:
	RawMISTDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(RawMISTDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RawMISTDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~RawMISTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(RawMISTDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMETERWIZARDDOC_H__B23B9770_1371_4268_B51E_C2EF814BCBD1__INCLUDED_)
