//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ModelBrowser.h
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
#if !defined(AFX_MODELBROWSER_H__F2E5F0E6_314E_4E82_AFFC_03EC262A0843__INCLUDED_)
#define AFX_MODELBROWSER_H__F2E5F0E6_314E_4E82_AFFC_03EC262A0843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelBrowser.h : header file
//
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxcview.h>
/////////////////////////////////////////////////////////////////////////////
// ModelBrowserForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "TreeWizard.h"

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ModelBrowserTree view

class ModelBrowser;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ModelBrowserList view

class ModelBrowserList : public CListView
{
protected:
	ModelBrowserList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ModelBrowserList)

// Attributes
public:
	ModelBrowser* parentbrowser;

// Operations
public:
	void SetParentBrowser(ModelBrowser* modelbrowser) {parentbrowser=modelbrowser;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModelBrowserList)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~ModelBrowserList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(ModelBrowserList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class ModelBrowserTree : public CTreeView
{
protected:
	ModelBrowserTree();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ModelBrowserTree)

// Attributes
public:
	ModelBrowser* parentbrowser;
// Operations
public:
	void SetParentBrowser(ModelBrowser* modelbrowser) {parentbrowser=modelbrowser;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModelBrowserTree)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~ModelBrowserTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(ModelBrowserTree)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class ModelBrowserForm : public CFormView
{
	DECLARE_DYNCREATE(ModelBrowserForm)
public:
	ModelBrowserForm();           // protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(ModelBrowserForm)
	enum { IDD = IDD_BROWSERFORM };
	CEdit	m_ModelName;
	//}}AFX_DATA

// Attributes
public:
	ModelBrowser* parentbrowser;

// Operations
public:
	void SetParentBrowser(ModelBrowser* modelbrowser) {parentbrowser=modelbrowser;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModelBrowserForm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ModelBrowserForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ModelBrowserForm)
	afx_msg void OnCreateModel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// ModelBrowser frame

class ModelBrowser : public CMDIChildWnd
{
	DECLARE_DYNCREATE(ModelBrowser)
public:
	ModelBrowser();           // protected constructor used by dynamic creation

// Attributes
public:

	CSplitterWnd m_splitterwnd1;
	CSplitterWnd m_splitterwnd2;
	ModelBrowserTree* pTreeView;
	ModelBrowserList* pListView;
	ModelBrowserForm* pFormView;

	InheritanceTree itree;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModelBrowser)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~ModelBrowser();

	// Generated message map functions
	//{{AFX_MSG(ModelBrowser)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELBROWSER_H__F2E5F0E6_314E_4E82_AFFC_03EC262A0843__INCLUDED_)
