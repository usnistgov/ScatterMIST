#if !defined(AFX_PROGRESSBAR1_H__F15C1140_7897_4E8A_AE6E_0039D6B3E5F9__INCLUDED_)
#define AFX_PROGRESSBAR1_H__F15C1140_7897_4E8A_AE6E_0039D6B3E5F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressBar1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ProgressBar dialog

class ProgressBar : public CDialog
{
// Construction
public:
	ProgressBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ProgressBar)
	enum { IDD = IDD_RUNNING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ProgressBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ProgressBar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSBAR1_H__F15C1140_7897_4E8A_AE6E_0039D6B3E5F9__INCLUDED_)
