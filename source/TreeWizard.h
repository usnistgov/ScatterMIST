//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: TreeWizard.h
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
#if !defined(AFX_TREEWIZARD_H__DF9F797F_76DF_4782_86B4_05BCF80D75A5__INCLUDED_)
#define AFX_TREEWIZARD_H__DF9F797F_76DF_4782_86B4_05BCF80D75A5__INCLUDED_

#include <list>
#include "inherit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 100
// TreeWizard.h : header file
//

class InheritanceTree {
public:
	typedef std::list<InheritanceTree*>::iterator iterator;

	InheritanceTree(const SCATMECH::Inheritance& inh) : inheritance(inh) {
		using namespace SCATMECH;
		using namespace std;
		Register((Model*)0);
		InheritanceList ilist = inh.get_children();
		itreelist.resize(ilist.size());
		iterator q;
		SCATMECH::InheritanceList::iterator p;
		for (q=itreelist.begin(),p=ilist.begin();q!=itreelist.end();++q,++p) {
			if ((*p)!=&inh) {
				(*q) = new InheritanceTree(**p);
			} else {
				(*q) = NULL;
			}
		}
	}
	~InheritanceTree() {
		using namespace SCATMECH;
		using namespace std;
		for (iterator q=itreelist.begin();q!=itreelist.end();++q) {
			if ((*q)!=NULL) delete (*q);
		}
	}

	void BuildTreeCtrl(CTreeCtrl& box,HTREEITEM* parent=NULL);
	const SCATMECH::Inheritance* get_inheritance(HTREEITEM item);
	HTREEITEM htreeitem;
	const SCATMECH::Inheritance& inheritance;	
	std::list<InheritanceTree*> itreelist;
};


/////////////////////////////////////////////////////////////////////////////
// TreeWizard dialog

class TreeWizard : public CDialog
{
// Construction
public:
	TreeWizard(CWinMIST2Doc* _doc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TreeWizard)
	enum { IDD = IDD_TREEWIZARD };
	CEdit	m_Edit;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	CWinMIST2Doc* doc;

	InheritanceTree itree;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TreeWizard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TreeWizard)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_TREEWIZARD_H__DF9F797F_76DF_4782_86B4_05BCF80D75A5__INCLUDED_)
