#if !defined(AFX_GRAPHRESULTSFORMVIEW_H__839B46D8_5726_4779_9D6D_B0210B8C82FD__INCLUDED_)
#define AFX_GRAPHRESULTSFORMVIEW_H__839B46D8_5726_4779_9D6D_B0210B8C82FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphResultsFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphResultsFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CGraphResultsFormView : public CFormView
{
protected:
	CGraphResultsFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGraphResultsFormView)

// Form Data
public:
	//{{AFX_DATA(CGraphResultsFormView)
	enum { IDD = IDD_GRAPHRESULTSFORMVIEW_FORM };
	CComboBox	m_YAxis;
	CComboBox	m_XAxis;
	//}}AFX_DATA

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphResultsFormView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGraphResultsFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGraphResultsFormView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHRESULTSFORMVIEW_H__839B46D8_5726_4779_9D6D_B0210B8C82FD__INCLUDED_)
