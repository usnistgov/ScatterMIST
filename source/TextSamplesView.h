//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: TextSamplesView.h
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
#if !defined(AFX_TEXTSAMPLESVIEW_H__9B10CC11_28FA_46C2_A59C_46F0B6A67B71__INCLUDED_)
#define AFX_TEXTSAMPLESVIEW_H__9B10CC11_28FA_46C2_A59C_46F0B6A67B71__INCLUDED_

#include "WinMIST2Doc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextSamplesView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextSamplesView view

class CTextSamplesView : public CEditView
{
protected:
	CTextSamplesView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextSamplesView)

// Attributes
public:

// Operations
public:
	CWinMIST2Doc* GetDocument() {return (CWinMIST2Doc*)m_pDocument;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextSamplesView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextSamplesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextSamplesView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTSAMPLESVIEW_H__9B10CC11_28FA_46C2_A59C_46F0B6A67B71__INCLUDED_)
