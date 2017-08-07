//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: TreeWizard.cpp
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
// TreeWizard.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "TreeWizard.h"
#undef max
#include "brdf.h"

using namespace SCATMECH;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TreeWizard dialog


TreeWizard::TreeWizard(CWinMIST2Doc* _doc,CWnd* pParent /*=NULL*/)
: doc(_doc),CDialog(TreeWizard::IDD, pParent),itree(Model::inheritance)
{
	//{{AFX_DATA_INIT(TreeWizard)
	//}}AFX_DATA_INIT
}


void TreeWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TreeWizard)
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TreeWizard, CDialog)
	//{{AFX_MSG_MAP(TreeWizard)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TreeWizard message handlers


BOOL TreeWizard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	itree.BuildTreeCtrl(m_Tree,NULL);
	/*	
	model = m_Tree.InsertItem("MODEL",main);
	uservariables = m_Tree.InsertItem("VARIABLES (user)",main);
	modelvariables = m_Tree.InsertItem("VARIABLES (model)",main);
	vary = m_Tree.InsertItem("VARY",main);
	integrals = m_Tree.InsertItem("INTEGRALS",main);
	outputs = m_Tree.InsertItem("OUTPUTS",main);
	files = m_Tree.InsertItem("FILES",main);
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void InheritanceTree::BuildTreeCtrl(CTreeCtrl& box,HTREEITEM* parent)
{
	if (parent==NULL) {
		htreeitem = box.InsertItem(inheritance.get_name().c_str());
	} else {
		htreeitem = box.InsertItem(inheritance.get_name().c_str(),*parent);
	}
	for (iterator q=itreelist.begin();q!=itreelist.end();++q) {
		if ((*q)!=NULL) (*q)->BuildTreeCtrl(box,&htreeitem);
	}
}

const Inheritance* 
InheritanceTree::get_inheritance(HTREEITEM item)
{
	if (htreeitem==item) return &inheritance;
	for (iterator q=itreelist.begin();q!=itreelist.end();++q) {
		if ((*q)!=NULL) {
			const Inheritance* p = (*q)->get_inheritance(item);
			if (p!=NULL) return p;
		}
	}
	return NULL;
}

void TreeWizard::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	
	HTREEITEM item = m_Tree.GetSelectedItem( );
	const Inheritance* inheritance = itree.get_inheritance(item);
	
	if (inheritance!=NULL) {
		ModelParameterList mpl = inheritance->get_parameters();
		string editstring;
		editstring += "; " + inheritance->get_name() + " : " + inheritance->get_description() + "\r\n";
		editstring += "\r\n";
		for (ModelParameterList::iterator q=mpl.begin();q!=mpl.end();++q) {
			char prechar;
			if ((*q)->type=="int") prechar = '#';
			else if ((*q)->type=="double") prechar = '#';
			else if ((*q)->type=="float") prechar = '#';
			else if ((*q)->type=="long") prechar = '#';
			else if ((*q)->type=="bool") prechar = '#';
			else prechar = '$';
			editstring += prechar + (*q)->name + " =  *****\t ; ";
			editstring += (*q)->description + " <";
			editstring += (*q)->type + ">\r\n";
		}
		m_Edit.SetWindowText(editstring.c_str());
	} else throw SCATMECH_exception("Problem here!");

	*pResult = 0;
}
