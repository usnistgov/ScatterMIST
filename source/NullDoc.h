#if !defined(AFX_NULLDOC_H__5B15C9DB_FE31_4DEF_85A3_52755F3BC801__INCLUDED_)
#define AFX_NULLDOC_H__5B15C9DB_FE31_4DEF_85A3_52755F3BC801__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NullDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNullDoc document

class CNullDoc : public CDocument
{
protected:
	CNullDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CNullDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNullDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNullDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CNullDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NULLDOC_H__5B15C9DB_FE31_4DEF_85A3_52755F3BC801__INCLUDED_)
