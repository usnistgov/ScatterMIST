#if !defined(AFX_WINTEXTDOC_H__9041035B_8ECE_440F_B28C_D3EF4E20B099__INCLUDED_)
#define AFX_WINTEXTDOC_H__9041035B_8ECE_440F_B28C_D3EF4E20B099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WinTextDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWinTextDoc document

class CWinTextDoc : public CDocument
{
protected:
	CWinTextDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWinTextDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinTextDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWinTextDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWinTextDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINTEXTDOC_H__9041035B_8ECE_440F_B28C_D3EF4E20B099__INCLUDED_)
