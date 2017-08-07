#if !defined(AFX_TEXT_H__95FCD93D_E01F_4788_B7B6_4E0EDD07FF5F__INCLUDED_)
#define AFX_TEXT_H__95FCD93D_E01F_4788_B7B6_4E0EDD07FF5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// text.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// text window

class text : public CEdit
{
// Construction
public:
	text();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(text)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~text();

	// Generated message map functions
protected:
	//{{AFX_MSG(text)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXT_H__95FCD93D_E01F_4788_B7B6_4E0EDD07FF5F__INCLUDED_)
