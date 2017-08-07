#if !defined(AFX_CWinPolarDoc_H__BB6C5589_93FE_4717_97B5_1EDCBD723DD9__INCLUDED_)
#define AFX_CWinPolarDoc_H__BB6C5589_93FE_4717_97B5_1EDCBD723DD9__INCLUDED_

#include <vector>
#include <string>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CWinPolarDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWinPolarDoc document

class CWinPolarDoc : public CDocument
{
protected:
	CWinPolarDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWinPolarDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinPolarDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWinPolarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWinPolarDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWinPolarDoc_H__BB6C5589_93FE_4717_97B5_1EDCBD723DD9__INCLUDED_)
